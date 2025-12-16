#include "Snowman.h"
#include "World.h"

extern "C" {
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/frappe_snowland/frappe_snowland_data.h"
#include "assets/textures/tracks/frappe_snowland/frappe_snowland_data.h"
#include "assets/models/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
}
#include "port/interpolation/FrameInterpolation.h"

static const char* sSnowmanHeadList[] = { d_course_frappe_snowland_snowman_head };

size_t OSnowman::_count = 0;

OSnowman::OSnowman(const SpawnParams& params) : OObject(params) {
    Name = "Snowman";
    ResourceName = "mk:snowman";
    _idx = _count;
    Pos = params.Location.value_or(FVector(0, 0, 0));

    find_unused_obj_index(&_headIndex);
    init_object(_headIndex, 0);
    _objectIndex = _headIndex;
    gObjectList[_headIndex].origin_pos[0] = Pos.x * xOrientation;
    gObjectList[_headIndex].origin_pos[1] = Pos.y + 5.0 + 3.0;
    gObjectList[_headIndex].origin_pos[2] = Pos.z;
    gObjectList[_headIndex].pos[0] = Pos.x * xOrientation;
    gObjectList[_headIndex].pos[1] = Pos.y + 5.0 + 3.0;
    gObjectList[_headIndex].pos[2] = Pos.z;

    find_unused_obj_index(&_bodyIndex);
    init_object(_bodyIndex, 0);
    gObjectList[_bodyIndex].origin_pos[0] = Pos.x * xOrientation;
    gObjectList[_bodyIndex].origin_pos[1] = Pos.y + 3.0;
    gObjectList[_bodyIndex].origin_pos[2] = Pos.z;
    gObjectList[_bodyIndex].unk_0D5 = 0; // Section Id no longer used.

    gObjectList[_bodyIndex].pos[0] = Pos.x * xOrientation;
    gObjectList[_bodyIndex].pos[1] = Pos.y + 3.0;
    gObjectList[_bodyIndex].pos[2] = Pos.z;

    _count++;
}

void OSnowman::Tick() {
    s32 var_s3;
    s32 var_s4;
    s32 objectIndex;
    Object* object;

    //! @todo quick hack to add the snow particles on hit. Need to separate into its own class
    if (_idx == 0) {
        for (size_t i = 0; i < gObjectParticle2_SIZE; i++) {
            objectIndex = gObjectParticle2[i];

            if (objectIndex == DELETED_OBJECT_ID) {
                continue;
            }

            if (gObjectList[objectIndex].state == 0) {
                continue;
            }
            func_8008379C(objectIndex);
            if (gObjectList[objectIndex].state != 0) {
                continue;
            }
            delete_object_wrapper(&gObjectParticle2[i]);
        }
    }

    // for (var_s0 = 0; var_s0 < NUM_SNOWMEN; var_s0++) {
    var_s4 = _bodyIndex;
    var_s3 = _headIndex;
    OSnowman::func_80083A94(var_s3); // snowman head
    OSnowman::func_80083C04(var_s4); // snowman body
    if (is_obj_index_flag_status_inactive(var_s4, 0x00001000) != 0) {
        object = &gObjectList[var_s4];
        if ((are_players_in_course_section(object->unk_0D5 - 1, object->unk_0D5 + 1) != 0) &&
            (func_80089B50(var_s4) != 0)) {
            set_object_flag(var_s4, 0x00001000);
            clear_object_flag(var_s4, 0x00000010);
            func_800726CC(var_s4, 0x0000000A);
            func_8008701C(var_s3, 0x0000000A);
            OSnowman::func_800836F0(object->pos);
        }
    } else if (func_80072320(var_s4, 2) != 0) {
        func_800722CC(var_s4, 2);
        func_8008701C(var_s3, 0x00000014);
    }
    //}
}

void OSnowman::Draw(s32 cameraId) {

    OSnowman::DrawHead(cameraId);
    OSnowman::DrawBody(cameraId);
}

