#include <libultraship/bridge.h>

#include <vector>
#include <map>
#include <unordered_map>
#include <math.h>
#include "port/Engine.h"
#include "FrameInterpolation.h"
#include "matrix.h"
#include "engine/Matrix.h"

extern "C" {
#include "math_util.h"
#include "math_util_2.h"
#include "render_player.h"

extern Mat4* gInterpolationMatrix;
void mtxf_translate(Mat4, Vec3f);
}
/*
Frame interpolation.

The idea of this code is to interpolate all matrices.

The code contains two approaches. The first is to interpolate
all inputs in transformations, such as angles, scale and distances,
and then perform the same transformations with the interpolated values.
After evaluation for some reason some animations such rolling look strange.

The second approach is to simply interpolate the final matrices. This will
more or less simply interpolate the world coordinates for movements.
This will however make rotations ~180 degrees get the "paper effect".
The mitigation is to identify this case for actors and interpolate the
matrix but in model coordinates instead, by "removing" the rotation-
translation before interpolating, create a rotation matrix with the
interpolated angle which is then applied to the matrix.

Currently the code contains both methods but only the second one is currently
used.

Both approaches build a tree of instructions, containing matrices
at leaves. Every node is built from OPEN_DISPS/CLOSE_DISPS and manually
inserted FrameInterpolation_OpenChild/FrameInterpolation_Close child calls.
These nodes contain information that should suffice to identify the matrix,
so we can find it in an adjacent frame.

We can interpolate an arbitrary amount of frames between two original frames,
given a specific interpolation factor (0=old frame, 0.5=average of frames,
1.0=new frame).
*/

static bool invert_matrix(const float m[16], float invOut[16]);

using namespace std;

extern "C" {
extern Mat4* gInterpolationMatrix;
void mtxf_translate(Mat4, Vec3f);
}

namespace {

enum class Op {
    Marker,
    OpenChild,
    CloseChild,

    MatrixPush,
    MatrixPop,
    MatrixPut,
    MatrixMult,
    MatrixTranslate,
    MatrixScale,
    MatrixRotate1Coord,
    MatrixMult4x4,
    MatrixPosRotXYZ,
    MatrixMultVec3fNoTranslate,
    MatrixMultVec3f,
    MatrixMtxFToMtx,
    MatrixToMtx,
    MatrixRotateAxis,
    SkinMatrixMtxFToMtx,
    SetTransformMatrix,
    SetMatrixTransformation,
    SetTranslateRotate,
    SetTextMatrix,
    SetMatrixPosRotScaleXY,
};

typedef pair<const void*, uintptr_t> label;

union Data {
    Data() {
    }

    struct {
        Mat4** matrix;
    } matrix_ptr;

    struct {
        const char* file;
        int line;
    } marker;

    struct {
        Mat4* matrix;
        MtxF mf;
        u8 mode;
    } matrix_mult;

    struct {
        Mat4* matrix;
        Vec3fInterp b;
    } matrix_translate;

    struct {
        Mat4* matrix;
        f32 scale;
    } matrix_scale;

    struct {
        Mat4* matrix;
        u32 coord;
        s16 value;
    } matrix_rotate_1_coord;

    struct {
        Mat4* matrix;
        Vec3f src;
        Vec3f dest;
    } matrix_vec_translate;

    struct {
        Mat4* matrix;
        Vec3f src;
        Vec3f dest;
    } matrix_vec_no_translate;

    struct {
        Mat4* dest;
        Mat4 mtx1;
        Mat4 mtx2;
    } matrix_mult_4x4;

    struct {
        Mat4* matrix;
        Vec3fInterp pos;
        Vec3sInterp orientation;
    } matrix_pos_rot_xyz;

    struct {
        Mat4* matrix;
        s32 x;
        s32 y;
        u16 angle;
        f32 scale;
    } matrix_pos_rot_scale_xy;

    struct {
        Mat4* matrix;
        Vec3f translation;
        Vec3s rotation;
    } matrix_translate_rotate_zyx;

    struct {
        Mat4* matrix;
        f32 translateX, translateY, translateZ;
        Vec3s rot;
        // MtxF mtx;
        bool has_mtx;
    } matrix_set_translate_rotate_yxz;

