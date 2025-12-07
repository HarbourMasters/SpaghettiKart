#include <libultraship.h>
#include <libultra/gbi.h>
#include "TrashBin.h"
#include "World.h"
#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "main.h"
#include "code_800029B0.h"
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "assets/models/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
#include "audio/external.h"
}

#define DEGREES_FLOAT_TO_SHORT(Degrees) ((s16)((Degrees) * (0x8000 / 180.0f)))

OTrashBin::OTrashBin(const SpawnParams& params) : OObject(params) {
    Name = "Trash Bin";
    ResourceName = "mk:trash_bin";
    _pos = params.Location.value_or(FVector(0, 0, 0));
    _rot = params.Rotation.value_or(IRotator(0, 0, 0));
    _scale = params.Scale.value_or(FVector(0, 0, 0)).y; // Only y
    _bhv = static_cast<Behaviour>(params.Behaviour.value_or(0));

    find_unused_obj_index(&_objectIndex);

    init_object(_objectIndex, 0);

    if (!IsBansheeBoardwalk()) {
        _drawBin = true;
    }
}

void OTrashBin::Tick() {
    OTrashBin::func_8007E00C(_objectIndex);

    switch(_bhv) {
        case STATIC:
            break;
        case MUNCHING:
            func_8007DDC0(_objectIndex);
            break;
    }
}

void OTrashBin::Draw(s32 cameraId) {
    Object* object;

    func_8008A364(_objectIndex, cameraId, 0x5555U, 0x00000320);
    if (is_obj_flag_status_active(_objectIndex, VISIBLE) != 0) {
        object = &gObjectList[_objectIndex];
        if (object->state >= 2) {
            func_80043220(object->pos, object->orientation, object->sizeScaling, object->model);
        }

        if (_drawBin) {
            Mat4 mtx;
            Vec3f Pos = { _pos.x + 63, _pos.y + 12, _pos.z + 25 };
            Vec3s Rot = { 0, 0x4000, 0 };

            // @port: Tag the transform.
            FrameInterpolation_RecordOpenChild("OTrashBin", (uintptr_t) object);

            mtxf_pos_rotation_xyz(mtx, Pos, Rot);
            //mtxf_scale(mtx, 1.0f);
            if (render_set_position(mtx, 0) != 0) {
                gSPDisplayList(gDisplayListHead++, BinMod);
            }

            // @port Pop the transform id.
            FrameInterpolation_RecordCloseChild();
        }
    }
}

void OTrashBin::init_bb_trash_bin(s32 objectIndex) {
    gObjectList[objectIndex].sizeScaling = 1.0f;
    gObjectList[objectIndex].model = (Gfx*)d_course_banshee_boardwalk_dl_trash_bin;
    gObjectList[objectIndex].unk_04C = 0;
    gObjectList[objectIndex].unk_084[7] = 0;
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    gObjectList[objectIndex].orientation[0] = DEGREES_FLOAT_TO_SHORT(_rot.pitch);
    gObjectList[objectIndex].orientation[1] = DEGREES_FLOAT_TO_SHORT(_rot.yaw);
    gObjectList[objectIndex].orientation[2] = DEGREES_FLOAT_TO_SHORT(_rot.roll);
    gObjectList[objectIndex].pos[0] = _pos.x;
    if (_drawBin) {
        // Position the lid on-top of the box.
        // This might not be the best fix, but this allows the box to be placed on a surface
        // when spawned at 0,0,0
        gObjectList[objectIndex].pos[0] = _pos.y + 32.0f;
    } else {
        // Stock implementation for banshee boardwalk
        gObjectList[objectIndex].pos[1] = _pos.y;
    }
    gObjectList[objectIndex].pos[2] = _pos.z;
    set_obj_velocity(objectIndex, 0.0f, 0.0f, 0.0f);
    gObjectList[objectIndex].type = 0;
    object_next_state(objectIndex);
}

#undef DEGREES_FLOAT_TO_SHORT

void OTrashBin::func_8007E00C(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            init_bb_trash_bin(objectIndex);
            break;
        case 3:
            D_8018CFB0 = 1;
            object_next_state(objectIndex);
            break;
        case 4:
            set_and_run_timer_object(objectIndex, 0x000000D2);
            if (D_80165594 == 0) {
                if (gCCSelection < CC_150) {
                    func_8007D714(1);
                    func_8007D714(1);
                } else {
                    func_8007D714(1);
                    func_8007D714(1);
                    func_8007D714(1);
                    func_8007D714(1);
                }
            }
            func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, -0x00002000, 0, 0x00000400, 0, -1);
            gObjectList[objectIndex].orientation[2] = gObjectList[objectIndex].primAlpha;
            if (gObjectList[objectIndex].unk_084[7] == 0) {
                func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4E));
                gObjectList[objectIndex].unk_084[7] = 0x0014;
            } else {
                gObjectList[objectIndex].unk_084[7]--;
            }
            break;
        case 5:
            gObjectList[objectIndex].orientation[2] = func_800417B4(gObjectList[objectIndex].orientation[2], 0U);
            if (gObjectList[objectIndex].orientation[2] == 0) {
                object_next_state(objectIndex);
            }
            break;
        case 6:
            gObjectList[objectIndex].orientation[2] = 0;
            gObjectList[objectIndex].unk_084[7] = 0;
            object_next_state(objectIndex);
            D_8018CFB0 = 0;
            break;
        case 0:
        case 2:
        default:
            break;
    }
}

