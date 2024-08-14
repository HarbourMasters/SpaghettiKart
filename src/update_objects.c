#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <decode.h>
#include <mk64.h>

#include "update_objects.h"
#include "main.h"
#include "memory.h"
#include "camera.h"
#include "math_util.h"
#include "math_util_2.h"
#include "render_objects.h"
#include "objects.h"
#include "waypoints.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "code_80086E70.h"
#include <assets/common_data.h>
#include "audio/external.h"
#include "sounds.h"
#include <actors.h>
#include "race_logic.h"
#include "collision.h"
#include "effects.h"
#include "code_80091440.h"
#include "code_80091750.h"
#include "podium_ceremony_actors.h"
#include "courses/all_course_data.h"
#include <assets/ceremony_data.h>
#include "src/ending/ceremony_and_credits.h"
#include "menus.h"
#include <assets/other_textures.h>
#include "data/some_data.h"
#include "memory.h"
#include <assets/luigi_raceway_data.h>
#include <assets/moo_moo_farm_data.h>
#include <assets/bowsers_castle_data.h>
#include <assets/frappe_snowland_data.h>
#include <assets/boo_frames.h>

//! @todo unused?
f32 D_800E43B0[] = {
    65536.0, 0.0,     1.0, 0.0,
    0.0,     65536.0, 0.0, 1.0,
    0.0,     0.0,     0.0, 0.0,
    0.0,     0.0,     0.0, 0.0
};