    struct {
        MtxF src;
        Mtx* dest;
    } matrix_mtxf_to_mtx;

    struct {
        Mtx* dest;
        MtxF src;
        bool has_adjusted;
    } matrix_to_mtx;

    struct {
        MtxF mf;
    } matrix_replace_rotation;

    struct {
        f32 angle;
        Vec3f axis;
        u8 mode;
    } matrix_rotate_axis;

    struct {
        Mat4* dest;
        Vec3f orientationVector;
        Vec3f positionVector;
        u16 rotationAngle;
        f32 scaleFactor;
    } set_transform_matrix_data;

    struct {
        Mat4* dest;
        Vec3f location;
        Vec3su rotation;
        f32 scale;
    } set_matrix_transformation_data;

    struct {
        Mat4* dest;
        Vec3f location;
        Vec3s rotation;
    } set_translate_rotate_data;

    struct {
        Mat3* dest;
        f32 arg1;
        f32 arg2;
        f32 arg3;
        s16 rot;
    } set_orientation_matrix_data;

    struct {
        Mat4* matrix;
        f32 x;
        f32 y;
        f32 arg3;
        f32 arg4;
    } matrix_text;

    struct {
        label key;
        size_t idx;
    } open_child;
};

struct Path {
    map<label, vector<Path>> children;
    map<Op, vector<Data>> ops;
    vector<pair<Op, size_t>> items;
};

struct Recording {
    Path root_path;
};

bool is_recording;
vector<Path*> current_path;
uint32_t camera_epoch;
uint32_t previous_camera_epoch;
Recording current_recording;
Recording previous_recording;

bool next_is_actor_pos_rot_matrix;
bool has_inv_actor_mtx;
MtxF inv_actor_mtx;
size_t inv_actor_mtx_path_index;

Data& append(Op op) {
    auto& m = current_path.back()->ops[op];
    current_path.back()->items.emplace_back(op, m.size());
    return m.emplace_back();
}

MtxF* Matrix_GetCurrent() {
    return (MtxF*) gInterpolationMatrix;
}

struct InterpolateCtx {
    float step;
    float w;
    unordered_map<Mtx*, MtxF> mtx_replacements;
    MtxF tmp_mtxf, tmp_mtxf2;
    Mat3 tmp_mat3;
    Vec3f tmp_vec3f, tmp_vec3f2;
    Vec3s tmp_vec3s;
    int32_t tmp32[2];
    MtxF actor_mtx;

    MtxF* new_replacement(Mtx* addr) {
        return &mtx_replacements[addr];
    }

