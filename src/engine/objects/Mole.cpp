#include <libultraship.h>
#include <libultra/gbi.h>
#include "Mole.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "defines.h"
#include "camera.h"
#include "update_objects.h"
#include "render_objects.h"
#include "actors.h"
#include "code_80057C60.h"
#include "code_80086E70.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80005FD0.h"
#include "some_data.h"
#include "ceremony_and_credits.h"
#include "assets/moo_moo_farm_data.h"
#include "sounds.h"
#include "external.h"
}

OMole::OMole(Vec3f pos) {

    find_unused_obj_index(&indexObjectList1[0]);
}

void OMole::Tick() {
    s32 var_s1;
    s32 objectIndex;

    for (var_s1 = 0; var_s1 < D_8018D1C8; var_s1++) {
        objectIndex = indexObjectList1[var_s1];
        if (gObjectList[objectIndex].state == 0) {
            if (are_players_in_course_section(8, 9) != 0) {
                func_80081FF4(objectIndex, 1);
            }
        } else {
            OMole::func_800821AC(objectIndex, 1);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D0; var_s1++) {
        objectIndex = indexObjectList2[var_s1];
        if (gObjectList[objectIndex].state == 0) {
            if (are_players_in_course_section(0x0010, 0x0013) != 0) {
                OMole::func_80081FF4(objectIndex, 2);
            }
        } else {
            OMole::func_800821AC(objectIndex, 2);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D8; var_s1++) {
        objectIndex = indexObjectList3[var_s1];
        if (gObjectList[objectIndex].state == 0) {
            if (are_players_in_course_section(0x0011, 0x0014) != 0) {
                func_80081FF4(objectIndex, 3);
            }
        } else {
            OMole::func_800821AC(objectIndex, 3);
        }
    }

    for (var_s1 = 0; var_s1 < gObjectParticle2_SIZE; var_s1++) {
        objectIndex = gObjectParticle2[var_s1];
        if (gObjectList[objectIndex].state != 0) {
            OMole::func_80081790(objectIndex);
        }
    }
}

void OMole::Draw(s32 cameraId) {
}

void OMole::func_80081790(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break; /* irregular */
        case 1:
            if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.3f, gObjectList[objectIndex].unk_034,
                              gObjectList[objectIndex].orientation[1], 0x00000032) != 0) {
                object_next_state(objectIndex);
            }
            object_calculate_new_pos_offset(objectIndex);
            break;
        case 2:
            func_80072428(objectIndex);
            func_80086F60(objectIndex);
            break;
    }
}

void OMole::func_80081AFC(s32 objectIndex, s32 arg1) {
    s8* sp2C;
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 0x1:
            OMole::func_80081848(objectIndex);
            break;
        case 0x2:
            if (object->unk_04C == 0) {
                func_80086EAC(objectIndex, 2, 1);
                object_next_state(objectIndex);
                set_object_flag(objectIndex, 0x00000200);
            } else {
                object->unk_04C--;
            }
            break;
        case 0x3:
            if (object->unk_0AE == 0) {
                func_80086EAC(objectIndex, 2, 4);
                func_8008153C(objectIndex);
                object_next_state(objectIndex);
                func_800C98B8(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x07));
            }
            break;
        case 0x4:
            if (func_80072E54(objectIndex, 1, 6, 1, 2, 0) != 0) {
                func_800726CC(objectIndex, 0x00000064);
            }
            break;
        case 0xA:
            func_80072E54(objectIndex, 1, 6, 1, 0, -1);
            if (object->unk_0AE == 0) {
                func_800726CC(objectIndex, 0x00000064);
            }
            break;
        case 0x64:
            if (object->unk_0AE == 0) {
                clear_object_flag(objectIndex, 0x00000200);
                func_80072428(objectIndex);
                switch (arg1) { /* switch 1; irregular */
                    case 1:     /* switch 1 */
                        sp2C = D_8018D198;
                        break;
                    case 2: /* switch 1 */
                        sp2C = D_8018D1A8;
                        break;
                    case 3: /* switch 1 */
                        sp2C = D_8018D1B8;
                        break;
                }
                sp2C[object->type] = 0;
            }
            break;
        case 0:
        default:
            break;
    }
    if (object->state >= 2) {
        func_80073514(objectIndex);
    }
}