void OSnowman::func_800836F0(Vec3f pos) {
    s32 objectIndex;
    s32 i;

    for (i = 0; i < D_8018D3BC; i++) {
        objectIndex = add_unused_obj_index(&gObjectParticle2[0], &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
        OSnowman::func_80083538(objectIndex, pos, i, D_8018D3BC);
    }
}

void OSnowman::DrawHead(s32 cameraId) {
    s32 objectIndex;
    Camera* camera = &camera1[cameraId];
    objectIndex = _bodyIndex;
    if (gObjectList[objectIndex].state >= 2) {
        func_8008A364(objectIndex, cameraId, 0x2AABU, 0x00000258);
        if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {

            // @port: Tag the transform.
            FrameInterpolation_RecordOpenChild("OSnowman::DrawHead", (uintptr_t) &gObjectList[objectIndex]);

            D_80183E80[0] = (s16) gObjectList[objectIndex].orientation[0];
            D_80183E80[1] =
                func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
            D_80183E80[2] = (u16) gObjectList[objectIndex].orientation[2];
            if (is_obj_flag_status_active(objectIndex, 0x00000010) != 0) {
                rsp_set_matrix_transformation(gObjectList[objectIndex].pos, D_80183E80,
                                          gObjectList[objectIndex].sizeScaling);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007D78);
            gDPLoadTLUT_pal256(gDisplayListHead++, gObjectList[objectIndex].activeTLUT);
            rsp_load_texture((u8*) gObjectList[objectIndex].activeTexture, 64, 64);
            gSPVertex(gDisplayListHead++, (uintptr_t) gObjectList[objectIndex].vertex, 4, 0);
            gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
            gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
            }
            objectIndex = _headIndex;
            D_80183E80[0] = (s16) gObjectList[objectIndex].orientation[0];
            D_80183E80[2] = (u16) gObjectList[objectIndex].orientation[2];
            rsp_set_matrix_transformation(gObjectList[objectIndex].pos, D_80183E80,
                                          gObjectList[objectIndex].sizeScaling);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007D78);
            gDPLoadTLUT_pal256(gDisplayListHead++, gObjectList[objectIndex].activeTLUT);
            rsp_load_texture((u8*) gObjectList[objectIndex].activeTexture, 64, 64);
            gSPVertex(gDisplayListHead++, (uintptr_t) gObjectList[objectIndex].vertex, 4, 0);
            gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
            gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);

            // @port Pop the transform id.
            FrameInterpolation_RecordCloseChild();
        }
    }
}

void OSnowman::DrawBody(s32 cameraId) {
    UNUSED s32 stackPadding[2];
    Camera* sp44;
    s32 someIndex;
    s32 objectIndex;
    Object* object;

    sp44 = &camera1[cameraId];
    load_texture_and_tlut((u8*) d_course_frappe_snowland_snow_tlut, (u8*) d_course_frappe_snowland_snow, 0x00000020,
                          0x00000020);

    //! @todo quick hack to add the snow particles on hit. Need to separate into its own class
    if (_idx == 0) {
        for (someIndex = 0; someIndex < gObjectParticle2_SIZE; someIndex++) {
            objectIndex = gObjectParticle2[someIndex];
            if (objectIndex != NULL_OBJECT_ID) {
                object = &gObjectList[objectIndex];
                if (object->state > 0) {
                    func_8008A364(objectIndex, cameraId, 0x2AABU, 0x000001F4);
                    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {

                        // @port: Tag the transform.
                        FrameInterpolation_RecordOpenChild("OSnowman::DrawBody", (uintptr_t) object);

                        object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], sp44->pos);
                        rsp_set_matrix_gObjectList(objectIndex);
                        gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D0069E0);

                        // @port Pop the transform id.
                        FrameInterpolation_RecordCloseChild();
                    }
                }
            }
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void OSnowman::func_80083C04(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 0:
            break;
        case 1:
            OSnowman::func_80083B0C(objectIndex);
            break;
        case 2:
            set_and_run_timer_object(objectIndex, 0x00000096);
            break;
        case 10:
            if (set_and_run_timer_object(objectIndex, 0x0000012C) != 0) {
                func_800722A4(objectIndex, 2);
            }
            break;
        case 11:
            if (set_and_run_timer_object(objectIndex, 0x0000000A) != 0) {
                set_object_flag(objectIndex, 0x00000010);
                object->sizeScaling = 0.001f;
            }
            break;
        case 12:
            if (func_80074118(objectIndex, &object->sizeScaling, 0.001f, 0.1f, 0.0025f, 0, 0) != 0) {
                object_next_state(objectIndex);
            }
            break;
        case 13:
            func_800726CC(objectIndex, 2);
            clear_object_flag(objectIndex, 0x00001000);
            break;
    }
    if (object->state >= 2) {
        func_80073514(objectIndex);
    }
    OSnowman::func_80083BE4(objectIndex);
}

void OSnowman::func_80083BE4(s32 objectIndex) {
    object_calculate_new_pos_offset(objectIndex);
}