    void interpolate_mtxf(MtxF* res, MtxF* o, MtxF* n) {
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                res->mf[i][j] = w * o->mf[i][j] + step * n->mf[i][j];
            }
        }
    }

    float lerp(f32 o, f32 n) {
        return w * o + step * n;
    }

    s16 lerp_s16(s16 o, s16 n) {
        return w * o + step * n;
    }

    s32 lerp_s32(s32 o, s32 n) {
        return w * o + step * n;
    }

    void lerp_vec3s(Vec3s* res, Vec3s o, Vec3s n) {
        *res[0] = lerp_s16(o[0], n[0]);
        *res[1] = lerp_s16(o[1], n[1]);
        *res[2] = lerp_s16(o[2], n[2]);
    }

    void lerp_vec3f(Vec3f* res, Vec3f* o, Vec3f* n) {
        *res[0] = lerp(*o[0], *n[0]);
        *res[1] = lerp(*o[1], *n[1]);
        *res[2] = lerp(*o[2], *n[2]);
    }

    float interpolate_angle(f32 o, f32 n) {
        if (o == n)
            return n;
        o = fmodf(o, 2 * M_PI);
        if (o < 0.0f) {
            o += 2 * M_PI;
        }
        n = fmodf(n, 2 * M_PI);
        if (n < 0.0f) {
            n += 2 * M_PI;
        }
        if (fabsf(o - n) > M_PI) {
            if (o < n) {
                o += 2 * M_PI;
            } else {
                n += 2 * M_PI;
            }
        }
        if (fabsf(o - n) > M_PI / 2) {
            // return n;
        }
        return lerp(o, n);
    }

    s16 interpolate_angle(s16 os, s16 ns) {
        if (os == ns)
            return ns;
        int o = (u16) os;
        int n = (u16) ns;
        u16 res;
        int diff = o - n;
        if (-0x8000 <= diff && diff <= 0x8000) {
            if (diff < -0x4000 || diff > 0x4000) {
                return ns;
            }
            res = (u16) (w * o + step * n);
        } else {
            if (o < n) {
                o += 0x10000;
            } else {
                n += 0x10000;
            }
            diff = o - n;
            if (diff < -0x4000 || diff > 0x4000) {
                return ns;
            }
            res = (u16) (w * o + step * n);
        }
        if (os / 327 == ns / 327 && (s16) res / 327 != os / 327) {
            int bp = 0;
        }
        return res;
    }

    void interpolate_vecs(Vec3f* res, Vec3f* o, Vec3f* n) {
        *res[0] = interpolate_angle(*o[0], *n[0]);
        *res[1] = interpolate_angle(*o[1], *n[1]);
        *res[2] = interpolate_angle(*o[2], *n[2]);
    }

    void interpolate_angles(Vec3s* res, Vec3s* o, Vec3s* n) {
        *res[0] = interpolate_angle(*o[0], *n[0]);
        *res[1] = interpolate_angle(*o[1], *n[1]);
        *res[2] = interpolate_angle(*o[2], *n[2]);
    }

    void interpolate_branch(Path* old_path, Path* new_path) {
        for (auto& item : new_path->items) {
            Data& new_op = new_path->ops[item.first][item.second];

            if (item.first == Op::OpenChild) {
                if (auto it = old_path->children.find(new_op.open_child.key);
                    it != old_path->children.end() && new_op.open_child.idx < it->second.size()) {
                    interpolate_branch(&it->second[new_op.open_child.idx],
                                       &new_path->children.find(new_op.open_child.key)->second[new_op.open_child.idx]);
                } else {
                    interpolate_branch(&new_path->children.find(new_op.open_child.key)->second[new_op.open_child.idx],
                                       &new_path->children.find(new_op.open_child.key)->second[new_op.open_child.idx]);
                }
                continue;
            }

            if (auto it = old_path->ops.find(item.first); it != old_path->ops.end()) {
                if (item.second < it->second.size()) {
                    Data& old_op = it->second[item.second];
                    switch (item.first) {
                        case Op::OpenChild:
                        case Op::CloseChild:
                        case Op::Marker:
                            break;

                        case Op::MatrixPush:
                            Matrix_Push((Matrix**) &gInterpolationMatrix);
                            break;

                        case Op::MatrixPop:
                            Matrix_Pop((Matrix**) &gInterpolationMatrix);
                            break;

                            // Unused on SF64
                            // case Op::MatrixPut:
                            //     interpolate_mtxf(&tmp_mtxf, &old_op.matrix_put.src, &new_op.matrix_put.src);
                            //     Matrix_Put(&tmp_mtxf);
                            //     break;

                        case Op::MatrixMult:
                            interpolate_mtxf(&tmp_mtxf, &old_op.matrix_mult.mf, &new_op.matrix_mult.mf);
                            mtxf_multiplication(*gInterpolationMatrix, tmp_mtxf.mf, new_op.matrix_mult.mf.mf);
                            // Matrix_Mult(gInterpolationMatrix, (Matrix*) &tmp_mtxf, new_op.matrix_mult.mode);
                            break;

                        case Op::MatrixTranslate:
                            Vec3f temp;

                            temp[0] = lerp(old_op.matrix_translate.b.x, new_op.matrix_translate.b.x);
                            temp[1] = lerp(old_op.matrix_translate.b.y, new_op.matrix_translate.b.y);
                            temp[2] = lerp(old_op.matrix_translate.b.z, new_op.matrix_translate.b.z);

                            mtxf_translate(*gInterpolationMatrix, temp);
                            break;
                        case Op::MatrixPosRotXYZ:
                            Vec3f tempF;
                            Vec3s tempS;

                            tempF[0] = lerp(old_op.matrix_pos_rot_xyz.pos.x, new_op.matrix_pos_rot_xyz.pos.x);
                            tempF[1] = lerp(old_op.matrix_pos_rot_xyz.pos.y, new_op.matrix_pos_rot_xyz.pos.y);
                            tempF[2] = lerp(old_op.matrix_pos_rot_xyz.pos.z, new_op.matrix_pos_rot_xyz.pos.z);

                            tempS[0] =
                                lerp(old_op.matrix_pos_rot_xyz.orientation.x, new_op.matrix_pos_rot_xyz.orientation.x);
                            tempS[1] =
                                lerp(old_op.matrix_pos_rot_xyz.orientation.y, new_op.matrix_pos_rot_xyz.orientation.y);
                            tempS[2] =
                                lerp(old_op.matrix_pos_rot_xyz.orientation.z, new_op.matrix_pos_rot_xyz.orientation.z);

                            mtxf_pos_rotation_xyz(*gInterpolationMatrix, tempF, tempS);
                            break;

                        case Op::MatrixScale:
                            mtxf_scale(*gInterpolationMatrix, lerp(old_op.matrix_scale.scale, new_op.matrix_scale.scale));
                            break;

                        case Op::MatrixRotate1Coord: {
                            s16 v = interpolate_angle(old_op.matrix_rotate_1_coord.value,
                                                      new_op.matrix_rotate_1_coord.value);
                            switch (new_op.matrix_rotate_1_coord.coord) {
                                case 0:
                                    mtxf_rotate_x(*gInterpolationMatrix, v);
                                    break;

                                case 1:
                                    mtxf_rotate_y(*gInterpolationMatrix, v);
                                    break;

                                case 2:
                                    mtxf_s16_rotate_z(*gInterpolationMatrix, v);
                                    break;
                            }
                            break;
                        }
                        case Op::MatrixMultVec3fNoTranslate: {
                            interpolate_vecs(&tmp_vec3f, &old_op.matrix_vec_no_translate.src,
                                             &new_op.matrix_vec_no_translate.src);
                            interpolate_vecs(&tmp_vec3f2, &old_op.matrix_vec_no_translate.dest,
                                             &new_op.matrix_vec_no_translate.dest);
                            // Matrix_MultVec3fNoTranslate(gInterpolationMatrix, &tmp_vec3f, &tmp_vec3f2);
                            break;
                        }
                        case Op::MatrixMultVec3f: {
                            interpolate_vecs(&tmp_vec3f, &old_op.matrix_vec_translate.src,
                                             &new_op.matrix_vec_translate.src);
                            interpolate_vecs(&tmp_vec3f2, &old_op.matrix_vec_translate.dest,
                                             &new_op.matrix_vec_translate.dest);
                            // Matrix_MultVec3f(gInterpolationMatrix, &tmp_vec3f, &tmp_vec3f2);
                            break;
                        }

                        case Op::MatrixMtxFToMtx:
                            interpolate_mtxf(new_replacement(new_op.matrix_mtxf_to_mtx.dest),
                                             &old_op.matrix_mtxf_to_mtx.src, &new_op.matrix_mtxf_to_mtx.src);
                            break;

                        case Op::MatrixToMtx: {
                            //*new_replacement(new_op.matrix_to_mtx.dest) = *Matrix_GetCurrent();
                            if (old_op.matrix_to_mtx.has_adjusted && new_op.matrix_to_mtx.has_adjusted) {
                                interpolate_mtxf(&tmp_mtxf, &old_op.matrix_to_mtx.src, &new_op.matrix_to_mtx.src);
                                // Matrix_MtxFMtxFMult(&actor_mtx, &tmp_mtxf,
                                //                         new_replacement(new_op.matrix_to_mtx.dest));
                            } else {
                                interpolate_mtxf(new_replacement(new_op.matrix_to_mtx.dest), &old_op.matrix_to_mtx.src,
                                                 &new_op.matrix_to_mtx.src);
                            }
                            break;
                        }

                        case Op::MatrixRotateAxis: {
                            lerp_vec3f(&tmp_vec3f, &old_op.matrix_rotate_axis.axis, &new_op.matrix_rotate_axis.axis);
                            auto tmp =
                                interpolate_angle(old_op.matrix_rotate_axis.angle, new_op.matrix_rotate_axis.angle);
                            // Matrix_RotateAxis((Matrix*) &tmp_vec3f, tmp, 1.0f, 1.0f, 1.0f,
                            // new_op.matrix_rotate_axis.mode);
                            break;
                        }

                        case Op::SetTransformMatrix: {
                            lerp_vec3f(&tmp_vec3f, &old_op.set_transform_matrix_data.orientationVector,
                                       &new_op.set_transform_matrix_data.orientationVector);

                            lerp_vec3f(&tmp_vec3f2, &old_op.set_transform_matrix_data.positionVector,
                                       &new_op.set_transform_matrix_data.positionVector);

                            u16 rotationAngleTemp = lerp_s16(old_op.set_transform_matrix_data.rotationAngle,
                                       new_op.set_transform_matrix_data.rotationAngle);
                            f32 scaleFactorTemp = lerp(old_op.set_transform_matrix_data.scaleFactor, new_op.set_transform_matrix_data.scaleFactor);

                            set_transform_matrix(*gInterpolationMatrix, tmp_vec3f, tmp_vec3f2, rotationAngleTemp, scaleFactorTemp);

                            break;
                        }

                        case Op::SetMatrixTransformation: {

                            lerp_vec3f(&tmp_vec3f, &old_op.set_matrix_transformation_data.location,
                                       &new_op.set_matrix_transformation_data.location);

                            lerp_vec3s(&tmp_vec3s, *(Vec3s*)&old_op.set_matrix_transformation_data.rotation,
                                                   *(Vec3s*)&new_op.set_matrix_transformation_data.rotation);

                            f32 scaleFactorTemp = lerp(old_op.set_matrix_transformation_data.scale, new_op.set_matrix_transformation_data.scale);

                            mtxf_set_matrix_transformation(*gInterpolationMatrix, tmp_vec3f, *(Vec3su*)&tmp_vec3s, scaleFactorTemp);
                            break;
                        }
                        
                        case Op::SetTranslateRotate: {
                            lerp_vec3f(&tmp_vec3f, &old_op.set_translate_rotate_data.location,
                                       &new_op.set_translate_rotate_data.location);

                            lerp_vec3s(&tmp_vec3s, old_op.set_translate_rotate_data.rotation,
                                                   new_op.set_translate_rotate_data.rotation);

                            mtxf_translate_rotate(*gInterpolationMatrix, tmp_vec3f, tmp_vec3s);
                            break;
                        }
                        case Op::SetTextMatrix: {

                            tmp_vec3f[0] = lerp(old_op.matrix_text.x, new_op.matrix_text.x);
                            tmp_vec3f[1] = lerp(old_op.matrix_text.y, new_op.matrix_text.y);
                            tmp_vec3f[2] = lerp(old_op.matrix_text.arg3, new_op.matrix_text.arg3);
                            tmp_vec3f2[0] = lerp(old_op.matrix_text.arg4, new_op.matrix_text.arg4);

                            SetTextMatrix(*gInterpolationMatrix, tmp_vec3f[0], tmp_vec3f[1], tmp_vec3f[2], tmp_vec3f2[0]);
                            break;
                        }
                        case Op::SetMatrixPosRotScaleXY: {
                            tmp32[0] = lerp_s32(old_op.matrix_pos_rot_scale_xy.x, new_op.matrix_pos_rot_scale_xy.x);
                            tmp32[1] = lerp_s32(old_op.matrix_pos_rot_scale_xy.y, new_op.matrix_pos_rot_scale_xy.y);

                            tmp_vec3s[0] = lerp_s16(old_op.matrix_pos_rot_scale_xy.angle, new_op.matrix_pos_rot_scale_xy.angle);

                            tmp_vec3f[0] = lerp(old_op.matrix_pos_rot_scale_xy.scale, new_op.matrix_pos_rot_scale_xy.scale);

                            mtxf_translation_x_y_rotate_z_scale_x_y(*gInterpolationMatrix, tmp32[0], tmp32[1], tmp_vec3s[0], tmp_vec3f[0]);
                            break;
                        }
                    }
                }
            }
        }
    }
};

} // anonymous namespace