void OMole::func_80081D34(s32 objectIndex) {
    Player* player;
    Camera* var_s4;
    s32 var_s2;
    s32 var_s5;
    Object* object;

    var_s5 = 0;
    player = gPlayerOne;
    var_s4 = camera1;
    for (var_s2 = 0; var_s2 < D_8018D158; var_s2++, player++, var_s4++) {
        if ((is_obj_flag_status_active(objectIndex, 0x00000200) != 0) && !(player->effects & 0x80000000) &&
            (has_collided_with_player(objectIndex, player) != 0)) {
            if ((player->type & 0x8000) && !(player->type & 0x100)) {
                var_s5 = 1;
                object = &gObjectList[objectIndex];
                if (is_obj_flag_status_active(objectIndex, 0x04000000) != 0) {
                    func_80072180();
                }
                if (player->effects & 0x200) {
                    func_800C9060(var_s2, 0x1900A046U);
                } else {
                    player->soundEffects |= 2;
                }
                object->direction_angle[1] = var_s4->rot[1];
                object->velocity[1] = (player->unk_094 / 2) + 3.0;
                object->unk_034 = player->unk_094 + 1.0;
                if (object->velocity[1] >= 5.0) {
                    object->velocity[1] = 5.0f;
                }
                if (object->unk_034 >= 4.0) {
                    object->velocity[1] = 4.0f;
                }
            }
        }
    }
    if (var_s5 != 0) {
        object = &gObjectList[objectIndex];
        clear_object_flag(objectIndex, 0x00000200);
        func_80086F60(objectIndex);
        set_obj_origin_pos(objectIndex, object->pos[0], object->pos[1], object->pos[2]);
        set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
        func_80086EAC(objectIndex, 2, 0x000A);
        func_800726CC(objectIndex, 0x0000000A);
    }
}

void OMole::func_80081FF4(s32 objectIndex, s32 arg1) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 moleCount;
    s16 var_v1;
    s16 offset;
    s32 var_a0;
    s8* var_a2;

    init_object(objectIndex, 0);
    gObjectList[objectIndex].unk_04C = random_int(0x001EU) + 5;
    switch (arg1) { /* irregular */
        case 1:
            var_a2 = D_8018D198;
            moleCount = NUM_GROUP1_MOLES;
            offset = 0;
            break;
        case 2:
            var_a2 = D_8018D1A8;
            moleCount = NUM_GROUP2_MOLES;
            offset = 24;
            // offset = NUM_GROUP1_MOLES;
            break;
        case 3:
            var_a2 = D_8018D1B8;
            moleCount = NUM_GROUP3_MOLES;
            offset = 57;
            // offset = NUM_GROUP1_MOLES + NUM_GROUP2_MOLES;
            break;
    }
    var_v1 = random_int(moleCount);
    for (var_a0 = 0; var_a0 < moleCount; var_a0++) {
        if (var_a2[var_v1] != 0) {
            var_v1++;
            if (var_v1 == moleCount) {
                var_v1 = 0;
            }
        } else {
            var_a2[var_v1] = 1;
            gObjectList[objectIndex].type = var_v1;
            break;
        }
    }
    /*
    Ideally `gMoleSpawns` wouldn't be a union at all and its just be a list of Vec3s
    Even more ideally each mole group would have its own array for its spawns
    gObjectList[objectIndex].origin_pos[0] = gMoleSpawns.asVec3sList[offset + var_v1][0] * xOrientation;
    gObjectList[objectIndex].origin_pos[1] = gMoleSpawns.asVec3sList[offset + var_v1][1] - 9.0;
    gObjectList[objectIndex].origin_pos[2] = gMoleSpawns.asVec3sList[offset + var_v1][2];
    */
    gObjectList[objectIndex].origin_pos[0] = gMoleSpawns.asFlatList[offset + (var_v1 * 3) + 0] * xOrientation;
    gObjectList[objectIndex].origin_pos[1] = gMoleSpawns.asFlatList[offset + (var_v1 * 3) + 1] - 9.0;
    gObjectList[objectIndex].origin_pos[2] = gMoleSpawns.asFlatList[offset + (var_v1 * 3) + 2];
}

void OMole::func_80081848(s32 objectIndex) {
    u8* mole = (u8*) LOAD_ASSET_RAW(d_course_moo_moo_farm_mole_frames);
    u8* tlut = (u8*) LOAD_ASSET_RAW(d_course_moo_moo_farm_mole_tlut);
    init_texture_object(objectIndex, (u8*) d_course_moo_moo_farm_mole_tlut, (const char**) mole, 0x20U,
                        (u16) 0x00000040);
    gObjectList[objectIndex].sizeScaling = 0.15f;
    gObjectList[objectIndex].textureListIndex = 0;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    gObjectList[objectIndex].boundingBoxSize = 6;
    gObjectList[objectIndex].velocity[1] = 4.0f;
    set_object_flag(objectIndex, 0x04000000);
    object_next_state(objectIndex);
}