Vtx D_800E43F0[] = {
    {{{  -24,  -19,    0}, 0, {    0,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,  -19,    0}, 0, { 3008,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,   20,    0}, 0, { 3008, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{  -24,   20,    0}, 0, {    0, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{  -24,  -19,    0}, 0, { 3008,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,  -19,    0}, 0, {    0,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,   20,    0}, 0, {    0, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{  -24,   20,    0}, 0, { 3008, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
};

Vtx D_800E4470[] = {
    {{{  -24,  -19,    0}, 0, {    0,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,  -19,    0}, 0, { 3008,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,   20,    0}, 0, { 3008, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{  -24,   20,    0}, 0, {    0, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
};

Vtx D_800E44B0[] = {
    {{{  -24,  -19,    0}, 0, { 3008,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,  -19,    0}, 0, {    0,    0}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{   23,   20,    0}, 0, {    0, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
    {{{  -24,   20,    0}, 0, { 3008, 2496}, { 0xFF, 0xFF, 0xFF, 0xFF}}},
};

u16 *gItemWindowTLUTs[] = {
    common_tlut_item_window_none, common_tlut_item_window_banana, common_tlut_item_window_banana_bunch, common_tlut_item_window_green_shell,
    common_tlut_item_window_triple_green_shell, common_tlut_item_window_red_shell, common_tlut_item_window_triple_red_shell, common_tlut_item_window_blue_shell,
    common_tlut_item_window_thunder_bolt, common_tlut_item_window_fake_item_box, common_tlut_item_window_star, common_tlut_item_window_boo,
    common_tlut_item_window_mushroom, common_tlut_item_window_double_mushroom, common_tlut_item_window_triple_mushroom, common_tlut_item_window_super_mushroom
};

u8 *gItemWindowTextures[] = {
    common_texture_item_window_none, common_texture_item_window_banana, common_texture_item_window_banana_bunch, common_texture_item_window_green_shell,
    common_texture_item_window_triple_green_shell, common_texture_item_window_red_shell, common_texture_item_window_triple_red_shell, common_texture_item_window_blue_shell,
    common_texture_item_window_thunder_bolt, common_texture_item_window_fake_item_box, common_texture_item_window_star, common_texture_item_window_boo,
    common_texture_item_window_mushroom, common_texture_item_window_double_mushroom, common_texture_item_window_triple_mushroom, common_texture_item_window_super_mushroom
};

u16 *gHudLapTextures[] = {
    common_texture_hud_lap_1_on_3, common_texture_hud_lap_2_on_3, common_texture_hud_lap_3_on_3
};

u16 *gPortraitTLUTs[] = {
    common_tlut_portrait_mario, common_tlut_portrait_luigi, common_tlut_portrait_yoshi, common_tlut_portrait_toad,
    common_tlut_portrait_donkey_kong, common_tlut_portrait_wario, common_tlut_portrait_peach, common_tlut_portrait_bowser
};

u8 *gPortraitTextures[] = {
    common_texture_portrait_mario, common_texture_portrait_luigi, common_texture_portrait_yoshi, common_texture_portrait_toad,
    common_texture_portrait_donkey_kong, common_texture_portrait_wario, common_texture_portrait_peach, common_texture_portrait_bowser
};

s32 find_unused_obj_index(s32* arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = objectListSize;
    temp_v0 = 0;
    do {
        ++temp_v1;
        ++temp_v0;

        if (temp_v1 == OBJECT_LIST_SIZE) {
            temp_v1 = 0;
        }
    } while ((gObjectList[temp_v1].unk_0CA != 0) && (temp_v0 != OBJECT_LIST_SIZE));

    gObjectList[temp_v1].unk_0CA = 1;

    *arg0 = temp_v1;
    objectListSize = temp_v1;
    return temp_v1;
}

//! @warning Does not clear struct members.
void delete_object(s32 *objectIndex) {
    func_80072428(*objectIndex);
    gObjectList[*objectIndex].unk_0CA = 0;
    *objectIndex = NULL_OBJECT_ID;
}

s32 func_80071FBC(void) {
    s32 objectIndex;
    s32 someCount = 0;
    for (objectIndex = 0; objectIndex < OBJECT_LIST_SIZE; objectIndex++) {
        if (gObjectList[objectIndex].unk_0CA != 0) {
            someCount++;
        }
    }
    return someCount;
}

s32 add_unused_obj_index(s32 *listIdx, s32 *nextFree, s32 size) {
    s32 count;
    s32 objectIndex;
    s32 *id;

    if (*nextFree >= size) {
        *nextFree = 0;
    }
    count = 0;
    id = &listIdx[*nextFree];
    /**
     * @todo This HAS to be a for-loop of some variety, but I can't make a for-loop to match.
     * If you replace this with ```for(var_v1 = 0; var_v1 < size; var_v1++)```
     * The diff gets massive.
    */
    if (size > 0) {
loop_3:
        if (*id == NULL_OBJECT_ID) {
            objectIndex = find_unused_obj_index(id);
            *nextFree += 1;
        } else {
            *nextFree += 1;
            if (*nextFree >= size) {
                *nextFree = 0;
            }
            count += 1;
            id = &listIdx[*nextFree];
            if (count != size) { // check if don't check all element of the list
                goto loop_3;
            }
        }
    }
    if (count == size) {
        objectIndex = NULL_OBJECT_ID;
    }
    return objectIndex;
}

void delete_object_wrapper(s32 *arg0) {
    delete_object(arg0);
}

void func_80072120(s32 *arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < arg1; i++) {
            set_object_flag_status_false(*arg0, 0x00600000);
            arg0++;
    }
}

void func_80072180(void) {
    if (gModeSelection == TIME_TRIALS) {
        if (((gPlayerOne->type & PLAYER_EXISTS) != 0) &&
        ((gPlayerOne->type & (PLAYER_INVISIBLE_OR_BOMB | PLAYER_KART_AI)) == 0)) {
            D_80162DF8 = 1;
        }
    }
}

void set_object_flag_status_true(s32 objectIndex, s32 flag) {
    gObjectList[objectIndex].status |= flag;
}

void set_object_flag_status_false(s32 objectIndex, s32 flag) {
    gObjectList[objectIndex].status &= ~flag;
}

UNUSED void func_80072214(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].status ^= arg1;
}

bool is_obj_flag_status_active(s32 objectIndex, s32 arg1) {
    s32 phi_v1 = false;
    if ((gObjectList[objectIndex].status & arg1) != 0) {
        phi_v1 = true;
    }
    return phi_v1;
}

s32 is_obj_index_flag_status_inactive(s32 objectIndex, s32 arg1) {
    s32 phi_v1 = 0;
    if ((gObjectList[objectIndex].status & arg1) == 0) {
        phi_v1 = 1;
    }
    return phi_v1;
}

void func_800722A4(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_058 |= arg1;
}

void func_800722CC(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_058 &= ~arg1;
}

UNUSED void func_800722F8(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_058 ^= arg1;
}

bool func_80072320(s32 objectIndex, s32 arg1) {
    s32 b = false;
    if ((gObjectList[objectIndex].unk_058 & arg1) != 0) {
        b = true;
    }
    return b;
}

bool func_80072354(s32 objectIndex, s32 arg1) {
    s32 b = false;
    if ((gObjectList[objectIndex].unk_058 & arg1) == 0) {
        b = true;
    }
    return b;
}

void set_object_unk_0CB(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_0CB = arg1;
}

void init_object(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].status = 0;
    gObjectList[objectIndex].unk_058 = 0;
    gObjectList[objectIndex].unk_05C = 0;
    gObjectList[objectIndex].unk_0CD = 0;
    gObjectList[objectIndex].unk_0CF = 0;
    set_object_unk_0CB(objectIndex, 0);
    gObjectList[objectIndex].unk_0D8 = arg1;
    gObjectList[objectIndex].state = 1;
}

UNUSED void func_80072408(s32 objectIndex) {
    set_object_unk_0CB(objectIndex, 0);
}

void func_80072428(s32 objectIndex) {
    gObjectList[objectIndex].state = 0;
    gObjectList[objectIndex].unk_0D8 = 0;
    set_object_unk_0CB(objectIndex, 0);
    gObjectList[objectIndex].status = 0;
    gObjectList[objectIndex].unk_058 = 0;
    gObjectList[objectIndex].unk_05C = 0;
    func_80086F60(objectIndex);
}

void func_80072488(s32 objectIndex) {
    set_object_unk_0CB(objectIndex,  0);
    set_object_flag_status_false(objectIndex, 0x2000);
    gObjectList[objectIndex].state++;
}

void func_800724DC(s32 objectIndex) {
    gObjectList[objectIndex].unk_0D7 = 0;
}

void func_800724F8(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_084[gObjectList[objectIndex].unk_0D7] = arg1;
    gObjectList[objectIndex].unk_0D7++;
}

s16 func_80072530(s32 objectIndex) {
    gObjectList[objectIndex].unk_0D7--;
    return gObjectList[objectIndex].unk_084[gObjectList[objectIndex].unk_0D7];
}

void func_80072568(s32 objectIndex, s32 arg1) {
    set_object_unk_0CB(objectIndex, 0);
    set_object_flag_status_false(objectIndex, 0x2000);
    set_object_flag_status_false(objectIndex, 8);
    func_80072488(objectIndex);
    func_800724F8(objectIndex, gObjectList[objectIndex].state);
    gObjectList[objectIndex].state = arg1;
}

void func_800725E8(s32 objectIndex, s32 arg1, s32 arg2) {
    set_object_unk_0CB(objectIndex, 0);
    set_object_flag_status_false(objectIndex, 0x2000);
    set_object_flag_status_false(objectIndex, 8);
    gObjectList[objectIndex].state = arg2;
    func_800724F8(objectIndex, gObjectList[objectIndex].state);
    gObjectList[objectIndex].state =  arg1;
}

s16 func_80072530(s32); // extern

void func_8007266C(s32 objectIndex) {
    set_object_unk_0CB(objectIndex, 0);
    set_object_flag_status_false(objectIndex, 0x2000);
    set_object_flag_status_false(objectIndex, 8);
    gObjectList[objectIndex].state = func_80072530(objectIndex);
}

void func_800726CC(s32 objectIndex, s32 arg1) {
    set_object_unk_0CB(objectIndex, 0);
    set_object_flag_status_false(objectIndex, 0x2000);
    gObjectList[objectIndex].state = arg1;
}

UNUSED void func_8007271C(s32 objectIndex, s32 arg1) {
    if (gObjectList[objectIndex].unk_0CB == 0) {
        gObjectList[objectIndex].state = arg1;
    }
}

UNUSED void func_8007274C(s32 objectIndex) {
    if (gObjectList[objectIndex].unk_0CB == 0) {
        func_80072488(objectIndex);
    }
}

s32 func_8007278C(s32 objectIndex, s32 arg1) {
    s32 phi_v1;

    phi_v1 = 0;
    if (gObjectList[objectIndex].unk_0CB == 0) {
        set_object_unk_0CB(objectIndex, 1);
        gObjectList[objectIndex].unk_050 = arg1;
    }

    gObjectList[objectIndex].unk_050--;
    if (gObjectList[objectIndex].unk_050 < 0) {
        set_object_unk_0CB(objectIndex, 0);
        func_80072488(objectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

UNUSED s32 func_8007281C(s32 objectIndex, s32 arg1) {
    s32 phi_a2;

    phi_a2 = 0;
    if (gObjectList[objectIndex].unk_0CB == 0) {
        set_object_unk_0CB(objectIndex, 1);
        gObjectList[objectIndex].unk_050 = random_int((u16) arg1);
    }

    gObjectList[objectIndex].unk_050--;
    if (gObjectList[objectIndex].unk_050 < 0) {
        set_object_unk_0CB(objectIndex, 0);
        func_80072488(objectIndex);
        phi_a2 = 1;
    }

    return phi_a2;
}

// arg1 or arg2 may be a uintptr_t
UNUSED s32 func_800728B0(s32 objectIndex, s32 arg1, s32 arg2) {
    s32 phi_a3;

    phi_a3 = 0;
    if (gObjectList[objectIndex].unk_0CB == 0) {
        set_object_unk_0CB(objectIndex, 1);
        gObjectList[objectIndex].unk_050 = random_int((u16) arg2) + arg1;
    }

    gObjectList[objectIndex].unk_050--;
    if (gObjectList[objectIndex].unk_050 < 0) {
        set_object_unk_0CB(objectIndex, 0);
        func_80072488(objectIndex);
        phi_a3 = 1;
    }

    return phi_a3;
}

void func_80072950(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 == arg2) {
        gObjectList[objectIndex].state = arg3;
    }
}

UNUSED void func_80072974(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 != arg2) {
        gObjectList[objectIndex].state = arg3;
    }
}

UNUSED void func_80072998(s32 objectIndex) {
        gObjectList[objectIndex].unk_0CC = 0;
}

void func_800729B4(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_0A2 = arg1;
}

void set_type_object(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].type = arg1;
}

void func_800729EC(s32 objectIndex) {
    u32 temp_v1 = 1;
    s32 i;

    start_race();
    func_80072488(objectIndex);
    D_8018D2BC = 1;
    D_8018D2A4 = 1;

    if (gCurrentCourseId != COURSE_YOSHI_VALLEY) {
        for (i = 0; i < gPlayerCount; i++) {
                playerHUD[i].unk_81 = temp_v1;
        }
    }
    func_8005AB20();
}

UNUSED void func_80072A78(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].itemDisplay = arg1;
    func_80072488(objectIndex);
}

s32 func_80072AAC(s32 objectIndex, s32 arg1, s32 arg2) {
    s32 phi_v1;

    phi_v1 = 0;
    if (gObjectList[objectIndex].unk_0CB == 0) {

        set_object_unk_0CB(objectIndex, 1);
        gObjectList[objectIndex].itemDisplay = arg1;
        gObjectList[objectIndex].unk_050 = arg2;
    }

    gObjectList[objectIndex].unk_050--;
    if (gObjectList[objectIndex].unk_050 < 0) {
        set_object_unk_0CB(objectIndex, 0);
        func_80072488(objectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

s32 func_80072B48(s32 objectIndex, s32 arg1) {
    s32 phi_v1;

    phi_v1 = 0;
    if (gObjectList[objectIndex].unk_0CB == 0) {
        set_object_unk_0CB(objectIndex, 1);
        set_object_flag_status_true(objectIndex, 0x80000);
        gObjectList[objectIndex].itemDisplay = D_8018D140;
        gObjectList[objectIndex].unk_050 = arg1;
    }

    gObjectList[objectIndex].unk_050--;
    if (gObjectList[objectIndex].unk_050 < 0) {
        set_object_flag_status_false(objectIndex, 0x80000);
        set_object_unk_0CB(objectIndex, 0);
        func_80072488(objectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

void func_80072C00(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3) {
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].unk_050 = arg2;
        gObjectList[objectIndex].itemDisplay = arg1;
        gObjectList[objectIndex].unk_0D4 = 1;
        gObjectList[objectIndex].unk_0CC = arg3;
        set_object_flag_status_true(objectIndex, 0x2000);
    } else {
        gObjectList[objectIndex].unk_050--;
        if (gObjectList[objectIndex].unk_050 < 0) {
            gObjectList[objectIndex].unk_050 = arg2;
            gObjectList[objectIndex].unk_0D4--;
            if ((gObjectList[objectIndex].unk_0D4 & 1) != 0) {
                set_object_flag_status_false(objectIndex, 0x80000);
            } else {
                set_object_flag_status_true(objectIndex, 0x80000);
            }

            if (gObjectList[objectIndex].unk_0D4 < 0) {
                gObjectList[objectIndex].unk_0D4 = 1;

                if (gObjectList[objectIndex].unk_0CC > 0) {
                    gObjectList[objectIndex].unk_0CC--;
                }

                if (gObjectList[objectIndex].unk_0CC == 0) {
                    set_object_flag_status_false(objectIndex, 0x2000);
                    func_80072488(objectIndex);
                }
            }
        }
    }
}

void func_80072D3C(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].unk_050 = arg3;
        gObjectList[objectIndex].itemDisplay = arg1;
        gObjectList[objectIndex].unk_0D4 = 1;
        gObjectList[objectIndex].unk_0CC = arg4;
        set_object_flag_status_true(objectIndex, 0x2000);
        return;
    }

    gObjectList[objectIndex].unk_050--;
    if (gObjectList[objectIndex].unk_050 < 0) {
        gObjectList[objectIndex].unk_050 = arg3;
        gObjectList[objectIndex].unk_0D4--;
        if ((gObjectList[objectIndex].unk_0D4 & 1) != 0) {
            gObjectList[objectIndex].itemDisplay = arg1;
        } else {
            gObjectList[objectIndex].itemDisplay = arg2;
        }

        if (gObjectList[objectIndex].unk_0D4 < 0) {
            gObjectList[objectIndex].unk_0D4 = 1;
            if (gObjectList[objectIndex].unk_0CC > 0) {
                gObjectList[objectIndex].unk_0CC--;
            }

            if (gObjectList[objectIndex].unk_0CC == 0) {
                set_object_flag_status_false(objectIndex, 0x2000);
                func_80072488(objectIndex);
            }
        }
    }
}

s32 func_80072E54(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = 0;
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].itemDisplay = arg1;
        gObjectList[objectIndex].unk_050 = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_unk_0CB(objectIndex, 1);
        set_object_flag_status_true(objectIndex, 0x2000);
    } else {
        gObjectList[objectIndex].unk_050--;
        if (gObjectList[objectIndex].unk_050 <= 0) {
            gObjectList[objectIndex].unk_050 = arg4;
            gObjectList[objectIndex].itemDisplay += arg3;
            if (arg2 < gObjectList[objectIndex].itemDisplay) {

                if (gObjectList[objectIndex].unk_0CC > 0) {
                    gObjectList[objectIndex].unk_0CC--;
                }
                if (gObjectList[objectIndex].unk_0CC == 0) {
                    gObjectList[objectIndex].itemDisplay = arg2;
                    set_object_flag_status_false(objectIndex, 0x2000);
                    set_object_unk_0CB(objectIndex, 0);
                    func_80072488(objectIndex);
                    sp24 = 1;
                } else {
                    gObjectList[objectIndex].itemDisplay = arg1;
                }
            }
        }
    }
    return sp24;
}

bool func_80072F88(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = false;
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].itemDisplay = arg1;
        gObjectList[objectIndex].unk_050 = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_unk_0CB(objectIndex, 1);
        set_object_flag_status_true(objectIndex, 0x2000);
    } else {
        gObjectList[objectIndex].unk_050--;
        if (gObjectList[objectIndex].unk_050 <= 0) {
            gObjectList[objectIndex].unk_050 = arg4;
            gObjectList[objectIndex].itemDisplay -= arg3;
            if (gObjectList[objectIndex].itemDisplay < arg2) {
                if (gObjectList[objectIndex].unk_0CC > 0) {
                    gObjectList[objectIndex].unk_0CC--;
                }
                if (gObjectList[objectIndex].unk_0CC == 0) {
                    gObjectList[objectIndex].itemDisplay = arg2;
                    set_object_flag_status_false(objectIndex, 0x2000);
                    set_object_unk_0CB(objectIndex, 0);
                    func_80072488(objectIndex);
                    sp24 = true;
                } else {
                    gObjectList[objectIndex].itemDisplay = arg1;
                }
            }
        }
    }
    return sp24;
}

bool func_800730BC(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = false;
    if (is_obj_index_flag_status_inactive(objectIndex ,0x2000) != 0) {
        gObjectList[objectIndex].itemDisplay = arg1;
        gObjectList[objectIndex].unk_050 = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_unk_0CB(objectIndex, 1);
        set_object_flag_status_true(objectIndex, 0x2000);
        set_object_flag_status_false(objectIndex, 0x4000);
    } else {
        gObjectList[objectIndex].unk_050--;
        if (gObjectList[objectIndex].unk_050 <= 0) {
            gObjectList[objectIndex].unk_050 = arg4;
            if (is_obj_index_flag_status_inactive(objectIndex, 0x4000) != 0) {
                gObjectList[objectIndex].itemDisplay += arg3;
                if (gObjectList[objectIndex].itemDisplay >= arg2) {
                    gObjectList[objectIndex].itemDisplay = arg2;
                    set_object_flag_status_true(objectIndex, 0x4000);
                }
            } else {
                gObjectList[objectIndex].itemDisplay -= arg3;
                if (arg1 >= gObjectList[objectIndex].itemDisplay) {
                    gObjectList[objectIndex].itemDisplay = arg1;
                    if (gObjectList[objectIndex].unk_0CC > 0) {
                        gObjectList[objectIndex].unk_0CC--;
                    }

                    if (gObjectList[objectIndex].unk_0CC == 0) {
                        set_object_flag_status_false(objectIndex, 0x80);
                        set_object_flag_status_false(objectIndex, 0x2000);
                        set_object_unk_0CB(objectIndex, 0);
                        func_80072488(objectIndex);
                        sp24 = true;
                    } else {
                        set_object_flag_status_false(objectIndex, 0x4000);
                        set_object_flag_status_true(objectIndex, 0x80);
                    }
                }
            }
        }
    }
    return sp24;
}

s32 func_8007326C(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = 0;
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].itemDisplay = arg1;
        gObjectList[objectIndex].unk_050 = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_unk_0CB(objectIndex, 1);
        set_object_flag_status_true(objectIndex, 0x2000);
        set_object_flag_status_false(objectIndex, 0x4000);
    } else {
        gObjectList[objectIndex].unk_050--;
        if (gObjectList[objectIndex].unk_050 <= 0) {
            gObjectList[objectIndex].unk_050 = arg4;
            if (is_obj_index_flag_status_inactive(objectIndex, 0x4000) != 0) {
                gObjectList[objectIndex].itemDisplay -= arg3;
                if (arg2 >= gObjectList[objectIndex].itemDisplay) {
                    gObjectList[objectIndex].itemDisplay = arg2;
                    set_object_flag_status_true(objectIndex, 0x4000);
                }
            } else {
                gObjectList[objectIndex].itemDisplay += arg3;
                if (gObjectList[objectIndex].itemDisplay >= arg1) {
                    gObjectList[objectIndex].itemDisplay = arg1;
                    if (gObjectList[objectIndex].unk_0CC > 0) {
                        gObjectList[objectIndex].unk_0CC--;
                    }
                    if (gObjectList[objectIndex].unk_0CC == 0) {
                        set_object_flag_status_false(objectIndex, 0x2000);
                        set_object_unk_0CB(objectIndex, 0);
                        func_80072488(objectIndex);
                        sp24 = 1;
                    } else {
                        set_object_flag_status_false(objectIndex, 0x4000);
                    }
                }
            }
        }
    }
    return sp24;
}

void func_80073404(s32 objectIndex, u8 arg1, u8 arg2, Vtx *arg3) {
  gObjectList[objectIndex].vertex = arg3;
  gObjectList[objectIndex].textureWidth = arg1;
  gObjectList[objectIndex].textureHeight = arg2;
  gObjectList[objectIndex].itemDisplay = 0;
  gObjectList[objectIndex].unk_0D3 = -1;
  gObjectList[objectIndex].status = 0;
}

void init_texture_object(s32 objectIndex, u8 *texture, const char **arg2, u8 arg3, u16 arg4) {
    gObjectList[objectIndex].tlutList = texture;
    gObjectList[objectIndex].textureList = arg2;
    gObjectList[objectIndex].textureWidth = arg3;
    gObjectList[objectIndex].textureHeight = arg4;
    gObjectList[objectIndex].itemDisplay = 0;
    gObjectList[objectIndex].unk_0D3 = -1;
    gObjectList[objectIndex].status = 0;
}

UNUSED void func_8007348C(s32 objectIndex, u8 *arg1, u8 arg2, u8 arg3, Vtx *arg4) {
    gObjectList[objectIndex].activeTLUT = arg1;
    gObjectList[objectIndex].tlutList = arg1;
    gObjectList[objectIndex].textureWidth = arg2;
    gObjectList[objectIndex].textureHeight = arg3;
    gObjectList[objectIndex].vertex = arg4;
    gObjectList[objectIndex].itemDisplay = 0;
    gObjectList[objectIndex].unk_0D3 = -1;
    gObjectList[objectIndex].status = 0;
}

UNUSED void func_800734D4() {
}

void update_neon_texture(s32 objectIndex) {
    // I have no idea why this typecast works
    gObjectList[objectIndex].activeTLUT = (u8*)((u32*)gObjectList[objectIndex].tlutList + (gObjectList[objectIndex].itemDisplay * 128));
    gObjectList[objectIndex].activeTexture = gObjectList[objectIndex].textureList;
}

void func_80073514(s32 objectIndex) {
    gObjectList[objectIndex].activeTLUT = gObjectList[objectIndex].tlutList;
    gObjectList[objectIndex].activeTexture = gObjectList[objectIndex].textureList[gObjectList[objectIndex].itemDisplay];
}

UNUSED void func_80073568() {
}

UNUSED void func_80073570(s32 objectIndex) {
    s16 *test = &gObjectList[objectIndex].state;

    D_8018D1EC++;
    if (D_8018D1EC == 5) {
        D_8018D1EC = 0;
    }
    (*test)++;
}

void func_800735BC(s32 objectIndex, Gfx *arg1, f32 arg2) {
    gObjectList[objectIndex].status = 0;
    gObjectList[objectIndex].model = arg1;
    gObjectList[objectIndex].sizeScaling = arg2;
    func_80072488(objectIndex);
}

void func_80073600(s32 objectIndex) {
    set_object_unk_0CB(objectIndex, 0);
    gObjectList[objectIndex].unk_0D6 = 0;
    gObjectList[objectIndex].unk_04C = -1;
    set_object_flag_status_false(objectIndex, 0x8000);
}

void func_80073654(s32 objectIndex) {
    set_object_flag_status_false(objectIndex, 0x8000);
    ++gObjectList[objectIndex].unk_0D6;
}

UNUSED void func_8007369C(s32 objectIndex, s32 arg1) {
    set_object_flag_status_false(objectIndex, 0x8000);
    gObjectList[objectIndex].unk_0D6 = arg1;
}

void func_800736E0(s32 objectIndex) {
    set_object_flag_status_false(objectIndex, 0x8000);
    gObjectList[objectIndex].unk_0D6 = 1;
}

void func_80073720(s32 objectIndex) {
    set_object_flag_status_false(objectIndex, 0x8000);
    gObjectList[objectIndex].unk_0D6 = 0;
}

bool func_8007375C(s32 objectIndex, s32 arg1) {
    s32 sp24;

    sp24 = false;
    if (is_obj_index_flag_status_inactive(objectIndex, 0x00008000) != 0) {
        gObjectList[objectIndex].unk_04C = arg1;
        set_object_flag_status_true(objectIndex, 0x00008000);
    }
    gObjectList[objectIndex].unk_04C--;
    if (gObjectList[objectIndex].unk_04C < 0) {
        set_object_flag_status_false(objectIndex, 0x00008000);
        func_80073654(objectIndex);
        sp24 = true;
    }
    return sp24;
}

void func_80073800(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_0CF = arg1;
}

void func_8007381C(s32 objectIndex) {
    gObjectList[objectIndex].unk_0DC++;
}

void func_80073844(s32 objectIndex) {
    func_80073800(objectIndex, 0);
    gObjectList[objectIndex].unk_0DC = 1;
}

void func_80073884(s32 objectIndex) {
    gObjectList[objectIndex].unk_0DC = 0;
    gObjectList[objectIndex].unk_0CF = 0;
}

s32 func_800738A8(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 phi_t0;

    phi_t0 = 0;
    if (gObjectList[objectIndex].unk_0CF == 0) {
        *arg1 = arg2;
        gObjectList[objectIndex].unk_0AC = arg5;
        gObjectList[objectIndex].unk_0D0 = arg6;
        func_80073800(objectIndex, 1);
    } else {
        gObjectList[objectIndex].unk_0AC--;
        if (gObjectList[objectIndex].unk_0AC < 0) {
            gObjectList[objectIndex].unk_0AC = arg5;
            *arg1 += arg4;
            if (arg3 < *arg1) {
                if (gObjectList[objectIndex].unk_0D0 > 0) {
                    gObjectList[objectIndex].unk_0D0--;
                }

                if (gObjectList[objectIndex].unk_0D0 == 0) {
                    *arg1 = arg3;
                    func_80073800(objectIndex, 0);
                    func_8007381C(objectIndex);
                    phi_t0 = 1;
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return phi_t0;
}

void func_80073998(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    func_800738A8(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

UNUSED void func_800739CC(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (*arg1 < arg3) {
        func_800738A8(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

bool func_80073A10(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    bool phi_t0;

    phi_t0 = false;
    if (gObjectList[objectIndex].unk_0CF == 0) {
        *arg1 = arg2;
        gObjectList[objectIndex].unk_0AC = arg5;
        gObjectList[objectIndex].unk_0D0 = arg6;
        func_80073800(objectIndex, 1);
    } else {
        gObjectList[objectIndex].unk_0AC--;
        if (gObjectList[objectIndex].unk_0AC < 0) {
            gObjectList[objectIndex].unk_0AC = arg5;
            *arg1 -= arg4;
            if (*arg1 < arg3) {
                if (gObjectList[objectIndex].unk_0D0 > 0) {
                    gObjectList[objectIndex].unk_0D0--;
                }

                if (gObjectList[objectIndex].unk_0D0 == 0) {
                    *arg1 = arg3;
                    func_80073800(objectIndex, 0);
                    func_8007381C(objectIndex);
                    phi_t0 = true;
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return phi_t0;
}

s32 func_80073B00(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return func_80073A10(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

s32 func_80073B34(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (arg3 < *arg1) {
        return func_80073A10(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

bool func_80073B78(s32 arg0, s32 objectIndex, s16* arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 phi_t0;

    phi_t0 = false;
    if (gObjectList[objectIndex].unk_0CF == 0) {
        gObjectList[objectIndex].unk_0AC = arg6;
        if (arg0 != 0) {
            *arg2 = arg3;
        }
        gObjectList[objectIndex].unk_0D0 = arg7;
        func_80073800(objectIndex, 1);
    } else {
        gObjectList[objectIndex].unk_0AC--;
        if (gObjectList[objectIndex].unk_0AC < 0) {
            gObjectList[objectIndex].unk_0AC = arg6;
            if (gObjectList[objectIndex].unk_0CF == 1) {
                *arg2 += arg5;
                if (*arg2 >= arg4) {
                    *arg2 = arg4;
                    gObjectList[objectIndex].unk_0CF++;
                }
            } else {
                *arg2 -= arg5;
                if (arg3 >= *arg2) {
                    *arg2 = arg3;
                    if (gObjectList[objectIndex].unk_0D0 > 0) {
                        gObjectList[objectIndex].unk_0D0--;
                    }
                    if (gObjectList[objectIndex].unk_0D0 == 0) {
                        func_80073800(objectIndex, 0);
                        func_8007381C(objectIndex);
                        phi_t0 = true;
                    } else {
                        gObjectList[objectIndex].unk_0CF = 1;
                    }
                }
            }
        }
    }

    return phi_t0;
}

bool func_80073CB0(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return func_80073B78(1, objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

bool func_80073D0C(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return func_80073B78(0, objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

UNUSED void func_80073D68(s32 objectIndex, s16* arg1, s16 arg2, s32 arg3) {
    *arg1 = *arg1 + arg3;
    if (*arg1 >= arg2) {
        *arg1 = arg2;
        func_80073800(objectIndex, 0);
        func_8007381C(objectIndex);
    }
}

void func_80073DC0(s32 objectIndex, s16* arg1, s16 arg2, s32 arg3) {
    *arg1 = *arg1 - arg3;
    if (arg2 >= *arg1) {
        *arg1 = arg2;
        func_80073800(objectIndex, 0);
        func_8007381C(objectIndex);
    }
}

bool func_80073E18(s32 objectIndex, u16* arg1, u16 arg2, s32 arg3) {
    bool phi_t0;
    s32 temp_v1;

    phi_t0 = false;
    if (gObjectList[objectIndex].unk_0CF == 0) {
        func_80073800(objectIndex, 1);
        gObjectList[objectIndex].unk_048 = arg3;
    }

    temp_v1 = gObjectList[objectIndex].unk_048 - arg2;
    if (temp_v1 <= 0) {
        *arg1 += gObjectList[objectIndex].unk_048;
        func_80073800(objectIndex, 0);
        phi_t0 = true;
    } else {
        *arg1 += arg2;
        gObjectList[objectIndex].unk_048 = temp_v1;
    }

    return phi_t0;
}

UNUSED bool func_80073ED4(s32 objectIndex, u16* arg1, u16 arg2, s32 arg3) {
    bool phi_t0;
    s32 temp_v1;

    phi_t0 = false;
    if (gObjectList[objectIndex].unk_0CF == 0) {
        func_80073800(objectIndex, 1);
        gObjectList[objectIndex].unk_048 = arg3;
    }

    temp_v1 = gObjectList[objectIndex].unk_048 - arg2;
    if (temp_v1 <= 0) {
        *arg1 += gObjectList[objectIndex].unk_048;
        func_80073800(objectIndex, 0);
        phi_t0 = true;
    } else {
        *arg1 -= arg2;
        gObjectList[objectIndex].unk_048 = temp_v1;
    }
    return phi_t0;
}

void func_80073F90(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].unk_0CD = arg1;
}

void func_80073FAC(s32 objectIndex) {
    gObjectList[objectIndex].unk_0DB++;
}

void func_80073FD4(s32 objectIndex) {
    func_80073F90(objectIndex, 0);
    gObjectList[objectIndex].unk_0DB = 1;
}

UNUSED void func_80074014(void) {

}

bool func_8007401C(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    bool phi_a3;

    phi_a3 = false;
    if (gObjectList[objectIndex].unk_0CD == 0) {
        *arg1 = arg2;
        gObjectList[objectIndex].unk_0AA = arg5;
        gObjectList[objectIndex].unk_0CE = arg6;
        func_80073F90(objectIndex, 1);
    } else {
        gObjectList[objectIndex].unk_0AA--;
        if ((s32) gObjectList[objectIndex].unk_0AA < 0) {
            gObjectList[objectIndex].unk_0AA = arg5;
            *arg1 += arg4;
            if (arg3 < *arg1) {
                if ((s32) gObjectList[objectIndex].unk_0CE > 0) {
                    gObjectList[objectIndex].unk_0CE--;
                }
                if (gObjectList[objectIndex].unk_0CE == 0) {
                    *arg1 = arg3;
                    func_80073F90(objectIndex, 0);
                    func_80073FAC(objectIndex);
                    phi_a3 = true;
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return phi_a3;
}

s32 func_80074118(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    return func_8007401C(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

s32 func_8007415C(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (*arg1 < arg3) {
        return func_8007401C(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

s32 func_800741B4(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (gObjectList[objectIndex].unk_0CD == 0) {
        *arg1 = arg2;
        gObjectList[objectIndex].unk_0AA = arg5;
        gObjectList[objectIndex].unk_0CE = arg6;
        func_80073F90(objectIndex, 1);
    } else {
        gObjectList[objectIndex].unk_0AA += -1;
        if (gObjectList[objectIndex].unk_0AA < 0) {
            gObjectList[objectIndex].unk_0AA = arg5;
            *arg1 -= arg4;
            if (*arg1 < arg3) {
                if (gObjectList[objectIndex].unk_0CE > 0) {
                    gObjectList[objectIndex].unk_0CE--;
                }
                if (gObjectList[objectIndex].unk_0CE == 0) {
                    *arg1 = arg3;
                    func_80073F90(objectIndex, 0);
                    func_80073FAC(objectIndex);
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return 0;
}


UNUSED void func_800742A8(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    func_800741B4(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

UNUSED void func_800742EC(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (arg3 < *arg1) {
        func_800741B4(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

void func_80074344(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (gObjectList[objectIndex].unk_0CD == 0) {
        *arg1 = arg2;
        gObjectList[objectIndex].unk_0AA = arg5;
        gObjectList[objectIndex].unk_0CE = arg6;
        func_80073F90(objectIndex, 1);
        return;
    }

    gObjectList[objectIndex].unk_0AA--;
    if (gObjectList[objectIndex].unk_0AA < 0) {
        gObjectList[objectIndex].unk_0AA = arg5;
        if (gObjectList[objectIndex].unk_0CD == 1) {
            *arg1 += arg4;
            if (arg3 <= *arg1) {
                *arg1 = arg3;
                gObjectList[objectIndex].unk_0CD++;
            }
        } else {
            *arg1 -= arg4;
            if (*arg1 <= arg2) {
                *arg1 = arg2;

                if (gObjectList[objectIndex].unk_0CE > 0) {
                    gObjectList[objectIndex].unk_0CE--;
                }
                if (gObjectList[objectIndex].unk_0CE == 0) {
                    func_80073F90(objectIndex, 0);
                    func_80073FAC(objectIndex);
                } else {
                    gObjectList[objectIndex].unk_0CD = 1;
                }
            }
        }
    }
}

void func_80074478(s32 objectIndex) {
    gObjectList[objectIndex].status |= 1;
}

void func_800744A0(s32 objectIndex) {
    gObjectList[objectIndex].status &= ~1;
}

void func_800744CC(void) {
    if (D_8018D224 != 0) {
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
        D_8018D224 = 0;
    }
}

uintptr_t devaddr2;

void func_80074510(const char **lakituTexturePtr, const char* devAddr, size_t vaddr, size_t nbytes) {
    func_800744CC();
    lakituTexturePtr[vaddr] = devAddr;
    D_8018D224 = 1;
}

void func_80074574(const char **lakituTexturePtr, const char *devAddr, size_t vaddr, u16 width, u16 height) {
    func_80074510(lakituTexturePtr, devAddr, vaddr, width * height);
}

//! @todo arg1 should likely be a u8 *
void func_800745C8(s32 objectIndex, const char **lakituTexturePtr) {
    s32 phi_a1;

    if ((gObjectList[objectIndex].status & 1) != 0) {
        phi_a1 = 0;
        if (gObjectList[objectIndex].tlutList != gObjectList[objectIndex].activeTLUT) {
            gObjectList[objectIndex].activeTLUT = gObjectList[objectIndex].tlutList;
        }

        gObjectList[objectIndex].status ^= 2;
        if ((gObjectList[objectIndex].status & 2) != 0) {
            phi_a1 = 1;
        }

        gObjectList[objectIndex].activeTexture = lakituTexturePtr[phi_a1];
        func_800744A0(objectIndex);
    }
}

void func_8007466C(s32 objectIndex, const char **lakituTexturePtr) {
    s32 phi_a1;

    if ((gObjectList[objectIndex].status & 1) != 0) {
        // I have no idea why this typecase works
        gObjectList[objectIndex].activeTLUT = (u8*) ((u32*)gObjectList[objectIndex].tlutList + (gObjectList[objectIndex].unk_0D3 << 7));
        gObjectList[objectIndex].status ^= 2;
        phi_a1 = 0;
        if ((gObjectList[objectIndex].status & 2) != 0) {
            phi_a1 = 1;
        }
        gObjectList[objectIndex].activeTexture = lakituTexturePtr[phi_a1];
        func_800744A0(objectIndex);
    }
}

void func_80074704(s32 objectIndex, const char **lakituTexturePtr) {
    s32 phi_a1;

    if ((gObjectList[objectIndex].status & 1) != 0) {
        gObjectList[objectIndex].activeTLUT = gObjectList[objectIndex].tlutList;
        gObjectList[objectIndex].status ^= 2;
        phi_a1 = 0;
        if ((gObjectList[objectIndex].status & 2) != 0) {
            phi_a1 = 1;
        }
        gObjectList[objectIndex].activeTexture = lakituTexturePtr[phi_a1];
        func_800744A0(objectIndex);
    }
}

// Since the buffer only holds two textures, phi_a2 chooses which spot to load into
size_t func_80074790(s32 objectIndex, const char **lakituTexturePtr) {
    s32 phi_a2;

    gObjectList[objectIndex].status ^= 4;
    phi_a2 = 0;
    if ((gObjectList[objectIndex].status & 4) != 0) {
        phi_a2 = 1;
    }
    return phi_a2;
}

void func_800747F0(s32 objectIndex, const char **lakituTexturePtr) {
    const char *nextTexture = NULL;
    if (gObjectList[objectIndex].itemDisplay != gObjectList[objectIndex].unk_0D3) {
        nextTexture = gObjectList[objectIndex].textureList[gObjectList[objectIndex].itemDisplay];

        if (nextTexture != NULL) {
            func_80074574(lakituTexturePtr, nextTexture, func_80074790(objectIndex, lakituTexturePtr), gObjectList[objectIndex].textureWidth, gObjectList[objectIndex].textureHeight);
            gObjectList[objectIndex].unk_0D3 = gObjectList[objectIndex].itemDisplay;
            func_80074478(objectIndex);
        }
    }
}

void func_80074894(s32 objectIndex, const char **lakituTexturePtr) {
    func_800747F0(objectIndex, lakituTexturePtr);
    func_800745C8(objectIndex, lakituTexturePtr);
}

void func_800748C4(s32 objectIndex, const char **lakituTexturePtr) {
    func_800747F0(objectIndex, lakituTexturePtr);
    func_8007466C(objectIndex, lakituTexturePtr);
}

void func_800748F4(s32 objectIndex, const char **lakituTexturePtr) {
    func_800747F0(objectIndex, lakituTexturePtr);
    func_80074704(objectIndex, lakituTexturePtr);
}

void func_80074924(s32 objectIndex) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s16 temp_v0;
    s32 temp_a0;
    Object *object;

    object = &gObjectList[objectIndex];
    object->sizeScaling = 0.15f;
    temp_v0 = gCurrentCourseId;
    switch (temp_v0) {                              /* irregular */
    case COURSE_MARIO_RACEWAY:
        sp2C = random_int(0x00C8U);
        sp28 = random_int(D_80165748);
        sp24 = random_int(0x0096U);
        sp20 = random_int(0x2000U);
        object->origin_pos[0] = (f32) ((((f64) D_80165718 + 100.0) - (f64) sp2C) * (f64) xOrientation);
        object->origin_pos[1] = (f32) (D_80165720 + sp28);
        object->origin_pos[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
        break;
    case COURSE_ROYAL_RACEWAY:
        sp2C = random_int(0x0168U);
        sp28 = random_int(D_80165748);
        sp24 = random_int(0x00B4U);
        sp20 = random_int(0x2000U);
        object->origin_pos[0] = (f32) ((((f64) D_80165718 + 180.0) - (f64) sp2C) * (f64) xOrientation);
        object->origin_pos[1] = (f32) (D_80165720 + sp28);
        object->origin_pos[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
        break;
    case COURSE_LUIGI_RACEWAY:
        sp2C = random_int(0x012CU);
        sp28 = random_int(D_80165748);
        sp24 = random_int(0x0096U);
        sp20 = random_int(0x2000U);
        object->origin_pos[0] = (f32) ((((f64) D_80165718 + 150.0) - (f64) sp2C) * (f64) xOrientation);
        object->origin_pos[1] = (f32) (D_80165720 + sp28);
        object->origin_pos[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
        break;
    }
    set_obj_origin_offset(objectIndex, 0, 0, 0);
    if (gPlayerCount == 1) {
        object->velocity[1] = (f32) (((f64) (f32) (sp2C % 4) * 0.25) + 0.8);
    } else {
        object->velocity[1] = (f32) (((f64) (f32) (sp2C % 3) * 0.2) + 0.4);
    }
    temp_a0 = sp2C % 8;
    object->unk_084[0] = D_800E6F30[temp_a0][0];
    object->unk_084[1] = D_800E6F30[temp_a0][1];
    object->unk_084[2] = D_800E6F30[temp_a0][2];
    object->unk_084[3] = D_800E6F48[temp_a0][0];
    object->unk_084[4] = D_800E6F48[temp_a0][1];
    object->unk_084[5] = D_800E6F48[temp_a0][2];
    object->unk_084[6] = sp20 - 0x1000;
    if (sp2C & 1) {
        object->unk_084[7] = (sp20 / 32) + 0x100;
    } else {
        object->unk_084[7] = -0x100 - (sp20 / 32);
    }
    object->primAlpha = 0x00E6;
    func_80072488(objectIndex);
}

void func_80074D94(s32 objectIndex) {
    if (gObjectList[objectIndex].unk_0AE == 1) {
        if ((D_80165740 <= gObjectList[objectIndex].offset[1]) && (s16_step_down_towards(&gObjectList[objectIndex].primAlpha, 0, 8) != 0)) {
            func_80086F60(objectIndex);
        }
        object_add_velocity_offset_y(objectIndex);
    }
    object_calculate_new_pos_offset(objectIndex);
}

void func_80074E28(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        func_80074924(objectIndex);
        break;
    case 2:
        if (func_8007278C(objectIndex, 1) != 0) {
            func_80086E70(objectIndex);
            break;
        }
    case 0:
        break;
    case 3:
        func_80041480(&gObjectList[objectIndex].unk_084[6], -0x1000, 0x1000, &gObjectList[objectIndex].unk_084[7]);
        if (gObjectList[objectIndex].unk_0AE == 0) {
            func_80072428(objectIndex);
        }
        break;
    }
}

void func_80074EE8(void) {
    s32 someIndex;
    s32 objectIndex;
    s32 someCount;
    Object *object;

    someCount = 0;
    for (someIndex = 0; someIndex < D_80165738; someIndex++) {
        objectIndex = gObjectParticle3[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state != 0) {
                func_80074E28(objectIndex);
                func_80074D94(objectIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&gObjectParticle3[someIndex]);
                }
                someCount += 1;
            }
        }
    }
    if (someCount == 0) {
        D_80165730 = 0;
    }
}

void func_80074FD8(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 0:
        break;
    case 1:
        if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.12f, gObjectList[objectIndex].unk_034, gObjectList[objectIndex].direction_angle[1], 0x00000064) != 0) {
            func_80072488(objectIndex);
        }
        object_calculate_new_pos_offset(objectIndex);
        gObjectList[objectIndex].orientation[0] += D_8016582C[0];
        gObjectList[objectIndex].orientation[1] += D_8016582C[1];
        gObjectList[objectIndex].orientation[2] += D_8016582C[2];
        break;
    case 2:
        func_80086F60(objectIndex);
        func_80072428(objectIndex);
        break;
    }
}

void func_800750D8(s32 objectIndex, s32 arg1, Vec3f arg2, s32 arg3, s32 arg4) {
    s32 sp24;
    s32 temp_v0;
    Object *object;

    init_object(objectIndex, 0);
    sp24 = random_int(0x01F4U);
    temp_v0 = random_int(0x0032U);
    object = &gObjectList[objectIndex];
    object->unk_0D5 = arg3;
    object->sizeScaling = ((f64) (f32) sp24 * 0.0005) + 0.05;
    // ALL HAIL THE FAKE MATCH GODS!!!!!
    object->velocity[1] = ((f64) (f32) temp_v0 * (0.05 * 1.0)) + 2.0;
    object->unk_034 = ((f64) (f32)(temp_v0 % 5) * 0.1) + 1.0;
    object->direction_angle[1] = (arg1 << 0x10) / arg4;
    object->origin_pos[0] = (arg2[0] + (temp_v0 / 2)) - 12.0f;
    object->origin_pos[1] = (arg2[1] - 10.0) + random_int(0x000AU);
    object->origin_pos[2] = (arg2[2] + (temp_v0 / 2)) - 12.0f;
    object->orientation[0] = sp24 << 7;
    object->orientation[1] = temp_v0 * 0x50;
    object->orientation[2] = temp_v0 * 0x50;
}

void func_80075304(Vec3f arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_s1;
    s32 objectIndex;

    for (var_s1 = 0; var_s1 < arg3; var_s1++) {
        switch (arg1) {                             /* irregular */
        case 1:
            objectIndex = add_unused_obj_index(gObjectParticle1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
            break;
        case 2:
            objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
            break;
        case 3:
            objectIndex = add_unused_obj_index(gObjectParticle3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
            break;
        }
        if (objectIndex == NULL_OBJECT_ID) break;
        func_800750D8(objectIndex, var_s1, arg0, arg2, arg3);
    }
}

void func_8007542C(s32 arg0) {
    s32 objectIndex;
    s32 var_s2;
    s32 *var_s3;
    Object *object;

    D_8016582C[0] += 0x2000;
    D_8016582C[1] += 0x1000;
    D_8016582C[2] += 0x1800;
    for (var_s2 = 0; var_s2 < 0x80; var_s2++) {
        switch (arg0) {                             /* irregular */
        case 1:
            var_s3 = gObjectParticle1;
            break;
        case 2:
            var_s3 = gObjectParticle2;
            break;
        case 3:
            var_s3 = gObjectParticle3;
            break;
        }
        objectIndex = var_s3[var_s2];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state != 0) {
                func_80074FD8(objectIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&var_s3[var_s2]);
                }
            }
        }

    }
}

void func_80075574(s32 objectIndex, Vec3f arg1, f32 arg2) {
    Object *object;
    UNUSED s32 pad[2];

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->origin_pos[0] = arg1[0];
    object->origin_pos[1] = arg1[1];
    object->origin_pos[2] = arg1[2];
    object->velocity[1] = arg2;
    object->type = random_int(0x0064U) + 0x1E;
}

s32 func_800755FC(s32 arg0, Vec3f arg1, f32 arg2) {
    s32 objectIndex;

    if (arg0 == 0) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            func_80075574(objectIndex, arg1, arg2);
        }
    } else {
        objectIndex = add_unused_obj_index(gObjectParticle3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            func_80075574(objectIndex, arg1, arg2);
        }
    }
    return objectIndex;
}

void func_80075698(s32 objectIndex) {
    gObjectList[objectIndex].unk_0D5 = 1;
    gObjectList[objectIndex].activeTexture = D_8018D490;
    gObjectList[objectIndex].textureList = D_8018D490;
    gObjectList[objectIndex].primAlpha = 0xFF;
    gObjectList[objectIndex].direction_angle[1] = 0;
    gObjectList[objectIndex].orientation[0] = 0;
    gObjectList[objectIndex].orientation[2] = 0;
    gObjectList[objectIndex].offset[0] = 0.0f;
    gObjectList[objectIndex].offset[1] = 0.0f;
    gObjectList[objectIndex].offset[2] = 0.0f;
    gObjectList[objectIndex].sizeScaling = 0.5f;
    func_80072488(objectIndex);
}

void func_80075714(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        func_80075698(objectIndex);
        break;
    case 2:
        gObjectList[objectIndex].velocity[1] -= 0.03;
        f32_step_up_towards(&gObjectList[objectIndex].offset[1], 100.0f, gObjectList[objectIndex].velocity[1]);
        func_8007415C(objectIndex, &gObjectList[objectIndex].sizeScaling, 0.55f, 1.0f, 0.1f, 1, 0);
        if (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0xFF, 0x1E, 7, 0, 0) != 0) {
            func_80072488(objectIndex);
        }
        break;
    case 3:
        func_80072428(objectIndex);
        break;
    case 0:
        break;
    }

    object_calculate_new_pos_offset(objectIndex);
}

void update_train_smoke(void) {
    UNUSED s32 pad[2];
    s32 count;
    s32 i;
    s32 temp_a0;
    Object *object;
    if (D_8016578C != 0) {
        D_8016578C -= 1;
    }
    if (D_80165790 != 0) {
        D_80165790 -= 1;
    }
    if ((gTrainList[0].someFlags != 0) || (D_8016578C != 0)) {
    count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = gObjectParticle2[i];
            if (temp_a0 != -1) {
                object = &gObjectList[temp_a0];
                if (object->state != 0) {
                    func_80075714(temp_a0);
                    if (object->state == 0) {
                        delete_object_wrapper(&gObjectParticle2[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_8016578C = 100;
        }
    }
    if ((gTrainList[1].someFlags != 0) || (D_80165790 != 0)) {
        count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = gObjectParticle3[i];
            if (temp_a0 != -1) {
                object = &gObjectList[temp_a0];
                if (object->state != 0) {
                    func_80075714(temp_a0);
                    if (object->state == 0) {
                        delete_object_wrapper(&gObjectParticle3[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_80165790 = 100;
       }
    }
}

void func_800759EC(s32 objectIndex, Vec3f arg1, f32 arg2) {
    Object *object;

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->origin_pos[0] = arg1[0];
    object->origin_pos[1] = arg1[1];
    object->origin_pos[2] = arg1[2];
    object->velocity[1] = arg2;
    object->type = 0x00FF;
    object->unk_0A2 = 0x0096;
}

s32 func_80075A6C(s32 arg0, Vec3f arg1, f32 arg2) {
    s32 objectIndex;

    if (arg0 == 0) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            func_800759EC(objectIndex, arg1, arg2);
        }
    } else {
        objectIndex = add_unused_obj_index(gObjectParticle3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            func_800759EC(objectIndex, arg1, arg2);
        }
    }

    return objectIndex;
}

void func_80075B08(s32 objectIndex) {
    gObjectList[objectIndex].unk_0D5 = 6;
    gObjectList[objectIndex].activeTexture = D_8018D490;
    gObjectList[objectIndex].textureList = D_8018D490;
    gObjectList[objectIndex].primAlpha = 0xFF;
    gObjectList[objectIndex].direction_angle[1] = 0;
    gObjectList[objectIndex].orientation[0] = 0;
    gObjectList[objectIndex].orientation[2] = 0;
    gObjectList[objectIndex].offset[0] = 0.0f;
    gObjectList[objectIndex].offset[1] = 0.0f;
    gObjectList[objectIndex].offset[2] = 0.0f;
    gObjectList[objectIndex].sizeScaling = 0.5f;
    func_80072488(objectIndex);
}

void func_80075B84(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        func_80075B08(objectIndex);
        break;
    case 2:
        gObjectList[objectIndex].velocity[1] -= 0.03;
        f32_step_up_towards(&gObjectList[objectIndex].offset[1], 100.0f, gObjectList[objectIndex].velocity[1]);
        func_8007415C(objectIndex, &gObjectList[objectIndex].sizeScaling, 0.55f, 1.0f, 0.1f, 1, 0);
        if (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0xFF, 0x1E, 7, 0, 0) != 0) {
            func_80072488(objectIndex);
        }
        break;
    case 3:
        func_80072428(objectIndex);
        break;
    case 0:
        break;
    }

    object_calculate_new_pos_offset(objectIndex);
}

void update_ferries_smoke_particle(void) {
    UNUSED s32 pad[2];
    s32 count;
    s32 i;
    s32 temp_a0;
    Object *object;
    if (D_8016578C != 0) {
        D_8016578C -= 1;
    }
    if (D_80165790 != 0) {
        D_80165790 -= 1;
    }
    if ((gPaddleBoats[0].someFlags != 0) || (D_8016578C != 0)) {
        count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = gObjectParticle2[i];
            if (temp_a0 != -1) {
                object = &gObjectList[temp_a0];
                if (object->state != 0) {
                    func_80075B84(temp_a0);
                    if (object->state == 0) {
                        delete_object_wrapper(&gObjectParticle2[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_8016578C = 100;
        }
    }
    if ((gPaddleBoats[1].someFlags != 0) || (D_80165790 != 0)) {
        count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = gObjectParticle3[i];
            if (temp_a0 != -1) {
                object = &gObjectList[temp_a0];
                if (object->state != 0) {
                    func_80075B84(temp_a0);
                    if (object->state == 0) {
                        delete_object_wrapper(&gObjectParticle3[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_80165790 = 100;
       }
    }
}

void func_80075E5C(s32 objectIndex, Vec3f arg1, u16 arg2, f32 arg3, s32 arg4) {
    Object *object;

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->sizeScaling = 0.5f;
    object->unk_0D5 = 5;
    object->origin_pos[0] = arg1[0];
    object->origin_pos[1] = arg1[1];
    object->origin_pos[2] = arg1[2];
    object->direction_angle[0] = 0x0C00;
    object->direction_angle[2] = 0;
    object->unk_034 = arg3 * 4.0;
    object->direction_angle[1] = arg2;
    object->type = 0x00FF;
    object->unk_0A2 = 0x00FF;
    object->unk_048 = arg4 * 2;
}

s32 func_80075F28(Vec3f arg0, u16 arg1, f32 arg2, s32 arg3) {
    s32 objectIndex;

    objectIndex = add_unused_obj_index(gObjectParticle1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        func_80075E5C(objectIndex, arg0, arg1, arg2, arg3);
    }
    return objectIndex;
}

void func_80075F98(Vec3f arg0, u16 arg1, f32 arg2) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 10; someIndex++) {
        if (func_80075F28(arg0, arg1, arg2, someIndex) == -1) break;
    }
}

void func_8007601C(s32 objectIndex) {
    if (gObjectList[objectIndex].unk_04C > 0) {
        gObjectList[objectIndex].unk_04C--;
        if (gObjectList[objectIndex].unk_04C == 0) {
            func_800722CC(objectIndex, 1);
        }
    }

    if (gObjectList[objectIndex].unk_048 > 0) {
        gObjectList[objectIndex].unk_048--;
        if (gObjectList[objectIndex].unk_048 == 0) {
            func_800C9EF4(gObjectList[objectIndex].pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x0A));
        }
    }

    if (gObjectList[objectIndex].unk_04C == 0) {
        func_8008A6DC(objectIndex, 300.0f);
        if ((is_obj_flag_status_active(objectIndex, VISIBLE) != 0) && (func_80072354(objectIndex, 1) != 0)) {
            func_800722A4(objectIndex, 1);
            func_80075F98(gObjectList[objectIndex].pos, gObjectList[objectIndex].direction_angle[1], 1.0f);
            func_800C9D80(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x0A));
            if (gObjectList[objectIndex].type > 0) {
                gObjectList[objectIndex].type--;
                gObjectList[objectIndex].unk_04C = 0x5A;
            } else {
                gObjectList[objectIndex].unk_04C = 0x12C;
            }

            gObjectList[objectIndex].unk_048 = 0x3C;
        }
    }

    if (func_8008A8B0(9, 0xB) == 0) {
        gObjectList[objectIndex].type = 2;
    }
}

void func_8007614C(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 4; someIndex++) {
        func_8007601C(indexObjectList3[someIndex]);
    }
}

void func_80076194(s32 objectIndex, Vec3f arg1, f32 arg2, s32 arg3) {
    Object *object;

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->unk_0D5 = 4;
    object->sizeScaling = 1.0f;
    object->origin_pos[0] = arg1[0];
    object->origin_pos[1] = arg1[1];
    object->origin_pos[2] = arg1[2];
    object->direction_angle[0] = 0x0C00;
    object->direction_angle[2] = 0;
    object->direction_angle[1] = 0x2100;
    if (gIsMirrorMode != 0) {
        object->direction_angle[1] += -0x4000;
    }
    object->type = 0x00FF;
    object->unk_0A2 = 0x00FF;
    object->unk_048 = arg3 * 2;
    object->unk_034 = arg2 * 8.0;
}

s32 func_80076278(Vec3f arg0, f32 arg1, s32 arg2) {
    s32 objectIndex;

    objectIndex = add_unused_obj_index(gObjectParticle1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        func_80076194(objectIndex, arg0, arg1, arg2);
    }
    return objectIndex;
}

void func_800762DC(Vec3f arg0, f32 arg1) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 0x14; someIndex++) {
        if (func_80076278(arg0, arg1, someIndex) == -1) break;
    }
}

void func_8007634C(s32 objectIndex) {
    Object *object;

    u8 *asset = LOAD_ASSET(common_texture_particle_smoke);

    object = &gObjectList[objectIndex];
    object->activeTexture = asset[0];
    object->textureList = asset[0];
    object->primAlpha = 0x00FF;
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    func_80072488(objectIndex);
}

void func_800763CC(s32 objectIndex) {
    Object *object;

    if(objectIndex){}
    object = &gObjectList[objectIndex];
    switch (object->state) {
    case 1:
        func_8007634C(objectIndex);
        break;
    case 2:
        if (func_8007278C(objectIndex, object->unk_048) != 0) {
            func_80086E70(objectIndex);
        }
        break;
    case 3:
        if (object->unk_0D5 == 4) {
            f32_step_towards(&object->sizeScaling, 4.0f, 0.1f);
            s16_step_towards(&object->type, 0, 0x0018);
            s16_step_towards(&object->unk_0A2, 0x0080, 0x000C);
        } else {
            f32_step_towards(&object->sizeScaling, 1.0f, 0.1f);
            s16_step_towards(&object->type, 0, 0x0018);
            s16_step_towards(&object->unk_0A2, 0x0080, 0x000C);
        }
        if ((object->unk_0AE >= 2) && (func_80073B00(objectIndex, &object->primAlpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
            func_80072488(objectIndex);
        }
        break;
    case 4:
        func_80072428(objectIndex);
        func_80086F60(objectIndex);
        break;
    case 0:
    default:
        break;
    }
}

void func_80076538(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 0:
        break;
    case 1:
        if ((u8) gObjectList[objectIndex].unk_0D5 == 4) {
            if (func_80087060(objectIndex, 0x0000000E) != 0) {
                func_80086FD4(objectIndex);
            }
        } else if (func_80087060(objectIndex, 2) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        u16_step_down_towards(&gObjectList[objectIndex].direction_angle[0], 0, 0x00000400);
        break;
    }
    if (gObjectList[objectIndex].unk_0AE > 0) {
        func_80087844(objectIndex);
        object_calculate_new_pos_offset(objectIndex);
    }
}

void func_8007661C(void) {
    s32 objectIndex;

    objectIndex = indexObjectList2[0];
    if (gObjectList[objectIndex].unk_04C > 0) {
        gObjectList[objectIndex].unk_04C--;
        if (gObjectList[objectIndex].unk_04C == 0) {
            func_800722CC(objectIndex, 1);
        }
    }
    if (gObjectList[objectIndex].unk_048 > 0) {
        gObjectList[objectIndex].unk_048--;
        if (gObjectList[objectIndex].unk_048 == 0) {
            func_800C9EF4(gObjectList[objectIndex].pos, SOUND_ARG_LOAD(0x51, 0x03, 0x80, 0x09));
        }
    }
    if (gObjectList[objectIndex].unk_04C == 0) {
        func_8008A6DC(objectIndex, 750.0f);
        if ((is_obj_flag_status_active(objectIndex, VISIBLE) != 0) && (func_80072354(objectIndex, 1) != 0)) {
            func_800722A4(objectIndex, 1);
            func_800762DC(gObjectList[objectIndex].pos, 1.0f);
            func_800C9D80(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, 0x51038009U);
            if (gObjectList[objectIndex].type > 0) {
                gObjectList[objectIndex].type--;
                gObjectList[objectIndex].unk_04C = 0x0000005A;
            } else {
                gObjectList[objectIndex].unk_04C = 0x0000012C;
            }
            gObjectList[objectIndex].unk_048 = 0x0000003C;
        }
    }
    if (func_8008A8B0(4, 5) == 0) {
        gObjectList[objectIndex].type = 2;
    }
}

void func_8007675C(s32 objectIndex, Vec3s arg1, s32 arg2) {
    Object *object;

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->unk_0D5 = 9;
    object->sizeScaling = 1.0f;
    object->origin_pos[0] = arg1[0];
    object->origin_pos[1] = arg1[1];
    object->origin_pos[2] = arg1[2];
    object->direction_angle[0] = 0x0C00;
    object->direction_angle[1] = 0x2100;
    object->direction_angle[2] = 0;
    object->type = 0x00FF;
    object->unk_0A2 = 0x00FF;
    object->unk_034 = 8.0f;
    object->velocity[1] = 8.0f;
    object->unk_048 = arg2;
}

s32 func_80076828(Vec3s arg0, s32 arg1) {
    s32 objectIndex;

    objectIndex = add_unused_obj_index(gObjectParticle1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        func_8007675C(objectIndex, arg0, arg1);
    }
    return objectIndex;
}

void func_80076884(s32 arg0) {
    UNUSED s32 stackPadding0;
    s32 i;
    s32 temp_v0;
    s16 *var_s2;
    temp_v0 = random_int(0x000FU);
    switch (arg0)
    {
        case 0:
        var_s2 = D_800E5740 + (temp_v0 * 3);
        break;

        case 1:
        var_s2 = D_800E579C + (temp_v0 * 3);
        break;

        case 2:
        var_s2 = D_800E57F8 + (temp_v0 * 3);
        break;

    }

    for (i = 0; i < 1; i++)
    {
        if (func_80076828(var_s2, i) == (-1))
        {
        break;
        }
    }
}

void func_80076958(s32 objectIndex) {
    Object *object;
    u8 *tex = (u8 *) LOAD_ASSET(common_texture_particle_smoke);

    object = &gObjectList[objectIndex];
    object->activeTexture = tex[0];
    object->textureList = tex[0];
    object->primAlpha = 0x00FF;
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    func_80072488(objectIndex);
}

void func_800769D8(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 0:
            break;
        case 1:
            func_80076958(objectIndex);
            break;
        case 2:
            if (func_8007278C(objectIndex, object->unk_048) != 0) {
                func_80086E70(objectIndex);
            }
            break;
        case 3:
            f32_step_towards(&object->sizeScaling, 2.0f, 0.05f);
            s16_step_towards(&object->type, 0, 0x0018);
            if ((object->unk_0AE >= 2) && (func_80073B00(objectIndex, &object->primAlpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                func_80072488(objectIndex);
            }
            break;
        case 4:
            func_80072428(objectIndex);
            func_80086F60(objectIndex);
            break;
    }
}

void func_80076AEC(s32 objectIndex) {
    s32 thing;
    thing = gObjectList[objectIndex].unk_0AE;
    if(thing) {}
    if (thing != 0) {
        if (thing == 1) {
            if (func_80087060(objectIndex, 0x0000000A) != 0) {
                func_80086FD4(objectIndex);
            }
        } else {
            thing = gObjectList[objectIndex].unk_0AE;
        }
    }
    if (gObjectList[objectIndex].unk_0AE > 0) {
        object_add_velocity_offset_y(objectIndex);
        object_calculate_new_pos_offset(objectIndex);
    }
}

void func_80076B7C(void) {

}

void update_flame_particle(void) {
    s32 someIndex;
    s32 objectIndex;
    Object *object;

    func_8007661C();
    func_8007614C();
    for (someIndex = 0; someIndex < gObjectParticle1_SIZE; someIndex++) {
        objectIndex = gObjectParticle1[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state != 0) {
                if ((object->unk_0D5 == 4) || (object->unk_0D5 == 5)) {
                    func_800763CC(objectIndex);
                    func_80076538(objectIndex);
                } else if (object->unk_0D5 == 9) {
                    func_800769D8(objectIndex);
                    func_80076AEC(objectIndex);
                }
                if (object->state == 0) {
                    delete_object_wrapper(&gObjectParticle1[someIndex]);
                }
            }
        }
    }
}

void init_object_smoke_paticle(s32 objectIndex, Vec3f arg1, s16 arg2) {
    Object *object;

    init_object(objectIndex, (s32) arg2);
    object = &gObjectList[objectIndex];
    object->unk_0D5 = 0x0A;
    object->activeTexture = common_texture_particle_smoke[0];
    object->textureList = common_texture_particle_smoke[0];
    object->sizeScaling = 0.3f;
    set_obj_origin_pos(objectIndex, arg1[0], arg1[1], arg1[2]);
    object->type = 0x00FF;
    object->unk_034 = 0.0f;
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
}

void init_smoke_particle(Vec3f arg0, UNUSED f32 arg1, s16 arg2) {
    s32 objectIndex;

    objectIndex = add_unused_obj_index(gObjectParticle4, &gNextFreeObjectParticle4, gObjectParticle4_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        init_object_smoke_paticle(objectIndex, arg0, arg2);
    }
}

void func_80076DC4(s32 objectIndex) {
    func_80072488(objectIndex);
    if (gObjectList[objectIndex].unk_0D5 != 0x0B) {
        func_80086E70(objectIndex);
    }
}

void func_80076E14(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            func_80076DC4(objectIndex);
            break;
        case 2:
            if ((gObjectList[objectIndex].unk_0AE >= 2) && (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                func_80072488(objectIndex);
            }
            break;
        case 0:
            break;
        case 3:
            func_80072428(objectIndex);
            func_80086F60(objectIndex);
            break;
    }
}

void func_80076ED8(s32 objectIndex) {
    if ((gObjectList[objectIndex].unk_0AE != 0) && (gObjectList[objectIndex].unk_0AE == 1)) {
        func_80086FD4(objectIndex);
    }
    object_calculate_new_pos_offset(objectIndex);
}

void func_80076F2C(void) {
    s32 someIndex;
    s32 flameIndex;
    Object *object;

    for (someIndex = 0; someIndex < gObjectParticle4_SIZE; someIndex++) {
        flameIndex = gObjectParticle4[someIndex];
        if (flameIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[flameIndex];
            if (object->state != 0) {
                func_80076E14(flameIndex);
                func_80076ED8(flameIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&gObjectParticle4[someIndex]);
                }
            }
        }
    }
}

void init_object_smoke_particle(s32 objectIndex, s32 flameIndex) {
    init_object(objectIndex, 3);

    gObjectList[objectIndex].unk_0D5 = 0xB;
    gObjectList[objectIndex].activeTexture = common_texture_particle_smoke[0];
    gObjectList[objectIndex].textureList = common_texture_particle_smoke[0];
    gObjectList[objectIndex].sizeScaling = 0.8f;

    gObjectList[objectIndex].origin_pos[0] = (f32)*(gTorchSpawns + (flameIndex * 3) + 0) * xOrientation;
    gObjectList[objectIndex].origin_pos[1] = (f32)*(gTorchSpawns + (flameIndex * 3) + 1);
    gObjectList[objectIndex].origin_pos[2] = (f32)*(gTorchSpawns + (flameIndex * 3) + 2);
    gObjectList[objectIndex].unk_034 = 0;
    gObjectList[objectIndex].type = 255;
    gObjectList[objectIndex].unk_0A2 = 255;
    gObjectList[objectIndex].primAlpha = 255;
    set_obj_orientation(objectIndex, 0, 0, 0);
    set_obj_origin_offset(objectIndex, 0, 0, 0);
}

void init_smoke_particles(s32 arg0) {
    s32 objectIndex;

    objectIndex = add_unused_obj_index(gObjectParticle4, &gNextFreeObjectParticle4, gObjectParticle4_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        init_object_smoke_particle(objectIndex, arg0);
    }
}

void func_80077138(s32 objectIndex, Vec3f arg1, s32 arg2) {
    s8 temp_v0_3;
    Vec3s sp30;

    init_object(objectIndex, arg2);
    gObjectList[objectIndex].unk_0D5 = 0x0C;
    gObjectList[objectIndex].sizeScaling = 0.05f;
    set_obj_origin_pos(objectIndex, arg1[0], arg1[1], arg1[2]);
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    switch (arg2) {
        case 0:
            gObjectList[objectIndex].velocity[1] = -1.0f;
            gObjectList[objectIndex].unk_034 = (f32) ((random_int(0x004BU) * 0.01) + 0.25);
            gObjectList[objectIndex].direction_angle[1] = random_int(0x0040U) << 0xA;
            func_8008751C(objectIndex);
            gObjectList[objectIndex].unk_084[5] = 0x001E;
            break;
        case 1:
            gObjectList[objectIndex].velocity[1] = 1.5f;
            gObjectList[objectIndex].unk_034 = (f32) ((random_int(0x0064U) * 0.01) + 0.5);
            gObjectList[objectIndex].direction_angle[1] = random_int(0x0040U) << 0xA;
            func_8008751C(objectIndex);
            gObjectList[objectIndex].unk_084[5] = 0x0032;
            break;
    }
    temp_v0_3 = random_int(0x000CU);
    if (temp_v0_3 < 9) {
        func_8005C674(temp_v0_3, &sp30[2], &sp30[1], sp30);
        gObjectList[objectIndex].unk_048 = 0;
        gObjectList[objectIndex].unk_084[0] = sp30[2];
        gObjectList[objectIndex].unk_084[1] = sp30[1];
        gObjectList[objectIndex].unk_084[2] = sp30[0];
    } else {
        temp_v0_3 = random_int(3U);
        func_8005C6B4(temp_v0_3, &sp30[2], &sp30[1], sp30);
        gObjectList[objectIndex].unk_084[0] = sp30[2];
        gObjectList[objectIndex].unk_084[1] = sp30[1];
        gObjectList[objectIndex].unk_084[2] = sp30[0];
        gObjectList[objectIndex].unk_084[4] = temp_v0_3;
        gObjectList[objectIndex].unk_048 = 1;
    }
    gObjectList[objectIndex].primAlpha = 0x00FF;
    gObjectList[objectIndex].unk_084[3] = random_int(0x0800U) + 0x400;
    if ((gObjectList[objectIndex].direction_angle[1] < 0x3000) || (gObjectList[objectIndex].direction_angle[1] >= 0xB001)) {
        gObjectList[objectIndex].unk_084[3] = -gObjectList[objectIndex].unk_084[3];
    }
}

void func_800773D8(f32 *arg0, s32 arg1) {
    s32 objectIndex = add_unused_obj_index(gObjectParticle3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        func_80077138(objectIndex, arg0, arg1);
    }
}

void func_80077428(s32 arg0) {
    func_80072488(arg0);
    func_80086E70(arg0);
}

void func_80077450(s32 objectIndex) {
    UNUSED s16 stackPadding0;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;

    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            func_80077428(objectIndex);
            break;
        case 2:
            f32_step_up_towards(&gObjectList[objectIndex].sizeScaling, 0.1f, 0.01f);
            if ((gObjectList[objectIndex].pos[1] <= gObjectList[objectIndex].unk_084[5]) && (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0x000000FF, 0, 0x00000010, 0, 0) != 0)) {
                func_80086F60(objectIndex);
                func_80072428(objectIndex);
            }
            break;
    }
    if (gObjectList[objectIndex].unk_048 != 0) {
        gObjectList[objectIndex].unk_084[4] = (s16) ((s32) (gObjectList[objectIndex].unk_084[4] + 1) % 3);
        func_8005C6B4(gObjectList[objectIndex].unk_084[4], &sp3C, &sp3A, &sp38);
        gObjectList[objectIndex].unk_084[0] = sp3C;
        gObjectList[objectIndex].unk_084[1] = sp3A;
        gObjectList[objectIndex].unk_084[2] = sp38;
    }
}

void func_80077584(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    if ((object->unk_0AE != 0) && (object->unk_0AE == 1) && ((u8) object->unk_0D8 != 0)) {
        if (object->velocity[1] >= -0.5) {
            object->velocity[1] -= 0.15;
        } else {
            object->velocity[2] = 0.0f;
            object->velocity[0] = 0.0f;
        }
    }
    object->orientation[2] += object->unk_084[3];
    object_add_velocity_offset_xyz(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void func_80077640(void) {
    s32 someIndex;
    s32 objectIndex;
    Object *object;

    for(someIndex = 0; someIndex < gObjectParticle3_SIZE; someIndex++) {
        objectIndex = gObjectParticle3[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state != 0) {
                func_80077450(objectIndex);
                func_80077584(objectIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&gObjectParticle3[someIndex]);
                }
            }
        }
    }
}

void init_object_leaf_particle(s32 objectIndex, Vec3f arg1, s32 num) {
    UNUSED s32 stackPadding1;
    UNUSED u16 stackPadding0;
    u16 temp_s0;
    u16 sp3E;
    u16 sp3C;

    init_object(objectIndex, 0);
    gObjectList[objectIndex].unk_0D5 = 7;
    gObjectList[objectIndex].activeTLUT = (u8*) common_texture_particle_leaf;
    gObjectList[objectIndex].tlutList = (u8*) common_texture_particle_leaf;
    gObjectList[objectIndex].sizeScaling = 0.1f;
    gObjectList[objectIndex].surfaceHeight = arg1[1];
    switch (gCurrentCourseId) {
        case COURSE_MARIO_RACEWAY:
            object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            gObjectList[objectIndex].unk_034 = 1.5f;
            gObjectList[objectIndex].velocity[1] = 1.5f;
            break;
        case COURSE_YOSHI_VALLEY:
            object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            gObjectList[objectIndex].unk_034 = 2.0f;
            gObjectList[objectIndex].velocity[1] = 2.0f;
            break;
        case COURSE_ROYAL_RACEWAY:
            object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 30.0, arg1[2], 0x10, 0x28, 0x10);
            gObjectList[objectIndex].unk_034 = 2.0f;
            gObjectList[objectIndex].velocity[1] = 2.0f;
            break;
        case COURSE_LUIGI_RACEWAY:
            object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            gObjectList[objectIndex].unk_034 = 1.5f;
            gObjectList[objectIndex].velocity[1] = 1.0f;
            break;
    }
    temp_s0 = random_int(0x0010U) << 0xC;
    sp3E = random_int(0x0010U) << 0xC;
    sp3C = random_int(0x0010U) << 0xC;
    set_obj_direction_angle(objectIndex, 0U, (num * 0xFFFF) / 20, 0U);
    set_obj_orientation(objectIndex, temp_s0, sp3E, sp3C);
}

s32 init_leaf_particle(Vec3f arg0, s32 num) {
    s32 objectIndex;

    objectIndex = add_unused_obj_index(gLeafParticle, &gNextFreeLeafParticle, gLeafParticle_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        init_object_leaf_particle(objectIndex, arg0, num);
    }
    return objectIndex;
}

void spawn_leaf(Vec3f arg0, UNUSED s32 arg1) {
    s32 i;

    for (i = 0; i < gLeafParticle_SPAWN_SIZE; i++) {
        if (init_leaf_particle(arg0, i) == NULL_OBJECT_ID) break;
    }
}

void func_80077B14(s32 arg0) {
    func_80072488(arg0);
    func_80086E70(arg0);
}

void func_80077B3C(s32 objectIndex) {
    Object *object;
    object = &gObjectList[objectIndex];

    switch (object->state) {
        case 0:
            break;
        case 1:
            func_80077B14(objectIndex);
            break;
        case 2:
            if (object->unk_0AE == 0) {
                func_80072488(objectIndex);
            }
            break;
        case 3:
            func_80072428(objectIndex);
            break;
    }
}

void func_80077BCC(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.2f, gObjectList[objectIndex].unk_034, (s16) (s32) gObjectList[objectIndex].direction_angle[1], 0x0000000A);
            break;
        case 2:
            if (func_80087B84(objectIndex, 0.4f, gObjectList[objectIndex].surfaceHeight) != 0) {
                func_80086F60(objectIndex);
            }
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
    gObjectList[objectIndex].orientation[0] += 0x1000;
    gObjectList[objectIndex].orientation[1] += 0x1000;
}

void update_leaf(void) {
    s32 someIndex;
    s32 leafIndex;
    Object *object;

    for(someIndex = 0; someIndex < gLeafParticle_SIZE; someIndex++) {
        leafIndex = gLeafParticle[someIndex];
        if (leafIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[leafIndex];
            if (object->state != 0) {
                func_80077B3C(leafIndex);
                func_80077BCC(leafIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&gLeafParticle[someIndex]);
                }
            }
        }
    }
}

void func_80077D5C(s32 arg0) {
    s32 objectIndex;
    s32 var_a1;

    if (D_8016559C == 0) {
        for (var_a1 = 0; var_a1 < D_8018D1F0; var_a1++) {
            D_8018D17C += 1;
            if (D_8018D17C >= D_8018D1F0) {
                D_8018D17C = 0;
            }
            objectIndex = D_8018CC80[arg0 + D_8018D17C];
            if (gObjectList[objectIndex].state == 0) {
                init_object(objectIndex, 1);
                break;
            }
        }
    }
}

void func_80077E20(s32 objectIndex) {
    u8 *tex = (u8 *) LOAD_ASSET(D_0D0293D8);
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_rectangle);
    Object *object;

    object = &gObjectList[objectIndex];
    object->activeTexture = tex;
    object->textureList = tex;
    //! @bug frappe snowland There's something up with the handling of common_vtx_rectangle and the loading of 0x10 right here
    // root function: func_80078C70
    object->vertex = vtx;
    object->textureHeight = 0x10;
    object->textureWidth = object->textureHeight;
    object->sizeScaling = 0.15f;
    set_object_flag_status_true(objectIndex, 0x00000010);
    func_80086EF0(objectIndex);
    object->primAlpha = 0x00FF;
    object->unk_0D5 = 0;
    object->type = 0;
    func_80072488(objectIndex);
}

void func_80077EB8(s32 objectIndex, u16 arg1, Camera *camera) {
    s16 temp_v0;

    temp_v0 = camera->rot[1] - arg1;
    if ((temp_v0 >= D_8018D210) || (D_8018D208 >= temp_v0)) {
        gObjectList[objectIndex].offset[0] = D_8018D218 + (D_8018D1E8 * (f32) temp_v0);
        set_object_flag_status_true(objectIndex, 0x00000010);
        return;
    }
    set_object_flag_status_false(objectIndex, 0x00000010);
}

void func_80077F64(s32 objectIndex, Camera *camera) {

    f64 rand;

    switch (gObjectList[objectIndex].unk_0AE) {                              /* irregular */
        case 1:
            gObjectList[objectIndex].direction_angle[1] = (camera->rot[1] + random_int(0x4000U)) - 0x2000;
            object_origin_pos_randomize_around_y(objectIndex, 0x00B4, 0x0014U);
            rand = random_int(0x0064U);

            gObjectList[objectIndex].velocity[1] = (f32) (-0.75 - (f64) (f32) (rand * 0.01));
            gObjectList[objectIndex].offset[0] = 0.0f;
            gObjectList[objectIndex].offset[1] = 0.0f;
            func_80086FD4(objectIndex);
            return;
        case 2:
            func_80077EB8(objectIndex, gObjectList[objectIndex].direction_angle[1], camera);
            object_add_velocity_offset_y(objectIndex);
            object_calculate_new_pos_offset(objectIndex);
            func_8008BFC0(objectIndex);
            if (gObjectList[objectIndex].pos[1] <= 0.0f) {
                func_80086FD4(objectIndex);
                return;
            }
        case 0:
            return;
        case 3:
            func_80086F60(objectIndex);
            break;
    }
}

void func_800780CC(s32 objectIndex, Camera *camera) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 1:
        func_80077E20(objectIndex);
        return;
    case 2:
        func_80077F64(objectIndex, camera);
        if (gObjectList[objectIndex].unk_0AE == 0) {
            func_80072488(objectIndex);
            return;
        }
    case 0:
        return;
    case 3:
        func_80072428(objectIndex);
        break;
    }
}

void func_80078170(s32 arg0, Camera *arg1) {
    s32 objectIndex;
    s32 i;

    func_80077D5C(arg0);
    for (i = 0; i < D_8018D1F0; i++) {
        objectIndex = D_8018CC80[arg0 + i];
        if (gObjectList[objectIndex].state != 0) {
            func_800780CC(objectIndex, arg1);
        }
    }
}

void func_80078220(s32 objectIndex) {
    u8 *tex = (u8 *) LOAD_ASSET(D_0D0293D8);
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_rectangle);
    Object *object;

    object = &gObjectList[objectIndex];
    object->activeTexture = tex;
    object->textureList = tex;
    object->vertex = vtx;
    object->sizeScaling = 0.15f;
    func_80086EF0(objectIndex);
    func_80072488(objectIndex);
}

void func_80078288(s32 objectIndex) {
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    UNUSED u16 pad;
    u16 temp_t6;

    switch (gObjectList[objectIndex].unk_0AE) {                              /* irregular */
    case 0:
        break;
    case 1:
        if (gGamestate != 9) {
            sp3A = ((gPlayerOneCopy->unk_094 / 18) * 216) / 2;
            sp3E = (random_int(0x000FU) - sp3A) + 0x2D;
            sp3C = random_int(0x012CU) + 0x1E;
            temp_t6 = camera1->rot[1] + ((s32) (random_int(0x3000U) - 0x1800) / (s16) ((sp3A / 15) + 1));
            gObjectList[objectIndex].origin_pos[0] = gPlayerOneCopy->pos[0] + (sins(temp_t6) * sp3C);
            gObjectList[objectIndex].origin_pos[1] = sp3E + gPlayerOneCopy->unk_074;
            gObjectList[objectIndex].origin_pos[2] = gPlayerOneCopy->pos[2] + (coss(temp_t6) * sp3C);
            gObjectList[objectIndex].unk_0C4 = random_int(0x0400U) + 0x100;
            gObjectList[objectIndex].unk_01C[0] = (f32) (((f32) random_int(0x0064U) * 0.03) + 2.0);
            gObjectList[objectIndex].velocity[1] = (f32) (-0.3 - (f64) (f32) (random_int(0x0032U) * 0.01));
            gObjectList[objectIndex].offset[0] = 0.0f;
            gObjectList[objectIndex].offset[1] = 0.0f;
            func_80086FD4(objectIndex);
        } else {
            sp3C = random_int(0x0064U) + 0x28;
            temp_t6 = camera1->rot[1] + random_int(0x3000U) - 0x1800;
            gObjectList[objectIndex].origin_pos[0] = camera1->pos[0] + (sins(temp_t6) * sp3C);
            gObjectList[objectIndex].origin_pos[1] = camera1->pos[1] + 45.0;
            gObjectList[objectIndex].origin_pos[2] = camera1->pos[2] + (coss(temp_t6) * sp3C);
            gObjectList[objectIndex].unk_0C4 = random_int(0x0400U) + 0x100;
            gObjectList[objectIndex].unk_01C[0] = (f32) (((f32) random_int(0x0064U) * 0.03) + 2.0);
            gObjectList[objectIndex].velocity[1] = (f32) (-0.6 - (f64) (f32) (random_int(0x0032U) * 0.01));
            gObjectList[objectIndex].offset[0] = 0.0f;
            gObjectList[objectIndex].offset[1] = 0.0f;
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        object_add_velocity_offset_y(objectIndex);
        gObjectList[objectIndex].direction_angle[0] += gObjectList[objectIndex].unk_0C4;
        gObjectList[objectIndex].offset[0] = sins(gObjectList[objectIndex].direction_angle[0]) * gObjectList[objectIndex].unk_01C[0];
        object_calculate_new_pos_offset(objectIndex);
        if ((f64) gObjectList[objectIndex].pos[1] <= 0.0) {
            func_80086FD4(objectIndex);
        }
        gObjectList[objectIndex].orientation[1] = angle_between_object_camera(objectIndex, camera1);
        break;
    case 3:
        func_80086F60(objectIndex);
        break;
    }
}

void func_800786EC(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 0:
        break;
    case 1:
        func_80078220(objectIndex);
        break;
    case 2:
        func_80078288(objectIndex);
        if (gObjectList[objectIndex].unk_0AE == 0) {
            func_80072488(objectIndex);
        }
        break;
    case 3:
        func_80072428(objectIndex);
        break;
    }
}

void func_80078790(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        D_8018D174 += 1;
        if (D_8018D174 >= NUM_SNOWFLAKES) {
            D_8018D174 = 0;
        }
        if (gObjectList[gObjectParticle1[D_8018D174]].state == 0) {
            init_object(gObjectParticle1[D_8018D174], 1);
            break;
        }
    }
}

void update_snowflakes(void) {
    s32 someIndex;
    s32 snowflakeIndex;

    if (gGamestate != CREDITS_SEQUENCE) {
        func_80078790();
    } else {
        func_80078790();
        func_80078790();
        func_80078790();
        func_80078790();
    }
    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        snowflakeIndex = gObjectParticle1[someIndex];
        if (gObjectList[snowflakeIndex].state != 0) {
            func_800786EC(snowflakeIndex);
        }
    }
}

void func_800788F8(s32 objectIndex, u16 rot, Camera *camera) {
    s16 temp_v0;

    temp_v0 = camera->rot[1] + rot;
    if ((temp_v0 >= D_8018D210) && (D_8018D208 >= temp_v0)) {
        gObjectList[objectIndex].unk_09C = (D_8018D218 + (D_8018D1E8 * temp_v0));
        set_object_flag_status_true(objectIndex, 0x00000010);
        return;
    }
    set_object_flag_status_false(objectIndex, 0x00000010);
}

void update_clouds(s32 arg0, Camera *arg1, CloudData *cloudList) {
    s32 cloudIndex;
    s32 objectIndex;
    CloudData *cloud;

    for (cloudIndex = 0; cloudIndex < D_8018D1F0; cloudIndex++) {
        cloud = &cloudList[cloudIndex];
        objectIndex = D_8018CC80[arg0 + cloudIndex];
        func_800788F8(objectIndex, cloud->rotY, arg1);
    }
}

void update_stars(s32 arg0, Camera *camera, StarData *starList) {
    s32 starIndex;
    s32 objectIndex;
    StarData *star;

    for (starIndex = 0; starIndex < D_8018D1F0; starIndex++) {
        star = &starList[starIndex];
        objectIndex = D_8018CC80[arg0 + starIndex];
        func_800788F8(objectIndex, star->rotY, camera);
        switch (starIndex % 5U) {
            case 0:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000028, 0x000000B4, 0x000000FF, 0, -1);
                break;
            case 1:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000080, 0x000000FF, 0x000000FF, 0, -1);
                break;
            case 2:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000050, 0x000000C8, 0x000000FF, 0, -1);
                break;
            case 3:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0, 0x0000009B, 0x000000FF, 0, -1);
                break;
            case 4:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x0000005A, 0x00000080, 0x000000FF, 0, -1);
                break;
        }
    }
}

UNUSED void func_80078C68() {

}

void func_80078C70(s32 arg0) {
    s32 sp1C;
    Camera *camera;

    if (D_801657C8 == 0) {
        switch (arg0) {                             /* switch 1 */
        case 0:                                     /* switch 1 */
            sp1C = 0;
            camera = camera1;
            D_8018D200 = gCameraZoom[0] + 40.0f;
            break;
        case 1:                                     /* switch 1 */
            sp1C = 0;
            camera = camera1;
            D_8018D200 = gCameraZoom[0] + 40.0f;
            break;
        case 2:                                     /* switch 1 */
            camera = camera2;
            sp1C = D_8018D1F0;
            D_8018D200 = gCameraZoom[1] + 40.0f;
            break;
        case 3:                                     /* switch 1 */
            sp1C = 0;
            camera = camera1;
            D_8018D200 = gCameraZoom[0] + 40.0f;
            break;
        case 4:                                     /* switch 1 */
            camera = camera2;
            sp1C = D_8018D1F0;
            D_8018D200 = gCameraZoom[1] + 40.0f;
            break;
        }

        D_8018D208 = ( (D_8018D200 / 2) * 0xB6) + 0x71C;
        D_8018D210 = (-(D_8018D200 / 2) * 0xB6) - 0x71C;
        D_8018D1E8 = 1.7578125 / D_8018D200;
        D_8018D218 = 0xA0;
        switch (gCurrentCourseId) {                          /* switch 2 */
            case COURSE_MARIO_RACEWAY:                                     /* switch 2 */
                // Uses Luigi Raceway's clouds for display purposes?
                update_clouds(sp1C, camera, gLuigiRacewayClouds);
                break;
            case COURSE_YOSHI_VALLEY:                                     /* switch 2 */
                update_clouds(sp1C, camera, gYoshiValleyMooMooFarmClouds);
                break;
            case COURSE_FRAPPE_SNOWLAND:                                     /* switch 2 */
                func_80078170(sp1C, camera);
                break;
            case COURSE_KOOPA_BEACH:                                     /* switch 2 */
                update_clouds(sp1C, camera, gKoopaTroopaBeachClouds);
                break;
            case COURSE_ROYAL_RACEWAY:                                     /* switch 2 */
                update_clouds(sp1C, camera, gRoyalRacewayClouds);
                break;
            case COURSE_LUIGI_RACEWAY:                                     /* switch 2 */
                update_clouds(sp1C, camera, gLuigiRacewayClouds);
                break;
            case COURSE_MOO_MOO_FARM:                                     /* switch 2 */
                update_clouds(sp1C, camera, gYoshiValleyMooMooFarmClouds);
                break;
            case COURSE_TOADS_TURNPIKE:                                    /* switch 2 */
                update_stars(sp1C, camera, gToadsTurnpikeRainbowRoadStars);
                break;
            case COURSE_KALAMARI_DESERT:                                    /* switch 2 */
                update_clouds(sp1C, camera, gKalimariDesertClouds);
                break;
            case COURSE_SHERBET_LAND:                                    /* switch 2 */
                update_clouds(sp1C, camera, gSherbetLandClouds);
                break;
            case COURSE_RAINBOW_ROAD:                                    /* switch 2 */
                update_stars(sp1C, camera, gToadsTurnpikeRainbowRoadStars);
                break;
            case COURSE_WARIO_STADIUM:                                    /* switch 2 */
                update_stars(sp1C, camera, gWarioStadiumStars);
                break;
        }
    }
}

void func_80078F64(void) {
    switch (gScreenModeSelection) {                              /* irregular */
    case SCREEN_MODE_1P:
        init_object(gIndexLakituList[0], 1);
        break;
    case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
        init_object(gIndexLakituList[0], 1);
        init_object(gIndexLakituList[1], 1);
        break;
    case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        init_object(gIndexLakituList[0], 1);
        init_object(gIndexLakituList[1], 1);
        break;
    case SCREEN_MODE_3P_4P_SPLITSCREEN:
        init_object(gIndexLakituList[0], 1);
        init_object(gIndexLakituList[1], 1);
        init_object(gIndexLakituList[2], 1);
        init_object(gIndexLakituList[3], 1);
        break;
    }
}

void func_80079054(s32 playerId) {
    init_object(gIndexLakituList[playerId], 2);
}

void func_80079084(s32 playerId) {
    init_object(gIndexLakituList[playerId], 4);
}

void func_800790B4(s32 playerId) {
    init_object(gIndexLakituList[playerId], 5);
}

void func_800790E4(s32 playerId) {
    init_object(gIndexLakituList[playerId], 6);
}

void func_80079114(s32 objectIndex, s32 playerId, s32 arg2) {
    s32 a;
    if (gObjectList[objectIndex].state >= 2) {
        if ((u8)gObjectList[objectIndex].unk_0D8 == 1) {
            if (playerId == 0) {
                func_80074894(objectIndex, gLakituTexturePtr);
                return;
            }
            a = gIndexLakituList[0];
            gObjectList[objectIndex].activeTLUT = gObjectList[a].activeTLUT;
            gObjectList[objectIndex].activeTexture = gObjectList[a].activeTexture;
            if (0) { }
            return;
        }
        switch (arg2) {
            case 0:
                func_800748F4(objectIndex, gLakituTexturePtr);
                break;
            case 1:
                func_800748C4(objectIndex, gLakituTexturePtr);
                break;
            case 2:
                func_80074894(objectIndex, gLakituTexturePtr);
                break;
        }
    }
}

void func_800791F0(s32 objectIndex, s32 playerId) {
    Player *player = &gPlayerOne[playerId];

    if ((gObjectList[objectIndex].unk_0D8 != 3) && (gObjectList[objectIndex].unk_0D8 != 7)) {
        func_800722CC(objectIndex, 1);
        if (gCurrentCourseId == COURSE_SHERBET_LAND) {
            player->unk_0CA &= 0xFFEF;
        }
    } else {
        // ?????
    }
    if (gCurrentCourseId == COURSE_SHERBET_LAND) {
        func_800722CC(objectIndex, 0x00000010);
        player->unk_0CA &= 0xFFDF;
    }
    func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
}


static const char *sLakituTextures[] = {
    gTextureLakituNoLights1,   gTextureLakituNoLights2,   gTextureLakituNoLights3,
    gTextureLakituNoLights4,   gTextureLakituNoLights5,   gTextureLakituNoLights6,
    gTextureLakituNoLights7,   gTextureLakituNoLights8,   gTextureLakituRedLights01,
    gTextureLakituRedLights02, gTextureLakituRedLights03, gTextureLakituRedLights04,
    gTextureLakituRedLights05, gTextureLakituRedLights06, gTextureLakituRedLights07,
    gTextureLakituRedLights08, gTextureLakituRedLights09, gTextureLakituRedLights10,
    gTextureLakituRedLights11, gTextureLakituRedLights12, gTextureLakituRedLights13,
    gTextureLakituRedLights14, gTextureLakituRedLights15, gTextureLakituRedLights16,
    gTextureLakituBlueLight1, gTextureLakituBlueLight2, gTextureLakituBlueLight3,
    gTextureLakituBlueLight4, gTextureLakituBlueLight5, gTextureLakituBlueLight6,
    gTextureLakituBlueLight7, gTextureLakituBlueLight8,
};

void init_obj_lakitu_red_flag_countdown(s32 objectIndex, s32 playerId) {
    if (playerId == 0) {
        D_801656F0 = 0;
        D_8018D168 = 0;
    }

    //u8 *tlut = (u8 *) LOAD_ASSET(common_tlut_lakitu_countdown);
    //u8 *lights = (u8 *) LOAD_ASSET(gTextureLakituNoLights1);

    init_texture_object(objectIndex, (u8 *) load_lakitu_tlut_x64(common_tlut_lakitu_countdown, ARRAY_COUNT(common_tlut_lakitu_countdown)), sLakituTextures, 56, (u16) 72);
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_lakitu);
    gObjectList[objectIndex].vertex = vtx;
    gObjectList[objectIndex].sizeScaling = 0.15f;
    set_object_flag_status_false(objectIndex, 0x00000010);
    func_80072488(objectIndex);
    gObjectList[objectIndex].unk_048 = D_8018D180;
}

void update_object_lakitu_countdown(s32 objectIndex, s32 playerId) {
    UNUSED s32 pad;
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            init_obj_lakitu_red_flag_countdown(objectIndex, playerId);
            break;
        case 2:
            func_8007278C(objectIndex, gObjectList[objectIndex].unk_048);
            if ((gObjectList[objectIndex].unk_050 == 0x00000055) && (gPlayerCount == 3) && (playerId == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 3:
            set_object_flag_status_true(objectIndex, 0x00000010);
            func_80086F10(objectIndex, 1, &D_800E67B8);
            func_80072488(objectIndex);
            break;
        case 4:
            if ((func_8007278C(objectIndex, 0x0000001E) != 0) && (gPlayerCount != 3) && (playerId == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 5:
            func_8007278C(objectIndex, 0x0000001E);
            break;
        case 6:
            func_80072E54(objectIndex, 1, 7, 1, 2, 0);
            break;
        case 7:
            if (func_8007278C(objectIndex, 0x00000014) != 0) {
                gObjectList[objectIndex].tlutList += 0x200;
                if (playerId == 0) {
                    play_sound2(SOUND_ACTION_COUNTDOWN_LIGHT);
                }
            }
            break;
        case 8:
            func_80072E54(objectIndex, 8, 0x0000000F, 1, 6, 0);
            break;
        case 9:
            if ((func_8007278C(objectIndex, 8) != 0) && (playerId == 0)) {
                play_sound2(SOUND_ACTION_COUNTDOWN_LIGHT);
            }
            break;
        case 10:
            if ((func_80072E54(objectIndex, 0x00000010, 0x00000017, 1, 6, 0) != 0) && (playerId == 0)) {
                D_801656F0 = 1;
            }
            break;
        case 11:
            if (func_8007278C(objectIndex, 8) != 0) {
                gObjectList[objectIndex].tlutList += 0x200;
                if (playerId == 0) {
                    play_sound2(SOUND_ACTION_GREEN_LIGHT);
                }
            }
            break;
        case 12:
            func_80072E54(objectIndex, 0x00000018, 0x0000001B, 1, 6, 0);
            break;
        case 13:
            if (playerId == 0) {
                func_800729EC(objectIndex);
                D_8018D160 = 1;
                break;
            }
            func_80072488(objectIndex);
            break;
        case 14:
            func_8007278C(objectIndex, 0x00000078);
            break;
        case 15:
            func_80072428(objectIndex);
            break;
    }
}

static const char *sLakituCheckeredList[] = {
    gTextureLakituCheckeredFlag01, gTextureLakituCheckeredFlag02, gTextureLakituCheckeredFlag03,
    gTextureLakituCheckeredFlag04, gTextureLakituCheckeredFlag05, gTextureLakituCheckeredFlag06,
    gTextureLakituCheckeredFlag07, gTextureLakituCheckeredFlag08, gTextureLakituCheckeredFlag09,
    gTextureLakituCheckeredFlag10, gTextureLakituCheckeredFlag11, gTextureLakituCheckeredFlag12,
    gTextureLakituCheckeredFlag13, gTextureLakituCheckeredFlag14, gTextureLakituCheckeredFlag15,
    gTextureLakituCheckeredFlag16, gTextureLakituCheckeredFlag17, gTextureLakituCheckeredFlag18,
    gTextureLakituCheckeredFlag19, gTextureLakituCheckeredFlag20, gTextureLakituCheckeredFlag21,
    gTextureLakituCheckeredFlag22, gTextureLakituCheckeredFlag23, gTextureLakituCheckeredFlag24,
    gTextureLakituCheckeredFlag25, gTextureLakituCheckeredFlag26, gTextureLakituCheckeredFlag27,
    gTextureLakituCheckeredFlag28, gTextureLakituCheckeredFlag29, gTextureLakituCheckeredFlag30,
    gTextureLakituCheckeredFlag31, gTextureLakituCheckeredFlag32
};

void init_obj_lakitu_red_flag(s32 objectIndex, s32 playerIndex) {
    Object *object;

    func_800791F0(objectIndex, playerIndex);

    u8 *tex = (u8 *) LOAD_ASSET(common_tlut_lakitu_checkered_flag);
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_also_lakitu);

    init_texture_object(objectIndex, (u8*) tex, sLakituCheckeredList, 0x48U, (u16) 0x00000038);
    object = &gObjectList[objectIndex];
    object->activeTexture = *gObjectList[objectIndex].textureList;
    object->vertex = vtx;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->sizeScaling = 0.15f;
    func_80086F10(objectIndex, 2, &D_800E6834);
    set_object_flag_status_false(objectIndex, 0x00000010);
    func_80072488(objectIndex);
}

void update_object_lakitu_red_flag(s32 objectIndex, s32 playerIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            init_obj_lakitu_red_flag(objectIndex, playerIndex);
            break;
        case 2:
            set_object_flag_status_true(objectIndex, 0x00000010);
            func_80072488(objectIndex);
            break;
        case 3:
            func_80072E54(objectIndex, 0, 0x0000001F, 1, 2, -1);
            break;
        case 4:
            func_80072428(objectIndex);
            break;
    }
}

void func_800797AC(s32 playerId) {
    s32 objectIndex;
    Player *player;

    objectIndex = gIndexLakituList[playerId];
    player = &gPlayerOne[playerId];
    if ((gCurrentCourseId == COURSE_SHERBET_LAND) && (player->unk_0CA & 1)) {
        init_object(objectIndex, 7);
        player->unk_0CA |= 0x10;
    } else {
        init_object(objectIndex, 3);
    }
    func_800722A4(objectIndex, 1);
}

void func_80079860(s32 playerId) {
    s32 objectIndex;
    Player *player;

    objectIndex = gIndexLakituList[playerId];
    player = &gPlayerOne[playerId];
    if ((func_80072354(objectIndex, 1) != 0) &&
        (
            ((func_802ABDF4(player->collision.meshIndexZX) != 0) && (player->collision.surfaceDistance[2] <= 3.0f)) ||
            (player->unk_0CA & 1) ||
            ((player->surfaceType == OUT_OF_BOUNDS) && !(player->effects & 8))
        )
        ) {
        func_80090778(player);
        func_800797AC(playerId);
    }
}

void func_8007993C(s32 objectIndex, Player *player) {
    if (player->unk_0CA & 4) {
        func_800722A4(objectIndex, 2);
        gObjectList[objectIndex].primAlpha = player->unk_0C6;
        return;
    }
    func_800722CC(objectIndex, 2);
}

static const char *sLakituFishingTextures[] = {
    gTextureLakituFishing1, gTextureLakituFishing2, gTextureLakituFishing3, gTextureLakituFishing4
};

void init_obj_lakitu_red_flag_fishing(s32 objectIndex, s32 arg1) {

    u8 *tlut = (u8 *) LOAD_ASSET(common_tlut_lakitu_fishing);
    Vtx *vtx = (Vtx *) LOAD_ASSET(D_0D005F30);

    func_800791F0(objectIndex, arg1);
    init_texture_object(objectIndex, tlut, sLakituFishingTextures, 0x38U, (u16) 0x00000048);
    gObjectList[objectIndex].vertex = vtx;
    gObjectList[objectIndex].sizeScaling = 0.15f;
    func_80086E70(objectIndex);
    set_object_flag_status_false(objectIndex, 0x00000010);
    func_80073720(objectIndex);
    func_80072488(objectIndex);
    func_800C8F80((u8)arg1, 0x0100FA28);
}

void func_80079A5C(s32 objectIndex, UNUSED Player *player) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 0:
        break;
    case 1:
        gObjectList[objectIndex].origin_pos[2] = 0.0f;
        gObjectList[objectIndex].origin_pos[1] = 0.0f;
        gObjectList[objectIndex].origin_pos[0] = 0.0f;
        gObjectList[objectIndex].offset[2] = 0.0f;
        gObjectList[objectIndex].offset[0] = 0.0f;
        gObjectList[objectIndex].offset[1] = 80.0f;
        func_80086FD4(objectIndex);
        break;
    case 2:
        if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 5.0f, 1.0f) != 0) {
            func_80086F60(objectIndex);
        }
        break;
    case 3:
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], 100.0f, 1.0f) != 0) {
            func_80086F60(objectIndex);
        }
        break;
    }
}

void update_object_lakitu_fishing(s32 objectIndex, s32 playerId) {
    Player *player = &gPlayerOne[playerId];

    switch (gObjectList[objectIndex].state) {                              /* switch 1; irregular */
        case 0:                                         /* switch 1 */
            break;
        case 1:                                         /* switch 1 */
            init_obj_lakitu_red_flag_fishing(objectIndex, playerId);
            break;
        case 2:                                         /* switch 1 */
            set_object_flag_status_true(objectIndex, 0x00000010);
            func_800736E0(objectIndex);
            func_80072488(objectIndex);
            break;
        case 3:                                         /* switch 1 */
            func_800730BC(objectIndex, 0, 3, 1, 2, -1);
            break;
    }
    switch (gObjectList[objectIndex].unk_0D6) {
        case 0:
            break;
        case 1:
            if (func_80086FA4(objectIndex) != 0) {
                func_80073654(objectIndex);
            }
            break;
        case 2:
            func_80090868(player);
            func_80073654(objectIndex);
            break;
        case 3:
            if (!(player->unk_0CA & 2)) {
                func_80086EAC(objectIndex, 0, 3);
                func_80073654(objectIndex);
            }
            break;
        case 4:
            if (func_80086FA4(objectIndex) != 0) {
                func_80073654(objectIndex);
            }
            break;
        case 5:
            func_800722CC(objectIndex, 1);
            func_800C9018((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            func_80072428(objectIndex);
            func_80073720(objectIndex);
            break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        func_8007993C(objectIndex, player);
    }
    func_80079A5C(objectIndex, player);
}

void update_object_lakitu_fishing2(s32 objectIndex, s32 playerId) {
    Player *player = &gPlayerOne[playerId];

    switch (gObjectList[objectIndex].state) {                              /* switch 1; irregular */
        case 0:                                         /* switch 1 */
            break;
        case 1:                                         /* switch 1 */
            init_obj_lakitu_red_flag_fishing(objectIndex, playerId);
            break;
        case 2:                                         /* switch 1 */
            set_object_flag_status_true(objectIndex, 0x00000010);
            func_800736E0(objectIndex);
            player->unk_0CA |= 0x80;
            func_80072488(objectIndex);
            break;
        case 3:                                         /* switch 1 */
            func_800730BC(objectIndex, 0, 3, 1, 2, -1);
            break;
    }
    switch (gObjectList[objectIndex].unk_0D6) {
        case 1:
            if (func_80086FA4(objectIndex) != 0) {
                func_800C9060((u8) playerId, 0x1900A055U);
                func_80073654(objectIndex);
            }
            break;
        case 2:
            func_80090868(player);
            func_800722A4(objectIndex, 4);
            func_80073654(objectIndex);
            break;
        case 3:
            if ((player->surfaceType == ICE) && !(player->unk_0CA & 1) && ((f64) player->collision.surfaceDistance[2] <= 30.0)) {
                func_800722A4(objectIndex, 8);
            }
            if (!(player->unk_0CA & 2)) {
                func_80086EAC(objectIndex, 0, 3);
                func_80073654(objectIndex);
            }
            break;
        case 4:
            func_8007375C(objectIndex, 0x0000001E);
            break;
        case 5:
            player->unk_0CA &= 0xFF7F;
            func_800722A4(objectIndex, 0x00000010);
            func_800722A4(objectIndex, 0x00000020);
            func_800722CC(objectIndex, 4);
            func_800722CC(objectIndex, 8);
            func_80073654(objectIndex);
            func_800C9060((u8) playerId, 0x1900A056U);
            break;
        case 6:
            if (func_8007375C(objectIndex, 0x000000A0) != 0) {
                func_800722CC(objectIndex, 0x00000010);
                player->unk_0CA &= 0xFFEF;
                player->unk_0CA |= 0x20;
            }
            break;
        case 7:
            func_8007375C(objectIndex, 0x0000003C);
            break;
        case 8:
            func_80073720(objectIndex);
            func_80072428(objectIndex);
            player->unk_0CA &= 0xFFDF;
            func_800722CC(objectIndex, 1);
            func_800C9018((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            break;
    }

    if (gObjectList[objectIndex].state >= 2) {
        func_8007993C(objectIndex, player);
    }
    func_80079A5C(objectIndex, player);
}

static const char* sLakituSecondLapTextures[] = {
    gTextureLakituSecondLap01, gTextureLakituSecondLap02, gTextureLakituSecondLap03, gTextureLakituSecondLap04,
    gTextureLakituSecondLap05, gTextureLakituSecondLap06, gTextureLakituSecondLap07, gTextureLakituSecondLap08,
    gTextureLakituSecondLap09, gTextureLakituSecondLap10, gTextureLakituSecondLap11, gTextureLakituSecondLap12,
    gTextureLakituSecondLap13, gTextureLakituSecondLap14, gTextureLakituSecondLap15, gTextureLakituSecondLap16
};

void func_8007A060(s32 objectIndex, s32 playerIndex) {
    Object *object;

    func_800791F0(objectIndex, playerIndex);

    u8 *tlut = (u8 *) LOAD_ASSET(common_tlut_lakitu_second_lap);
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_also_lakitu);

    init_texture_object(objectIndex, tlut, sLakituSecondLapTextures, 0x48U, (u16) 0x00000038);
    object = &gObjectList[objectIndex];
    object->activeTexture = *gObjectList[objectIndex].textureList;
    object->vertex = vtx;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->sizeScaling = 0.15f;
    set_object_flag_status_false(objectIndex, 0x00000010);
    func_80086F10(objectIndex, 5, &D_800E694C);
    func_80072488(objectIndex);
}

void update_object_lakitu_second_lap(s32 objectIndex, s32 playerIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            func_8007A060(objectIndex, playerIndex);
            break;
        case 2:
            set_object_flag_status_true(objectIndex, 0x00000010);
            func_80072488(objectIndex);
            break;
        case 3:
            func_8007278C(objectIndex, 0x00000014);
            break;
        case 4:
            func_80072E54(objectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            func_8007278C(objectIndex, 0x0000003C);
            break;
        case 6:
            func_80072F88(objectIndex, 0x0000000F, 0, 1, 2, 1);
            break;
        case 7:
            if (gObjectList[objectIndex].unk_0AE == 0) {
                func_80072428(objectIndex);
            }
            break;
    }
}

static const char* sLakituFinalLapTextures[] = {
    gTextureLakituFinalLap01, gTextureLakituFinalLap02, gTextureLakituFinalLap03, gTextureLakituFinalLap04,
    gTextureLakituFinalLap05, gTextureLakituFinalLap06, gTextureLakituFinalLap07, gTextureLakituFinalLap08,
    gTextureLakituFinalLap09, gTextureLakituFinalLap10, gTextureLakituFinalLap11, gTextureLakituFinalLap12,
    gTextureLakituFinalLap13, gTextureLakituFinalLap14, gTextureLakituFinalLap15, gTextureLakituFinalLap16,
};

void func_8007A228(s32 objectIndex, s32 playerIndex) {
    Object *object;

    func_800791F0(objectIndex, playerIndex);
    
    u8 *tlut = (u8 *) LOAD_ASSET(common_tlut_lakitu_final_lap);
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_also_lakitu);

    init_texture_object(objectIndex, tlut, sLakituFinalLapTextures, 0x48U, (u16) 0x00000038);
    object = &gObjectList[objectIndex];
    object->activeTexture = *gObjectList[objectIndex].textureList;
    object->vertex = vtx;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->sizeScaling = 0.15f;
    set_object_flag_status_false(objectIndex, 0x00000010);
    func_80086F10(objectIndex, 5, &D_800E694C);
    func_80072488(objectIndex);
}

void update_object_lakitu_final_lap(s32 objectIndex, s32 playerIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            func_8007A228(objectIndex, playerIndex);
            break;
        case 2:
            set_object_flag_status_true(objectIndex, 0x00000010);
            func_80072488(objectIndex);
            break;
        case 3:
            func_8007278C(objectIndex, 0x00000014);
            break;
        case 4:
            func_80072E54(objectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            func_8007278C(objectIndex, 0x0000003C);
            break;
        case 6:
            func_80072F88(objectIndex, 0x0000000F, 0, 1, 2, 1);
            break;
        case 7:
            if (gObjectList[objectIndex].unk_0AE == 0) {
                func_80072428(objectIndex);
            }
            break;
    }
}

static const char* sLakituReverseTextures[] = {
    gTextureLakituReverse01, gTextureLakituReverse02, gTextureLakituReverse03, gTextureLakituReverse04,
    gTextureLakituReverse05, gTextureLakituReverse06, gTextureLakituReverse07, gTextureLakituReverse08,
    gTextureLakituReverse09, gTextureLakituReverse10, gTextureLakituReverse11, gTextureLakituReverse12,
    gTextureLakituReverse13, gTextureLakituReverse14, gTextureLakituReverse15, gTextureLakituReverse16
};

void func_8007A3F0(s32 objectIndex, s32 arg1) {
    f32 var = 5000.0f;
    func_800791F0(objectIndex, arg1);

    u8 *tlut = (u8 *) LOAD_ASSET(common_tlut_lakitu_reverse);
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_also_lakitu);

    init_texture_object(objectIndex, tlut, sLakituReverseTextures, 72, (u16) 56);
    gObjectList[objectIndex].activeTexture = *gObjectList[objectIndex].textureList;
    gObjectList[objectIndex].vertex = vtx;
    gObjectList[objectIndex].pos[2] = var;
    gObjectList[objectIndex].pos[1] = var;
    gObjectList[objectIndex].pos[0] = var;
    gObjectList[objectIndex].sizeScaling = 0.15f;
    set_object_flag_status_false(objectIndex, 0x00000010);
    func_80086F10(objectIndex, 6, &D_800E69B0);
    gObjectList[objectIndex].unk_0D6 = 0;
    func_80072488(objectIndex);
    func_800C8F80((u8)arg1, 0x0100FA28);
}

void update_object_lakitu_reverse(s32 objectIndex, s32 playerId) {
    Player *sp2C = &gPlayerOne[playerId];

    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            func_8007A3F0(objectIndex, playerId);
            break;
        case 2:
            set_object_flag_status_true(objectIndex, 0x00000010);
            gObjectList[objectIndex].unk_0D6 = 1;
            func_80072488(objectIndex);
            break;
        case 3:
            func_800730BC(objectIndex, 0, 0x0000000F, 1, 2, -1);
            break;
        case 4:
            func_80072428(objectIndex);
            break;
    }
    switch (gObjectList[objectIndex].unk_0D6) {                              /* switch 1; irregular */
        case 1:                                         /* switch 1 */
            if ((gObjectList[objectIndex].state >= 3) && (!(sp2C->effects & 0x400000))) {
                func_80086F10(objectIndex, 6, &D_800E69F4);
                gObjectList[objectIndex].unk_0D6 = 2;
                gObjectList[objectIndex].unk_04C = 0x00000050;
                func_800C9018((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
                return;
            }
            return;
        case 2:                                         /* switch 1 */
            gObjectList[objectIndex].unk_04C--;
            if (gObjectList[objectIndex].unk_04C == 0) {
                func_80072488(objectIndex);
                gObjectList[objectIndex].unk_0D6 = 0;
            }
            break;
    }
}

void func_8007A66C(s32 objectIndex, Player *player, Camera *camera) {
    u16 temp_t8;

    temp_t8 = 0x8000 - camera->rot[1];
    gObjectList[objectIndex].pos[0] = (player->pos[0] + (coss(temp_t8) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) - (sins(temp_t8) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
    gObjectList[objectIndex].pos[1] = player->unk_074 + gObjectList[objectIndex].origin_pos[1] + gObjectList[objectIndex].offset[1];
    gObjectList[objectIndex].pos[2] = (player->pos[2] + (sins(temp_t8) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) + (coss(temp_t8) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
}

void func_8007A778(s32 objectIndex, Player *player, Camera *camera) {
    u16 temp_t8;

    temp_t8 = 0x8000 - camera->rot[1];
    gObjectList[objectIndex].pos[0] = (player->pos[0] + (coss(temp_t8) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) - (sins(temp_t8) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
    gObjectList[objectIndex].pos[1] = player->pos[1] + gObjectList[objectIndex].origin_pos[1] + gObjectList[objectIndex].offset[1];
    gObjectList[objectIndex].pos[2] = (player->pos[2] + (sins(temp_t8) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) + (coss(temp_t8) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
}

UNUSED void func_8007A884(void) {

}

void func_8007A88C(s32 playerId) {
    s32 objectIndex;
    Player *player;

    objectIndex = gIndexLakituList[playerId];
    player = &gPlayerOne[playerId];

    if ((gObjectList[objectIndex].state == 0) && (player->effects & 0x400000)) {
        func_800790E4(playerId);
    }
}

void func_8007A910(s32 arg0) {
    if (D_801657B4 == 0) {
        func_8007A88C(arg0);
    }
    func_80079860(arg0);
}

void update_object_lakitu(s32 playerId) {
    s32 objectIndex = gIndexLakituList[playerId];

    switch (gObjectList[objectIndex].unk_0D8) {
        case 0:
            break;
        case 1:
            update_object_lakitu_countdown(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 2:
            update_object_lakitu_red_flag(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 3:
            update_object_lakitu_fishing(objectIndex, playerId);
            break;
        case 4:
            update_object_lakitu_second_lap(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 5:
            update_object_lakitu_final_lap(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 6:
            update_object_lakitu_reverse(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 7:
            update_object_lakitu_fishing2(objectIndex, playerId);
            break;
    }
}

// animate lakitu?
void func_8007AA44(s32 playerId) {
    s32 objectIndex;

    func_8007A910(playerId);
    objectIndex = gIndexLakituList[playerId];
    gLakituTexturePtr = &gLakituTextureBuffer[playerId];
    switch (gObjectList[objectIndex].unk_0D8) {
    case 1:
        func_80079114(objectIndex, playerId, 2);
        func_8007A66C(objectIndex, D_8018CF1C, D_8018CF14);
        break;
    case 2:
        func_80079114(objectIndex, playerId, 0);
        func_8007A66C(objectIndex, D_8018CF1C, D_8018CF14);
        break;
    case 3:
        func_80079114(objectIndex, playerId, 0);
        func_8007A778(objectIndex, D_8018CF1C, D_8018CF14);
        break;
    case 4:
        func_80079114(objectIndex, playerId, 0);
        func_8007A66C(objectIndex, D_8018CF1C, D_8018CF14);
        break;
    case 5:
        func_80079114(objectIndex, playerId, 0);
        func_8007A66C(objectIndex, D_8018CF1C, D_8018CF14);
        break;
    case 6:
        func_80079114(objectIndex, playerId, 0);
        func_8007A66C(objectIndex, D_8018CF1C, D_8018CF14);
        break;
    case 7:
        func_80079114(objectIndex, playerId, 0);
        func_8007A778(objectIndex, D_8018CF1C, D_8018CF14);
        break;
    case 0:
    default:
        break;
    }
}

void func_8007ABFC(s32 playerId, bool arg1) {
    s32 itemWindow;

    if (playerHUD[playerId].raceCompleteBool == false) {
        itemWindow = gItemWindowObjectByPlayerId[playerId];
        if (func_80072354(itemWindow, 4) != 0) {
            init_object(itemWindow, 0);
            if (arg1 != 0) {
                playerHUD[playerId].itemOverride = arg1;
            }
        }
        func_800C9060(playerId, 0x19008406U);
    }
}

void consume_item(s32 playerId) {
    UNUSED s32 pad;
    Player *player;
    s32 objectIndex;
    ItemWindowObjects *itemWindow;

    player = &gPlayerOne[playerId];
    objectIndex = gItemWindowObjectByPlayerId[playerId];
    itemWindow = (ItemWindowObjects *) &gObjectList[objectIndex];
    if (itemWindow->currentItem == ITEM_SUPER_MUSHROOM) {
        if (func_80072354(objectIndex, 2) != 0) {
            func_800722A4(objectIndex, 2);
            itemWindow->goldenMushroomTimer = 0x0258;
            func_800726CC(objectIndex, 0x00000032);
        }
        if (itemWindow->goldenMushroomTimer == 0) {
            func_800722CC(objectIndex, 2);
            player->currentItemCopy = ITEM_NONE;
            itemWindow->currentItem = ITEM_NONE;
            set_object_unk_0CB(objectIndex, 0);
            func_800726CC(objectIndex, 9);
        }
    } else {
        player->currentItemCopy = ITEM_NONE;
        itemWindow->currentItem = ITEM_NONE;
        set_object_unk_0CB(objectIndex, 0);
    }
}

typedef struct {
    u8 banana;               // ITEM_BANANA
    u8 bananaBunch;          // ITEM_BANANA_BUNCH
    u8 greenShell;          // ITEM_GREEN_SHELL
    u8 tripleGreenShell;    // ITEM_TRIPLE_GREEN_SHELL
    u8 redShell;            // ITEM_RED_SHELL
    u8 tripleRedShell;      // ITEM_TRIPLE_RED_SHELL
    u8 blueSpinyShell;      // ITEM_BLUE_SPINY_SHELL
    u8 thunderbolt;         // ITEM_THUNDERBOLT
    u8 fakeItemBox;         // ITEM_FAKE_ITEM_BOX
    u8 star;                // ITEM_STAR
    u8 boo;                 // ITEM_BOO
    u8 mushroom;            // ITEM_MUSHROOM
    u8 doubleMushroom;      // ITEM_DOUBLE_MUSHROOM
    u8 tripleMushroom;      // ITEM_TRIPLE_MUSHROOM
    u8 superMushroom;       // ITEM_SUPER_MUSHROOM
} ItemProbabilities;

// Each row corresponds to a rank, each column to an item
ItemProbabilities grandPrixHumanProbabilityTable[] = {
    { .banana = 30, .bananaBunch = 5, .greenShell = 30, .tripleGreenShell = 5, .redShell = 5, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 10, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 0, .bananaBunch = 5, .greenShell = 5, .tripleGreenShell = 10, .redShell = 15, .tripleRedShell = 20, .blueSpinyShell = 0, .thunderbolt = 5, .fakeItemBox = 5, .star = 5, .boo = 5, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 15, .superMushroom = 5 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 10, .redShell = 20, .tripleRedShell = 20, .blueSpinyShell = 0, .thunderbolt = 5, .fakeItemBox = 0, .star = 10, .boo = 0, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 20, .superMushroom = 10 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 0, .redShell = 15, .tripleRedShell = 20, .blueSpinyShell = 5, .thunderbolt = 10, .fakeItemBox = 0, .star = 15, .boo = 0, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 20, .superMushroom = 10 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 0, .redShell = 10, .tripleRedShell = 20, .blueSpinyShell = 5, .thunderbolt = 10, .fakeItemBox = 0, .star = 15, .boo = 0, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 25, .superMushroom = 10 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 0, .redShell = 0, .tripleRedShell = 20, .blueSpinyShell = 10, .thunderbolt = 15, .fakeItemBox = 0, .star = 20, .boo = 0, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 25, .superMushroom = 10 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 0, .redShell = 0, .tripleRedShell = 20, .blueSpinyShell = 10, .thunderbolt = 20, .fakeItemBox = 0, .star = 30, .boo = 0, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 10, .superMushroom = 10 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 0, .redShell = 0, .tripleRedShell = 20, .blueSpinyShell = 15, .thunderbolt = 20, .fakeItemBox = 0, .star = 30, .boo = 0, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 5, .superMushroom = 10 },
};

ItemProbabilities grandPrixAIProbabilityTable[] = {
    { .banana = 60, .bananaBunch = 0, .greenShell = 25, .tripleGreenShell = 0, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 50, .bananaBunch = 0, .greenShell = 25, .tripleGreenShell = 5, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 40, .bananaBunch = 0, .greenShell = 25, .tripleGreenShell = 10, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 10, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 35, .bananaBunch = 0, .greenShell = 25, .tripleGreenShell = 15, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 10, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 30, .bananaBunch = 0, .greenShell = 20, .tripleGreenShell = 20, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 5, .star = 5, .boo = 0, .mushroom = 20, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 30, .bananaBunch = 0, .greenShell = 20, .tripleGreenShell = 20, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 5, .star = 5, .boo = 0, .mushroom = 20, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 30, .bananaBunch = 0, .greenShell = 20, .tripleGreenShell = 20, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 0, .star = 10, .boo = 0, .mushroom = 20, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 25, .bananaBunch = 0, .greenShell = 20, .tripleGreenShell = 20, .redShell = 0, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 1, .fakeItemBox = 0, .star = 10, .boo = 0, .mushroom = 24, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
};

ItemProbabilities versus2PlayerProbabilityTable[] = {
    { .banana = 25, .bananaBunch = 10, .greenShell = 30, .tripleGreenShell = 5, .redShell = 5, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 10, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 0, .bananaBunch = 5, .greenShell = 0, .tripleGreenShell = 5, .redShell = 5, .tripleRedShell = 15, .blueSpinyShell = 5, .thunderbolt = 15, .fakeItemBox = 0, .star = 15, .boo = 0, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 15, .superMushroom = 20 },
};

ItemProbabilities versus3PlayerProbabilityTable[] = {
    { .banana = 35, .bananaBunch = 5, .greenShell = 30, .tripleGreenShell = 0, .redShell = 5, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 10, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 5, .bananaBunch = 5, .greenShell = 0, .tripleGreenShell = 10, .redShell = 15, .tripleRedShell = 15, .blueSpinyShell = 0, .thunderbolt = 5, .fakeItemBox = 5, .star = 5, .boo = 5, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 20, .superMushroom = 5 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 0, .redShell = 10, .tripleRedShell = 20, .blueSpinyShell = 10, .thunderbolt = 15, .fakeItemBox = 0, .star = 15, .boo = 0, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 20, .superMushroom = 10 },
};

ItemProbabilities versus4PlayerProbabilityTable[] = {
    { .banana = 35, .bananaBunch = 5, .greenShell = 30, .tripleGreenShell = 0, .redShell = 5, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 10, .star = 0, .boo = 5, .mushroom = 10, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 },
    { .banana = 5, .bananaBunch = 5, .greenShell = 5, .tripleGreenShell = 10, .redShell = 15, .tripleRedShell = 15, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 5, .star = 5, .boo = 5, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 25, .superMushroom = 0 },
    { .banana = 0, .bananaBunch = 5, .greenShell = 0, .tripleGreenShell = 5, .redShell = 10, .tripleRedShell = 15, .blueSpinyShell = 5, .thunderbolt = 10, .fakeItemBox = 5, .star = 10, .boo = 0, .mushroom = 5, .doubleMushroom = 0, .tripleMushroom = 25, .superMushroom = 5 },
    { .banana = 0, .bananaBunch = 0, .greenShell = 0, .tripleGreenShell = 0, .redShell = 0, .tripleRedShell = 20, .blueSpinyShell = 10, .thunderbolt = 15, .fakeItemBox = 0, .star = 20, .boo = 0, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 25, .superMushroom = 10 },
};

ItemProbabilities battleProbabilityCurve[] = {
    { .banana = 10, .bananaBunch = 5, .greenShell = 5, .tripleGreenShell = 20, .redShell = 20, .tripleRedShell = 0, .blueSpinyShell = 0, .thunderbolt = 0, .fakeItemBox = 15, .star = 20, .boo = 5, .mushroom = 0, .doubleMushroom = 0, .tripleMushroom = 0, .superMushroom = 0 }
};

void getProbabilityArray(const ItemProbabilities *probStruct, u8 *probArray) {
    probArray[0] = probStruct->banana;
    probArray[1] = probStruct->bananaBunch;
    probArray[2] = probStruct->greenShell;
    probArray[3] = probStruct->tripleGreenShell;
    probArray[4] = probStruct->redShell;
    probArray[5] = probStruct->tripleRedShell;
    probArray[6] = probStruct->blueSpinyShell;
    probArray[7] = probStruct->thunderbolt;
    probArray[8] = probStruct->fakeItemBox;
    probArray[9] = probStruct->star;
    probArray[10] = probStruct->boo;
    probArray[11] = probStruct->mushroom;
    probArray[12] = probStruct->doubleMushroom;
    probArray[13] = probStruct->tripleMushroom;
    probArray[14] = probStruct->superMushroom;
}

/**
 * New random item system uses chance based on percent
 * Likely functionally equivallent to the old system but easier to modify
 */
u8 gen_random_item(s16 rank, s16 isCpu) {
    #define PERCENTAGE_BASE 100
    u16 rand = random_int(PERCENTAGE_BASE);
    #undef PERCENTAGE_BASE
    ItemProbabilities *distributionTable;
    u8 randomItem = 0;
    u8 cumulativeProbability = 0;

    switch(gModeSelection) {
        case GRAND_PRIX:
            if (isCpu == false) {
                distributionTable = &grandPrixHumanProbabilityTable[rank];
            } else {
                distributionTable = &grandPrixAIProbabilityTable[rank];
            }
            break;
        case VERSUS:
            switch (gPlayerCountSelection1) {
            case TWO_PLAYERS_SELECTED:
                distributionTable = &versus2PlayerProbabilityTable[rank];
                break;
            case THREE_PLAYERS_SELECTED:
                distributionTable = &versus3PlayerProbabilityTable[rank];
                break;
            case FOUR_PLAYERS_SELECTED:
                distributionTable = &versus4PlayerProbabilityTable[rank];
                break;
            }
            break;
        case BATTLE:
            distributionTable = &battleProbabilityCurve[rank];
            break;

    }

    u8 itemProbabilities[ITEM_MAX-1];
    getProbabilityArray(distributionTable, itemProbabilities);

    for (int i = 0; i < ITEM_MAX-1; i++) {
        cumulativeProbability += itemProbabilities[i];
        if (rand < cumulativeProbability) {
            randomItem = i + 1; // + 1 to account for the ITEM_NONE spot
            break;
        }
    }

    return randomItem;
}

u8 gen_random_item_human(UNUSED s16 arg0, s16 rank) {
    return gen_random_item(rank, false);
}

u8 kart_ai_gen_random_item(UNUSED s32 arg0, s16 rank) {
    return gen_random_item(rank, true);
}

s16 func_8007AFB0(s32 objectIndex, s32 arg1) {
    UNUSED s32 pad[3];
    s16 randomItem;

    randomItem = gen_random_item_human(gLapCountByPlayerId[arg1], gGPCurrentRaceRankByPlayerId[arg1]);

    if (playerHUD[arg1].itemOverride != 0) {
        randomItem = (s16) playerHUD[arg1].itemOverride;
        playerHUD[arg1].itemOverride = 0;
    }

    func_800729B4(objectIndex, (s32) randomItem);

    return randomItem;
}

s32 func_8007B040(s32 objectIndex, s32 playerId) {
    UNUSED s16 stackPadding;
    s32 temp_v1;
    s32 var_a3;
    s32 var_t3;
    s32 temp_a0;
    s32 var_v1;
    s32 sp50[4];
    s32 sp40[4];
    s32 var_v1_2;
    Player *sp38;
    s16 temp_a1;

    var_a3 = 0;
    var_t3 = 0;
    if (gModeSelection == GRAND_PRIX) {
        if (random_int(0x0064U) < 0x51) {
            var_v1 = gen_random_item_human(gLapCountByPlayerId[playerId], gGPCurrentRaceRankByPlayerId[playerId]);
        } else {
            var_v1 = 0;
            func_800C9060(playerId, 0x1900A058U);
        }
        var_t3 = 1;
        gObjectList[objectIndex].itemDisplay = gObjectList[objectIndex].unk_0A2 = var_v1;
    } else {
        for (var_v1_2 = 0; var_v1_2 < gPlayerCountSelection1; var_v1_2++) {
            temp_a0 = gItemWindowObjectByPlayerId[var_v1_2];
            if (var_v1_2 != playerId) {
                if (gObjectList[temp_a0].type != 0) {
                    sp50[var_a3] = var_v1_2;
                    sp40[var_a3] = gObjectList[temp_a0].type;
                    var_a3 += 1;
                }
            }
        }
        if (var_a3 != 0) {
            var_v1 = random_int(var_a3);
            temp_a1 = sp40[var_v1];
            gObjectList[objectIndex].unk_0A2 = temp_a1;
            gObjectList[objectIndex].itemDisplay = temp_a1;
            temp_v1 = sp50[var_v1];
            gObjectList[objectIndex].unk_0D1 = temp_v1;
            temp_a0 = gItemWindowObjectByPlayerId[temp_v1];
            sp38 = &gPlayerOne[temp_v1];
            func_800722A4(temp_a0, 1);
            gObjectList[temp_a0].type = 0;
            sp38->currentItemCopy = 0;
            if (func_80072320(temp_a0, 2) != 0) {
                func_800722CC(temp_a0, 2);
                gObjectList[temp_a0].timer = 0;
            }
            var_t3 = 1;
        }
    }
    return var_t3;
}

void func_8007B254(s32 objectIndex, s32 arg1) {
    s8 test; // why?

    func_80072428(objectIndex);
    gObjectList[objectIndex].state = 2;
    gObjectList[objectIndex].type = 0;
    gObjectList[objectIndex].tlutList = (u8 *) common_tlut_item_window_none;
    gObjectList[objectIndex].textureList = common_texture_item_window_none;
    gObjectList[objectIndex].textureWidth = 0x28;
    gObjectList[objectIndex].textureHeight = 0x20;
    gObjectList[objectIndex].unk_04C = 0x00000032;
    gObjectList[objectIndex].unk_0D6 = 1;
    gObjectList[objectIndex].timer = 0;
    func_800722A4(objectIndex, 4);
    if (D_80165888 != 0) {
        func_800726CC(objectIndex, 8);
        gObjectList[objectIndex].unk_0D6 = 2;
        gObjectList[objectIndex].type = 0x000E;
        test = gObjectList[objectIndex].type;
        gObjectList[objectIndex].itemDisplay = test;
        gObjectList[objectIndex].unk_0A2 = test;
        playerHUD[PLAYER_ONE].slideItemBoxY = 0;
        D_80165888 = 0;
    } else {
        func_800C8F80(arg1, 0x0100FE1CU);
    }
}

void func_8007B34C(s32 playerId) {
    s32 temp_s0;
    s32 sp40;
    Object *object;
    Player *sp38;
    struct Controller *new_var;

    temp_s0 = gItemWindowObjectByPlayerId[playerId];
    sp38 = &gPlayerOne[playerId];
    sp40 = 0;
    new_var = &gControllerOne[playerId];
    if (new_var->buttonPressed & 0x2000) {
        sp40 = 1;
    }
    if (D_80165888 != 0) {
        init_object(temp_s0, 0);
    }
    object = &gObjectList[temp_s0];
    switch (object->state) {
    case 1:
        func_8007B254(temp_s0, playerId);
        break;
    case 2:
        if (gActiveScreenMode == 0) {
            s16_step_up_towards(&playerHUD[playerId].slideItemBoxY, 0x0040, 4);
            if (playerHUD[playerId].slideItemBoxY == 0x0040) {
                func_80072488(temp_s0);
            }
        } else if (gActiveScreenMode == 3) {
            if ((playerId == 0) || (playerId == 2)) {
                s16_step_up_towards(&playerHUD[playerId].slideItemBoxX, 0x0080, 8);
                if (playerHUD[playerId].slideItemBoxX == 0x0080) {
                    func_80072488(temp_s0);
                }
            } else {
                s16_step_down_towards(&playerHUD[playerId].slideItemBoxX, -0x0080, 8);
                if (playerHUD[playerId].slideItemBoxX == -0x0080) {
                    func_80072488(temp_s0);
                }
            }
        } else {
            s16_step_up_towards(&playerHUD[playerId].slideItemBoxX, 0x0080, 8);
            if (playerHUD[playerId].slideItemBoxX == 0x0080) {
                func_80072488(temp_s0);
            }
        }
        break;
    case 3:
        func_80072E54(temp_s0, 1, 0x0000000F, 1, 2, 2);
        break;
    case 4:
        func_80072E54(temp_s0, 1, 6, 1, 8, 1);
        break;
    case 5:
        func_80072E54(temp_s0, 1, 4, 1, 0x00000010, 1);
        break;
    case 6:
        object->itemDisplay = func_8007AFB0(temp_s0, playerId);
        object->unk_04C = 8;
        object->unk_0D6 = 2;
        func_80072488(temp_s0);
        func_800C9018((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x1C));
        func_800C8F80((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x47));
        break;
    case 7:
        func_80072D3C(temp_s0, (s32) object->unk_0A2, 0, 8, 0x0000000A);
        break;
    case 9:
        func_800722CC(temp_s0, 4);
        func_80073600(temp_s0);
        object->itemDisplay = 0;
        func_80072488(temp_s0);
        break;
    case 10:
        func_8007278C(temp_s0, 0x00000014);
        break;
    case 11:
        if (gActiveScreenMode == 0) {
            if (s16_step_down_towards(&playerHUD[playerId].slideItemBoxY, 0, 4) != 0) {
                func_80072488(temp_s0);
            }
        } else if (gActiveScreenMode == 3) {
            if ((playerId == 0) || (playerId == 2)) {
                s16_step_down_towards(&playerHUD[playerId].slideItemBoxX, 0, 8);
                if (playerHUD[playerId].slideItemBoxX == 0) {
                    func_80072488(temp_s0);
                }
            } else {
                s16_step_up_towards(&playerHUD[playerId].slideItemBoxX, 0, 8);
                if (playerHUD[playerId].slideItemBoxX == 0) {
                    func_80072488(temp_s0);
                }
            }
        } else if (s16_step_down_towards(&playerHUD[playerId].slideItemBoxX, 0, 8) != 0) {
            func_80072488(temp_s0);
        }
        break;
    case 12:
        func_80072428(temp_s0);
        break;
    case 20:
        if (object->unk_0A2 == 0x000B) {
            func_80072D3C(temp_s0, object->unk_0A2, 0, 8, 0x0000000A);
        } else {
            func_80072D3C(temp_s0, object->unk_0A2, 0x0000000B, 8, 0x0000000A);
        }
        break;
    case 21:
        func_800726CC(temp_s0, 8);
        object->unk_0D6 = 2;
        break;
    case 30:
        if (object->unk_0A2 == 0x000B) {
            func_80072D3C(temp_s0, object->unk_0A2, 0, 8, 0x0000000A);
        } else {
            func_80072D3C(temp_s0, object->unk_0A2, 0x0000000B, 8, 0x0000000A);
        }
        break;
    case 31:
        func_800726CC(temp_s0, 9);
        break;
    case 40:
        if (object->unk_0A2 == 0x000D) {
            func_80072D3C(temp_s0, object->unk_0A2, 0x0000000E, 8, 0x0000000A);
        } else {
            func_80072D3C(temp_s0, object->unk_0A2, 0x0000000D, 8, 0x0000000A);
        }
        break;
    case 41:
        func_800726CC(temp_s0, 8);
        break;
    case 50:
        func_80072D3C(temp_s0, (s32) object->unk_0A2, 0, 8, 0x00000064);
        break;
    case 0:
    default:
        break;
    }
    if (func_80072320(temp_s0, 2) != 0) {
        if (object->timer == 0) {
            consume_item(playerId);
        } else {
            object->timer--;
        }
    }
    if (object->unk_04C >= 0) {
        if (object->unk_04C > 0) {
            object->unk_04C--;
        } else {
            switch (object->unk_0D6) {                    /* switch 1; irregular */
            case 1:                                 /* switch 1 */
                if (sp40 != 0) {
                    func_80073600(temp_s0);
                    func_800726CC(temp_s0, 6);
                }
                break;
            case 2:                                 /* switch 1 */
                set_type_object(temp_s0, (s32) object->unk_0A2);
                object->unk_0D6 = 3;
                break;
            case 3:                                 /* switch 1 */
                if (object->type == 0) {
                    if (func_80072354(temp_s0, 1) != 0) {
                        if (object->unk_0A2 == 0x000B) {
                            if (func_8007B040(temp_s0, playerId) != 0) {
                                func_800726CC(temp_s0, 0x00000014);
                                object->unk_0D6 = 0;
                            } else {
                                func_800726CC(temp_s0, 9);
                            }
                        } else if ((object->unk_0A2 == 0x000D) || (object->unk_0A2 == 0x000E)) {
                            object->unk_0A2--;
                            set_type_object(temp_s0, (s32) object->unk_0A2);
                            object->unk_0D6 = 3;
                            func_800726CC(temp_s0, 0x00000028);
                        } else {
                            func_800726CC(temp_s0, 9);
                        }
                    } else {
                        func_800722CC(temp_s0, 1);
                        func_800726CC(temp_s0, 0x0000001E);
                        object->unk_0D6 = 0;
                    }
                }
                break;
            }
        }
    }
    object->activeTLUT = (u8*) gItemWindowTLUTs[object->itemDisplay];
    object->activeTexture = gItemWindowTextures[object->itemDisplay];
    sp38->currentItemCopy = object->type;
}

void func_8007BB9C(s32 arg0) {
    func_8007B34C(arg0);
}

void func_8007BBBC(s32 objectIndex) {
    f32 var_f14;
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {                              /* irregular */
    case 1:
        func_800735BC(objectIndex, d_course_banshee_boardwalk_dl_cheep_cheep, 2.0f);
        set_object_flag_status_true(objectIndex, 0x00000010);
        object->unk_0D5 = 0;
        break;
    case 2:
        if (gIsMirrorMode != 0) {
            func_80087E08(objectIndex, 18.0f, 0.7f, 25.0f, (s16) -0x00005800, 0x0000012C);
        } else {
            func_80087E08(objectIndex, 18.0f, 0.7f, 25.0f, (s16) 0x00005800, 0x0000012C);
        }
        if (object->velocity[2] < 0.0f) {
            var_f14 = -object->velocity[2];
        } else {
            var_f14 = object->velocity[2];
        }
        object->direction_angle[0] = func_80041658(object->velocity[1], var_f14);
        func_8007278C(objectIndex, 0x00000046);
        break;
    case 3:
        func_80072428(objectIndex);
        break;
    case 0:
        break;
    }
}

void func_8007BD04(s32 playerId) {
    s32 objectIndex;

    objectIndex = indexObjectList2[0];
    if (gObjectList[objectIndex].state == 0) {
        if (((s32) gNearestWaypointByPlayerId[playerId] >= 0xA0) && ((s32) gNearestWaypointByPlayerId[playerId] < 0xAB)) {
            set_obj_origin_pos(objectIndex, xOrientation * -1650.0, -200.0f, -1650.0f);
            init_object(objectIndex, 1);
        }
    }
}

void update_cheep_cheep_race(void) {
    UNUSED s32 pad;
    s32 objectIndex;

    func_8007BD04(0);
    objectIndex = indexObjectList2[0];
    func_8007BBBC(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void init_var_cheep_cheep(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    object->unk_0D5 = 1;
    object->status = 0;
    object->model = d_course_banshee_boardwalk_dl_cheep_cheep;
    object->sizeScaling = 0.2f;
    func_80072488(objectIndex);
    set_obj_origin_pos(objectIndex, D_800E634C[0][0], D_800E634C[0][1] + 55.0, D_800E634C[0][2]);
    set_obj_origin_offset(objectIndex, 0.0f, 30.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0x3800U, 0U);
}

void func_8007BEC8(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
    case 1:
        init_var_cheep_cheep(objectIndex);
        break;
    case 2:
        if (func_8007278C(objectIndex, 0x0000003C) != 0) {
            set_object_flag_status_true(objectIndex, 0x00000010);
            func_80086E70(objectIndex);
        }
        break;
    case 3:
        if (object->unk_0AE == 0) {
            func_80072488(objectIndex);
        }
        break;
    case 4:
        if (func_8007278C(objectIndex, 0x0000000A) != 0) {
            func_8008701C(objectIndex, 2);
        }
        break;
    case 5:
        if (object->unk_0AE == 0) {
            func_80072428(objectIndex);
        }
        break;
    case 0:
    default:
        break;
    }
}

void func_8007BFB0(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->unk_0AE) {
    case 0:
        break;
    case 1:
        object->velocity[1] = -0.2f;
        if ((f64) object->offset[1] <= 0.0) {
            object->offset[1] = 0.0f;
            object->velocity[1] = 0.0f;
            func_80086F60(objectIndex);
        }
        break;
    case 2:
        if (func_800871AC(objectIndex, 0x00000014) != 0) {
            object->unk_084[7] = 0x0040;
        }
        break;
    case 3:
        object->sizeScaling = (f32) ((f64) object->sizeScaling - 0.0015);
        if ((s32) object->direction_angle[0] >= 0xA01) {
            object->unk_084[7] -= 4;
        }
        if (u16_step_up_towards(object->direction_angle, 0x0C00U, (u16) object->unk_084[7]) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 4:
        object->sizeScaling = (f32) ((f64) object->sizeScaling - 0.0015);
        object->unk_034 = 0.001f;
        func_80086FD4(objectIndex);
        object->unk_084[7] = 0;
        break;
    case 5:
        if (object->unk_034 <= 0.004) {
            object->unk_034 += 0.0002;
        }
        object->sizeScaling += object->unk_034;
        s16_step_up_towards(&object->unk_084[7], 0x0100, 0x0010);
        object->direction_angle[0] -= object->unk_084[7];
        if (func_80087060(objectIndex, 0x00000035) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 6:
        if (func_80087060(objectIndex, 0x0000000F) != 0) {
            func_80086FD4(objectIndex);
            D_801658CE = 1;
        }
        break;
    case 7:
        object->sizeScaling = (f32) ((f64) object->sizeScaling - 0.05);
        if ((f64) object->sizeScaling <= 0.01) {
            set_object_flag_status_false(objectIndex, 0x00000010);
            object->sizeScaling = 0.000001f;
            func_80086FD4(objectIndex);
        }
        break;
    case 8:
        func_80086F60(objectIndex);
        break;
    }
    if (object->unk_0AE < 0xA) {
        func_80074344(objectIndex, &object->sizeScaling, 0.2f, 0.21f, 0.001f, 0, -1);
    }
    object_add_velocity_offset_y(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void update_cheep_cheep_ending(void) {
    s32 objectIndex;

    objectIndex = indexObjectList2[0];
    if (D_801658BC == 1) {
        D_801658BC = 0;
        init_object(objectIndex, 0);
    }
    if (gObjectList[objectIndex].state != 0) {
        func_8007BEC8(objectIndex);
        func_8007BFB0(objectIndex);
    }
}

void update_cheep_cheep(s32 arg0) {
    switch (arg0) {
        case 0:
            update_cheep_cheep_race();
            break;
        case 1:
            update_cheep_cheep_ending();
            break;
    }
}

void wrapper_update_boos(void) {
    update_boos();
}

// Updates the display status on an object based on its relative direction to the camera
void func_8007C360(s32 objectIndex, Camera* camera) {
    u16 rot = camera->rot[1];
    u16 temp = ((u16)(gObjectList[objectIndex].direction_angle[1] - rot + 0x8000) * 0x24) / 0x10000;

    if (temp < 0x13) {
        set_object_flag_status_false(objectIndex, 0x80);
        gObjectList[objectIndex].itemDisplay = temp;
    } else {
        set_object_flag_status_true(objectIndex, 0x80);
        gObjectList[objectIndex].itemDisplay = 0x24 - temp;
    }

}

void func_8007C420(s32 objectIndex, Player *player, Camera *camera) {
    f32 x;
    f32 z;

    x = player->pos[0] - gObjectList[objectIndex].pos[0];
    z = player->pos[2] - gObjectList[objectIndex].pos[2];
    gObjectList[objectIndex].direction_angle[1] = func_800417B4(gObjectList[objectIndex].direction_angle[1], atan2s(x, z));
    func_8007C360(objectIndex, camera);
}

UNUSED void func_8007C49C(void) {

}

void func_8007C4A4(s32 objectIndex) {
    u16 var_t9;

    var_t9 = gObjectList[objectIndex].direction_angle[1] * 0x24 / 0x10000;

    if (var_t9 < 0x13) {
        set_object_flag_status_false(objectIndex, 0x80);
        gObjectList[objectIndex].itemDisplay = var_t9;
    } else {
        set_object_flag_status_true(objectIndex, 0x80);
        gObjectList[objectIndex].itemDisplay = 0x24 - var_t9;
    }
}

void func_8007C550(s32 objectIndex) {
    gObjectList[objectIndex].direction_angle[1] = func_800417B4(gObjectList[objectIndex].direction_angle[1], atan2s(gObjectList[objectIndex].velocity[0], gObjectList[objectIndex].velocity[2]));
    func_8007C4A4(objectIndex);
}

void func_8007C5B4(s32 objectIndex) {
    Object *object;

    init_texture_object(objectIndex, d_course_banshee_boardwalk_boo_tlut, gTextureGhosts, 48, 40);
    object = &gObjectList[objectIndex];
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    set_object_flag_status_true(objectIndex, 0x00000020);
    func_80072488(objectIndex);
    object->primAlpha = 0;
    func_80073844(objectIndex);
    object->sizeScaling = 0.15f;
    object->unk_034 = 1.0f;
    func_80073FD4(objectIndex);
    func_80086EF0(objectIndex);
    set_object_flag_status_true(objectIndex, 0x00000800);
    object->orientation[0] = 0;
    object->orientation[2] = 0x8000;
}

void func_8007C684(s32 objectIndex) {
    switch(gObjectList[objectIndex].state) {
    case 0:
        break;
    case 1:
        func_8007C5B4(objectIndex);
        break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        switch (gObjectList[objectIndex].unk_0DC) {
        case 0:
            break;
        case 1:
            func_80073998(objectIndex, &gObjectList[objectIndex].primAlpha, 0, 0x00000050, 2, 1, 0);
            break;
        case 2:
            func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000050, 0x00000078, 1, 0, -1);
            break;
        case 3:
            func_80073DC0(objectIndex, &gObjectList[objectIndex].primAlpha, 0, 2);
            break;
        case 4:
            func_80072428(objectIndex);
            func_80086F60(objectIndex);
            func_80073884(objectIndex);
            break;
        }
        func_80073514(objectIndex);
    }
}

void func_8007C7B4(s32 someIndex, s32 arg1) {
    s32 temp_a0;
    s32 objectIndex;
    s16 temp_s1_2;
    s16 temp_s4;
    s16 temp_s5;
    SplineData *something;

    for (temp_a0 = 0; temp_a0 < 5; temp_a0++) {
        objectIndex = indexObjectList3[someIndex + temp_a0];
        init_object(objectIndex, 1);
        gObjectList[objectIndex].unk_0D1 = arg1;
        temp_s1_2 = random_int(0x003CU) - 0x1E;
        temp_s4 = random_int(0x0014U) - 0xA;
        temp_s5 = random_int(0x0050U) - 0x28;
        random_int(0x1000U);
        something = D_800E5D9C[temp_a0];
        gObjectList[objectIndex].spline = something;
        gObjectList[objectIndex].origin_pos[0] = (f32) temp_s1_2;
        gObjectList[objectIndex].origin_pos[1] = (f32) temp_s4;
        gObjectList[objectIndex].origin_pos[2] = (f32) temp_s5;
    }
    func_800C9060(arg1, 0x1900705AU);

    if (someIndex == 0) {
        D_8018CFF0 = 1;
    } else {
        D_8018D048 = 1;
    }
}

void func_8007C91C(s32 someIndex) {
    s32 temp_a0;
    s32 objectIndex;

    for (temp_a0 = 0; temp_a0 < 5; temp_a0++) {
        objectIndex = indexObjectList3[someIndex + temp_a0];
        gObjectList[objectIndex].unk_0DC += 1;
    }

    if (someIndex == 0) {
        D_8018CFF0 = 0;
    } else {
        D_8018D048 = 0;
    }
}

s32 func_8007C9F8(void) {
    s32 ret;
    s32 first, second;
    if (gPlayerCount == 1) {
        ret = 0;
    } else {
        if (gPlayerCount == 2) {
            if (gModeSelection == 0) {
                first = gGPCurrentRaceRankByPlayerId[0];
                second = gGPCurrentRaceRankByPlayerId[1];
                if (first < second) {
                    ret =  0;
                } else {
                    ret =  1;
                }
            } else {
                ret =  gGPCurrentRacePlayerIdByRank[0];
            }
        } else {
            ret =  gGPCurrentRacePlayerIdByRank[0];
        }
    }
    return ret;
}

void func_8007CA70(void) {
    s32 playerId;
    // wtf is up with this pointer? Its such a dumb way of going about things
    u16 *test;

    if (D_8018CFF0 == 0) {
        playerId = func_8007C9F8();
        D_8018D018 = playerId;
        test = &gNearestWaypointByPlayerId[playerId];
        if ((*test >= 0xC9) && (*test < 0xD2)) {
            func_8007C7B4(0, (s32) playerId);
        }
    }
    if (D_8018CFF0 != 0) {
        playerId = D_8018D018;
        test = &gNearestWaypointByPlayerId[playerId];
        if ((*test >= 0xB5) && (*test < 0xBE)) {
            func_8007C91C(0);
        }
        if ((*test >= 0x119) && (*test < 0x122)) {
            func_8007C91C(0);
        }
    }
    if (D_8018D048 == 0) {
        playerId = func_8007C9F8();
        D_8018D110 = playerId;
        test = &gNearestWaypointByPlayerId[playerId];
        if ((*test >= 0x1FF) && (*test < 0x208)) {
            func_8007C7B4(5, (s32) playerId);
        }
    }
    if (D_8018D048 != 0) {
        playerId = D_8018D110;
        test = &gNearestWaypointByPlayerId[playerId];
        if ((*test >= 0x1EB) && (*test < 0x1F4)) {
            func_8007C91C(5);
        }
        if ((*test >= 0x26D) && (*test < 0x276)) {
            func_8007C91C(5);
        }
    }
}

void update_boos(void) {
    u16 temp_t4;
    s32 someIndex;
    s32 objectIndex;
    Player *player;
    Camera *camera;
    Object *object;

    func_8007CA70();
    for (someIndex = 0; someIndex < NUM_BOOS; someIndex++) {
        objectIndex = indexObjectList3[someIndex];
        object = &gObjectList[objectIndex];
        if (object->state != 0) {
            func_8007C684(objectIndex);
            func_8008B78C(objectIndex);
            player = &gPlayerOne[object->unk_0D1];
            camera = &camera1[object->unk_0D1];
            temp_t4 = (0x8000 - camera->rot[1]);
            object->pos[0] = player->pos[0] + (coss(temp_t4) * (object->origin_pos[0] + object->offset[0])) - (sins(temp_t4) * (object->origin_pos[2] + object->offset[2]));
            object->pos[1] = 6.5 + player->unk_074 + object->origin_pos[1] + object->offset[1];
            object->pos[2] = player->pos[2] + (sins(temp_t4) * (object->origin_pos[0] + object->offset[0])) + (coss(temp_t4) * (object->origin_pos[2] + object->offset[2]));
            func_8007C550(objectIndex);
            if (is_obj_flag_status_active(objectIndex, 0x00000080) != 0) {
                object->vertex = D_800E44B0;
            } else {
                object->vertex = D_800E4470;
            }
        }
    }
}

void func_8007CE0C(s32 objectIndex) {
    Object *object;

    init_texture_object(objectIndex, d_course_banshee_boardwalk_boo_tlut, gTextureGhosts, 0x30U, (u16) 0x00000028);
    object = &gObjectList[objectIndex];
    object->itemDisplay = 0x1C;
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    object->sizeScaling = 0.15f;
    func_80072488(objectIndex);
    object->primAlpha = 0;
    func_80073844(objectIndex);
    func_80086EF0(objectIndex);
    object->direction_angle[2] = 0x8000;
    object->direction_angle[1] = atan2s(D_8018CF1C->pos[0] - object->origin_pos[0], D_8018CF1C->pos[2] - object->origin_pos[2]);
}

void func_8007CEDC(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {                              /* switch 1 */
    case 0:                                         /* switch 1 */
        break;
    case 1:                                         /* switch 1 */
        func_8007CE0C(objectIndex);
        break;
    case 2:                                         /* switch 1 */
        func_8007278C(objectIndex, 0x0000001E);
        break;
    case 3:                                         /* switch 1 */
        func_8007278C(objectIndex, 0x000003E8);
        break;
    case 4:                                         /* switch 1 */
        func_80072428(objectIndex);
        break;
    default:                                        /* switch 2 */
        break;
    }
    if (object->state >= 2) {
        func_80072950(objectIndex, (s32) object->unk_0DC, 0, 4);
        func_80073514(objectIndex);
        switch (object->unk_0DC) {                          /* switch 2 */
        case 1:                                     /* switch 2 */
            func_80073998(objectIndex, &object->primAlpha, 0x00000028, 0x00000050, 4, 0, 0);
            break;
        case 2:                                     /* switch 2 */
            func_80073CB0(objectIndex, &object->primAlpha, 0x00000050, 0x000000B4, 2, 0, -1);
            if ((object->unk_0AE == 0) || (object->state == 3)) {
                object->unk_0DC += 1;
            }
            break;
        case 3:                                     /* switch 2 */
            func_80073DC0(objectIndex, &object->primAlpha, 0, 4);
            break;
        case 4:                                     /* switch 2 */
            func_80073884(objectIndex);
            break;
        case 0:
        default:                                        /* switch 2 */
            break;
        }
    }
}

void func_8007D070(void) {
    UNUSED s32 stackPadding;
    UNUSED s32 stackPadding2;
    u16 temp_s3;
    s16 temp_t5;
    s16 temp_s2;
    u16 temp_t2;
    s32 var_v0;
    s32 objectIndex;

    var_v0 = 0;
    if ((D_8016559C == 0) && (D_8018D2A4 != 0) && (D_8018CF68[0] < 0x1D) && (D_800E5DB4[D_8018CF68[0]] == 1)) {
        while (gObjectList[gObjectParticle1[var_v0 + 10]].state != 0) {
            var_v0++;
            if (var_v0 == 30) break;
        }
        objectIndex = gObjectParticle1[var_v0 + 10];
        if (var_v0 != 30) {
            if (gObjectList[objectIndex].state == 0) {
                init_object(objectIndex, 1);
                temp_s2 = random_int(0x012CU);
                temp_s3 = random_int(0x1000U) - 0x800;
                temp_t5 = random_int(0x000FU) - 5;
                gObjectList[objectIndex].direction_angle[1] = D_8018CF1C->rotation[1] + 0x8000;
                temp_t2 = (D_8018CF14->rot[1] + temp_s3);
                gObjectList[objectIndex].origin_pos[0] = D_8018CF1C->pos[0] + (sins(temp_t2) * temp_s2);
                gObjectList[objectIndex].origin_pos[1] = temp_t5;
                gObjectList[objectIndex].origin_pos[2] = D_8018CF1C->pos[2] + (coss(temp_t2) * temp_s2);
                gObjectList[objectIndex].spline = &D_800E5D54;
            }
        }
    }
    for (var_v0 = 0; var_v0 < 30; var_v0++) {
        objectIndex = gObjectParticle1[var_v0 + 10];
        if (gObjectList[objectIndex].state != 0) {
            func_8007CEDC(objectIndex);
            func_8008B724(objectIndex);
            gObjectList[objectIndex].pos[0] = gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0];
            gObjectList[objectIndex].pos[1] = D_8018CF1C->unk_074 + gObjectList[objectIndex].origin_pos[1] + gObjectList[objectIndex].offset[1];
            gObjectList[objectIndex].pos[2] = gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2];
            func_8007C420(objectIndex, D_8018CF1C, D_8018CF14);
            if (is_obj_flag_status_active(objectIndex, 0x00000080) != 0) {
                gObjectList[objectIndex].vertex = D_800E44B0;
            } else {
                gObjectList[objectIndex].vertex = D_800E4470;
            }
        }
    }
}

void func_8007D360(s32 objectIndex, s32 arg1) {
    if (arg1 == 1) {
        gObjectList[objectIndex].origin_pos[0] = (f32) ((-1775.0 - random_int(0x001EU)) * (f64) xOrientation);
        gObjectList[objectIndex].origin_pos[1] = (f32) (random_int(0x0019U) + 25.0);
        gObjectList[objectIndex].origin_pos[2] = (f32) (random_int(0x001EU) + 130.0);
        gObjectList[objectIndex].unk_01C[0] = (f32) ((f64) xOrientation * -2500.0);
        gObjectList[objectIndex].unk_01C[1] = 0.0f;
        gObjectList[objectIndex].unk_01C[2] = (f32) (220.0 - random_int(0x0096U));
        gObjectList[objectIndex].direction_angle[0] = 0xDC00;
        gObjectList[objectIndex].unk_0C6 = 0x0800;
    }
    if (arg1 == 2) {
        gObjectList[objectIndex].origin_pos[0] = (f32) (-0x55B - random_int(0x001EU)) * xOrientation;
        gObjectList[objectIndex].origin_pos[1] = (f32) (random_int(0x0019U) + 0xF);
        gObjectList[objectIndex].origin_pos[2] = (f32) (random_int(0x001EU) - 0xE8);
        gObjectList[objectIndex].unk_01C[0] = (f32) ((f64) xOrientation * -2100.0);
        gObjectList[objectIndex].unk_01C[1] = 0.0f;
        gObjectList[objectIndex].unk_01C[2] = (f32) (random_int(0x00C8U) + -290.0);
        gObjectList[objectIndex].direction_angle[0] = 0;
        gObjectList[objectIndex].unk_0C6 = 0;
    }
    gObjectList[objectIndex].direction_angle[1] = get_angle_between_xy(gObjectList[objectIndex].origin_pos[0], gObjectList[objectIndex].unk_01C[0], gObjectList[objectIndex].origin_pos[2], gObjectList[objectIndex].unk_01C[2]);
    gObjectList[objectIndex].direction_angle[2] = 0;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    gObjectList[objectIndex].unk_0B0 = 0;
    if (gCCSelection < CC_150) {
       gObjectList[objectIndex].unk_034 = (random_int(4U) + 4.0);
    } else {
        gObjectList[objectIndex].unk_034 = (random_int(4U) + 5.0);
    }
}

void func_8007D6A8(s32 objectIndex, s32 arg1) {
    UNUSED s32 pad[2];
    Object *object;

    object = &gObjectList[objectIndex];
    object->unk_0D5 = 0x0D;
    func_8007D360(objectIndex, arg1);
    object->sizeScaling = 0.1f;
    set_object_flag_status_true(objectIndex, 0x00000200);
    object->boundingBoxSize = 3;
}

void func_8007D714(s32 arg0) {
    s32 objectIndex;

    if (arg0 == 1) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, 0x28);
    } else {
        objectIndex = add_unused_obj_index(gObjectParticle3, &gNextFreeObjectParticle3, 0x1E);
    }
    if (objectIndex != NULL_OBJECT_ID) {
        init_object(objectIndex, 0);
        func_8007D6A8(objectIndex, arg0);
    }
}

void func_8007D794(s32 objectIndex) {
    if (gCCSelection == CC_50) {
        func_80089A04(objectIndex, 0.5f, 0.2f);
    } else if (gCCSelection == CC_100) {
        func_80089A04(objectIndex, 0.4f, 0.15f);
    } else {
        func_80089A04(objectIndex, 0.25f, 0.1f);
    }
}

s32 func_8007D804(s32 objectIndex) {
    s32 loopIndex;
    s32 someCount;

    someCount = 0;
    for (loopIndex = 0; loopIndex < gPlayerCountSelection1; loopIndex++) {
        if (is_object_visible_on_camera(objectIndex, &camera1[loopIndex], 0x4000U) != 0) {
            someCount += 1;
        }
    }
    return someCount;
}

void func_8007D8AC(s32 arg0) {
    func_80072488(arg0);
    func_80086E70(arg0);
}

void func_8007D8D4(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        func_8007D8AC(objectIndex);
        break;
    case 2:
        if (func_8007D804(objectIndex) == 0) {
            func_80072428(objectIndex);
        }
        if (arg1 == 1) {
            if (gIsMirrorMode != 0) {
                if (gObjectList[objectIndex].pos[0] >= 2540.0) {
                    func_80072428(objectIndex);
                }
            } else if (gObjectList[objectIndex].pos[0] <= -2540.0) {
                func_80072428(objectIndex);
            }
        } else if (gIsMirrorMode != 0) {
            if (gObjectList[objectIndex].pos[0] >= 2150.0) {
                func_80072428(objectIndex);
            }
        } else {
            if (gObjectList[objectIndex].pos[0] <= -2150.0) {
                func_80072428(objectIndex);
            }
        }
        break;
    case 0:
        break;
    }
}

void func_8007DA4C(s32 objectIndex) {
    func_8008781C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void func_8007DA74(s32 objectIndex) {
    UNUSED s32 pad;
    if ((gObjectList[objectIndex].unk_0AE != 0) && (gObjectList[objectIndex].unk_0AE == 1)) {
        if (func_80087060(objectIndex, 0x0000001E) != 0) {
            gObjectList[objectIndex].unk_0C6 = 0U;
        }
    }
    gObjectList[objectIndex].direction_angle[0] = func_800417B4(gObjectList[objectIndex].direction_angle[0], gObjectList[objectIndex].unk_0C6);
    func_80087844(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void func_8007DAF8(s32 objectIndex, s32 arg1) {
    switch (arg1) {                                 /* irregular */
    case 1:
        func_8007DA74(objectIndex);
        return;
    case 2:
        func_8007DA4C(objectIndex);
        return;
    }
}

void update_bat(void) {
    s32 var_s2;
    s32 temp_s0;
    Object *object;

    if (D_8018CFC8 != 0) {
        D_8018CFC8 -= 1;
    }
    if (D_8018D000 != 0) {
        D_8018D000 -= 1;
    }
    temp_s0 = indexObjectList1[0];
    func_80072E54(temp_s0, 0, 3, 1, 0, -1);
    func_80073514(temp_s0);
    object = &gObjectList[temp_s0];
    func_80073CB0(temp_s0, &object->primAlpha, -0x00001000, 0x00001000, 0x00000400, 0, -1);
    object->orientation[2] = object->primAlpha + 0x8000;
    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        D_8018CFD8 = 0;
        for (var_s2 = 0; var_s2 < 40; var_s2++) {
            temp_s0 = gObjectParticle2[var_s2];
            if (temp_s0 == -1) continue;

            object = &gObjectList[temp_s0];
            if (object->state == 0) continue;

            func_8007D8D4(temp_s0, 1);
            func_8007DAF8(temp_s0, 1);
            func_8007D794(temp_s0);
            if (object->state == 0) {
                delete_object_wrapper(&gObjectParticle2[var_s2]);
            }
            D_8018CFD8 += 1;
        }
        if (D_8018CFD8 != 0) {
            D_8018CFC8 = 0x012C;
        }
    }
    if ((D_8018CFE8 != 0) || (D_8018D000 != 0)) {
        D_8018D010 = 0;
        for (var_s2 = 0; var_s2 < 30; var_s2++) {
            temp_s0 = gObjectParticle3[var_s2];
            if (temp_s0 == -1) continue;

            object = &gObjectList[temp_s0];
            if (object->state == 0) continue;

            func_8007D8D4(temp_s0, 2);
            func_8007DAF8(temp_s0, 2);
            func_8007D794(temp_s0);
            if (object->state == 0) {
                delete_object_wrapper(&gObjectParticle3[var_s2]);
            }
            D_8018D010 += 1;
        }
        if (D_8018D010 != 0) {
            D_8018D000 = 0x012C;
        }
    }
}

void func_8007DDC0(s32 objectIndex) {
    f32 sp2C;
    Object *object;

    object = &gObjectList[objectIndex];
    if (object->unk_04C > 0) {
        object->unk_04C--;
        if (object->unk_04C == 0) {
            func_800722CC(objectIndex, 1);
        }
    }
    if (object->unk_048 > 0) {
        object->unk_048--;
        if (object->unk_048 == 0) {
            func_800C9EF4(object->pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
        }
    }
    if (object->unk_04C == 0) {
        if ((gCCSelection == CC_50) || (gCCSelection == CC_100) || (gCCSelection == CC_150) || (gCCSelection == CC_EXTRA)) {
            sp2C = 1150.0f;
        }
        func_8008A6DC(objectIndex, sp2C);
        if ((is_obj_flag_status_active(objectIndex, VISIBLE) != 0) && (func_80072354(objectIndex, 1) != 0)) {
            func_800722A4(objectIndex, 1);
            func_800C9D80(object->pos, object->velocity, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
            func_800726CC(objectIndex, 3);
            if (object->type > 0) {
                object->type--;
                object->unk_04C = 0x00000168;
            } else {
                object->unk_04C = 0x00000168;
            }
            object->unk_048 = 0x0000012C;
        }
    }
    if (func_8008A8B0(0x000F, 0x0012) == 0) {
        object->type = 2;
    }
}

void init_bb_trash_bin(s32 objectIndex) {
    gObjectList[objectIndex].sizeScaling = 1.0f;
    gObjectList[objectIndex].model = d_course_banshee_boardwalk_dl_trash_bin;
    gObjectList[objectIndex].unk_04C = 0;
    gObjectList[objectIndex].unk_084[7] = 0;
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    if (gIsMirrorMode != 0) {
        gObjectList[objectIndex].pos[0] = 1765.0f;
        gObjectList[objectIndex].pos[2] = 195.0f;
        gObjectList[objectIndex].orientation[1] = 0x8000;
    } else {
        gObjectList[objectIndex].pos[0] = -1765.0f;
        gObjectList[objectIndex].pos[2] = 70.0f;
    }
    gObjectList[objectIndex].pos[1] = 45.0f;
    set_obj_velocity(objectIndex, 0.0f, 0.0f, 0.0f);
    gObjectList[objectIndex].type = 0;
    func_80072488(objectIndex);
}

void func_8007E00C(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        init_bb_trash_bin(objectIndex);
        break;
    case 3:
        D_8018CFB0 = 1;
        func_80072488(objectIndex);
        break;
    case 4:
        func_8007278C(objectIndex, 0x000000D2);
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
            func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4E));
            gObjectList[objectIndex].unk_084[7] = 0x0014;
        } else {
            gObjectList[objectIndex].unk_084[7]--;
        }
        break;
    case 5:
        gObjectList[objectIndex].orientation[2] = func_800417B4(gObjectList[objectIndex].orientation[2], 0U);
        if (gObjectList[objectIndex].orientation[2] == 0) {
            func_80072488(objectIndex);
        }
        break;
    case 6:
        gObjectList[objectIndex].orientation[2] = 0;
        gObjectList[objectIndex].unk_084[7] = 0;
        func_80072488(objectIndex);
        D_8018CFB0 = 0;
        break;
    case 0:
    case 2:
    default:
        break;
    }
}

void update_trash_bin(void) {
    s32 objectIndex = indexObjectList1[1];
    func_8007E00C(objectIndex);
    if (gModeSelection != TIME_TRIALS) {
        func_8007DDC0(objectIndex);
    }
}

void func_8007E1F4(s32 objectIndex) {
    f32 sp2C;
    Object *object;

    object = &gObjectList[objectIndex];
    if (object->unk_04C > 0) {
        object->unk_04C--;
        if (object->unk_04C == 0) {
            func_800722CC(objectIndex, 1);
        }
    }
    if (object->unk_048 > 0) {
        object->unk_048--;
        if (object->unk_048 == 0) {
            func_800C9EF4(object->pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
        }
    }
    if (object->unk_04C == 0) {
        if ((gCCSelection == CC_50) || (gCCSelection == CC_100) || (gCCSelection == CC_150) || (gCCSelection == CC_EXTRA)) {
            sp2C = 700.0f;
        }
        func_8008A6DC(objectIndex, sp2C);
        if ((is_obj_flag_status_active(objectIndex, VISIBLE) != 0) && (func_80072354(objectIndex, 1) != 0)) {
            func_800722A4(objectIndex, 1);
            func_800C9D80(object->pos, object->velocity, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
            func_800726CC(objectIndex, 3);
            if (object->type > 0) {
                object->type--;
                object->unk_04C = 0x00000168;
            } else {
                object->unk_04C = 0x00000168;
            }
            object->unk_048 = 0x0000012C;
        }
    }
    if (func_8008A8B0(0x000F, 0x0013) == 0) {
        object->type = 2;
    }
}

void func_8007E358(s32 objectIndex) {
    gObjectList[objectIndex].pos[0] = -1371.0f * xOrientation;
    gObjectList[objectIndex].pos[1] = 31.0f;
    gObjectList[objectIndex].pos[2] = -217.0f;
    set_obj_velocity(objectIndex, 0.0f, 0.0f, 0.0f);
    gObjectList[objectIndex].type = 0;
    gObjectList[objectIndex].unk_04C = 0;
    gObjectList[objectIndex].unk_084[7] = 0;
    func_80072488(objectIndex);
}

void func_8007E3EC(s32 objectIndex) {

    switch (gObjectList[objectIndex].state) {
    case 1:
        func_8007E358(objectIndex);
        break;
    case 3:
        D_8018CFE8 = 1;
        func_80072488(objectIndex);
        break;
    case 4:
        func_8007278C(objectIndex, 0x000000D2);
        if (D_80165598 == 0) {
            if (gCCSelection < CC_150) {
                func_8007D714(2);
            } else {
                func_8007D714(2);
                func_8007D714(2);
            }
        }
        break;
    case 5:
        D_8018CFE8 = 0;
        func_80072488(objectIndex);
        break;
    case 0:
    case 2:
    default:
        break;
    }
}

void func_8007E4C4(void) {
    s32 objectIndex = indexObjectList1[2];
    func_8007E3EC(objectIndex);
    if (gModeSelection != TIME_TRIALS) {
        func_8007E1F4(objectIndex);
    }
}

s32 func_8007E50C(s32 objectIndex, Player *player, Camera *camera) {
    s32 sp24;

    sp24 = 0;
    if ((func_80072354(objectIndex, 4) != 0) && (is_within_horizontal_distance_of_player(objectIndex, player, 300.0f) != 0) && (func_8008A0B4(objectIndex, player, camera, 0x4000U) != 0) && (func_8008A060(objectIndex, camera, 0x1555U) != 0)) {
        func_800722A4(objectIndex, 4);
        sp24 = 1;
    }
    return sp24;
}

s32 func_8007E59C(s32 objectIndex) {
    Camera *camera;
    Player *player;
    s32 temp_v0;
    s32 someIndex;

    temp_v0 = 0;
    player = gPlayerOne;
    camera = camera1;
    for (someIndex = 0; someIndex < gPlayerCountSelection1; someIndex++) {
        temp_v0 = func_8007E50C(objectIndex, player++, camera++);
        if (temp_v0 != 0) break;
    }
    return temp_v0;
}

void func_8007E63C(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 0x32:
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], gObjectList[objectIndex].unk_01C[1] + 15.0, 1.5f) != 0) {
            set_object_flag_status_true(objectIndex, 0x00000200);
            func_800722A4(objectIndex, 1);
            func_800722CC(objectIndex, 2);
            func_80072488(objectIndex);
        }
        break;
    case 0x33:
        if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 0.0f, 2.0f) != 0) {
            if (gObjectList[objectIndex].offset[1] >= 16.0f) {
                gObjectList[objectIndex].itemDisplay = 0;
            } else if (gObjectList[objectIndex].offset[1] >= 8.0f) {
                gObjectList[objectIndex].itemDisplay = 1;
            } else {
                gObjectList[objectIndex].itemDisplay = 2;
            }
            func_800722CC(objectIndex, 1);
            if (is_obj_flag_status_active(objectIndex, 0x00010000) != 0) {
                func_800722A4(objectIndex, 0x00000010);
                if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                    func_800722A4(objectIndex, 0x00000020);
                }
            }
            if (func_80072320(objectIndex, 2) != 0) {
                func_800726CC(objectIndex, 0x00000064);
            } else {
                func_80072488(objectIndex);
            }
        }
        break;
    case 0x34:
        func_80072AAC(objectIndex, 3, 6);
        break;
    case 0x35:
        func_80072AAC(objectIndex, 2, 0x00000032);
        break;
    case 0x36:
        if (gObjectList[objectIndex].offset[1] >= 20.0f) {
            gObjectList[objectIndex].itemDisplay = 0;
        } else if (gObjectList[objectIndex].offset[1] >= 18.0f) {
            gObjectList[objectIndex].itemDisplay = 1;
        }
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], gObjectList[objectIndex].unk_01C[1], 0.5f) != 0) {
            set_object_flag_status_false(objectIndex, 0x00000200);
            func_8007266C(objectIndex);
        }
        break;
    case 0x64:
        func_80072E54(objectIndex, 3, 5, 1, 8, 0);
        break;
    case 0x65:
        func_8007278C(objectIndex, 0x0000001E);
        break;
    case 0x66:
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], 20.0f, 1.5f) != 0) {
            func_80072488(objectIndex);
        }
        break;
    case 0x67:
        if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 0.0f, 1.5f) != 0) {
            if (is_obj_flag_status_active(objectIndex, 0x00020000) != 0) {
                func_800722A4(objectIndex, 0x00000010);
                if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                    func_800722A4(objectIndex, 0x00000020);
                }
            }
            func_80072488(objectIndex);
        }
        break;
    case 0x68:
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], 12.0f, 1.5f) != 0) {
            func_80072488(objectIndex);
        }
        break;
    case 0x69:
        if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 0.0f, 1.5f) != 0) {
            if (is_obj_flag_status_active(objectIndex, 0x00020000) != 0) {
                func_800722A4(objectIndex, 0x00000010);
                if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                    func_800722A4(objectIndex, 0x00000020);
                }
            }
            func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
            func_80072488(objectIndex);
        }
        break;
    case 0x6A:
        if (func_8007326C(objectIndex, 5, 3, 1, 6, 3) != 0) {
            func_80080DE4(objectIndex);
        }
        break;
    case 0x6B:
        if (gObjectList[objectIndex].offset[1] >= 22.0f) {
            gObjectList[objectIndex].itemDisplay = 0;
        } else if (gObjectList[objectIndex].offset[1] >= 20.0f) {
            gObjectList[objectIndex].itemDisplay = 1;
        } else if (gObjectList[objectIndex].offset[1] >= 18.0f) {
            gObjectList[objectIndex].itemDisplay = 2;
        } else if (gObjectList[objectIndex].offset[1] >= 16.0f) {
            gObjectList[objectIndex].itemDisplay = 3;
        } else if (gObjectList[objectIndex].offset[1] >= 14.0f) {
            gObjectList[objectIndex].itemDisplay = 4;
        } else {
            func_800730BC(objectIndex, 3, 5, 1, 6, -1);
        }
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], gObjectList[objectIndex].unk_01C[1], 0.5f) != 0) {
            set_object_unk_0CB(objectIndex, 0);
            func_80072488(objectIndex);
        }
        break;
    case 0x6C:
        if (func_8007278C(objectIndex, 0x00000064) != 0) {
            func_800722CC(objectIndex, 2);
            set_object_flag_status_false(objectIndex, 0x00000200);
            func_8007266C(objectIndex);
        }
        break;
    case 0xC8:
        if (func_8007278C(objectIndex, 0x0000012C) != 0) {
            func_80072320(objectIndex, 0x00000080);
            func_80072428(objectIndex);
            func_800726CC(objectIndex, 1);
        }
        break;
    case 0x12C:
        if (func_80073E18(objectIndex, &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
            func_800722CC(objectIndex, 4);
            func_8007266C(objectIndex);
        }
        break;
    }
}

void func_8007EC30(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    if (gIsMirrorMode != 0) {
        set_obj_direction_angle(objectIndex, 0U, 0x4000U, 0U);
        set_obj_orientation(objectIndex, 0U, 0x4000U, 0U);
    } else {
        set_obj_direction_angle(objectIndex, 0U, 0xC000U, 0U);
        set_obj_orientation(objectIndex, 0U, 0xC000U, 0U);
    }
    init_texture_object(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object->model = d_course_bowsers_castle_dl_thwomp;
    object->boundingBoxSize = 0x000C;
    object->sizeScaling = 1.0f;
    object->unk_01C[1] = 30.0f;
    set_object_flag_status_true(objectIndex, 0x05000220);
    object->type = 0;
    object->unk_0DF = 6;
    func_800724DC(objectIndex);
    func_80072488(objectIndex);
}

void func_8007ED6C(s32 objectIndex) {
    UNUSED s32 stackPadding[4];
    switch (gObjectList[objectIndex].state) {
    case 0:
        break;
    case 1:
        func_8007EC30(objectIndex);
        break;
    case 2:
        func_8007278C(objectIndex, 0x0000003C);
        break;
    case 3:
        func_80072568(objectIndex, 0x00000032);
        break;
    case 4:
        if (func_8007E59C(objectIndex) != 0) {
            func_800725E8(objectIndex, 0x0000012C, 2);
        } else {
            func_800726CC(objectIndex, 2);
        }
        break;
    }
    func_8007E63C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    gObjectList[objectIndex].direction_angle[1] = gObjectList[objectIndex].orientation[1];
    func_80073514(objectIndex);
}

void func_8007EE5C(s32 objectIndex) {
    Object *object;

    init_texture_object(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->sizeScaling = 1.0f;
    object->model = d_course_bowsers_castle_dl_thwomp;
    object->boundingBoxSize = 0x000C;
    set_object_flag_status_true(objectIndex, 0x04000220);
    object->type = 0;
    object->unk_0DF = 6;
    func_80086E70(objectIndex);
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 20.0f, 0.0f);
    object->unk_01C[1] = 20.0f;
    if (gIsMirrorMode != 0) {
        set_obj_direction_angle(objectIndex, 0U, 0x4000U, 0U);
        set_obj_orientation(objectIndex, 0U, 0x4000U, 0U);
    } else {
        set_obj_direction_angle(objectIndex, 0U, 0xC000U, 0U);
        set_obj_orientation(objectIndex, 0U, 0xC000U, 0U);
    }
    object->unk_0AE = 1;
    if (object->primAlpha == 0) {
        object->unk_0DD = 1;
    } else {
        object->unk_0DD = 2;
    }
    func_80072488(objectIndex);
}

void func_8007EFBC(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 1:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0800U, 0x00008000) != 0) {
            gObjectList[objectIndex].unk_01C[0] = (f32) ((f64) xOrientation * 200.0);
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        if (f32_step_towards(gObjectList[objectIndex].offset, gObjectList[objectIndex].unk_01C[0], 4.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 3:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
            func_800726CC(objectIndex, 3);
            func_80086FD4(objectIndex);
        }
        break;
    case 5:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 6:
        if (f32_step_down_towards(&gObjectList[objectIndex].offset[2], -100.0f, 2.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 7:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00004000) != 0) {
            func_80086FD4(objectIndex);
            func_800726CC(objectIndex, 3);
        }
        break;
    case 9:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 10:
        if (f32_step_towards(gObjectList[objectIndex].offset, 0.0f, 4.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 11:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
            func_80086FD4(objectIndex);
            func_800726CC(objectIndex, 3);
        }
        break;
    case 13:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00014000) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 14:
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[2], 0.0f, 2.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 15:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
            func_80086FD4(objectIndex);
            func_800726CC(objectIndex, 3);
        }
        break;
    case 17:
        func_8008701C(objectIndex, 1);
        break;
    case 0:
    default:
        break;
    }
}

void func_8007F280(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 1:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
            gObjectList[objectIndex].unk_01C[0] = (f32) ((f64) xOrientation * -200.0);
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        if (f32_step_towards(gObjectList[objectIndex].offset, gObjectList[objectIndex].unk_01C[0], 4.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 3:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
            func_800726CC(objectIndex, 3);
            func_80086FD4(objectIndex);
        }
        break;
    case 5:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00004000) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 6:
        if (f32_step_up_towards(&gObjectList[objectIndex].offset[2], 100.0f, 2.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 7:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
            func_80086FD4(objectIndex);
            func_800726CC(objectIndex, 3);
        }
        break;
    case 9:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 10:
        if (f32_step_towards(gObjectList[objectIndex].offset, 0.0f, 4.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 11:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
            func_80086FD4(objectIndex);
            func_800726CC(objectIndex, 3);
        }
        break;
    case 13:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 14:
        if (f32_step_down_towards(&gObjectList[objectIndex].offset[2], 0.0f, 2.0f) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 15:
        if (func_80073E18(objectIndex, (u16 *) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00014000) != 0) {
            func_80086FD4(objectIndex);
            func_800726CC(objectIndex, 3);
        }
        break;
    case 17:
        func_8008701C(objectIndex, 1);
        break;
    case 0:
    default:
        break;
    }
}

void func_8007F544(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0DD) {                              /* irregular */
    case 1:
        func_8007EFBC(objectIndex);
        break;
    case 2:
        func_8007F280(objectIndex);
        break;
    }
}

void func_8007F5A8(s32 objectIndex) {

    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 0:
        break;
    case 1:
        func_8007EE5C(objectIndex);
        break;
    case 3:
        func_80072568(objectIndex, 0x00000032);
        break;
    case 4:
        func_80086FD4(objectIndex);
        func_80072488(objectIndex);
        break;
    }
    func_8007E63C(objectIndex);
    func_8007F544(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

void func_8007F660(s32 objectIndex, s32 arg1, s32 arg2) {
    Object *object;

    func_800722A4(objectIndex, 8);
    func_80086E70(objectIndex);
    object = &gObjectList[objectIndex];
    object->unk_0DD = 1;
    object->unk_0D1 = arg1;
    object->unk_048 = arg2;
}

void func_8007F6C4(s32 objectIndex, s32 playerId) {
    Player *player;

    player = &gPlayerOne[playerId];
    func_800722A4(objectIndex, 8);
    func_80086E70(objectIndex);
    gObjectList[objectIndex].unk_0DD = 2;
    gObjectList[objectIndex].unk_01C[0] = player->pos[0] - gObjectList[objectIndex].origin_pos[0];
    gObjectList[objectIndex].unk_0D1 = playerId;
}

s32 func_8007F75C(s32 playerId) {
    s32 someIndex;
    s32 objectIndex;
    s32 temp_s7;
    s32 var_s6;
    s32 waypoint;

    waypoint = gNearestWaypointByPlayerId[playerId];
    var_s6 = 0;
    if ((waypoint >= 0xAA) && (waypoint < 0xB5)) {
        temp_s7 = random_int(0x0032U) + 0x32;
        for (someIndex = 0; someIndex < gNumActiveThwomps; someIndex++) {
            objectIndex = indexObjectList1[someIndex];
            if (gObjectList[objectIndex].unk_0D5 == 3) {
                var_s6 = 1;
                func_8007F660(objectIndex, playerId, temp_s7);
            }
        }
    } else if ((waypoint >= 0xD7) && (waypoint < 0xE2)) {
        for (someIndex = 0; someIndex < gNumActiveThwomps; someIndex++) {
            objectIndex = indexObjectList1[someIndex];
            if (gObjectList[objectIndex].unk_0D5 == 3) {
                var_s6 = 1;
                func_8007F6C4(objectIndex, playerId);
            }
        }
    }
    return var_s6;
}

void func_8007F8D8(void) {
    Player *player;
    s32 objectIndex;
    s32 var_s0;
    s32 someIndex;
    s32 var_s4;
    Object *object;

    player = gPlayerOne;
    var_s4 = 1;
    for (someIndex = 0; someIndex < gNumActiveThwomps; someIndex++){
        objectIndex = indexObjectList1[someIndex];
        object = &gObjectList[objectIndex];
        if (object->unk_0D5 == 3) {
            var_s0 = 0;
            if ((object->state >= 2) && (func_80072354(objectIndex, 8) != 0)) {
                var_s0 = 1;
            }
            var_s4 *= var_s0;
        }
    }
    if (var_s4 != 0) {
        for (var_s0 = 0; var_s0 < 4; var_s0++, player++){
            if ((player->type & PLAYER_EXISTS) && !(player->type & PLAYER_KART_AI)) {
                if (func_8007F75C(var_s0) != 0) break;
            }
        }
    }
}

void func_8007FA08(s32 objectIndex) {
    Object *object;

    init_texture_object(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->model = d_course_bowsers_castle_dl_thwomp;
    object->boundingBoxSize = 0x000C;
    object->sizeScaling = 1.0f;
    set_object_flag_status_true(objectIndex, 0x04000220);
    object->type = 0;
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    if (gIsMirrorMode != 0) {
        set_obj_orientation(objectIndex, 0U, 0xC000U, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, 0x4000U, 0U);
    }
    object->velocity[0] = 0.0f;
    object->direction_angle[1] = object->orientation[1];
    object->unk_0DD = 1;
    object->unk_0DF = 8;
    object->offset[1] = 15.0f;
    object->unk_01C[1] = 15.0f;
    func_80072488(objectIndex);
}

void func_8007FB48(s32 objectIndex) {
    s32 var_v0;
    UNUSED s32 stackPadding;
    Player *player;

    player = &gPlayerOne[gObjectList[objectIndex].unk_0D1];
    switch (gObjectList[objectIndex].unk_0AE) {
    case 1:
        gObjectList[objectIndex].unk_0B0 = 0x00A0;
        gObjectList[objectIndex].offset[0] = 0.0f;
        gObjectList[objectIndex].offset[2] = 0.0f;
        gObjectList[objectIndex].velocity[2] = 0.0f;
        func_80086FD4(objectIndex);
        break;
    case 2:
        gObjectList[objectIndex].velocity[0] = player->unk_094 * xOrientation * 1.25;
        if (gObjectList[objectIndex].unk_048 >= gObjectList[objectIndex].unk_0B0) {
            if (gObjectList[objectIndex].unk_0B0 == gObjectList[objectIndex].unk_048) {
                if (D_8018D400 & 1) {
                    gObjectList[objectIndex].velocity[2] = 1.5f;
                } else {
                    gObjectList[objectIndex].velocity[2] = -1.5f;
                }
            }
            if (gObjectList[objectIndex].velocity[2] >= 0.0) {
                if (gObjectList[objectIndex].offset[2] >= 40.0) {
                    gObjectList[objectIndex].velocity[2] = -1.5f;
                }
            } else if ((f64) gObjectList[objectIndex].offset[2] <= -40.0) {
                gObjectList[objectIndex].velocity[2] = 1.5f;
            }
        }
        object_add_velocity_offset_xz(objectIndex);
        if (gObjectList[objectIndex].unk_0B0 < 0x65) {
            gObjectList[objectIndex].orientation[1] = func_800417B4(gObjectList[objectIndex].orientation[1], (gObjectList[objectIndex].direction_angle[1] + 0x8000));
            if (gObjectList[objectIndex].unk_0B0 == 0x0064) {
                gObjectList[objectIndex].itemDisplay = 1;
            }
        }
        var_v0 = 0;
        if (gIsMirrorMode != 0) {
            if (gObjectList[objectIndex].offset[0] <= -1000.0) {
                var_v0 = 1;
            }
        } else if (gObjectList[objectIndex].offset[0] >= 1000.0) {
            var_v0 = 1;
        }
        gObjectList[objectIndex].unk_0B0--;
        if ((gObjectList[objectIndex].unk_0B0 == 0) || (var_v0 != 0)) {
            gObjectList[objectIndex].unk_034 = 0.0f;
            func_800726CC(objectIndex, 3);
            func_80086FD4(objectIndex);
        }
        break;
    case 4:
        f32_step_towards(&gObjectList[objectIndex].offset[2], 0.0f, 2.0f);
        f32_step_towards(gObjectList[objectIndex].offset, 0.0f, 5.0f);
        if ((gObjectList[objectIndex].offset[0] + gObjectList[objectIndex].offset[2]) == 0.0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 5:
        gObjectList[objectIndex].orientation[1] = func_800417B4(gObjectList[objectIndex].orientation[1], gObjectList[objectIndex].direction_angle[1]);
        if (gObjectList[objectIndex].orientation[1] == gObjectList[objectIndex].direction_angle[1]) {
            func_800722CC(objectIndex, 8);
            func_80086FD4(objectIndex);
            gObjectList[objectIndex].itemDisplay = 0;
        }
        break;
    case 0:
    case 3:
    default:
        break;
    }
}

void func_8007FEA4(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->unk_0AE) {
    case 1:
        if (f32_step_towards(&object->offset[0], object->unk_01C[0], 5.0f) != 0) {
            func_800726CC(objectIndex, 3);
            func_80086FD4(objectIndex);
            break;
        }
    case 0:
    case 2:
        break;
    case 3:
        if (f32_step_towards(&object->offset[0], 0.0f, 5.0f) != 0) {
            func_80086FD4(objectIndex);
            func_800722CC(objectIndex, 8);
        }
        break;
    }
}

void func_8007FF5C(s32 objectIndex)
{
    switch(gObjectList[objectIndex].unk_0DD) {
        case 1:
            func_8007FB48(objectIndex);
            break;
        case 2:
            func_8007FEA4(objectIndex);
            break;
    }
}

void func_8007FFC0(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
        case 0:
            break;
        case 1:
            func_8007FA08(objectIndex);
            break;
        case 3:
            func_80072568(objectIndex, 0x00000032);
            break;
        case 4:
            func_80072488(objectIndex);
            func_80086FD4(objectIndex);
            break;
    }
    func_8007E63C(objectIndex);
    func_8007FF5C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

void func_80080078(s32 objectIndex) {
    Object *object;

    init_texture_object(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->model = d_course_bowsers_castle_dl_thwomp;
    object->boundingBoxSize = 0x000C;
    object->sizeScaling = 1.0f;
    set_object_flag_status_true(objectIndex, 0x04000220);
    object->type = 2;
    object->unk_0DF = 8;
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object->unk_01C[1] = 30.0f;
    if (gIsMirrorMode != 0) {
        set_obj_orientation(objectIndex, 0U, 0x4000U, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, 0xC000U, 0U);
    }
    switch (object->primAlpha) {                              /* irregular */
    case 0:
        object->unk_050 = 2;
        break;
    case 1:
        object->unk_050 = 0x0000003C;
        break;
    case 2:
        object->unk_050 = 0x00000078;
        break;
    case 3:
        object->unk_050 = 0x000000B4;
        break;
    }
    func_800724DC(objectIndex);
    func_80072488(objectIndex);
}

void func_800801FC(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
    case 0:
        break;
    case 1:
        func_80080078(objectIndex);
        break;
    case 2:
        func_8007278C(objectIndex, object->unk_050);
        break;
    case 3:
        func_80072568(objectIndex, 0x00000032);
        break;
    case 4:
        object->unk_050 = 0x0000003C;
        func_800726CC(objectIndex, 2);
        break;
    }
    func_8007E63C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

void func_800802C0(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 0;
    init_texture_object(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object->model = d_course_bowsers_castle_dl_thwomp;
    object->itemDisplay = 0;
    object->boundingBoxSize = 0x000C;
    object->sizeScaling = 1.5f;
    set_object_flag_status_true(objectIndex, 0x05000220);
    object->type = 1;
    object->unk_0DF = 6;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    object->offset[1] = 10.0f;
    object->unk_01C[1] = 10.0f;
    if (gIsMirrorMode != 0) {
        set_obj_orientation(objectIndex, 0U, 0x4000U, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, 0xC000U, 0U);
    }
    object->offset[0] = 0.0f;
    object->offset[2] = 0.0f;
    func_800724DC(objectIndex);
    func_80072488(objectIndex);
}

void func_80080408(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 0:
        break;
    case 1:
        func_800802C0(objectIndex);
        break;
    case 2:
        func_8008A6DC(objectIndex, 100.0f);
        if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
            func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
            func_80072488(objectIndex);
        }
        break;
    case 3:
        if (func_800730BC(objectIndex, 3, 5, 1, 6, 6) != 0) {
            gObjectList[objectIndex].itemDisplay = 0;
        }
        break;
    case 4:
        if (func_8007278C(objectIndex, 0x0000012C) != 0) {
            func_800726CC(objectIndex, 2);
        }
        break;
    }
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

void func_80080524(s32 objectIndex) {
    Object *object;

    init_texture_object(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->model = d_course_bowsers_castle_dl_thwomp;
    object->boundingBoxSize = 0x000C;
    object->itemDisplay = 0;
    object->sizeScaling = 1.0f;
    set_object_flag_status_true(objectIndex, 0x04000220);
    object->type = 0;
    object->unk_0DF = 0x0A;
    func_80086E70(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object->surfaceHeight = 70.0f;
    object->origin_pos[1] = 70.0f;
    object->unk_01C[1] = 0.0f;
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    if ((gIsMirrorMode != 0) || (gGamestate == 9)) {
        set_obj_orientation(objectIndex, 0U, 0xC000U, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, 0x4000U, 0U);
    }
    switch (object->primAlpha) {                              /* irregular */
    case 0:
        object->unk_0DD = 2;
        object->velocity[2] = -1.0f;
        break;
    case 1:
        object->unk_0DD = 2;
        object->velocity[2] = -1.5f;
        break;
    }
    func_800722A4(objectIndex, 0x00000080);
    func_80072488(objectIndex);
}

void func_800806BC(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 0:
        break;
    case 1:
        if (f32_step_towards(&gObjectList[objectIndex].offset[2], 250.0f, gObjectList[objectIndex].velocity[2]) != 0) {
            gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        if (f32_step_towards(&gObjectList[objectIndex].offset[2], 0.0f, gObjectList[objectIndex].velocity[2]) != 0) {
            gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
            func_8008701C(objectIndex, 1);
        }
        break;
    }
}

void func_8008078C(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 0:
        break;
    case 1:
        if (f32_step_towards(&gObjectList[objectIndex].offset[2], -250.0f, gObjectList[objectIndex].velocity[2]) != 0) {
            gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        if (f32_step_towards(&gObjectList[objectIndex].offset[2], 0.0f, gObjectList[objectIndex].velocity[2]) != 0) {
            gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
            func_8008701C(objectIndex, 1);
        }
        break;
    }
}

void func_8008085C(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0DD) {
    case 1:
        func_800806BC(objectIndex);
        break;
    case 2:
        func_8008078C(objectIndex);
        break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void func_800808CC(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                               /* irregular */
    case 0:
        break;
    case 1:
        func_80080524(objectIndex);
        break;
    case 2:
        func_800730BC(objectIndex, 3, 5, 1, 6, -1);
        break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        func_8007E63C(objectIndex);
        func_8008085C(objectIndex);
        func_80073514(objectIndex);
        if (gGamestate != 9) {
            if ((D_8018D40C == 0) && (gObjectList[objectIndex].state == 2)) {
                func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
            }
        } else if ((gCutsceneShotTimer < 0xBF) && (((s16) gCutsceneShotTimer % 88) == 0x0000001E)) {
            func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
        }
    }
}

void func_80080A14(s32 objectIndex, Player *player) {
    if (is_within_horizontal_distance_of_player(objectIndex, player, 12.0f) != 0) {
        player->tyres[FRONT_LEFT].unk_14 |= 3;
    }
}

void func_80080A4C(s32 objectIndex, s32 cameraPlayerId) {
    Camera *camera = &camera1[cameraPlayerId];
    Player *player = &gPlayerOne[cameraPlayerId];

    if (gScreenModeSelection != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        if ((func_80072320(objectIndex, 0x00000010) != 0) && (is_within_horizontal_distance_of_player(objectIndex, player, 500.0f) != false)) {
            func_8001CA10(camera);
            func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}

void func_80080B28(s32 objectIndex, s32 playerId) {
    f32 temp_f0;
    Player *temp_s0;

    temp_s0 = &gPlayerOne[playerId];
    if (is_obj_flag_status_active(objectIndex, 0x00000200) != 0) {
        if (!(temp_s0->soundEffects & 0x100)) {
            temp_f0 = func_80088F54(objectIndex, temp_s0);
            if ((temp_f0 <= 9.0) && !(temp_s0->effects & 0x04000000) && (has_collided_horizontally_with_player(objectIndex, temp_s0) != 0)) {
                if ((temp_s0->type & 0x8000) && !(temp_s0->type & 0x100)) {
                    if (!(temp_s0->effects & 0x200)) {
                        func_80089474(objectIndex, playerId, 1.4f, 1.1f, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x4C));
                    } else if (func_80072354(objectIndex, 0x00000040) != 0) {
                        if (temp_s0->type & 0x1000) {
                            func_800C98B8(temp_s0->pos, temp_s0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        } else {
                            func_800C9060((u8) playerId, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        }
                        func_80080DE4(objectIndex);
                        func_80075304(gObjectList[objectIndex].pos, 3, 3, D_8018D3C4);
                        set_object_flag_status_false(objectIndex, 0x00000200);
                        func_800722A4(objectIndex, 0x00000040);
                        func_80086F60(objectIndex);
                        func_800726CC(objectIndex, 0x000000C8);
                    }
                }
            } else if ((temp_f0 <= 17.5) && (func_80072320(objectIndex, 1) != 0) && (is_within_horizontal_distance_of_player(objectIndex, temp_s0, (temp_s0->unk_094 * 0.5) + 7.0) != 0)) {
                if ((temp_s0->type & 0x8000) && !(temp_s0->type & 0x100)) {
                    if (is_obj_flag_status_active(objectIndex, 0x04000000) != 0) {
                        func_80072180();
                    }
                    func_800722A4(objectIndex, 2);
                    temp_s0->unk_040 = (s16) objectIndex;
                    temp_s0->unk_046 |= 2;
                    temp_s0->soundEffects |= 0x100;
                    func_80088FF0(temp_s0);
                }
            }
        } else {
            func_80088FF0(temp_s0);
        }
    }
}

void func_80080DE4(s32 arg0) {
    Player *player;
    s32 var_v1;

    player = gPlayerOne;
    for (var_v1 = 0; var_v1 < NUM_PLAYERS; var_v1++, player++) {
        if (arg0 == player->unk_040) {
            player->soundEffects &= ~0x100;
            player->unk_040 = -1;
        }
    }
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/YMJDJ
// No idea what the problem is
void func_80080E8C(s32 objectIndex1, s32 objectIndex2, s32 arg2) {
    u16 anAngle;
    f32 thing0;
    f32 thing1;
    f32 *temp_v1;

    init_object(objectIndex1, arg2);
    temp_v1 = D_800E594C[arg2];
    gObjectList[objectIndex1].unk_0D5 = 2;
    anAngle = gObjectList[objectIndex2].direction_angle[1];
    thing1 = func_800416D8(temp_v1[1], temp_v1[0], anAngle);
    thing0 = func_80041724(temp_v1[1], temp_v1[0], anAngle);
    gObjectList[objectIndex1].origin_pos[0] = gObjectList[objectIndex2].pos[0] + thing0;
    gObjectList[objectIndex1].origin_pos[1] = gObjectList[objectIndex2].surfaceHeight - 9.0;
    gObjectList[objectIndex1].origin_pos[2] = gObjectList[objectIndex2].pos[2] + thing1;
    anAngle = D_800E597C[arg2] + gObjectList[objectIndex2].direction_angle[1];
    gObjectList[objectIndex1].velocity[0] = sins(anAngle) * 0.6;
    gObjectList[objectIndex1].velocity[2] = coss(anAngle) * 0.6;
}
#else
GLOBAL_ASM("asm/non_matchings/update_objects/func_80080E8C.s")
#endif

void func_80080FEC(s32 arg0) {
    s32 objectIndex;
    s32 i;

    for (i = 0; i < 6; i++) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) break;
        func_80080E8C(objectIndex, arg0, i);
    }
}

void func_80081080(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    object->activeTexture = D_8018D490;
    object->textureList = D_8018D490;
    object->primAlpha = 0x00FF;
    object->direction_angle[1] = 0;
    object->orientation[0] = 0;
    object->orientation[2] = 0;
    object->offset[0] = 0.0f;
    object->offset[1] = 0.0f;
    object->offset[2] = 0.0f;
    object->sizeScaling = 0.25f;
    func_80072488(objectIndex);
}

void func_800810F4(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 0:
        break;
    case 1:
        func_80081080(objectIndex);
        break;
    case 2:
        object_add_velocity_offset_xz(objectIndex);
        f32_step_up_towards(&gObjectList[objectIndex].offset[1], 14.0f, 0.5f);
        func_8007415C(objectIndex, &gObjectList[objectIndex].sizeScaling, 0.25f, 0.75f, 0.025f, 1, 0);
        if (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0x000000FF, 0, 4, 0, 0) != 0) {
            func_80072488(objectIndex);
        }
        break;
    case 3:
        func_80072428(objectIndex);
        break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

// update twomp I think
void func_80081208(void) {

}

void func_80081210(void) {
    Player *player;
    s32 objectIndex;
    s32 var_s2_3;
    s32 var_s4;

    D_80165834[0] += 0x100;
    D_80165834[1] += 0x200;
    for (var_s4 = 0; var_s4 < gNumActiveThwomps; var_s4++) {
        objectIndex = indexObjectList1[var_s4];
        func_800722CC(objectIndex, 0x00000010);
        func_8008A4CC(objectIndex);
    }
    func_8007F8D8();
    for (var_s4 = 0; var_s4 < gNumActiveThwomps; var_s4++) {
        objectIndex = indexObjectList1[var_s4];
        if (gObjectList[objectIndex].state != 0) {
            switch (gObjectList[objectIndex].unk_0D5) {
            case 1:
                func_8007ED6C(objectIndex);
                break;
            case 2:
                func_8007F5A8(objectIndex);
                break;
            case 3:
                func_8007FFC0(objectIndex);
                break;
            case 4:
                func_800801FC(objectIndex);
                break;
            case 6:
                func_80080408(objectIndex);
                break;
            case 5:
                func_800808CC(objectIndex);
                break;
            }
        }
    }
    player = gPlayerOne;
    for (var_s4 = 0; var_s4 < NUM_PLAYERS; var_s4++, player++) {
        player->tyres[FRONT_LEFT].unk_14 &= ~3;
        player->unk_046 &= ~0x0006;
        for (var_s2_3 = 0; var_s2_3 < gNumActiveThwomps; var_s2_3++) {
            objectIndex = indexObjectList1[var_s2_3];
            if (!(player->effects & BOO_EFFECT)) {
                func_80080B28(objectIndex, var_s4);
            }
            if (is_obj_flag_status_active(objectIndex, 0x00020000) != 0) {
                func_80080A14(objectIndex, player);
            }
            if (is_obj_flag_status_active(objectIndex, 0x00010000) != 0) {
                func_80080A4C(objectIndex, var_s4);
            }
        }
    }
    func_8007542C(3);
    for (var_s4 = 0; var_s4 < gNumActiveThwomps; var_s4++) {
        objectIndex = indexObjectList1[var_s4];
        if (func_80072320(objectIndex, 0x00000020) == 0) continue;

        func_800722CC(objectIndex, 0x00000020);
        func_80080FEC(objectIndex);
    }
    for (var_s4 = 0; var_s4 < gObjectParticle2_SIZE; var_s4++) {
        objectIndex = gObjectParticle2[var_s4];
        if (objectIndex == DELETED_OBJECT_ID) continue;
        if (gObjectList[objectIndex].state == 0) continue;
        func_800810F4(objectIndex);
        if (gObjectList[objectIndex].state != 0) continue;
        delete_object_wrapper(&gObjectParticle2[var_s4]);
    }
}

void func_8008153C(s32 objectIndex) {
    UNUSED s32 stackPadding[3];
    s32 sp70;
    s32 var_s1;
    s32 var_s7;
    s32 loopObjectIndex;

    if (gPlayerCountSelection1 == 1) {
        sp70 = 8;
    } else {
        sp70 = 4;
    }

    for (var_s7 = 0; var_s7 < sp70; var_s7++) {
        for (var_s1 = 0; var_s1 < gObjectParticle2_SIZE; var_s1++) {
            loopObjectIndex = gObjectParticle2[var_s1];

            if (gObjectList[loopObjectIndex].state != 0) continue;

            u8 *mole = (u8 *) LOAD_ASSET(d_course_moo_moo_farm_mole_dirt);

            init_object(loopObjectIndex, 0);
            gObjectList[loopObjectIndex].activeTLUT = d_course_moo_moo_farm_mole_dirt;
            gObjectList[loopObjectIndex].tlutList = mole;
            gObjectList[loopObjectIndex].sizeScaling = 0.15f;
            gObjectList[loopObjectIndex].velocity[1] = random_int(0x000AU);
            gObjectList[loopObjectIndex].velocity[1] = (gObjectList[loopObjectIndex].velocity[1] * 0.1) + 4.8;
            gObjectList[loopObjectIndex].unk_034 = random_int(5U);
            gObjectList[loopObjectIndex].unk_034 = (gObjectList[loopObjectIndex].unk_034 * 0.01) + 0.8;
            gObjectList[loopObjectIndex].orientation[1] = (0x10000 / sp70) * var_s1;
            gObjectList[loopObjectIndex].origin_pos[0] = gObjectList[objectIndex].origin_pos[0];
            gObjectList[loopObjectIndex].origin_pos[1] = gObjectList[objectIndex].origin_pos[1] - 13.0;
            gObjectList[loopObjectIndex].origin_pos[2] = gObjectList[objectIndex].origin_pos[2];
            break;
        }
    }
}

void func_80081790(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 0:
        break;                           /* irregular */
    case 1:
        if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.3f, gObjectList[objectIndex].unk_034, gObjectList[objectIndex].orientation[1], 0x00000032) != 0) {
            func_80072488(objectIndex);
        }
        object_calculate_new_pos_offset(objectIndex);
        break;
    case 2:
        func_80072428(objectIndex);
        func_80086F60(objectIndex);
        break;
    }
}

void func_80081848(s32 objectIndex) {
    u8 *mole = (u8 *) LOAD_ASSET(d_course_moo_moo_farm_mole_frames);
    u8 *tlut = (u8 *) LOAD_ASSET(d_course_moo_moo_farm_mole_tlut);
    init_texture_object(objectIndex, d_course_moo_moo_farm_mole_tlut, (u8*) mole, 0x20U, (u16) 0x00000040);
    gObjectList[objectIndex].sizeScaling = 0.15f;
    gObjectList[objectIndex].itemDisplay = 0;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    gObjectList[objectIndex].boundingBoxSize = 6;
    gObjectList[objectIndex].velocity[1] = 4.0f;
    set_object_flag_status_true(objectIndex, 0x04000000);
    func_80072488(objectIndex);
}

void func_80081924(s32 objectIndex) {
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

void func_80081A88(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0DD) {                              /* irregular */
    case 0:
        break;
    case 1:
        func_8008B724(objectIndex);
        break;
    case 2:
        func_80081924(objectIndex);
        break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void func_80081AFC(s32 objectIndex, s32 arg1) {
    s8 *sp2C;
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {                              /* irregular */
    case 0x1:
        func_80081848(objectIndex);
        break;
    case 0x2:
        if (object->unk_04C == 0) {
            func_80086EAC(objectIndex, 2, 1);
            func_80072488(objectIndex);
            set_object_flag_status_true(objectIndex, 0x00000200);
        } else {
            object->unk_04C--;
        }
        break;
    case 0x3:
        if (object->unk_0AE == 0) {
            func_80086EAC(objectIndex, 2, 4);
            func_8008153C(objectIndex);
            func_80072488(objectIndex);
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
            set_object_flag_status_false(objectIndex, 0x00000200);
            func_80072428(objectIndex);
            switch (arg1) {                         /* switch 1; irregular */
            case 1:                                 /* switch 1 */
                sp2C = D_8018D198;
                break;
            case 2:                                 /* switch 1 */
                sp2C = D_8018D1A8;
                break;
            case 3:                                 /* switch 1 */
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

void func_80081D34(s32 objectIndex) {
    Player *player;
    Camera *var_s4;
    s32 var_s2;
    s32 var_s5;
    Object *object;

    var_s5 = 0;
    player = gPlayerOne;
    var_s4 = camera1;
    for (var_s2 = 0; var_s2 < D_8018D158; var_s2++, player++, var_s4++) {
        if ((is_obj_flag_status_active(objectIndex, 0x00000200) != 0) && !(player->effects & 0x80000000) && (has_collided_with_player(objectIndex, player) != 0)) {
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
        set_object_flag_status_false(objectIndex, 0x00000200);
        func_80086F60(objectIndex);
        set_obj_origin_pos(objectIndex, object->pos[0], object->pos[1], object->pos[2]);
        set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
        func_80086EAC(objectIndex, 2, 0x000A);
        func_800726CC(objectIndex, 0x0000000A);
    }
}

void func_80081FF4(s32 objectIndex, s32 arg1) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 moleCount;
    s16 var_v1;
    s16 offset;
    s32 var_a0;
    s8 *var_a2;

    init_object(objectIndex, 0);
    gObjectList[objectIndex].unk_04C = random_int(0x001EU) + 5;
    switch (arg1) {                                 /* irregular */
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

void func_800821AC(s32 objectIndex, s32 arg1) {
    if (gObjectList[objectIndex].state != 0) {
        func_80081AFC(objectIndex, arg1);
        func_80081A88(objectIndex);
        func_80081D34(objectIndex);
    }
}

void update_moles(void) {
    s32 var_s1;
    s32 objectIndex;
    UNUSED s32 stackPadding;

    for (var_s1 = 0; var_s1 < D_8018D1C8; var_s1++) {
        objectIndex = indexObjectList1[var_s1];
        if (gObjectList[objectIndex].state == 0) {
            if (func_8008A8B0(8, 9) != 0) {
                func_80081FF4(objectIndex, 1);
            }
        } else {
            func_800821AC(objectIndex, 1);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D0; var_s1++) {
        objectIndex = indexObjectList2[var_s1];
        if (gObjectList[objectIndex].state == 0) {
            if (func_8008A8B0(0x0010, 0x0013) != 0) {
                func_80081FF4(objectIndex, 2);
            }
        } else {
            func_800821AC(objectIndex, 2);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D8; var_s1++) {
        objectIndex = indexObjectList3[var_s1];
        if (gObjectList[objectIndex].state == 0) {
            if (func_8008A8B0(0x0011, 0x0014) != 0) {
                func_80081FF4(objectIndex, 3);
            }
        } else {
            func_800821AC(objectIndex, 3);
        }
    }

    for (var_s1 = 0; var_s1 < gObjectParticle2_SIZE; var_s1++) {
        objectIndex = gObjectParticle2[var_s1];
        if (gObjectList[objectIndex].state != 0) {
            func_80081790(objectIndex);
        }
    }
}

void func_8008241C(s32 objectIndex, s32 arg1) {
    UNUSED s16 stackPadding0;
    s16 temp_f4;
    s16 sp22;
    s16 sp20;

    gObjectList[objectIndex].unk_0D8 = 1;
    gObjectList[objectIndex].model = (Gfx *) d_course_koopa_troopa_beach_unk4;
    gObjectList[objectIndex].vertex = (Vtx *) d_course_koopa_troopa_beach_unk_data5;
    gObjectList[objectIndex].sizeScaling = 0.2f;
    gObjectList[objectIndex].unk_0DD = 1;
    sp22 = random_int(0x00C8) + -100.0;
    sp20 = random_int(0x0014);
    temp_f4 = random_int(0x00C8) + -100.0;
    if (gGamestate == 9) {
        set_obj_origin_pos(objectIndex, sp22 + -360.0, sp20 + 60.0, temp_f4 + -1300.0);
    } else if (gObjectList[objectIndex].unk_0D5 != 0) {
        set_obj_origin_pos(objectIndex, (sp22 + 328.0) * xOrientation, sp20 + 20.0, temp_f4 + 2541.0);
    } else {
        set_obj_origin_pos(objectIndex, (sp22 + -985.0) * xOrientation, sp20 + 15.0, temp_f4 + 1200.0);
    }
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    gObjectList[objectIndex].unk_034 = 1.0f;
    func_80086EF0(objectIndex);
    gObjectList[objectIndex].spline = D_800E633C[arg1 % 4];
    set_object_flag_status_true(objectIndex, 0x00000800);
    func_80072488(objectIndex);
}

void func_80082714(s32 objectIndex, s32 arg1) {
    switch(gObjectList[objectIndex].state) {
    case 1:
        func_8008241C(objectIndex , arg1);
        break;
    case 0:
    default:
        break;
    }
}

void func_8008275C(s32 objectIndex) {
    UNUSED s32 stackPadding;
    switch (gObjectList[objectIndex].unk_0DD) {                              /* irregular */
    case 1:
        func_8008B78C(objectIndex);
        object_calculate_new_pos_offset(objectIndex);
        break;
    case 2:
        func_8008B78C(objectIndex);
        vec3f_copy(gObjectList[objectIndex].unk_01C, gObjectList[objectIndex].pos);
        func_8000D940(gObjectList[objectIndex].origin_pos, (s16 *) &gObjectList[objectIndex].unk_0C6, gObjectList[objectIndex].unk_034, 0.0f, 0);
        gObjectList[objectIndex].offset[0] *= 2.0;
        gObjectList[objectIndex].offset[1] *= 2.5;
        gObjectList[objectIndex].offset[2] *= 2.0;
        object_calculate_new_pos_offset(objectIndex);
        gObjectList[objectIndex].direction_angle[1] = get_angle_between_two_vectors(gObjectList[objectIndex].unk_01C, gObjectList[objectIndex].pos);
        break;
    }
    func_800873F4(objectIndex);
}

void update_seagulls(void) {
    Object *object;
    UNUSED s32 *var_s4;
    s32 temp_s0;
    s32 var_s3;

    for (var_s3 = 0; var_s3 < NUM_SEAGULLS; var_s3++) {
        temp_s0 = indexObjectList2[var_s3];

        object = &gObjectList[temp_s0];
        if (object->state == 0) continue;

        func_80082714(temp_s0, var_s3);
        func_8008275C(temp_s0);
        if (func_80072320(temp_s0, 2) != 0) {
            func_800722CC(temp_s0, 2);
            if (D_80165A90 != 0) {
                D_80165A90 = 0;
                D_80183E40[0] = 0.0f;
                D_80183E40[1] = 0.0f;
                D_80183E40[2] = 0.0f;
                if (gGamestate != CREDITS_SEQUENCE) {
                    func_800C98B8(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                } else {
                    temp_s0 = indexObjectList2[1];
                    if (gCutsceneShotTimer < 0x97) {
                        object = &gObjectList[temp_s0];
                        func_800C98B8(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                    }
                }
            }
        }
    }
    if (D_80165900 != 0) {
        D_80165900 -= 1;
    } else {
        if (gGamestate != 9) {
            D_80165900 = 0x003C;
        } else {
            D_80165900 = 0x000F;
        }
        if ((D_80165908 != 0) && (D_80165A90 == 0)) {
            D_80165A90 = 1;
        }
    }
    D_80165908 = 0;
}

void init_ktb_crab(s32 objectIndex) {
    Object *object;

    init_texture_object(objectIndex, d_course_koopa_troopa_beach_crab_tlut, (u8*) d_course_koopa_troopa_beach_crab_frames, 0x40U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->sizeScaling = 0.15f;
    object->itemDisplay = 0;
    func_80072488(objectIndex);
    object->boundingBoxSize = 1;
    set_object_flag_status_true(objectIndex, 0x04000420);
    func_80086EAC(objectIndex, 0, 1);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    object->unk_034 = 1.5f;
    set_object_flag_status_true(objectIndex, 0x00000200);
}

void func_80082B34(s32 objectIndex, UNUSED s32 unused) {
    switch (gObjectList[objectIndex].state) {                               /* irregular */
    case 0:
        break;
    case 1:
        init_ktb_crab(objectIndex);
        break;
    case 2:
        func_80072E54(objectIndex, 0, 3, 1, 2, -1);
        break;
    case 3:
        func_80072E54(objectIndex, 4, 6, 1, 2, -1);
        break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        func_80073514(objectIndex);
    }
}

void func_80082C30(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 1:
        if (func_80087A0C(objectIndex, gObjectList[objectIndex].origin_pos[0], gObjectList[objectIndex].unk_01C[0], gObjectList[objectIndex].origin_pos[2], gObjectList[objectIndex].unk_01C[2]) != 0) {
            func_800726CC(objectIndex, 3);
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        if (func_80087104(objectIndex, 0x003CU) != 0) {
            gObjectList[objectIndex].unk_034 = 0.8f;
            func_800726CC(objectIndex, 2);
            func_80086FD4(objectIndex);
        }
        break;
    case 3:
        if (func_80087954(objectIndex, 0x0000003C) != 0) {
            func_80086FD4(objectIndex);
            func_800726CC(objectIndex, 3);
        }
        break;
    case 4:
        if (func_80087104(objectIndex, 0x003CU) != 0) {
            func_800726CC(objectIndex, 2);
            func_80086FD4(objectIndex);
        }
        break;
    case 5:
        if (func_8008789C(objectIndex, 0x0000003C) != 0) {
            func_800726CC(objectIndex, 3);
            func_8008701C(objectIndex, 2);
        }
        break;
    }
    object_calculate_new_pos_offset(objectIndex);
    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
        func_80088538(objectIndex);
        gObjectList[objectIndex].pos[1] = (f32) (gObjectList[objectIndex].surfaceHeight + 2.5);
    }
}

void func_80082E18(s32 objectIndex) {
    if (gObjectList[objectIndex].state >= 2) {
        func_80089F24(objectIndex);
    }
}

void update_crabs(void) {
    s32 objectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_CRABS; var_s1++) {
        objectIndex = indexObjectList1[var_s1];
        if (gObjectList[objectIndex].state != 0) {
            func_80082B34(objectIndex, var_s1);
            func_8008A6DC(objectIndex, 500.0f);
            func_80082C30(objectIndex);
            func_80082E18(objectIndex);
        }
    }
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/PYAg4
// Stack issue caused by the `test` variable, but removing it causes much, much larger differences
void func_80082F1C(s32 objectIndex, s32 arg1) {
    YVFlagPoleSpawn *test;
    gObjectList[objectIndex].model = (Gfx *) d_course_yoshi_valley_unk5;
    gObjectList[objectIndex].vertex = (Vtx *) d_course_yoshi_valley_unk4;
    gObjectList[objectIndex].sizeScaling = 0.027f;
    if (test->rot && test->rot) {}
    test = &D_800E5DF4[arg1];
    func_80072488(objectIndex);
    set_obj_origin_pos(objectIndex, test->pos[0] * xOrientation, test->pos[1], test->pos[2]);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, test->rot, 0U);
}
#else
GLOBAL_ASM("asm/non_matchings/update_objects/func_80082F1C.s")
#endif

void func_80083018(s32 objectIndex, s32 arg1) {
    switch(gObjectList[objectIndex].state) {
    case 1:
        func_80082F1C(objectIndex , arg1);
        break;
    case 0:
    default:
        break;
    }
}

void func_80083060(s32 objectIndex) {
    object_calculate_new_pos_offset(objectIndex);
}

void func_80083080(void) {
    s32 objectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_YV_FLAG_POLES; var_s1++) {
        objectIndex = indexObjectList1[var_s1];
        if (gObjectList[objectIndex].state != 0) {
            func_80083018(objectIndex, var_s1);
            func_80083060(objectIndex);
        }
    }
}

void func_8008311C(s32 objectIndex, s32 arg1) {
    Object *object;
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_hedgehog);

    init_texture_object(objectIndex, d_course_yoshi_valley_hedgehog_tlut, d_course_yoshi_valley_hedgehog, 0x40U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->activeTLUT = d_course_yoshi_valley_hedgehog_tlut;
    object->activeTexture = d_course_yoshi_valley_hedgehog;
    object->vertex = vtx;
    object->sizeScaling = 0.2f;
    object->itemDisplay = 0;
    func_80072488(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    object->unk_034 = ((arg1 % 6) * 0.1) + 0.5;
    func_80086E70(objectIndex);
    set_object_flag_status_true(objectIndex, 0x04000600);
    object->boundingBoxSize = 2;
}

void func_80083248(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 0:
        break;
    case 1:
        if (func_80087A0C(objectIndex, gObjectList[objectIndex].origin_pos[0], gObjectList[objectIndex].unk_09C, gObjectList[objectIndex].origin_pos[2], gObjectList[objectIndex].unk_09E) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        func_800871AC(objectIndex, 0x0000003C);
        break;
    case 3:
        if (func_80087A0C(objectIndex, gObjectList[objectIndex].unk_09C, gObjectList[objectIndex].origin_pos[0], gObjectList[objectIndex].unk_09E, gObjectList[objectIndex].origin_pos[2]) != 0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 4:
        if (func_80087060(objectIndex, 0x0000003C) != 0) {
            func_8008701C(objectIndex, 1);
        }
        break;
    }
    object_calculate_new_pos_offset(objectIndex);
    if (is_obj_flag_status_active(objectIndex, 0x00200000) != 0) {
        if (is_obj_flag_status_active(objectIndex, 0x00400000) != 0) {
            func_8008861C(objectIndex);
        }
        gObjectList[objectIndex].pos[1] = gObjectList[objectIndex].surfaceHeight + 6.0;
    }
}

void func_800833D0(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 0:
        break;
    case 1:
        func_8008311C(objectIndex, arg1);
        break;
    case 2:
        func_80072D3C(objectIndex, 0, 1, 4, -1);
        break;
    }
    if (gObjectList[objectIndex].itemDisplay == 0) {
        Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_hedgehog);
        gObjectList[objectIndex].vertex = vtx;
    } else {
        Vtx *vtx = (Vtx *) LOAD_ASSET(D_0D006130);
        gObjectList[objectIndex].vertex = vtx;
    }
}

void func_80083474(s32 objectIndex) {
    if (gObjectList[objectIndex].state >= 2) {
        func_80089F24(objectIndex);
    }
}

void update_hedgehogs(void) {
    s32 temp_s1;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < NUM_HEDGEHOGS; var_s0++) {
        temp_s1 = indexObjectList2[var_s0];
        func_800833D0(temp_s1, var_s0);
        func_80083248(temp_s1);
        func_80083474(temp_s1);
    }
    func_80072120(indexObjectList2, 0x0000000F);
}

void func_80083538(s32 objectIndex, Vec3f arg1, s32 arg2, s32 arg3) {
    Object *object;

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->activeTexture = d_course_frappe_snowland_snow;
    object->textureList = d_course_frappe_snowland_snow;
    object->activeTLUT = d_course_frappe_snowland_snow_tlut;
    object->tlutList = d_course_frappe_snowland_snow_tlut;
    object->sizeScaling = random_int(0x0064U);
    object->sizeScaling = (object->sizeScaling * 0.001) + 0.05;
    object->velocity[1] = random_int(0x0014U);
    object->velocity[1] = (object->velocity[1] * 0.5) + 2.6;
    object->unk_034 = random_int(0x000AU);
    object->unk_034 = (object->unk_034 *0.1) + 4.5;
    object->direction_angle[1] = (arg2 << 0x10) / arg3;
    object->origin_pos[0] = arg1[0];
    object->origin_pos[1] = arg1[1];
    object->origin_pos[2] = arg1[2];
    object->primAlpha = random_int(0x4000U) + 0x1000;
}

void func_800836F0(Vec3f arg0) {
    s32 objectIndex;
    s32 i;

    for (i = 0; i < D_8018D3BC; i++) {
        objectIndex = add_unused_obj_index(&gObjectParticle2[0], &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) break;
        func_80083538(objectIndex, arg0, i, D_8018D3BC);
    }
}

void func_8008379C(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 0:
        break;
    case 1:
        if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.74f, gObjectList[objectIndex].unk_034, gObjectList[objectIndex].direction_angle[1], 0x00000064) != 0) {
            func_80072488(objectIndex);
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

static const char*sSnowmanHeadList[] = {
    d_course_frappe_snowland_snowman_head
};

void func_80083868(s32 objectIndex) {
    Object *object;
    Vtx *vtx = (Vtx *) LOAD_ASSET(D_0D0061B0);
    init_texture_object(objectIndex, d_course_frappe_snowland_snowman_tlut, sSnowmanHeadList, 0x40U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->vertex = vtx;
    object->sizeScaling = 0.1f;
    object->itemDisplay = 0;
    func_80072488(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object->orientation[0] = 0;
    object->orientation[1] = 0;
    object->orientation[2] = 0x8000;
    object->primAlpha = random_int(0x2000U) - 0x1000;
    func_80086E70(objectIndex);
    object->unk_034 = 1.5f;
    set_object_flag_status_true(objectIndex, 0x00000200);
}

void func_80083948(s32 objectIndex) {
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
    func_80073D0C(objectIndex, &gObjectList[objectIndex].primAlpha, -0x00001000, 0x00001000, 0x00000400, 1, -1);
    gObjectList[objectIndex].orientation[2] = gObjectList[objectIndex].primAlpha + 0x8000;
}

void func_80083A94(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 0:
        break;
    case 1: // snowman head
        func_80083868(objectIndex);
        break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        func_80073514(objectIndex);
    }
    func_80083948(objectIndex);
}

static const char*sSnowmanBodyList[] = {
    d_course_frappe_snowland_snowman_body
};

void func_80083B0C(s32 objectIndex) {
    Vtx *vtx = (Vtx *) LOAD_ASSET(common_vtx_hedgehog);
    init_texture_object(objectIndex, d_course_frappe_snowland_snowman_tlut, sSnowmanBodyList, 0x40U, (u16) 0x00000040);
    gObjectList[objectIndex].vertex = vtx;
    gObjectList[objectIndex].sizeScaling = 0.1f;
    gObjectList[objectIndex].itemDisplay = 0;
    func_80072488(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    gObjectList[objectIndex].orientation[0] = 0;
    gObjectList[objectIndex].orientation[1] = 0;
    gObjectList[objectIndex].orientation[2] = 0x8000;
    gObjectList[objectIndex].boundingBoxSize = 2;
    gObjectList[objectIndex].unk_034 = 1.5f;
    set_object_flag_status_true(objectIndex, 0x04000210);
}

void func_80083BE4(s32 objectIndex) {
    object_calculate_new_pos_offset(objectIndex);
}

void func_80083C04(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
    case 0:
        break;
    case 1: // snowmanbody
        func_80083B0C(objectIndex);
        break;
    case 2:
        func_8007278C(objectIndex, 0x00000096);
        break;
    case 10:
        if (func_8007278C(objectIndex, 0x0000012C) != 0) {
            func_800722A4(objectIndex, 2);
        }
        break;
    case 11:
        if (func_8007278C(objectIndex, 0x0000000A) != 0) {
            set_object_flag_status_true(objectIndex, 0x00000010);
            object->sizeScaling = 0.001f;
        }
        break;
    case 12:
        if (func_80074118(objectIndex, &object->sizeScaling, 0.001f, 0.1f, 0.0025f, 0, 0) != 0) {
            func_80072488(objectIndex);
        }
        break;
    case 13:
        func_800726CC(objectIndex, 2);
        set_object_flag_status_false(objectIndex, 0x00001000);
        break;
    }
    if (object->state >= 2) {
        func_80073514(objectIndex);
    }
    func_80083BE4(objectIndex);
}

void update_snowmen(void) {
    s32 var_s0;
    s32 var_s3;
    s32 var_s4;
    s32 objectIndex;
    Object *object;

    for (var_s0 = 0; var_s0 < gObjectParticle2_SIZE; var_s0++) {
        objectIndex = gObjectParticle2[var_s0];

        if (objectIndex == DELETED_OBJECT_ID) continue;

        if (gObjectList[objectIndex].state == 0) continue;
        func_8008379C(objectIndex);
        if (gObjectList[objectIndex].state != 0) continue;
        delete_object_wrapper(&gObjectParticle2[var_s0]);
        if (var_s0) {} // ??
    }

    for (var_s0 = 0; var_s0 < NUM_SNOWMEN; var_s0++) {
        var_s4 = indexObjectList1[var_s0];
        var_s3 = indexObjectList2[var_s0];
        func_80083A94(var_s3); // snowman head
        func_80083C04(var_s4); // snowman body
        if (is_obj_index_flag_status_inactive(var_s4, 0x00001000) != 0) {
            object = &gObjectList[var_s4];
            if ((func_8008A8B0(object->unk_0D5 - 1, object->unk_0D5 + 1) != 0) && (func_80089B50(var_s4) != 0)) {
                set_object_flag_status_true(var_s4, 0x00001000);
                set_object_flag_status_false(var_s4, 0x00000010);
                func_800726CC(var_s4, 0x0000000A);
                func_8008701C(var_s3, 0x0000000A);
                func_800836F0(object->pos);
            }
        } else if (func_80072320(var_s4, 2) != 0) {
            func_800722CC(var_s4, 2);
            func_8008701C(var_s3, 0x00000014);
        }
    }
}

void func_80083F18(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 0:
        break;
    case 1:
        if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.12f, gObjectList[objectIndex].unk_034, gObjectList[objectIndex].direction_angle[1], 0x00000064) != 0) {
            func_80072488(objectIndex);
        }
        object_calculate_new_pos_offset(objectIndex);
        break;
    case 2:
        func_80086F60(objectIndex);
        func_80072428(objectIndex);
        break;
    }
}

void func_80083FD0(s32 objectIndex, s32 arg1, s32 playerId) {
    Object *object;
    Player *sp20;

    object = &gObjectList[objectIndex];
    sp20 = &gPlayerOne[playerId];
    object->unk_084[7] = playerId;
    init_object(objectIndex, 0);
    object->activeTLUT = d_course_sherbet_land_ice;
    object->tlutList   = d_course_sherbet_land_ice;
    object->sizeScaling = ((f32) random_int(0x01F4U) * 0.0002) + 0.04;
    object->velocity[1]  = ((f32) random_int(0x0032U) * 0.05)   + 1.0;
    object->unk_034     = ((f32) random_int(0x000AU) * 0.1)    + 1.0;
    object->direction_angle[1] = D_801657A2 * arg1;
    object->origin_pos[0] = (sp20->pos[0] + random_int(0x0014U)) - 10.0f;
    object->origin_pos[1] = (sp20->pos[1] - 10.0) + random_int(0x000AU);
    object->origin_pos[2] = (sp20->pos[2] + random_int(0x0014U)) - 10.0f;
}

void func_8008421C(UNUSED s32 arg0, s32 playerId) {
    s32 objectIndex;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < D_8018D3C0; var_s0++) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) break;
        func_80083FD0(objectIndex, var_s0, playerId);
    }
}

void func_800842C8(void) {
    s32 objectIndex;
    s32 playerId;

    D_80165834[0] += 0x200;
    D_80165834[1] += 0x400;
    func_800419F8();
    D_8016582C[0] += 0x2000;
    D_8016582C[1] += 0x1000;
    D_8016582C[2] += 0x1800;
    for (playerId = 0; playerId < gPlayerCountSelection1; playerId++) {
        objectIndex = gIndexLakituList[playerId];
        if (func_80072320(objectIndex, 0x00000020) != 0) {
            func_800722CC(objectIndex, 0x00000020);
            func_8008421C(objectIndex, playerId);
        }
    }
    for (playerId = 0; playerId < gObjectParticle2_SIZE; playerId++) {
        objectIndex = gObjectParticle2[playerId];
        if (objectIndex != DELETED_OBJECT_ID) {
            if (gObjectList[objectIndex].state != 0) {
                func_80083F18(objectIndex);
                if (gObjectList[objectIndex].state == 0) {
                    delete_object_wrapper(&gObjectParticle2[playerId]);
                }
            }
        }
    }
}

void func_80084430(s32 objectIndex, UNUSED s32 arg1) {
    Object *object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 0;
    object->model = (Gfx *) d_course_sherbet_land_unk_data1;
    object->vertex = (Vtx *) d_course_sherbet_land_unk_data11;
    object->sizeScaling = 0.2f;
    object->boundingBoxSize = 0x000C;
    object->unk_09C = 1;
    set_obj_origin_pos(objectIndex, xOrientation * -383.0, 2.0f, -690.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->unk_0DD = 1;
    func_80086EF0(objectIndex);
    object->spline = D_800E672C[0];
    set_object_flag_status_true(objectIndex, 0x04000800);
    object->type = func_80004EAC(d_course_sherbet_land_unk_data11, 0);
    func_80072488(objectIndex);
}

void func_8008453C(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 0:
        break;
    case 1:
        if (gPlayerCountSelection1 == 1) {
            func_80084430(objectIndex, arg1);
        }
        break;
    case 2:
        func_80072E54(objectIndex, 0, gObjectList[objectIndex].type, 1, 0, -1);
        break;
    }
}

void func_800845C8(s32 objectIndex, s32 arg1) {
    Object *object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 0;
    object->model = (Gfx *) d_course_sherbet_land_unk_data1;
    object->vertex = (Vtx *) d_course_sherbet_land_unk_data11;
    object->boundingBoxSize = 4;
    object->unk_09C = 2;
    object->unk_04C = random_int(0x012CU);
    set_object_flag_status_true(objectIndex, 0x04000220);
    if ((arg1 > 0) && (arg1 < 9)) {
        if ((arg1 == 1) || (arg1 == 2)) {
            set_obj_origin_pos(objectIndex, xOrientation * -2960.0, -80.0f, 1521.0f);
            object->unk_0C6 = 0x0150;
            object->unk_01C[1] = 100.0f;
        } else if ((arg1 == 3) || (arg1 == 4)) {
            set_obj_origin_pos(objectIndex, xOrientation * -2490.0, -80.0f, 1612.0f);
            object->unk_0C6 = 0x0100;
            object->unk_01C[1] = 80.0f;
        } else if ((arg1 == 5) || (arg1 == 6)) {
            set_obj_origin_pos(objectIndex, xOrientation * -2098.0, -80.0f, 1624.0f);
            object->unk_0C6 = 0xFF00;
            object->unk_01C[1] = 80.0f;
        } else if ((arg1 == 7) || (arg1 == 8)) {
            set_obj_origin_pos(objectIndex, xOrientation * -2080.0, -80.0f, 1171.0f);
            object->unk_0C6 = 0x0150;
            object->unk_01C[1] = 80.0f;
        }
        object->unk_0C4 = (arg1 << 0xF) & 0xFFFF;
        object->surfaceHeight = -80.0f;
        object->sizeScaling = 0.08f;
        object->unk_0DD = 2;
        func_800722A4(objectIndex, 8);
    } else if ((arg1 > 8) && (arg1 < 15)) {
        switch (arg1) {                             /* irregular */
        case 9:
            if (gGamestate != 9) {
                set_obj_origin_pos(objectIndex, xOrientation * 146.0, 0.0f, -380.0f);
            } else {
                set_obj_origin_pos(objectIndex, xOrientation * 380.0, 0.0f, -535.0f);
                object->sizeScaling = 0.15f;
            }
            object->unk_0C6 = 0x9000;
            if (gIsMirrorMode != 0) {
                object->unk_0C6 -= 0x4000;
            }
            object->unk_0DD = 3;
            break;
        case 10:
            set_obj_origin_pos(objectIndex, xOrientation * 380.0, 0.0f, -766.0f);
            object->unk_0C6 = 0x5000;
            if (gIsMirrorMode != 0) {
                object->unk_0C6 += 0x8000;
            }
            object->unk_0DD = 4;
            break;
        case 11:
            set_obj_origin_pos(objectIndex, xOrientation * -2300.0, 0.0f, -210.0f);
            object->unk_0C6 = 0xC000;
            object->unk_0DD = 6;
            if (gIsMirrorMode != 0) {
                object->unk_0C6 += 0x8000;
            }
            break;
        case 12:
            set_obj_origin_pos(objectIndex, xOrientation * -2500.0, 0.0f, -250.0f);
            object->unk_0C6 = 0x4000;
            object->unk_0DD = 6;
            if (gIsMirrorMode != 0) {
                object->unk_0C6 += 0x8000;
            }
            break;
        case 13:
            set_obj_origin_pos(objectIndex, xOrientation * -535.0, 0.0f, 875.0f);
            object->unk_0C6 = 0x8000;
            object->unk_0DD = 6;
            if (gIsMirrorMode != 0) {
                object->unk_0C6 -= 0x4000;
            }
            break;
        case 14:
            set_obj_origin_pos(objectIndex, xOrientation * -250.0, 0.0f, 953.0f);
            object->unk_0C6 = 0x9000;
            object->unk_0DD = 6;
            if (gIsMirrorMode != 0) {
                object->unk_0C6 -= 0x4000;
            }
            break;
        default:
            break;
        }
        set_obj_direction_angle(objectIndex, 0U, object->unk_0C6 + 0x8000, 0U);
        object->surfaceHeight = 5.0f;
        object->sizeScaling = 0.04f;
        func_800722A4(objectIndex, 0x00000014);
    }
    func_80086EF0(objectIndex);
    object->unk_034 = 0.0f;
    object->type = func_80004EAC(d_course_sherbet_land_unk_data11, 0);
    func_80072488(objectIndex);
}

void func_80084B7C(s32 objectIndex, s32 arg1) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
    case 0:
        break;
    case 1:
        func_800845C8(objectIndex, arg1);
        break;
    case 2:
        func_80072E54(objectIndex, 0, (s32) object->type, (s32) object->unk_09C, 0, -1);
        if (func_80072354(objectIndex, 0x00000020) != 0) {
            if (object->unk_084[6] == 0) {
                object->unk_084[6] = random_int(0x005AU) + 0x5A;
                func_800722A4(objectIndex, 0x00000080);
            } else {
                object->unk_084[6]--;
            }
        }
        break;
    case 3:
        func_80072E54(objectIndex, 0, object->type, 1, 0, 0);
        break;
    case 4:
        func_800722CC(objectIndex, 2);
        func_80072488(objectIndex);
        break;
    }
    if (func_80072320(objectIndex, 0x00000020) != 0) {
        if (object->unk_084[6] == 0) {
            func_800722A4(objectIndex, 0x00000080);
            object->unk_084[6] = 0x0010;
        } else {
            object->unk_084[6]--;
        }
    }
    if (func_80072320(objectIndex, 0x00000080) != 0) {
        func_800722CC(objectIndex, 0x00000080);
        if (func_80072320(objectIndex, 0x00000010) != 0) {
            func_800C98B8(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x49));
        } else {
            func_800C98B8(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x17));
        }
    }
}

void func_80084D2C(s32 objectIndex, s32 arg1) {
    f32 sp24;

    switch (gObjectList[objectIndex].unk_0AE) {
    case 0:
        break;
    case 1:
        gObjectList[objectIndex].direction_angle[1] = func_800417B4(gObjectList[objectIndex].direction_angle[1], gObjectList[objectIndex].unk_0C6);
        if (gObjectList[objectIndex].direction_angle[1] == gObjectList[objectIndex].unk_0C6) {
            gObjectList[objectIndex].unk_09C = 4;
            gObjectList[objectIndex].unk_034 = 0.4f;
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        f32_step_towards(&gObjectList[objectIndex].unk_034, 0.8f, 0.02f);
        if (func_80087060(objectIndex, 0x0000000F) != 0) {
            func_800722A4(objectIndex, 1);
            func_800722A4(objectIndex, 2);
            gObjectList[objectIndex].unk_09C = 1;
            gObjectList[objectIndex].unk_0D8 = 1;
            gObjectList[objectIndex].itemDisplay = 0;
            gObjectList[objectIndex].type = func_80004EAC(d_course_sherbet_land_unk_data11, gObjectList[objectIndex].unk_0D8);
            func_800726CC(objectIndex, 3);
            func_80086FD4(objectIndex);
            if (func_80072354(objectIndex, 0x00000020) != 0) {
                func_800722A4(objectIndex, 0x00000080);
            }
        }
        break;
    case 3:
        switch (arg1) {                             /* switch 1; irregular */
        case 0:                                     /* switch 1 */
            sp24 = 1.0f;
            break;
        case 1:                                     /* switch 1 */
            sp24 = 1.5f;
            break;
        case 2:                                     /* switch 1 */
            sp24 = 2.0f;
            break;
        case 3:                                     /* switch 1 */
            sp24 = 2.5f;
            break;
        }
        f32_step_towards(&gObjectList[objectIndex].unk_034, sp24, 0.15f);
        if ((func_80072354(objectIndex, 2) != 0) && (sp24 == gObjectList[objectIndex].unk_034)) {
            func_80086FD4(objectIndex);
        }
        break;
    case 4:
        if (func_80087060(objectIndex, 0x0000001E) != 0) {
            func_800722CC(objectIndex, 1);
            func_80086FD4(objectIndex);
        }
        break;
    case 5:
        f32_step_towards(&gObjectList[objectIndex].unk_034, 0.4f, 0.2f);
        if (func_80087060(objectIndex, 0x0000000A) != 0) {
            func_800722A4(objectIndex, 2);
            gObjectList[objectIndex].unk_0D8 = 2;
            gObjectList[objectIndex].itemDisplay = 0;
            gObjectList[objectIndex].type = func_80004EAC(d_course_sherbet_land_unk_data11, gObjectList[objectIndex].unk_0D8);
            func_800726CC(objectIndex, 3);
            func_80086FD4(objectIndex);
        }
        break;
    case 6:
        if (func_80072354(objectIndex, 2) != 0) {
            gObjectList[objectIndex].unk_0D8 = 0;
            gObjectList[objectIndex].itemDisplay = 0;
            gObjectList[objectIndex].type = func_80004EAC(d_course_sherbet_land_unk_data11, gObjectList[objectIndex].unk_0D8);
            gObjectList[objectIndex].unk_0C6 += 0x8000;
            func_800726CC(objectIndex, 2);
            func_8008701C(objectIndex, 1);
        }
        break;
    }
    func_8008781C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void func_80085024(void) {

}

void func_8008502C(s32 objectIndex, UNUSED s32 arg1) {
    func_80088038(objectIndex, gObjectList[objectIndex].unk_01C[1], gObjectList[objectIndex].unk_0C6);
    object_calculate_new_pos_offset(objectIndex);
    func_800873F4(objectIndex);
}

void func_80085080(s32 objectIndex) {
    func_8008B78C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_800873F4(objectIndex);
}

void func_800850B0(s32 objectIndex, s32 arg1) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->unk_0DD) {
    case 1:
        func_80085080(objectIndex);
        break;
    case 2:
        func_8008502C(objectIndex, arg1);
        break;
    case 3:
        func_80084D2C(objectIndex, 0);
        break;
    case 4:
        func_80084D2C(objectIndex, 1);
        break;
    case 5:
        func_80084D2C(objectIndex, 2);
        break;
    case 6:
        func_80084D2C(objectIndex, 3);
        break;
    }
    if (func_80072320(objectIndex, 0x00000020) != 0) {
        if (func_80072320(objectIndex, 0x00000040) != 0) {
            func_800722CC(objectIndex, 0x00000040);
            object->unk_084[6] = 0;
            object->unk_084[7] = 0x0096;
        }
        if (object->unk_084[7] == 0) {
            func_800722CC(objectIndex, 0x00000020);
        } else {
            object->unk_084[7]--;
            object->orientation[0] = object->direction_angle[0];
            object->orientation[1] += 0x2000;
            object->orientation[2] = object->direction_angle[2];
        }
    } else {
        object->orientation[0] = object->direction_angle[0];
        object->orientation[1] = object->direction_angle[1];
        object->orientation[2] = object->direction_angle[2];
    }
}

void update_penguins(void) {
    UNUSED s32 var_s2;
    s32 objectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_PENGUINS; var_s1++) {
        objectIndex = indexObjectList1[var_s1];
        if (gObjectList[objectIndex].state != 0) {
            if (var_s1 == 0) {
                func_8008453C(objectIndex, var_s1);
            } else {
                func_80084B7C(objectIndex, var_s1);
            }
            func_800850B0(objectIndex, var_s1);
        }
        if (func_80072320(objectIndex, 1) != 0) {
            func_80089820(objectIndex, 1.75f, 1.5f, 0x1900A046U);
        } else if (func_80072320(objectIndex, 8) != 0) {
            func_80089820(objectIndex, 1.3f, 1.0f, 0x1900A046U);
        } else {
            func_80089820(objectIndex, 1.5f, 1.25f, 0x1900A046U);
        }
        if ((is_obj_flag_status_active(objectIndex, 0x02000000) != 0) && (func_80072354(objectIndex, 0x00000020) != 0)) {
            func_800722A4(objectIndex, 0x00000060);
            set_object_flag_status_false(objectIndex, 0x02000000);
        }
    }
}

void init_hot_air_balloon(s32 objectIndex) {
    gObjectList[objectIndex].sizeScaling = 1.0f;
    gObjectList[objectIndex].model = d_course_luigi_raceway_dl_F960;
    if (gGamestate != 9) {
        set_obj_origin_pos(objectIndex, xOrientation * -176.0, 0.0f, -2323.0f);
        set_obj_origin_offset(objectIndex, 0.0f, 300.0f, 0.0f);
    } else {
        set_obj_origin_pos(objectIndex, xOrientation * -1250.0, 0.0f, 1110.0f);
        set_obj_origin_offset(objectIndex, 0.0f, 300.0f, 0.0f);
    }
    func_8008B844(objectIndex);
    func_800886F4(objectIndex);
    func_80086EF0(objectIndex);
    gObjectList[objectIndex].velocity[1] = -2.0f;
    init_actor_hot_air_balloon_item_box(0.0f, 0.0f, 0.0f);
    func_80072488(objectIndex);
}

void func_80085534(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 1:
        if (gObjectList[objectIndex].offset[1] <= 18.0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 2:
        f32_step_towards(&gObjectList[objectIndex].velocity[1], 0.0f, 0.05f);
        if (gObjectList[objectIndex].velocity[1] == 0.0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 3:
        func_800871AC(objectIndex, 1);
        break;
    case 4:
        f32_step_towards(&gObjectList[objectIndex].velocity[1], 1.0f, 0.05f);
        if (gObjectList[objectIndex].velocity[1] == 1.0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 5:
        func_800871AC(objectIndex, 0x0000005A);
        break;
    case 6:
        f32_step_towards(&gObjectList[objectIndex].velocity[1], 0.0f, 0.05f);
        if (gObjectList[objectIndex].velocity[1] == 0.0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 7:
        f32_step_towards(&gObjectList[objectIndex].velocity[1], -1.0f, 0.05f);
        if (gObjectList[objectIndex].velocity[1] == -1.0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 8:
        func_800871AC(objectIndex, 0x0000005A);
        break;
    case 9:
        f32_step_towards(&gObjectList[objectIndex].velocity[1], 0.0f, 0.05f);
        if (func_80087060(objectIndex, 0x0000005A) != 0) {
            func_8008701C(objectIndex, 3);
        }
        break;
    }
    object_add_velocity_offset_y(objectIndex);
    gObjectList[objectIndex].direction_angle[1] += 0x100;
}

void func_80085768(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 1:
        init_hot_air_balloon(objectIndex);
        break;
    case 0:
    case 2:
        break;
    }
}

void update_hot_air_balloon(void) {
    s32 objectIndex;

    objectIndex = indexObjectList1[0];
    if (gObjectList[objectIndex].state != 0) {
        func_80085768(objectIndex);
        func_80085534(objectIndex);
        object_calculate_new_pos_offset(objectIndex);
        if (gObjectList[objectIndex].state >= 2) {
            gActorHotAirBalloonItemBox->pos[0] = gObjectList[objectIndex].pos[0];
            gActorHotAirBalloonItemBox->pos[1] = gObjectList[objectIndex].pos[1] - 10.0;
            gActorHotAirBalloonItemBox->pos[2] = gObjectList[objectIndex].pos[2];
        }
    }
}

void func_80085878(s32 objectIndex, s32 arg1) {
    TrackWaypoint *temp_v0;
    Object *object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 1;
    object->model = (Gfx *) d_rainbow_road_unk4;
    object->vertex = (Vtx *) d_rainbow_road_unk3;
    object->sizeScaling = 0.03f;
    object->boundingBoxSize = 0x000A;
    set_object_flag_status_true(objectIndex, 0x04000200);
    object->unk_084[8] = (arg1 * 0x12C) + 0x1F4;
    set_obj_origin_pos(objectIndex, 0.0f, -15.0f, 0.0f);
    temp_v0 = &D_80164490[(u16)object->unk_084[8]];
    set_obj_origin_offset(objectIndex, temp_v0->posX, temp_v0->posY, temp_v0->posZ);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->unk_034 = 4.0f;
    object->type = func_80004EAC(d_rainbow_road_unk3, 0);
    func_80072488(objectIndex);
}

void func_800859C8(s32 objectIndex, s32 arg1) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->state) {                              /* irregular */
    case 0:
        break;
    case 1:
        func_80085878(objectIndex, arg1);
        break;
    case 2:
        func_80072E54(objectIndex, 0, (s32) object->type, 1, 0, -1);
        break;
    }
    if (D_8018D40C == 0) {
        func_800C98B8(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x57));
    }
    func_80074344(objectIndex, &object->surfaceHeight, -0.8f, 0.8f, 0.03f, 0, -1);
}

void update_chain_chomps(void) {
    s32 objectIndex;
    s32 var_s4;
    Object *object;

    for (var_s4 = 0; var_s4 < NUM_CHAIN_CHOMPS; var_s4++) {
        objectIndex = indexObjectList2[var_s4];
        object = &gObjectList[objectIndex];
        if (object->state != 0) {
            func_800859C8(objectIndex, var_s4);
            vec3f_copy(object->unk_01C, object->offset);
            func_8000D940(object->offset, &object->unk_084[8], object->unk_034, object->surfaceHeight, 0);
            object->direction_angle[1] = get_angle_between_two_vectors(object->unk_01C, object->offset);
            object_calculate_new_pos_offset(objectIndex);
            func_80089CBC(objectIndex, 30.0f);
        }
    }
}

void func_80085BB4(s32 objectIndex) {
    gObjectList[objectIndex].sizeScaling = 8.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    func_80072488(objectIndex);
}

void init_obj_neon_mushroom(s32 objectIndex) {
    set_obj_origin_pos(objectIndex, xOrientation * -1431.0, 827.0f, -2957.0f);
    init_texture_object(objectIndex, (u8*) d_course_rainbow_road_neon_mushroom_tlut_list, d_course_rainbow_road_neon_mushroom, 0x40U, (u16) 0x00000040);
    func_80085BB4(objectIndex);
}

void func_80085CA0(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            init_obj_neon_mushroom(objectIndex);
            break;
        case 2:
            func_80072E54(objectIndex, 0, 4, 1, 0x0000000C, 5);
            break;
        case 3:
            func_80072D3C(objectIndex, 3, 4, 4, 0x0000000A);
            break;
        case 4:
            func_8007278C(objectIndex, 0x00000014);
            break;
        case 5:
            func_80072E54(objectIndex, 0, 4, 1, 0x0000000C, 5);
            break;
        case 6:
            func_8007278C(objectIndex, 0x00000014);
            break;
        case 7:
            func_80072D3C(objectIndex, 3, 4, 0, 0x00000014);
            break;
        case 8:
            func_800726CC(objectIndex, 2);
            break;
        case 0:
        default:
            break;
    }
}

void func_80085DB8(s32 objectIndex) {
    set_obj_origin_pos(objectIndex, xOrientation * 799.0, 1193.0f, -5891.0f);
    init_texture_object(objectIndex, (u8*) d_course_rainbow_road_neon_mario_tlut_list, d_course_rainbow_road_neon_mario, 0x40U, (u16) 0x00000040);
    func_80085BB4(objectIndex);
}

void func_80085E38(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        func_80085DB8(objectIndex);
        break;
    case 2:
        func_80072E54(objectIndex, 0, 4, 1, 0x0000000C, 1);
        break;
    case 3:
        func_80072D3C(objectIndex, 3, 4, 0x0000000C, 1);
        break;
    case 4:
        func_80072B48(objectIndex, 0x0000000C);
        break;
    case 5:
        func_800726CC(objectIndex, 2);
        break;
    case 0:
    default:
        break;
    }
}

void func_80085EF8(s32 objectIndex) {
    set_obj_origin_pos(objectIndex, xOrientation * -2013.0, 555.0f, 0.0f);
    init_texture_object(objectIndex, (u8*) d_course_rainbow_road_neon_boo_tlut_list, d_course_rainbow_road_neon_boo, 0x40U, (u16) 0x00000040);
    func_80085BB4(objectIndex);
}

void func_80085F74(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        func_80085EF8(objectIndex);
        break;
    case 2:
        func_80072E54(objectIndex, 0, 4, 1, 5, 1);
        break;
    case 3:
        func_8007278C(objectIndex, 0x0000001E);
        break;
    case 4:
        func_80072C00(objectIndex, 4, 0, 7);
        break;
    case 5:
        func_8007278C(objectIndex, 0x0000001E);
        break;
    case 6:
        func_80072F88(objectIndex, 3, 0, 1, 5, 1);
        break;
    case 7:
        func_80072B48(objectIndex, 0x0000000F);
        break;
    case 8:
        func_800726CC(objectIndex, 2);
        break;
    case 0:
    default:
        break;
    }
}

#ifdef NON_MATCHING
/**
 * arg1 is pushed onto the stack prior to set_obj_origin_pos, which is correct.
 * But then it get popped back into v0 when its supposed to be popped into t0.
 * I really, really don't get why that's happening and the permuter hasn't found anything
**/

void func_80086074(s32 objectIndex, s32 arg1) {
    set_obj_origin_pos(objectIndex, D_800E6734[arg1][0] * xOrientation, D_800E6734[arg1][1], D_800E6734[arg1][2]);
    init_texture_object(objectIndex, d_course_rainbow_road_static_tluts[arg1], d_course_rainbow_road_static_textures[arg1], 64, 64);
    func_80085BB4(objectIndex);
}
#else
GLOBAL_ASM("asm/non_matchings/update_objects/func_80086074.s")
#endif

void func_80086110(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) {
    case 1:
        func_80086074(objectIndex, arg1);
        break;
    case 0:
        break;
    }
}

void update_object_neon(s32 objectIndex, s32 id) {
    switch (id) {                                 /* irregular */
        case 0:
            func_80085CA0(objectIndex);
            break;
        case 1:
            func_80085E38(objectIndex);
            break;
        case 2:
            func_80085F74(objectIndex);
            break;
    }
    if (id >= 3) {
        func_80086110(objectIndex, id - 3);
    }
}

void update_neon(void) {
    s32 objectIndex;
    s32 id;

    for (id = 0; id < NUM_NEON_SIGNS; id++) {
        objectIndex = indexObjectList1[id];
        if (gObjectList[objectIndex].state != 0) {
            update_object_neon(objectIndex, id);
            if (gObjectList[objectIndex].state >= 2) {
                update_neon_texture(objectIndex);
                object_calculate_new_pos_offset(objectIndex);
            }
        }
    }
}

void func_8008629C(s32 objectIndex, s32 arg1) {
    switch (arg1) {                                 /* irregular */
    case 0:
        gObjectList[objectIndex].model = podium_dl3;
        gObjectList[objectIndex].unk_04C = 0x00000038;
        break;
    case 1:
        gObjectList[objectIndex].model = podium2_dl3;
        gObjectList[objectIndex].unk_04C = 0x0000002B;
        break;
    case 2:
        gObjectList[objectIndex].model = podium3_dl3;
        gObjectList[objectIndex].unk_04C = 0x0000001E;
        break;
    default:
        break;
    }
    gObjectList[objectIndex].sizeScaling = 1.0f;
    set_obj_origin_pos(objectIndex, D_800E634C[0][0] - 1.5, D_800E634C[0][1], D_800E634C[0][2]);
    set_obj_origin_offset(objectIndex, 0.0f, -10.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0xF8E4U, 0U);
    gObjectList[objectIndex].unk_048 = 0;
    func_80072488(objectIndex);
}

void func_80086424(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
    case 0:
        break;
    case 1:
        gObjectList[objectIndex].velocity[1] = 0.75f;
        func_80086FD4(objectIndex);
        break;
    case 2:
        if (gObjectList[objectIndex].offset[1] >= -2.0) {
            gObjectList[objectIndex].velocity[1] -= 0.1;
        }
        object_add_velocity_offset_y(objectIndex);
        if (gObjectList[objectIndex].offset[1] >= 0.0) {
            gObjectList[objectIndex].offset[1] = 0.0f;
            gObjectList[objectIndex].velocity[1] = 0.0f;
            func_80086F60(objectIndex);
        }
        break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void func_80086528(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 1:
        func_8008629C(objectIndex, arg1);
        break;
    case 2:
        if (func_8007278C(objectIndex, gObjectList[objectIndex].unk_04C) != 0) {
            func_80091440(arg1);
            func_80086E70(objectIndex);
            func_80072488(objectIndex);
        }
        break;
    case 0:
        break;
    case 3:
        if (gObjectList[objectIndex].unk_0AE == 0) {
            gObjectList[objectIndex].unk_048 = 1;
            func_80072488(objectIndex);
        }
        break;
    }
}

void func_80086604(void) {
    s32 objectIndex;
    s32 var_s1;

    if ((D_8016347C != 0) && (D_802874D8.unk1D < 3)) {
        if (D_801658C6 == 0) {
            for(var_s1 = 0; var_s1 < 3; var_s1++) {
               objectIndex = indexObjectList1[var_s1];
                init_object(objectIndex, 0);
            }
            D_801658C6 = 1;
        }
    }
    for(var_s1 = 0; var_s1 != 3; var_s1++) {
       objectIndex = indexObjectList1[var_s1];
        if (gObjectList[objectIndex].state != 0) {
            func_80086528(objectIndex, var_s1);
            func_80086424(objectIndex);
        }
    }
}

void func_80086700(s32 objectIndex) {
    if (gCCSelection < CC_150) {
        switch (D_802874D8.unk1D) {                       /* switch 1; irregular */
        case 0:                                     /* switch 1 */
            gObjectList[objectIndex].model = gold_trophy_dl10;
            break;
        case 1:                                     /* switch 1 */
            gObjectList[objectIndex].model = gold_trophy_dl12;
            break;
        case 2:                                     /* switch 1 */
            gObjectList[objectIndex].model = gold_trophy_dl14;
            break;
        default:                                    /* switch 1 */
            break;
        }
    } else {
        switch (D_802874D8.unk1D) {                       /* irregular */
        case 0:
            gObjectList[objectIndex].model = gold_trophy_dl11;
            break;
        case 1:
            gObjectList[objectIndex].model = gold_trophy_dl13;
            break;
        case 2:
            gObjectList[objectIndex].model = gold_trophy_dl15;
            break;
        default:
            break;
        }
    }
    gObjectList[objectIndex].sizeScaling = 0.005f;
    set_obj_origin_pos(objectIndex, gObjectList[indexObjectList2[0]].pos[0], gObjectList[indexObjectList2[0]].pos[1] + 16.0, gObjectList[indexObjectList2[0]].pos[2]);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    gObjectList[objectIndex].unk_084[1] = 0x0200;
    func_80072488(objectIndex);
    func_80086E70(objectIndex);
}

void func_80086940(s32 objectIndex) {
    Object *object;

    object = &gObjectList[objectIndex];
    switch (object->unk_0AE) {
    case 0:
        break;
    case 1:
        func_80086FD4(objectIndex);
        break;
    case 2:
        f32_step_towards(&object->sizeScaling, 0.025f, 0.001f);
        func_80087C48(objectIndex, 6.0f, 0.1f, 0x000000C8);
        if ((f64) object->velocity[1] <= 0.0) {
            func_8008701C(objectIndex, 3);
        }
        break;
    case 3:
        func_800871AC(objectIndex, 0x00000064);
        break;
    case 4:
        D_801658D6 = 1;
        object->velocity[1] = -0.4f;
        func_80086FD4(objectIndex);
        object->origin_pos[1] = 90.0f;
        object->offset[1] = 60.0f;
        switch (D_802874D8.unk1D) {                          /* switch 1; irregular */
        case 1:                                     /* switch 1 */
            object->origin_pos[0] -= 3.0;
            object->origin_pos[2] += 15.0;
            break;
        case 2:                                     /* switch 1 */
            object->origin_pos[0] -= 2.0;
            object->origin_pos[2] -= 15.0;
            break;
        }
        break;
    case 5:
        if ((f64) object->offset[1] <= 8.0) {
            f32_step_towards(&object->velocity[1], -0.1f, -0.01f);
        }
        object_add_velocity_offset_y(objectIndex);
        if ((f64) object->offset[1] <= 0.0) {
            func_80086FD4(objectIndex);
        }
        break;
    case 6:
        if (func_800871AC(objectIndex, 0x00000041) != 0) {
            D_801658F4 = 1;
        }
        break;
    case 7:
        if (func_800871AC(objectIndex, 0x00000064) != 0) {
            func_8009265C();
            func_80086F60(objectIndex);
        }
        break;
    }
    if (D_801658D6 != 0) {
        object->direction_angle[0] += 0x400;
        object->direction_angle[1] = 0xE800;
        object->direction_angle[2] = 0xDA00;
    } else {
        object->direction_angle[0] += 0x400;
        object->direction_angle[1] -= 0x200;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void func_80086C14(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {                              /* irregular */
    case 1:
        func_80086700(objectIndex);
        break;
    case 0:
    case 2:
        break;
    }
}

void func_80086C6C(s32 objectIndex) {
    Vec3f sp24;

    sp24[0] = (gObjectList[objectIndex].pos[0] - 5.0f) + random_int(0x000AU);
    sp24[2] = (gObjectList[objectIndex].pos[2] - 5.0f) + random_int(0x000AU);
    if (D_801658F4 != 0) {
        sp24[1] = gObjectList[objectIndex].pos[1] + 14.0;
    } else {
        sp24[1] = gObjectList[objectIndex].pos[1] - 2.0;
    }
    func_800773D8(sp24, (s32) D_801658F4);
}

void func_80086D80(void) {
    s32 temp_s2;
    s32 var_s0;

    if ((D_801658CE != 0) && (D_801658DC == 0)) {
        temp_s2 = indexObjectList1[3];
        init_object(temp_s2, 0);
        D_801658DC = 1;
    }
    temp_s2 = indexObjectList1[3];
    if (gObjectList[temp_s2].state != 0) {
        func_80086C14(temp_s2);
        func_80086940(temp_s2);
        if (D_801658F4 != 0) {
            if (D_8016559C == 0) {
                func_80086C6C(temp_s2);
            }
        } else {
            for (var_s0 = 0; var_s0 < 2; var_s0++) {
                func_80086C6C(temp_s2);
            }
        }
    }
}