unordered_map<Mtx*, MtxF> FrameInterpolation_Interpolate(float step) {
    InterpolateCtx ctx;
    ctx.step = step;
    ctx.w = 1.0f - step;
    ctx.interpolate_branch(&previous_recording.root_path, &current_recording.root_path);
    return ctx.mtx_replacements;
}

bool camera_interpolation = true;

void FrameInterpolation_ShouldInterpolateFrame(bool shouldInterpolate) {
    camera_interpolation = shouldInterpolate;
    is_recording = shouldInterpolate;
}

void FrameInterpolation_StartRecord(void) {
    previous_recording = move(current_recording);
    current_recording = {};
    current_path.clear();
    current_path.push_back(&current_recording.root_path);
    if (!camera_interpolation) {
        // default to interpolating
        camera_interpolation = true;
        is_recording = false;
        return;
    }
    if (GameEngine::GetInterpolationFPS() != 30) {
        is_recording = true;
    }
}

void FrameInterpolation_StopRecord(void) {
    previous_camera_epoch = camera_epoch;
    is_recording = false;
}

void FrameInterpolation_RecordOpenChild(const void* a, uintptr_t b) {
    if (!is_recording)
        return;
    label key = { a, b };
    auto& m = current_path.back()->children[key];
    append(Op::OpenChild).open_child = { key, m.size() };
    current_path.push_back(&m.emplace_back());
}