void OMole::func_80081924(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], 9.0f, 0.7f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            func_800871AC(objectIndex, 0x0000000A);
            break;
        case 3:
            if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 3.0f, 1.0f) != 0) {
                func_80086F60(objectIndex);
            }
            break;
        case 4:
            func_80087D24(objectIndex, 3.6f, 0.25f, 0.0f);
            break;
        case 5:
            func_80086F60(objectIndex);
            break;
        case 10:
            gObjectList[objectIndex].orientation[2] += 0x1000;
            gObjectList[objectIndex].velocity[1] -= 0.184;
            func_8008751C(objectIndex);
            object_add_velocity_offset_xyz(objectIndex);
            if (gObjectList[objectIndex].pos[1] <= -10.0) {
                func_80086F60(objectIndex);
            }
            break;
        case 0:
        default:
            break;
    }
}

void OMole::func_80081A88(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0DD) { /* irregular */
        case 0:
            break;
        case 1:
            func_8008B724(objectIndex);
            break;
        case 2:
            OMole::func_80081924(objectIndex);
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void OMole::func_800821AC(s32 objectIndex, s32 arg1) {
    if (gObjectList[objectIndex].state != 0) {
        OMole::func_80081AFC(objectIndex, arg1);
        OMole::func_80081A88(objectIndex);
        OMole::func_80081D34(objectIndex);
    }
}

void OMole::func_80054E10(s32 objectIndex) {
    if (gObjectList[objectIndex].state > 0) {
        if (is_obj_flag_status_active(objectIndex, 0x00800000) != 0) {
            D_80183E50[0] = gObjectList[objectIndex].pos[0];
            D_80183E50[1] = gObjectList[objectIndex].surfaceHeight + 0.8;
            D_80183E50[2] = gObjectList[objectIndex].pos[2];
            D_80183E70[0] = gObjectList[objectIndex].velocity[0];
            D_80183E70[1] = gObjectList[objectIndex].velocity[1];
            D_80183E70[2] = gObjectList[objectIndex].velocity[2];
            func_8004A9B8(gObjectList[objectIndex].sizeScaling);
        }
    }
}

// Almost certainly responsible for spawning/handling the moles on Moo Moo farm
void OMole::func_80054EB8() {
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_TOTAL_MOLES; someIndex++) {
        func_80054E10(gObjectParticle1[someIndex]);
    }
}

void OMole::func_80054D00(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    camera = &camera1[cameraId];
    if (gObjectList[objectIndex].state >= 3) {
        func_8008A364(objectIndex, cameraId, 0x2AABU, 0x0000012C);
        if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
            D_80183E80[0] = (s16) gObjectList[objectIndex].orientation[0];
            D_80183E80[1] =
                func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
            D_80183E80[2] = (u16) gObjectList[objectIndex].orientation[2];
            func_80048130(gObjectList[objectIndex].pos, (u16*) D_80183E80, gObjectList[objectIndex].sizeScaling,
                          (u8*) gObjectList[objectIndex].activeTLUT, (u8*) gObjectList[objectIndex].activeTexture,
                          (Vtx*) LOAD_ASSET_RAW(D_0D0062B0), 0x00000020, 0x00000040, 0x00000020, 0x00000040, 5);
        }
    }
}

void OMole::func_80054F04(s32 cameraId) {
    s32 var_s2;
    s32 objectIndex;
    Camera* sp44;
    Object* object;

    sp44 = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D0079C8);
    load_texture_block_rgba16_mirror((u8*) LOAD_ASSET_RAW(d_course_moo_moo_farm_mole_dirt), 0x00000010, 0x00000010);
    for (var_s2 = 0; var_s2 < gObjectParticle2_SIZE; var_s2++) {
        objectIndex = gObjectParticle2[var_s2];
        object = &gObjectList[objectIndex];
        if (object->state > 0) {
            func_8008A364(objectIndex, cameraId, 0x2AABU, 0x000000C8);
            if ((is_obj_flag_status_active(objectIndex, VISIBLE) != 0) && (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX)) {
                object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], sp44->pos);
                rsp_set_matrix_gObjectList(objectIndex);
                gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D006980);
            }
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void OMole::render_object_moles(s32 cameraId) {
    s32 i;

    for (i = 0; i < NUM_GROUP1_MOLES; i++) {
        OMole::func_80054D00(indexObjectList1[i], cameraId);
    }
    for (i = 0; i < NUM_GROUP2_MOLES; i++) {
        OMole::func_80054D00(indexObjectList2[i], cameraId);
    }
    for (i = 0; i < NUM_GROUP3_MOLES; i++) {
        OMole::func_80054D00(indexObjectList3[i], cameraId);
    }
    OMole::func_80054EB8();
    OMole::func_80054F04(cameraId);
}