Gfx OTrashBin::BinMod[] =
{
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture651B20),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 256),
	gsSPVertex(BinVtx, 20, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSPVertex(BinVtx2, 20, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
    gsSPEndDisplayList(),
};







Vtx OTrashBin::BinVtx[20] = {
	{{ {25, -13, 62}, 0, {0, -2303}, {255, 255, 255, 0} }},
	{{ {-25, -13, 62}, 0, {1280, -2303}, {255, 255, 255, 0} }},
	{{ {-25, -13, -63}, 0, {1280, 896}, {255, 255, 255, 0} }},
	{{ {25, -13, -63}, 0, {0, 895}, {255, 255, 255, 0} }},
	{{ {25, -13, -63}, 0, {0, 1113}, {255, 255, 255, 0} }},
	{{ {25, 20, -63}, 0, {0, 256}, {255, 255, 255, 0} }},
	{{ {25, 20, 62}, 0, {3199, 256}, {255, 255, 255, 0} }},
	{{ {25, -13, 62}, 0, {3199, 1113}, {255, 255, 255, 0} }},
	{{ {-25, -13, -63}, 0, {0, 1113}, {255, 255, 255, 0} }},
	{{ {-25, 20, -63}, 0, {0, 255}, {255, 255, 255, 0} }},
	{{ {25, 20, -63}, 0, {1280, 255}, {255, 255, 255, 0} }},
	{{ {25, -13, -63}, 0, {1280, 1113}, {255, 255, 255, 0} }},
	{{ {-25, -13, 62}, 0, {0, 1113}, {255, 255, 255, 0} }},
	{{ {-25, 20, 62}, 0, {0, 255}, {255, 255, 255, 0} }},
	{{ {-25, 20, -63}, 0, {3200, 255}, {255, 255, 255, 0} }},
	{{ {-25, -13, -63}, 0, {3200, 1113}, {255, 255, 255, 0} }},
	{{ {25, -13, 62}, 0, {0, 1113}, {255, 255, 255, 0} }},
	{{ {25, 20, 62}, 0, {0, 256}, {255, 255, 255, 0} }},
	{{ {-25, 20, 62}, 0, {1279, 256}, {255, 255, 255, 0} }},
	{{ {-25, -13, 62}, 0, {1279, 1113}, {255, 255, 255, 0} }},
};

Vtx OTrashBin::BinVtx2[20] = {
	{{ {19, -8, 52}, 0, {3199, 1113}, {255, 255, 255, 0} }},
	{{ {25, 20, 62}, 0, {3199, 256}, {255, 255, 255, 0} }},
	{{ {25, 20, -63}, 0, {0, 256}, {255, 255, 255, 0} }},
	{{ {19, -8, -52}, 0, {0, 1113}, {255, 255, 255, 0} }},
	{{ {19, -8, -52}, 0, {1280, 1113}, {255, 255, 255, 0} }},
	{{ {25, 20, -63}, 0, {1280, 255}, {255, 255, 255, 0} }},
	{{ {-25, 20, -63}, 0, {0, 255}, {255, 255, 255, 0} }},
	{{ {-19, -8, -52}, 0, {0, 1113}, {255, 255, 255, 0} }},
	{{ {-19, -8, 52}, 0, {1279, 1113}, {255, 255, 255, 0} }},
	{{ {-25, 20, 62}, 0, {1279, 256}, {255, 255, 255, 0} }},
	{{ {25, 20, 62}, 0, {0, 256}, {255, 255, 255, 0} }},
	{{ {19, -8, 52}, 0, {0, 1113}, {255, 255, 255, 0} }},
	{{ {19, -8, -52}, 0, {0, 895}, {255, 255, 255, 0} }},
	{{ {-19, -8, -52}, 0, {1280, 896}, {255, 255, 255, 0} }},
	{{ {-19, -8, 52}, 0, {1280, -2303}, {255, 255, 255, 0} }},
	{{ {19, -8, 52}, 0, {0, -2303}, {255, 255, 255, 0} }},
	{{ {-19, -8, -52}, 0, {3200, 1113}, {255, 255, 255, 0} }},
	{{ {-25, 20, -63}, 0, {3200, 255}, {255, 255, 255, 0} }},
	{{ {-25, 20, 62}, 0, {0, 255}, {255, 255, 255, 0} }},
	{{ {-19, -8, 52}, 0, {0, 1113}, {255, 255, 255, 0} }},
};