void FrameInterpolation_RecordCloseChild(void) {
    if (!is_recording)
        return;
    // append(Op::CloseChild);
    if (has_inv_actor_mtx && current_path.size() == inv_actor_mtx_path_index) {
        has_inv_actor_mtx = false;
    }
    current_path.pop_back();
}

void FrameInterpolation_DontInterpolateCamera(void) {
    camera_epoch = previous_camera_epoch + 1;
}

int FrameInterpolation_GetCameraEpoch(void) {
    return (int) camera_epoch;
}

void FrameInterpolation_Record_SetTextMatrix(Mat4* matrix, f32 x, f32 y, f32 arg3, f32 arg4) {
    if (!is_recording)
        return;
    append(Op::SetTextMatrix).matrix_text = {matrix, x, y, arg3, arg4};
}

void FrameInterpolation_RecordActorPosRotMatrix(void) {
    if (!is_recording)
        return;
    next_is_actor_pos_rot_matrix = true;
}

void FrameInterpolation_RecordMatrixPush(Mat4* matrix) {
    if (!is_recording)
        return;

    append(Op::MatrixPush).matrix_ptr = { (Mat4**) matrix };
}

void FrameInterpolation_RecordMarker(const char* file, int line) {
    if (!is_recording)
        return;

    append(Op::Marker).marker = { file, line };
}