Vtx common_vtx_snowman[] = {
    {{{   -32,    -31,    -12}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -31,    -12}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,    -12}, 0, {  4032,   3968}, {255, 255, 255, 255}}},
    {{{   -32,     31,    -12}, 0, {     0,   3968}, {255, 255, 255, 255}}},
    {{{   -32,    -31,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,    -31,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     31,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
};

void OSnowman::func_80083868(s32 objectIndex) {
    Object* object;
    init_texture_object(objectIndex, (u8*) d_course_frappe_snowland_snowman_tlut, (const char**) sSnowmanHeadList,
                        0x40U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->vertex = common_vtx_snowman;
    object->sizeScaling = 0.1f;
    object->textureListIndex = 0;
    object_next_state(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object->orientation[0] = 0;
    object->orientation[1] = 0;
    object->orientation[2] = 0x8000;
    object->primAlpha = random_int(0x2000U) - 0x1000;
    func_80086E70(objectIndex);
    object->unk_034 = 1.5f;
    set_object_flag(objectIndex, 0x00000200);
}

void OSnowman::func_80083948(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            func_80086FD4(objectIndex);
            break;
        case 2:
            func_800871AC(objectIndex, 0x00000014);
            break;
        case 3:
            func_8008701C(objectIndex, 1);
            break;
        case 10:
            func_80087C48(objectIndex, 10.0f, 0.5f, 0x0000000A);
            break;
        case 11:
            func_80087D24(objectIndex, 0.0f, 0.2f, -7.0f);
            break;
        case 20:
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], 0.0f, 0.2f) != 0) {
                func_80073800(objectIndex, 0);
                func_8008701C(objectIndex, 1);
            }
            break;
        case 0:
        default:
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
    OSnowman::func_80073D0C(objectIndex, &gObjectList[objectIndex].primAlpha, -0x00001000, 0x00001000, 0x00000400, 1,
                            -1);
    gObjectList[objectIndex].orientation[2] = gObjectList[objectIndex].primAlpha + 0x8000;
}

bool OSnowman::func_80073D0C(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return func_80073B78(0, objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

void OSnowman::func_80083A94(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OSnowman::func_80083868(objectIndex);
            break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        func_80073514(objectIndex);
    }
    OSnowman::func_80083948(objectIndex);
}

static const char* sSnowmanBodyList[] = { d_course_frappe_snowland_snowman_body };

Vtx common_vtx_snowman2[] = {
    { { { -32, -31, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, -31, 0 }, 0, { 4032, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, 31, 0 }, 0, { 4032, 3968 }, { 255, 255, 255, 255 } } },
    { { { -32, 31, 0 }, 0, { 0, 3968 }, { 255, 255, 255, 255 } } },
};

void OSnowman::func_80083B0C(s32 objectIndex) {
    init_texture_object(objectIndex, (u8*) d_course_frappe_snowland_snowman_tlut, (const char**) sSnowmanBodyList,
                        0x40U, (u16) 0x00000040);
    gObjectList[objectIndex].vertex = common_vtx_snowman2;
    gObjectList[objectIndex].sizeScaling = 0.1f;
    gObjectList[objectIndex].textureListIndex = 0;
    object_next_state(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    gObjectList[objectIndex].orientation[0] = 0;
    gObjectList[objectIndex].orientation[1] = 0;
    gObjectList[objectIndex].orientation[2] = 0x8000;
    gObjectList[objectIndex].boundingBoxSize = 2;
    gObjectList[objectIndex].unk_034 = 1.5f;
    set_object_flag(objectIndex, 0x04000210);
}

void OSnowman::Translate(FVector pos) {
    if ((_objectIndex != -1) && (_bodyIndex != -1)) {
        SpawnPos = pos;

        Object* object = &gObjectList[_objectIndex];

        object->pos[0] = pos.x;
        object->pos[1] = pos.y;
        object->pos[2] = pos.z;
        object->origin_pos[0] = pos.x;
        object->origin_pos[1] = pos.y;
        object->origin_pos[2] = pos.z;

        object = &gObjectList[_bodyIndex];
        object->pos[0] = pos.x;
        object->pos[1] = pos.y - 5.0;
        object->pos[2] = pos.z;
        object->origin_pos[0] = pos.x;
        object->origin_pos[1] = pos.y - 5.0;
        object->origin_pos[2] = pos.z;
    } else {
        printf("Editor tried to translate null OObject\n");
    }
}

void OSnowman::func_80083538(s32 objectIndex, Vec3f arg1, s32 arg2, s32 arg3) {
    Object* object;

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->activeTexture = (const char*) d_course_frappe_snowland_snow;
    object->textureList = (const char**) d_course_frappe_snowland_snow;
    object->activeTLUT = d_course_frappe_snowland_snow_tlut;
    object->tlutList = (u8*) d_course_frappe_snowland_snow_tlut;
    object->sizeScaling = random_int(0x0064U);
    object->sizeScaling = (object->sizeScaling * 0.001) + 0.05;
    object->velocity[1] = random_int(0x0014U);
    object->velocity[1] = (object->velocity[1] * 0.5) + 2.6;
    object->unk_034 = random_int(0x000AU);
    object->unk_034 = (object->unk_034 * 0.1) + 4.5;
    object->direction_angle[1] = (arg2 << 0x10) / arg3;
    object->origin_pos[0] = arg1[0];
    object->origin_pos[1] = arg1[1];
    object->origin_pos[2] = arg1[2];
    object->primAlpha = random_int(0x4000U) + 0x1000;
}

void OSnowman::func_8008379C(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.74f,
                              gObjectList[objectIndex].unk_034, gObjectList[objectIndex].direction_angle[1],
                              0x00000064) != 0) {
                object_next_state(objectIndex);
            }
            break;
        case 2:
            func_80086F60(objectIndex);
            func_80072428(objectIndex);
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
    gObjectList[objectIndex].orientation[2] += gObjectList[objectIndex].primAlpha;
}