void FrameInterpolation_RecordMatrixPop(Mat4* matrix) {
    if (!is_recording)
        return;
    append(Op::MatrixPop).matrix_ptr = { (Mat4**) matrix };
}

void FrameInterpolation_RecordMatrixPut(MtxF* src) {
    if (!is_recording)
        return;
    //    append(Op::MatrixPut).matrix_put = { matrix, *src };
}

void FrameInterpolation_RecordMatrixMult(Mat4* matrix, MtxF* mf, u8 mode) {
    if (!is_recording)
        return;
    append(Op::MatrixMult).matrix_mult = { matrix, *mf, mode };
}

void FrameInterpolation_RecordMatrixTranslate(Mat4* matrix, Vec3f b) {
    if (!is_recording)
        return;

    append(Op::MatrixTranslate).matrix_translate = { matrix, *((Vec3fInterp*) &b) };
}

void FrameInterpolation_RecordMatrixScale(Mat4* matrix, f32 scale) {
    if (!is_recording)
        return;
    append(Op::MatrixScale).matrix_scale = { matrix, scale };
}

void FrameInterpolation_RecordMatrixMultVec3fNoTranslate(Mat4* matrix, Vec3f src, Vec3f dest) {
    if (!is_recording)
        return;
    // append(Op::MatrixMultVec3fNoTranslate).matrix_vec_no_translate = { matrix, src, dest };
}

void FrameInterpolation_RecordSetTransformMatrix(Mat4* dest, Vec3f orientationVector, Vec3f positionVector, u16 rotationAngle,
                          f32 scaleFactor) {
    if (!is_recording)
        return;
    append(Op::SetTransformMatrix).set_transform_matrix_data = { dest, {orientationVector[0], orientationVector[1], orientationVector[2]}, { positionVector[0], positionVector[1], positionVector[2] }, rotationAngle, scaleFactor};
}

void FrameInterpolation_RecordTranslateRotate(Mat4* dest, Vec3f pos, Vec3s rotation) {
    if (!is_recording) { return; }

    append(Op::SetTranslateRotate).set_translate_rotate_data = { dest, {pos[0], pos[1], pos[2]}, { rotation[0], rotation[1], rotation[2] }};
}

void FrameInterpolation_RecordSetMatrixTransformation(Mat4* dest, Vec3f location, Vec3su rotation, f32 scale) {
    if (!is_recording)
        return;
    append(Op::SetMatrixTransformation).set_matrix_transformation_data = { dest, {location[0], location[1], location[2]}, { rotation[0], rotation[1], rotation[2] }, scale};
}

void FrameInterpolation_RecordCalculateOrientationMatrix(Mat3* dest, f32 x, f32 y, f32 z, s16 rot) {
    if (!is_recording) return;

  //  append(Op::SetMatrixTransformation).set_calculate_orientation_matrix_data = { dest, x, y, z, rot};
}

// Make a template for deref

void FrameInterpolation_RecordMatrixPosRotXYZ(Mat4* out, Vec3f pos, Vec3s orientation) {
    if (!is_recording)
        return;
    append(Op::MatrixPosRotXYZ).matrix_pos_rot_xyz = { out, *((Vec3fInterp*) &pos), *((Vec3sInterp*) &orientation) };
}

void FrameInterpolation_RecordMatrixPosRotScaleXY(Mat4* matrix, s32 x, s32 y, u16 angle, f32 scale) {
    if (!is_recording)
        return;
    append(Op::SetMatrixPosRotScaleXY).matrix_pos_rot_scale_xy = { matrix, x, y, angle, scale };
}

void FrameInterpolation_RecordMatrixMultVec3f(Mat4* matrix, Vec3f src, Vec3f dest) {
    if (!is_recording)
        return;
    // append(Op::MatrixMultVec3f).matrix_vec_translate = { matrix, src, dest };
}

void FrameInterpolation_RecordMatrixRotate1Coord(Mat4* matrix, u32 coord, s16 value) {
    if (!is_recording)
        return;
    append(Op::MatrixRotate1Coord).matrix_rotate_1_coord = { matrix, coord, value };
}

void FrameInterpolation_RecordMatrixMtxFToMtx(MtxF* src, Mtx* dest) {
    if (!is_recording)
        return;
    append(Op::MatrixMtxFToMtx).matrix_mtxf_to_mtx = { *src, dest };
}

void FrameInterpolation_RecordMatrixToMtx(Mtx* dest, char* file, s32 line) {
    if (!is_recording)
        return;
    auto& d = append(Op::MatrixToMtx).matrix_to_mtx = { dest };
    if (has_inv_actor_mtx) {
        d.has_adjusted = true;
        // Matrix_MtxFMtxFMult(&inv_actor_mtx, Matrix_GetCurrent(), &d.src);
    } else {
        d.src = *Matrix_GetCurrent();
    }
}

void FrameInterpolation_RecordMatrixRotateAxis(f32 angle, Vec3f* axis, u8 mode) {
    if (!is_recording)
        return;
    // append(Op::MatrixRotateAxis).matrix_rotate_axis = { angle, axis, mode };
}

void FrameInterpolation_RecordSkinMatrixMtxFToMtx(MtxF* src, Mtx* dest) {
    if (!is_recording)
        return;
    FrameInterpolation_RecordMatrixMtxFToMtx(src, dest);
}

// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
static bool invert_matrix(const float m[16], float invOut[16]) {
    float inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] +
             m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] -
             m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] +
             m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] -
              m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] -
             m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] +
             m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] -
             m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] +
              m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] +
             m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] -
             m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] +
              m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] -
              m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] -
             m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] -
              m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0) {
        return false;
    }

    det = 1.0 / det;

    for (i = 0; i < 16; i++) {
        invOut[i] = inv[i] * det;
    }

    return true;
}