#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <decode.h>
#include <mk64.h>
#include <stdio.h>

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
#include <assets/models/common_data.h>
#include "audio/external.h"
#include "sounds.h"
#include <actors.h>
#include "race_logic.h"
#include "collision.h"
#include "effects.h"
#include "code_80091440.h"
#include "menu_items.h"
#include "podium_ceremony_actors.h"
#include "courses/all_course_data.h"
#include <assets/models/ceremony_data.h>
#include "src/ending/ceremony_and_credits.h"
#include "menus.h"
#include <assets/textures/other_textures.h>
#include "data/some_data.h"
#include "memory.h"
#include "courses/all_course_offsets.h"
#include "courses/all_course_data.h"
#include <assets/textures/boo_frames.h>
#include "port/Game.h"
#include <assets/textures/tracks/banshee_boardwalk/banshee_boardwalk_data.h>
#include <assets/textures/tracks/sherbet_land/sherbet_land_data.h>
#include <assets/textures/tracks/rainbow_road/rainbow_road_data.h>

float OTRGetAspectRatio(void);

//! @todo unused?
f32 D_800E43B0[] = { 65536.0, 0.0, 1.0, 0.0, 0.0, 65536.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

Vtx D_800E43F0[] = {
    { { { -24, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx D_800E4470[] = {
    { { { -24, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx D_800E44B0[] = {
    { { { -24, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

u16* gItemWindowTLUTs[] = { common_tlut_item_window_none,
                            common_tlut_item_window_banana,
                            common_tlut_item_window_banana_bunch,
                            common_tlut_item_window_green_shell,
                            common_tlut_item_window_triple_green_shell,
                            common_tlut_item_window_red_shell,
                            common_tlut_item_window_triple_red_shell,
                            common_tlut_item_window_blue_shell,
                            common_tlut_item_window_thunder_bolt,
                            common_tlut_item_window_fake_item_box,
                            common_tlut_item_window_star,
                            common_tlut_item_window_boo,
                            common_tlut_item_window_mushroom,
                            common_tlut_item_window_double_mushroom,
                            common_tlut_item_window_triple_mushroom,
                            common_tlut_item_window_super_mushroom };

u8* gItemWindowTextures[] = { common_texture_item_window_none,
                              common_texture_item_window_banana,
                              common_texture_item_window_banana_bunch,
                              common_texture_item_window_green_shell,
                              common_texture_item_window_triple_green_shell,
                              common_texture_item_window_red_shell,
                              common_texture_item_window_triple_red_shell,
                              common_texture_item_window_blue_shell,
                              common_texture_item_window_thunder_bolt,
                              common_texture_item_window_fake_item_box,
                              common_texture_item_window_star,
                              common_texture_item_window_boo,
                              common_texture_item_window_mushroom,
                              common_texture_item_window_double_mushroom,
                              common_texture_item_window_triple_mushroom,
                              common_texture_item_window_super_mushroom };

u16* gHudLapTextures[] = { common_texture_hud_lap_1_on_3, common_texture_hud_lap_2_on_3,
                           common_texture_hud_lap_3_on_3 };

u16* gPortraitTLUTs[] = { common_tlut_portrait_mario, common_tlut_portrait_luigi,       common_tlut_portrait_yoshi,
                          common_tlut_portrait_toad,  common_tlut_portrait_donkey_kong, common_tlut_portrait_wario,
                          common_tlut_portrait_peach, common_tlut_portrait_bowser };

u8* gPortraitTextures[] = { common_texture_portrait_mario,       common_texture_portrait_luigi,
                            common_texture_portrait_yoshi,       common_texture_portrait_toad,
                            common_texture_portrait_donkey_kong, common_texture_portrait_wario,
                            common_texture_portrait_peach,       common_texture_portrait_bowser };

s32 find_unused_obj_index(s32* arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = objectListSize;
    // clang-format off
    temp_v0 = 0; do { // this two SHOULD be on the same line
        // clang-format on
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
void delete_object(s32* objectIndex) {
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

s32 add_unused_obj_index(s32* listIdx, s32* nextFree, s32 size) {
    s32 count;
    s32 objectIndex;
    s32* id;

    if (*nextFree >= size) {
        *nextFree = 0;
    }
    count = 0;
    id = &listIdx[*nextFree];

    for (count = 0; count < size; count++) {
        if (*id == NULL_OBJECT_ID) {
            objectIndex = find_unused_obj_index(id);
            *nextFree += 1;
            break;
        } else {
            *nextFree += 1;
            if (*nextFree >= size) {
                *nextFree = 0;
            }
            id = &listIdx[*nextFree];
        }
    }
    if (count == size) {
        objectIndex = NULL_OBJECT_ID;
    }
    return objectIndex;
}

void delete_object_wrapper(s32* arg0) {
    delete_object(arg0);
}

void func_80072120(s32* arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        clear_object_flag(*arg0, 0x00600000);
        arg0++;
    }
}

void func_80072180(void) {
    if (gModeSelection == TIME_TRIALS) {
        if (((gPlayerOne->type & PLAYER_EXISTS) != 0) &&
            ((gPlayerOne->type & (PLAYER_INVISIBLE_OR_BOMB | PLAYER_CPU)) == 0)) {
            gPostTimeTrialReplayCannotSave = 1;
        }
    }
}

void set_object_flag(s32 objectIndex, s32 flag) {
    gObjectList[objectIndex].status |= flag;
}

void clear_object_flag(s32 objectIndex, s32 flag) {
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

void set_object_timer_state(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].isTimerActive = arg1;
}

void init_object(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].status = 0;
    gObjectList[objectIndex].unk_058 = 0;
    gObjectList[objectIndex].unk_05C = 0;
    gObjectList[objectIndex].unk_0CD = 0;
    gObjectList[objectIndex].unk_0CF = 0;
    set_object_timer_state(objectIndex, 0);
    gObjectList[objectIndex].unk_0D8 = arg1;
    gObjectList[objectIndex].state = 1;
}

UNUSED void func_80072408(s32 objectIndex) {
    set_object_timer_state(objectIndex, 0);
}

void func_80072428(s32 objectIndex) {
    gObjectList[objectIndex].state = 0;
    gObjectList[objectIndex].unk_0D8 = 0;
    set_object_timer_state(objectIndex, 0);
    gObjectList[objectIndex].status = 0;
    gObjectList[objectIndex].unk_058 = 0;
    gObjectList[objectIndex].unk_05C = 0;
    func_80086F60(objectIndex);
}

void object_next_state(s32 objectIndex) {
    set_object_timer_state(objectIndex, 0);
    clear_object_flag(objectIndex, 0x2000);
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
    set_object_timer_state(objectIndex, 0);
    clear_object_flag(objectIndex, 0x2000);
    clear_object_flag(objectIndex, 8);
    object_next_state(objectIndex);
    func_800724F8(objectIndex, gObjectList[objectIndex].state);
    gObjectList[objectIndex].state = arg1;
}

void func_800725E8(s32 objectIndex, s32 arg1, s32 arg2) {
    set_object_timer_state(objectIndex, 0);
    clear_object_flag(objectIndex, 0x2000);
    clear_object_flag(objectIndex, 8);
    gObjectList[objectIndex].state = arg2;
    func_800724F8(objectIndex, gObjectList[objectIndex].state);
    gObjectList[objectIndex].state = arg1;
}

s16 func_80072530(s32); // extern

void func_8007266C(s32 objectIndex) {
    set_object_timer_state(objectIndex, 0);
    clear_object_flag(objectIndex, 0x2000);
    clear_object_flag(objectIndex, 8);
    gObjectList[objectIndex].state = func_80072530(objectIndex);
}

void func_800726CC(s32 objectIndex, s32 arg1) {
    set_object_timer_state(objectIndex, 0);
    clear_object_flag(objectIndex, 0x2000);
    gObjectList[objectIndex].state = arg1;
}

UNUSED void func_8007271C(s32 objectIndex, s32 arg1) {
    if (gObjectList[objectIndex].isTimerActive == 0) {
        gObjectList[objectIndex].state = arg1;
    }
}

UNUSED void func_8007274C(s32 objectIndex) {
    if (gObjectList[objectIndex].isTimerActive == 0) {
        object_next_state(objectIndex);
    }
}

/**
 * @brief will return true if the timer is done
 */
bool set_and_run_timer_object(s32 objectIndex, s32 timer) {
    bool phi_v1;

    phi_v1 = false;
    if (gObjectList[objectIndex].isTimerActive == false) {
        set_object_timer_state(objectIndex, true);
        gObjectList[objectIndex].timer = timer;
    }

    gObjectList[objectIndex].timer--;
    if (gObjectList[objectIndex].timer < 0) {
        set_object_timer_state(objectIndex, false);
        object_next_state(objectIndex);
        phi_v1 = true;
    }

    return phi_v1;
}

UNUSED s32 func_8007281C(s32 objectIndex, s32 arg1) {
    s32 phi_a2;

    phi_a2 = 0;
    if (gObjectList[objectIndex].isTimerActive == 0) {
        set_object_timer_state(objectIndex, 1);
        gObjectList[objectIndex].timer = random_int((u16) arg1);
    }

    gObjectList[objectIndex].timer--;
    if (gObjectList[objectIndex].timer < 0) {
        set_object_timer_state(objectIndex, 0);
        object_next_state(objectIndex);
        phi_a2 = 1;
    }

    return phi_a2;
}

// arg1 or arg2 may be a uintptr_t
UNUSED s32 func_800728B0(s32 objectIndex, s32 arg1, s32 arg2) {
    s32 phi_a3;

    phi_a3 = 0;
    if (gObjectList[objectIndex].isTimerActive == 0) {
        set_object_timer_state(objectIndex, 1);
        gObjectList[objectIndex].timer = random_int((u16) arg2) + arg1;
    }

    gObjectList[objectIndex].timer--;
    if (gObjectList[objectIndex].timer < 0) {
        set_object_timer_state(objectIndex, 0);
        object_next_state(objectIndex);
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

UNUSED void func_80072A78(s32 objectIndex, s32 arg1) {
    gObjectList[objectIndex].textureListIndex = arg1;
    object_next_state(objectIndex);
}

s32 func_80072AAC(s32 objectIndex, s32 arg1, s32 arg2) {
    s32 phi_v1;

    phi_v1 = 0;
    if (gObjectList[objectIndex].isTimerActive == 0) {

        set_object_timer_state(objectIndex, 1);
        gObjectList[objectIndex].textureListIndex = arg1;
        gObjectList[objectIndex].timer = arg2;
    }

    gObjectList[objectIndex].timer--;
    if (gObjectList[objectIndex].timer < 0) {
        set_object_timer_state(objectIndex, 0);
        object_next_state(objectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

s32 func_80072B48(s32 objectIndex, s32 arg1) {
    s32 phi_v1;

    phi_v1 = 0;
    if (gObjectList[objectIndex].isTimerActive == 0) {
        set_object_timer_state(objectIndex, 1);
        set_object_flag(objectIndex, 0x80000);
        gObjectList[objectIndex].textureListIndex = D_8018D140;
        gObjectList[objectIndex].timer = arg1;
    }

    gObjectList[objectIndex].timer--;
    if (gObjectList[objectIndex].timer < 0) {
        clear_object_flag(objectIndex, 0x80000);
        set_object_timer_state(objectIndex, 0);
        object_next_state(objectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

void func_80072C00(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3) {
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].timer = arg2;
        gObjectList[objectIndex].textureListIndex = arg1;
        gObjectList[objectIndex].unk_0D4 = 1;
        gObjectList[objectIndex].unk_0CC = arg3;
        set_object_flag(objectIndex, 0x2000);
    } else {
        gObjectList[objectIndex].timer--;
        if (gObjectList[objectIndex].timer < 0) {
            gObjectList[objectIndex].timer = arg2;
            gObjectList[objectIndex].unk_0D4--;
            if ((gObjectList[objectIndex].unk_0D4 & 1) != 0) {
                clear_object_flag(objectIndex, 0x80000);
            } else {
                set_object_flag(objectIndex, 0x80000);
            }

            if (gObjectList[objectIndex].unk_0D4 < 0) {
                gObjectList[objectIndex].unk_0D4 = 1;

                if (gObjectList[objectIndex].unk_0CC > 0) {
                    gObjectList[objectIndex].unk_0CC--;
                }

                if (gObjectList[objectIndex].unk_0CC == 0) {
                    clear_object_flag(objectIndex, 0x2000);
                    object_next_state(objectIndex);
                }
            }
        }
    }
}

void func_80072D3C(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].timer = arg3;
        gObjectList[objectIndex].textureListIndex = arg1;
        gObjectList[objectIndex].unk_0D4 = 1;
        gObjectList[objectIndex].unk_0CC = arg4;
        set_object_flag(objectIndex, 0x2000);
        return;
    }

    gObjectList[objectIndex].timer--;
    if (gObjectList[objectIndex].timer < 0) {
        gObjectList[objectIndex].timer = arg3;
        gObjectList[objectIndex].unk_0D4--;
        if ((gObjectList[objectIndex].unk_0D4 & 1) != 0) {
            gObjectList[objectIndex].textureListIndex = arg1;
        } else {
            gObjectList[objectIndex].textureListIndex = arg2;
        }

        if (gObjectList[objectIndex].unk_0D4 < 0) {
            gObjectList[objectIndex].unk_0D4 = 1;
            if (gObjectList[objectIndex].unk_0CC > 0) {
                gObjectList[objectIndex].unk_0CC--;
            }

            if (gObjectList[objectIndex].unk_0CC == 0) {
                clear_object_flag(objectIndex, 0x2000);
                object_next_state(objectIndex);
            }
        }
    }
}

s32 func_80072E54(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = 0;
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].textureListIndex = arg1;
        gObjectList[objectIndex].timer = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_timer_state(objectIndex, 1);
        set_object_flag(objectIndex, 0x2000);
    } else {
        gObjectList[objectIndex].timer--;
        if (gObjectList[objectIndex].timer <= 0) {
            gObjectList[objectIndex].timer = arg4;
            gObjectList[objectIndex].textureListIndex += arg3;
            if (arg2 < gObjectList[objectIndex].textureListIndex) {

                if (gObjectList[objectIndex].unk_0CC > 0) {
                    gObjectList[objectIndex].unk_0CC--;
                }
                if (gObjectList[objectIndex].unk_0CC == 0) {
                    gObjectList[objectIndex].textureListIndex = arg2;
                    clear_object_flag(objectIndex, 0x2000);
                    set_object_timer_state(objectIndex, 0);
                    object_next_state(objectIndex);
                    sp24 = 1;
                } else {
                    gObjectList[objectIndex].textureListIndex = arg1;
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
        gObjectList[objectIndex].textureListIndex = arg1;
        gObjectList[objectIndex].timer = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_timer_state(objectIndex, 1);
        set_object_flag(objectIndex, 0x2000);
    } else {
        gObjectList[objectIndex].timer--;
        if (gObjectList[objectIndex].timer <= 0) {
            gObjectList[objectIndex].timer = arg4;
            gObjectList[objectIndex].textureListIndex -= arg3;
            if (gObjectList[objectIndex].textureListIndex < arg2) {
                if (gObjectList[objectIndex].unk_0CC > 0) {
                    gObjectList[objectIndex].unk_0CC--;
                }
                if (gObjectList[objectIndex].unk_0CC == 0) {
                    gObjectList[objectIndex].textureListIndex = arg2;
                    clear_object_flag(objectIndex, 0x2000);
                    set_object_timer_state(objectIndex, 0);
                    object_next_state(objectIndex);
                    sp24 = true;
                } else {
                    gObjectList[objectIndex].textureListIndex = arg1;
                }
            }
        }
    }
    return sp24;
}

bool func_800730BC(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = false;
    if (is_obj_index_flag_status_inactive(objectIndex, 0x2000) != 0) {
        gObjectList[objectIndex].textureListIndex = arg1;
        gObjectList[objectIndex].timer = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_timer_state(objectIndex, 1);
        set_object_flag(objectIndex, 0x2000);
        clear_object_flag(objectIndex, 0x4000);
    } else {
        gObjectList[objectIndex].timer--;
        if (gObjectList[objectIndex].timer <= 0) {
            gObjectList[objectIndex].timer = arg4;
            if (is_obj_index_flag_status_inactive(objectIndex, 0x4000) != 0) {
                gObjectList[objectIndex].textureListIndex += arg3;
                if (gObjectList[objectIndex].textureListIndex >= arg2) {
                    gObjectList[objectIndex].textureListIndex = arg2;
                    set_object_flag(objectIndex, 0x4000);
                }
            } else {
                gObjectList[objectIndex].textureListIndex -= arg3;
                if (arg1 >= gObjectList[objectIndex].textureListIndex) {
                    gObjectList[objectIndex].textureListIndex = arg1;
                    if (gObjectList[objectIndex].unk_0CC > 0) {
                        gObjectList[objectIndex].unk_0CC--;
                    }

                    if (gObjectList[objectIndex].unk_0CC == 0) {
                        clear_object_flag(objectIndex, 0x80);
                        clear_object_flag(objectIndex, 0x2000);
                        set_object_timer_state(objectIndex, 0);
                        object_next_state(objectIndex);
                        sp24 = true;
                    } else {
                        clear_object_flag(objectIndex, 0x4000);
                        set_object_flag(objectIndex, 0x80);
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
        gObjectList[objectIndex].textureListIndex = arg1;
        gObjectList[objectIndex].timer = arg4;
        gObjectList[objectIndex].unk_0CC = arg5;
        set_object_timer_state(objectIndex, 1);
        set_object_flag(objectIndex, 0x2000);
        clear_object_flag(objectIndex, 0x4000);
    } else {
        gObjectList[objectIndex].timer--;
        if (gObjectList[objectIndex].timer <= 0) {
            gObjectList[objectIndex].timer = arg4;
            if (is_obj_index_flag_status_inactive(objectIndex, 0x4000) != 0) {
                gObjectList[objectIndex].textureListIndex -= arg3;
                if (arg2 >= gObjectList[objectIndex].textureListIndex) {
                    gObjectList[objectIndex].textureListIndex = arg2;
                    set_object_flag(objectIndex, 0x4000);
                }
            } else {
                gObjectList[objectIndex].textureListIndex += arg3;
                if (gObjectList[objectIndex].textureListIndex >= arg1) {
                    gObjectList[objectIndex].textureListIndex = arg1;
                    if (gObjectList[objectIndex].unk_0CC > 0) {
                        gObjectList[objectIndex].unk_0CC--;
                    }
                    if (gObjectList[objectIndex].unk_0CC == 0) {
                        clear_object_flag(objectIndex, 0x2000);
                        set_object_timer_state(objectIndex, 0);
                        object_next_state(objectIndex);
                        sp24 = 1;
                    } else {
                        clear_object_flag(objectIndex, 0x4000);
                    }
                }
            }
        }
    }
    return sp24;
}

void func_80073404(s32 objectIndex, u8 arg1, u8 arg2, Vtx* arg3) {
    gObjectList[objectIndex].vertex = arg3;
    gObjectList[objectIndex].textureWidth = arg1;
    gObjectList[objectIndex].textureHeight = arg2;
    gObjectList[objectIndex].textureListIndex = 0;
    gObjectList[objectIndex].unk_0D3 = -1;
    gObjectList[objectIndex].status = 0;
}

void init_texture_object(s32 objectIndex, u8* texture, const char** arg2, u8 arg3, u16 arg4) {
    gObjectList[objectIndex].tlutList = texture;
    gObjectList[objectIndex].textureList = arg2;
    gObjectList[objectIndex].textureWidth = arg3;
    gObjectList[objectIndex].textureHeight = arg4;
    gObjectList[objectIndex].textureListIndex = 0;
    gObjectList[objectIndex].unk_0D3 = -1;
    gObjectList[objectIndex].status = 0;
}

UNUSED void func_8007348C(s32 objectIndex, u8* arg1, u8 arg2, u8 arg3, Vtx* arg4) {
    gObjectList[objectIndex].activeTLUT = arg1;
    gObjectList[objectIndex].tlutList = arg1;
    gObjectList[objectIndex].textureWidth = arg2;
    gObjectList[objectIndex].textureHeight = arg3;
    gObjectList[objectIndex].vertex = arg4;
    gObjectList[objectIndex].textureListIndex = 0;
    gObjectList[objectIndex].unk_0D3 = -1;
    gObjectList[objectIndex].status = 0;
}

UNUSED void func_800734D4() {
}

void update_neon_texture(s32 objectIndex) {
    // I have no idea why this typecast works
    gObjectList[objectIndex].activeTLUT =
        (u8*) ((u32*) gObjectList[objectIndex].tlutList + (gObjectList[objectIndex].textureListIndex * 128));
    int idx = gObjectList[objectIndex].textureListIndex;
    char* texture = gObjectList[objectIndex].textureList[idx];
    gObjectList[objectIndex].activeTexture =
        gObjectList[objectIndex].textureList[gObjectList[objectIndex].textureListIndex];
}

void func_80073514(s32 objectIndex) {
    gObjectList[objectIndex].activeTLUT = gObjectList[objectIndex].tlutList;
    gObjectList[objectIndex].activeTexture =
        gObjectList[objectIndex].textureList[gObjectList[objectIndex].textureListIndex];
}

UNUSED void func_80073568() {
}

UNUSED void func_80073570(s32 objectIndex) {
    s16* test = &gObjectList[objectIndex].state;

    D_8018D1EC++;
    if (D_8018D1EC == 5) {
        D_8018D1EC = 0;
    }
    (*test)++;
}

void func_800735BC(s32 objectIndex, Gfx* arg1, f32 arg2) {
    gObjectList[objectIndex].status = 0;
    gObjectList[objectIndex].model = arg1;
    gObjectList[objectIndex].sizeScaling = arg2;
    object_next_state(objectIndex);
}

void func_80073600(s32 objectIndex) {
    set_object_timer_state(objectIndex, 0);
    gObjectList[objectIndex].unk_0D6 = 0;
    gObjectList[objectIndex].unk_04C = -1;
    clear_object_flag(objectIndex, 0x8000);
}

void func_80073654(s32 objectIndex) {
    clear_object_flag(objectIndex, 0x8000);
    ++gObjectList[objectIndex].unk_0D6;
}

UNUSED void func_8007369C(s32 objectIndex, s32 arg1) {
    clear_object_flag(objectIndex, 0x8000);
    gObjectList[objectIndex].unk_0D6 = arg1;
}

void func_800736E0(s32 objectIndex) {
    clear_object_flag(objectIndex, 0x8000);
    gObjectList[objectIndex].unk_0D6 = 1;
}

void func_80073720(s32 objectIndex) {
    clear_object_flag(objectIndex, 0x8000);
    gObjectList[objectIndex].unk_0D6 = 0;
}

bool func_8007375C(s32 objectIndex, s32 arg1) {
    s32 sp24;

    sp24 = false;
    if (is_obj_index_flag_status_inactive(objectIndex, 0x00008000) != 0) {
        gObjectList[objectIndex].unk_04C = arg1;
        set_object_flag(objectIndex, 0x00008000);
    }
    gObjectList[objectIndex].unk_04C--;
    if (gObjectList[objectIndex].unk_04C < 0) {
        clear_object_flag(objectIndex, 0x00008000);
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

void func_80074510(const char** lakituTexturePtr, const char* devAddr, size_t vaddr, size_t nbytes) {
    func_800744CC();
    lakituTexturePtr[vaddr] = devAddr;
    D_8018D224 = 1;
}

void func_80074574(const char** lakituTexturePtr, const char* devAddr, size_t vaddr, u16 width, u16 height) {
    func_80074510(lakituTexturePtr, devAddr, vaddr, width * height);
}

//! @todo arg1 should likely be a u8 *
void func_800745C8(s32 objectIndex, const char** lakituTexturePtr) {
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

void func_8007466C(s32 objectIndex, const char** lakituTexturePtr) {
    s32 phi_a1;

    if ((gObjectList[objectIndex].status & 1) != 0) {
        // I have no idea why this typecase works
        gObjectList[objectIndex].activeTLUT =
            (u8*) ((u32*) gObjectList[objectIndex].tlutList + (gObjectList[objectIndex].unk_0D3 << 7));
        gObjectList[objectIndex].status ^= 2;
        phi_a1 = 0;
        if ((gObjectList[objectIndex].status & 2) != 0) {
            phi_a1 = 1;
        }
        gObjectList[objectIndex].activeTexture = lakituTexturePtr[phi_a1];
        func_800744A0(objectIndex);
    }
}

void func_80074704(s32 objectIndex, const char** lakituTexturePtr) {
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
size_t func_80074790(s32 objectIndex, const char** lakituTexturePtr) {
    s32 phi_a2;

    gObjectList[objectIndex].status ^= 4;
    phi_a2 = 0;
    if ((gObjectList[objectIndex].status & 4) != 0) {
        phi_a2 = 1;
    }
    return phi_a2;
}

void func_800747F0(s32 objectIndex, const char** lakituTexturePtr) {
    const char* nextTexture = NULL;
    if (gObjectList[objectIndex].textureListIndex != gObjectList[objectIndex].unk_0D3) {
        nextTexture = gObjectList[objectIndex].textureList[gObjectList[objectIndex].textureListIndex];

        if (nextTexture != NULL) {
            func_80074574(lakituTexturePtr, nextTexture, func_80074790(objectIndex, lakituTexturePtr),
                          gObjectList[objectIndex].textureWidth, gObjectList[objectIndex].textureHeight);
            gObjectList[objectIndex].unk_0D3 = gObjectList[objectIndex].textureListIndex;
            func_80074478(objectIndex);
        }
    }
}

void func_80074894(s32 objectIndex, const char** lakituTexturePtr) {
    func_800747F0(objectIndex, lakituTexturePtr);
    func_800745C8(objectIndex, lakituTexturePtr);
}

void func_800748C4(s32 objectIndex, const char** lakituTexturePtr) {
    func_800747F0(objectIndex, lakituTexturePtr);
    func_8007466C(objectIndex, lakituTexturePtr);
}

void func_800748F4(s32 objectIndex, const char** lakituTexturePtr) {
    func_800747F0(objectIndex, lakituTexturePtr);
    func_80074704(objectIndex, lakituTexturePtr);
}

void func_800750D8(s32 objectIndex, s32 arg1, Vec3f arg2, s32 arg3, s32 arg4) {
    s32 sp24;
    s32 temp_v0;
    Object* object;

    init_object(objectIndex, 0);
    sp24 = random_int(0x01F4U);
    temp_v0 = random_int(0x0032U);
    object = &gObjectList[objectIndex];
    object->unk_0D5 = arg3;
    object->sizeScaling = ((f64) (f32) sp24 * 0.0005) + 0.05;
    // ALL HAIL THE FAKE MATCH GODS!!!!!
    object->velocity[1] = ((f64) (f32) temp_v0 * (0.05 * 1.0)) + 2.0;
    object->unk_034 = ((f64) (f32) (temp_v0 % 5) * 0.1) + 1.0;
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
        switch (arg1) { /* irregular */
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
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
        func_800750D8(objectIndex, var_s1, arg0, arg2, arg3);
    }
}

void init_train_smoke(s32 objectIndex, Vec3f pos, f32 velocity) {
    Object* object;
    UNUSED s32 pad[2];

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->origin_pos[0] = pos[0];
    object->origin_pos[1] = pos[1];
    object->origin_pos[2] = pos[2];
    object->velocity[1] = velocity;
    object->type = random_int(0x0064U) + 0x1E;
}

s32 spawn_train_smoke(s32 trainIndex, Vec3f pos, f32 velocity) {
    s32 objectIndex;

    if (trainIndex == 0) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            init_train_smoke(objectIndex, pos, velocity);
        }
    } else {
        objectIndex = add_unused_obj_index(gObjectParticle3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            init_train_smoke(objectIndex, pos, velocity);
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
    object_next_state(objectIndex);
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
                object_next_state(objectIndex);
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
    Object* object;
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

void init_ferry_smoke(s32 objectIndex, Vec3f pos, f32 velocity) {
    Object* object;

    init_object(objectIndex, 0);
    object = &gObjectList[objectIndex];
    object->origin_pos[0] = pos[0];
    object->origin_pos[1] = pos[1];
    object->origin_pos[2] = pos[2];
    object->velocity[1] = velocity;
    object->type = 0x00FF;
    object->unk_0A2 = 0x0096;
}

s32 spawn_ferry_smoke(s32 ferryIndex, Vec3f pos, f32 velocity) {
    s32 objectIndex;

    if (ferryIndex == 0) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            init_ferry_smoke(objectIndex, pos, velocity);
        }
    } else {
        objectIndex = add_unused_obj_index(gObjectParticle3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            init_ferry_smoke(objectIndex, pos, velocity);
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
    object_next_state(objectIndex);
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
                object_next_state(objectIndex);
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
    Object* object;
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
    Object* object;

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
        if (func_80075F28(arg0, arg1, arg2, someIndex) == -1) {
            break;
        }
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
            func_800C9D80(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity,
                          SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x0A));
            if (gObjectList[objectIndex].type > 0) {
                gObjectList[objectIndex].type--;
                gObjectList[objectIndex].unk_04C = 0x5A;
            } else {
                gObjectList[objectIndex].unk_04C = 0x12C;
            }

            gObjectList[objectIndex].unk_048 = 0x3C;
        }
    }

    if (are_players_in_course_section(9, 0xB) == 0) {
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
    Object* object;

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
        if (func_80076278(arg0, arg1, someIndex) == -1) {
            break;
        }
    }
}

void func_8007634C(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->activeTexture = common_texture_particle_smoke;
    object->textureList = common_texture_particle_smoke;
    object->primAlpha = 0x00FF;
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object_next_state(objectIndex);
}

void func_800763CC(s32 objectIndex) {
    Object* object;

    if (objectIndex) {}
    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 1:
            func_8007634C(objectIndex);
            break;
        case 2:
            if (set_and_run_timer_object(objectIndex, object->unk_048) != 0) {
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
            if ((object->unk_0AE >= 2) &&
                (func_80073B00(objectIndex, &object->primAlpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                object_next_state(objectIndex);
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
    if (are_players_in_course_section(4, 5) == 0) {
        gObjectList[objectIndex].type = 2;
    }
}

void func_8007675C(s32 objectIndex, Vec3s arg1, s32 arg2) {
    Object* object;

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
    s16* var_s2;
    temp_v0 = random_int(0x000FU);
    switch (arg0) {
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

    for (i = 0; i < 1; i++) {
        if (func_80076828(var_s2, i) == (-1)) {
            break;
        }
    }
}

void func_80076958(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->activeTexture = common_texture_particle_smoke;
    object->textureList = common_texture_particle_smoke;
    object->primAlpha = 0x00FF;
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object_next_state(objectIndex);
}

void func_800769D8(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 0:
            break;
        case 1:
            func_80076958(objectIndex);
            break;
        case 2:
            if (set_and_run_timer_object(objectIndex, object->unk_048) != 0) {
                func_80086E70(objectIndex);
            }
            break;
        case 3:
            f32_step_towards(&object->sizeScaling, 2.0f, 0.05f);
            s16_step_towards(&object->type, 0, 0x0018);
            if ((object->unk_0AE >= 2) &&
                (func_80073B00(objectIndex, &object->primAlpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                object_next_state(objectIndex);
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
    if (thing) {}
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
    Object* object;

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

void init_object_smoke_particle(s32 objectIndex, Vec3f arg1, s16 arg2) {
    Object* object;

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
        init_object_smoke_particle(objectIndex, arg0, arg2);
    }
}

void func_80076DC4(s32 objectIndex) {
    object_next_state(objectIndex);
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
            if ((gObjectList[objectIndex].unk_0AE >= 2) &&
                (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0x000000FF, 0x00000050, 0x00000020, 0,
                               0) != 0)) {
                object_next_state(objectIndex);
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
    Object* object;

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

void init_object_smoke_particle2(s32 objectIndex, s32 flameIndex) {
    init_object(objectIndex, 3);

    gObjectList[objectIndex].unk_0D5 = 0xB;
    gObjectList[objectIndex].activeTexture = common_texture_particle_smoke[0];
    gObjectList[objectIndex].textureList = common_texture_particle_smoke[0];
    gObjectList[objectIndex].sizeScaling = 0.8f;

    gObjectList[objectIndex].origin_pos[0] = (f32) * (gTorchSpawns + (flameIndex * 3) + 0) * xOrientation;
    gObjectList[objectIndex].origin_pos[1] = (f32) * (gTorchSpawns + (flameIndex * 3) + 1);
    gObjectList[objectIndex].origin_pos[2] = (f32) * (gTorchSpawns + (flameIndex * 3) + 2);
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
        init_object_smoke_particle2(objectIndex, arg0);
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

    if (IsMarioRaceway()) {
        object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
        gObjectList[objectIndex].unk_034 = 1.5f;
        gObjectList[objectIndex].velocity[1] = 1.5f;
    } else if (IsYoshiValley()) {
        object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
        gObjectList[objectIndex].unk_034 = 2.0f;
        gObjectList[objectIndex].velocity[1] = 2.0f;
    } else if (IsRoyalRaceway()) {
        object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 30.0, arg1[2], 0x10, 0x28, 0x10);
        gObjectList[objectIndex].unk_034 = 2.0f;
        gObjectList[objectIndex].velocity[1] = 2.0f;
    } else if (IsLuigiRaceway()) {
        object_origin_pos_randomize_around_xyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
        gObjectList[objectIndex].unk_034 = 1.5f;
        gObjectList[objectIndex].velocity[1] = 1.0f;
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
        if (init_leaf_particle(arg0, i) == NULL_OBJECT_ID) {
            break;
        }
    }
}

void func_80077B14(s32 arg0) {
    object_next_state(arg0);
    func_80086E70(arg0);
}

void func_80077B3C(s32 objectIndex) {
    Object* object;
    object = &gObjectList[objectIndex];

    switch (object->state) {
        case 0:
            break;
        case 1:
            func_80077B14(objectIndex);
            break;
        case 2:
            if (object->unk_0AE == 0) {
                object_next_state(objectIndex);
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
            func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.2f, gObjectList[objectIndex].unk_034,
                          (s16) (s32) gObjectList[objectIndex].direction_angle[1], 0x0000000A);
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
    Object* object;

    for (someIndex = 0; someIndex < gLeafParticle_SIZE; someIndex++) {
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
    Vtx* vtx = (Vtx*) LOAD_ASSET(common_vtx_rectangle);
    Object* object;

    object = &gObjectList[objectIndex];
    object->activeTexture = D_0D0293D8;
    object->textureList = D_0D0293D8;
    //! @bug frappe snowland There's something up with the handling of common_vtx_rectangle and the loading of 0x10
    //! right here
    // root function: func_80078C70
    object->vertex = vtx;
    object->textureHeight = 0x10;
    object->textureWidth = object->textureHeight;
    object->sizeScaling = 0.15f;
    set_object_flag(objectIndex, 0x00000010);
    func_80086EF0(objectIndex);
    object->primAlpha = 0x00FF;
    object->unk_0D5 = 0;
    object->type = 0;
    object_next_state(objectIndex);
}

void func_80077EB8(s32 objectIndex, u16 arg1, Camera* camera) {
    s16 temp_v0;

    temp_v0 = camera->rot[1] - arg1;
    if ((temp_v0 >= D_8018D210) || (D_8018D208 >= temp_v0)) {
        gObjectList[objectIndex].offset[0] = D_8018D218 + (D_8018D1E8 * (f32) temp_v0);
        set_object_flag(objectIndex, 0x00000010);
        return;
    }
    clear_object_flag(objectIndex, 0x00000010);
}

void func_80077F64(s32 objectIndex, Camera* camera) {

    f64 rand;

    switch (gObjectList[objectIndex].unk_0AE) { /* irregular */
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

void func_800780CC(s32 objectIndex, Camera* camera) {
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 1:
            func_80077E20(objectIndex);
            return;
        case 2:
            func_80077F64(objectIndex, camera);
            if (gObjectList[objectIndex].unk_0AE == 0) {
                object_next_state(objectIndex);
                return;
            }
        case 0:
            return;
        case 3:
            func_80072428(objectIndex);
            break;
    }
}

void func_80078170(s32 arg0, Camera* arg1) {
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
    u8* tex = (u8*) LOAD_ASSET(D_0D0293D8);
    Vtx* vtx = (Vtx*) LOAD_ASSET(common_vtx_rectangle);
    Object* object;

    object = &gObjectList[objectIndex];
    object->activeTexture = tex;
    object->textureList = tex;
    object->vertex = vtx;
    object->sizeScaling = 0.15f;
    func_80086EF0(objectIndex);
    object_next_state(objectIndex);
}

void func_80078288(s32 objectIndex) {
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    UNUSED u16 pad;
    u16 temp_t6;

    switch (gObjectList[objectIndex].unk_0AE) { /* irregular */
        case 0:
            break;
        case 1:
            if (gGamestate != 9) {
                sp3A = ((gPlayerOne->speed / 18) * 216) / 2;
                sp3E = (random_int(0x000FU) - sp3A) + 0x2D;
                sp3C = random_int(0x012CU) + 0x1E;
                temp_t6 = camera1->rot[1] + ((s32) (random_int(0x3000U) - 0x1800) / (s16) ((sp3A / 15) + 1));
                gObjectList[objectIndex].origin_pos[0] = gPlayerOne->pos[0] + (sins(temp_t6) * sp3C);
                gObjectList[objectIndex].origin_pos[1] = sp3E + gPlayerOne->unk_074;
                gObjectList[objectIndex].origin_pos[2] = gPlayerOne->pos[2] + (coss(temp_t6) * sp3C);
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
            gObjectList[objectIndex].offset[0] =
                sins(gObjectList[objectIndex].direction_angle[0]) * gObjectList[objectIndex].unk_01C[0];
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
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 0:
            break;
        case 1:
            func_80078220(objectIndex);
            break;
        case 2:
            func_80078288(objectIndex);
            if (gObjectList[objectIndex].unk_0AE == 0) {
                object_next_state(objectIndex);
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

// This function adjusted to place clouds in the sky correctly
void func_800788F8(s32 objectIndex, u16 rot, Camera* camera) {
    s16 cameraRot;
    // Adjustable culling factor
    const float cullingFactor = OTRGetAspectRatio();

    // Calculate the cloud's rotation relative to the camera
    cameraRot = camera->rot[1] + rot;

    // Adjust bounds based on the culling factor
    s16 adjustedLowerBound = (s16) (D_8018D210 * cullingFactor);
    s16 adjustedUpperBound = (s16) (D_8018D208 * cullingFactor);

    // Check if the object is within the adjusted bounds
    if ((cameraRot >= adjustedLowerBound) && (adjustedUpperBound >= cameraRot)) {
        // Calculate and update the object's position
        gObjectList[objectIndex].unk_09C = (D_8018D218 + (D_8018D1E8 * cameraRot));

        // Mark the object as visible
        set_object_flag(objectIndex, 0x10);
    } else {
        // If outside the bounds, mark the object as not visible
        set_object_flag(objectIndex, 0x10);
    }
}

void update_clouds(s32 arg0, Camera* arg1, CloudData* cloudList) {
    s32 cloudIndex;
    s32 objectIndex;
    CloudData* cloud;

    for (cloudIndex = 0; cloudIndex < D_8018D1F0; cloudIndex++) {
        cloud = &cloudList[cloudIndex];
        objectIndex = D_8018CC80[arg0 + cloudIndex];
        func_800788F8(objectIndex, cloud->rotY, arg1);
    }
}

void update_stars(s32 arg0, Camera* camera, StarData* starList) {
    s32 starIndex;
    s32 objectIndex;
    StarData* star;

    for (starIndex = 0; starIndex < D_8018D1F0; starIndex++) {
        star = &starList[starIndex];
        objectIndex = D_8018CC80[arg0 + starIndex];
        func_800788F8(objectIndex, star->rotY, camera);
        switch (starIndex % 5U) {
            case 0:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000028, 0x000000B4, 0x000000FF, 0,
                              -1);
                break;
            case 1:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000080, 0x000000FF, 0x000000FF, 0,
                              -1);
                break;
            case 2:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000050, 0x000000C8, 0x000000FF, 0,
                              -1);
                break;
            case 3:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0, 0x0000009B, 0x000000FF, 0, -1);
                break;
            case 4:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x0000005A, 0x00000080, 0x000000FF, 0,
                              -1);
                break;
        }
    }
}

UNUSED void func_80078C68() {
}

void func_80078C70(s32 arg0) {
    s32 sp1C;
    Camera* camera;

    if (D_801657C8 == 0) {
        switch (arg0) { /* switch 1 */
            case 0:     /* switch 1 */
                sp1C = 0;
                camera = camera1;
                D_8018D200 = gCameraFOV[0] + 40.0f;
                break;
            case 1: /* switch 1 */
                sp1C = 0;
                camera = camera1;
                D_8018D200 = gCameraFOV[0] + 40.0f;
                break;
            case 2: /* switch 1 */
                camera = camera2;
                sp1C = D_8018D1F0;
                D_8018D200 = gCameraFOV[1] + 40.0f;
                break;
            case 3: /* switch 1 */
                sp1C = 0;
                camera = camera1;
                D_8018D200 = gCameraFOV[0] + 40.0f;
                break;
            case 4: /* switch 1 */
                camera = camera2;
                sp1C = D_8018D1F0;
                D_8018D200 = gCameraFOV[1] + 40.0f;
                break;
        }

        D_8018D208 = ((D_8018D200 / 2) * 0xB6) + 0x71C;
        D_8018D210 = (-(D_8018D200 / 2) * 0xB6) - 0x71C;
        D_8018D1E8 = 1.7578125 / D_8018D200;
        D_8018D218 = 0xA0;

        CM_UpdateClouds(sp1C, camera);

        // switch (gCurrentCourseId) {    /* switch 2 */
        //     case COURSE_MARIO_RACEWAY: /* switch 2 */
        //         // Uses Luigi Raceway's clouds for display purposes?
        //         //update_clouds(sp1C, camera, gLuigiRacewayClouds);
        //         break;
        // case COURSE_YOSHI_VALLEY: /* switch 2 */
        //     update_clouds(sp1C, camera, gYoshiValleyMooMooFarmClouds);
        //     break;
        // case COURSE_FRAPPE_SNOWLAND: /* switch 2 */
        //     func_80078170(sp1C, camera);
        //     break;
        // case COURSE_KOOPA_BEACH: /* switch 2 */
        //     update_clouds(sp1C, camera, gKoopaTroopaBeachClouds);
        //     break;
        // case COURSE_ROYAL_RACEWAY: /* switch 2 */
        //     update_clouds(sp1C, camera, gRoyalRacewayClouds);
        //     break;
        // case COURSE_LUIGI_RACEWAY: /* switch 2 */
        //     update_clouds(sp1C, camera, gLuigiRacewayClouds);
        //     break;
        // case COURSE_MOO_MOO_FARM: /* switch 2 */
        //     update_clouds(sp1C, camera, gYoshiValleyMooMooFarmClouds);
        //     break;
        // case COURSE_TOADS_TURNPIKE: /* switch 2 */
        //     update_stars(sp1C, camera, gToadsTurnpikeRainbowRoadStars);
        //     break;
        // case COURSE_KALIMARI_DESERT: /* switch 2 */
        //     update_clouds(sp1C, camera, gKalimariDesertClouds);
        //     break;
        // case COURSE_SHERBET_LAND: /* switch 2 */
        //     update_clouds(sp1C, camera, gSherbetLandClouds);
        //     break;
        // case COURSE_RAINBOW_ROAD: /* switch 2 */
        //     update_stars(sp1C, camera, gToadsTurnpikeRainbowRoadStars);
        //     break;
        // case COURSE_WARIO_STADIUM: /* switch 2 */
        //     update_stars(sp1C, camera, gWarioStadiumStars);
        //     break;
        //}
    }
}

void func_8007ABFC(s32 playerId, s32 itemId) {
    s32 itemWindow;

    if (playerHUD[playerId].raceCompleteBool == false) {
        itemWindow = gItemWindowObjectByPlayerId[playerId];
        if (func_80072354(itemWindow, 4) != 0) {
            init_object(itemWindow, 0);
            if (itemId != ITEM_NONE) {
                playerHUD[playerId].itemOverride = itemId;
            }
        }
        func_800C9060(playerId, 0x19008406U);
    }
}

void consume_item(s32 playerId) {
    UNUSED s32 pad;
    Player* player;
    s32 objectIndex;
    ItemWindowObjects* itemWindow;

    player = &gPlayerOne[playerId];
    objectIndex = gItemWindowObjectByPlayerId[playerId];
    itemWindow = (ItemWindowObjects*) &gObjectList[objectIndex];
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
            set_object_timer_state(objectIndex, 0);
            func_800726CC(objectIndex, 9);
        }
    } else {
        player->currentItemCopy = ITEM_NONE;
        itemWindow->currentItem = ITEM_NONE;
        set_object_timer_state(objectIndex, 0);
    }
}

typedef struct {
    u8 none;             // ITEM_NONE
    u8 banana;           // ITEM_BANANA
    u8 bananaBunch;      // ITEM_BANANA_BUNCH
    u8 greenShell;       // ITEM_GREEN_SHELL
    u8 tripleGreenShell; // ITEM_TRIPLE_GREEN_SHELL
    u8 redShell;         // ITEM_RED_SHELL
    u8 tripleRedShell;   // ITEM_TRIPLE_RED_SHELL
    u8 blueSpinyShell;   // ITEM_BLUE_SPINY_SHELL
    u8 thunderbolt;      // ITEM_THUNDERBOLT
    u8 fakeItemBox;      // ITEM_FAKE_ITEM_BOX
    u8 star;             // ITEM_STAR
    u8 boo;              // ITEM_BOO
    u8 mushroom;         // ITEM_MUSHROOM
    u8 doubleMushroom;   // ITEM_DOUBLE_MUSHROOM
    u8 tripleMushroom;   // ITEM_TRIPLE_MUSHROOM
    u8 superMushroom;    // ITEM_SUPER_MUSHROOM
} ItemProbabilities;

// Each row corresponds to a rank, each column to an item
ItemProbabilities grandPrixHumanProbabilityTable[] = {
    { .none = 0,
      .banana = 30,
      .bananaBunch = 5,
      .greenShell = 30,
      .tripleGreenShell = 5,
      .redShell = 5,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 5,
      .greenShell = 5,
      .tripleGreenShell = 10,
      .redShell = 15,
      .tripleRedShell = 20,
      .blueSpinyShell = 0,
      .thunderbolt = 5,
      .fakeItemBox = 5,
      .star = 5,
      .boo = 5,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 15,
      .superMushroom = 5 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 10,
      .redShell = 20,
      .tripleRedShell = 20,
      .blueSpinyShell = 0,
      .thunderbolt = 5,
      .fakeItemBox = 0,
      .star = 10,
      .boo = 0,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 20,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 15,
      .tripleRedShell = 20,
      .blueSpinyShell = 5,
      .thunderbolt = 10,
      .fakeItemBox = 0,
      .star = 15,
      .boo = 0,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 20,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 10,
      .tripleRedShell = 20,
      .blueSpinyShell = 5,
      .thunderbolt = 10,
      .fakeItemBox = 0,
      .star = 15,
      .boo = 0,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 25,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 0,
      .tripleRedShell = 20,
      .blueSpinyShell = 10,
      .thunderbolt = 15,
      .fakeItemBox = 0,
      .star = 20,
      .boo = 0,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 25,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 0,
      .tripleRedShell = 20,
      .blueSpinyShell = 10,
      .thunderbolt = 20,
      .fakeItemBox = 0,
      .star = 30,
      .boo = 0,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 10,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 0,
      .tripleRedShell = 20,
      .blueSpinyShell = 15,
      .thunderbolt = 20,
      .fakeItemBox = 0,
      .star = 30,
      .boo = 0,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 5,
      .superMushroom = 10 },
};

ItemProbabilities grandPrixCPUProbabilityTable[] = {
    { .none = 0,
      .banana = 60,
      .bananaBunch = 0,
      .greenShell = 25,
      .tripleGreenShell = 0,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 50,
      .bananaBunch = 0,
      .greenShell = 25,
      .tripleGreenShell = 5,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 40,
      .bananaBunch = 0,
      .greenShell = 25,
      .tripleGreenShell = 10,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 35,
      .bananaBunch = 0,
      .greenShell = 25,
      .tripleGreenShell = 15,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 30,
      .bananaBunch = 0,
      .greenShell = 20,
      .tripleGreenShell = 20,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 5,
      .star = 5,
      .boo = 0,
      .mushroom = 20,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 30,
      .bananaBunch = 0,
      .greenShell = 20,
      .tripleGreenShell = 20,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 5,
      .star = 5,
      .boo = 0,
      .mushroom = 20,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 30,
      .bananaBunch = 0,
      .greenShell = 20,
      .tripleGreenShell = 20,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 0,
      .star = 10,
      .boo = 0,
      .mushroom = 20,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 25,
      .bananaBunch = 0,
      .greenShell = 20,
      .tripleGreenShell = 20,
      .redShell = 0,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 1,
      .fakeItemBox = 0,
      .star = 10,
      .boo = 0,
      .mushroom = 24,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
};

ItemProbabilities grandPrixHardCPUProbabilityTable[] = {
    { .none = 5,
      .banana = 25,
      .bananaBunch = 5,
      .greenShell = 30,
      .tripleGreenShell = 5,
      .redShell = 5,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 5,
      .bananaBunch = 5,
      .greenShell = 5,
      .tripleGreenShell = 10,
      .redShell = 15,
      .tripleRedShell = 20,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 5,
      .star = 5,
      .boo = 5,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 15,
      .superMushroom = 5 },
    { .none = 0,
      .banana = 5,
      .bananaBunch = 3,
      .greenShell = 0,
      .tripleGreenShell = 10,
      .redShell = 20,
      .tripleRedShell = 19,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 0,
      .star = 10,
      .boo = 0,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 19,
      .superMushroom = 9 },
    { .none = 0,
      .banana = 5,
      .bananaBunch = 0,
      .greenShell = 5,
      .tripleGreenShell = 5,
      .redShell = 15,
      .tripleRedShell = 10,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 5,
      .star = 15,
      .boo = 5,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 20,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 10,
      .tripleRedShell = 20,
      .blueSpinyShell = 5,
      .thunderbolt = 0,
      .fakeItemBox = 5,
      .star = 15,
      .boo = 0,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 25,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 5,
      .tripleRedShell = 25,
      .blueSpinyShell = 10,
      .thunderbolt = 0,
      .fakeItemBox = 0,
      .star = 20,
      .boo = 0,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 25,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 5,
      .tripleGreenShell = 0,
      .redShell = 5,
      .tripleRedShell = 25,
      .blueSpinyShell = 10,
      .thunderbolt = 0,
      .fakeItemBox = 0,
      .star = 30,
      .boo = 0,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 10,
      .superMushroom = 10 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 5,
      .redShell = 5,
      .tripleRedShell = 20,
      .blueSpinyShell = 15,
      .thunderbolt = 10,
      .fakeItemBox = 0,
      .star = 30,
      .boo = 0,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 5,
      .superMushroom = 10 },
};

ItemProbabilities versus2PlayerProbabilityTable[] = {
    { .none = 0,
      .banana = 25,
      .bananaBunch = 10,
      .greenShell = 30,
      .tripleGreenShell = 5,
      .redShell = 5,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 5,
      .greenShell = 0,
      .tripleGreenShell = 5,
      .redShell = 5,
      .tripleRedShell = 15,
      .blueSpinyShell = 5,
      .thunderbolt = 15,
      .fakeItemBox = 0,
      .star = 15,
      .boo = 0,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 15,
      .superMushroom = 20 },
};

ItemProbabilities versus3PlayerProbabilityTable[] = {
    { .none = 0,
      .banana = 35,
      .bananaBunch = 5,
      .greenShell = 30,
      .tripleGreenShell = 0,
      .redShell = 5,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 5,
      .bananaBunch = 5,
      .greenShell = 0,
      .tripleGreenShell = 10,
      .redShell = 15,
      .tripleRedShell = 15,
      .blueSpinyShell = 0,
      .thunderbolt = 5,
      .fakeItemBox = 5,
      .star = 5,
      .boo = 5,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 20,
      .superMushroom = 5 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 10,
      .tripleRedShell = 20,
      .blueSpinyShell = 10,
      .thunderbolt = 15,
      .fakeItemBox = 0,
      .star = 15,
      .boo = 0,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 20,
      .superMushroom = 10 },
};

ItemProbabilities versus4PlayerProbabilityTable[] = {
    { .none = 0,
      .banana = 35,
      .bananaBunch = 5,
      .greenShell = 30,
      .tripleGreenShell = 0,
      .redShell = 5,
      .tripleRedShell = 0,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 10,
      .star = 0,
      .boo = 5,
      .mushroom = 10,
      .doubleMushroom = 0,
      .tripleMushroom = 0,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 5,
      .bananaBunch = 5,
      .greenShell = 5,
      .tripleGreenShell = 10,
      .redShell = 15,
      .tripleRedShell = 15,
      .blueSpinyShell = 0,
      .thunderbolt = 0,
      .fakeItemBox = 5,
      .star = 5,
      .boo = 5,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 25,
      .superMushroom = 0 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 5,
      .greenShell = 0,
      .tripleGreenShell = 5,
      .redShell = 10,
      .tripleRedShell = 15,
      .blueSpinyShell = 5,
      .thunderbolt = 10,
      .fakeItemBox = 5,
      .star = 10,
      .boo = 0,
      .mushroom = 5,
      .doubleMushroom = 0,
      .tripleMushroom = 25,
      .superMushroom = 5 },
    { .none = 0,
      .banana = 0,
      .bananaBunch = 0,
      .greenShell = 0,
      .tripleGreenShell = 0,
      .redShell = 0,
      .tripleRedShell = 20,
      .blueSpinyShell = 10,
      .thunderbolt = 15,
      .fakeItemBox = 0,
      .star = 20,
      .boo = 0,
      .mushroom = 0,
      .doubleMushroom = 0,
      .tripleMushroom = 25,
      .superMushroom = 10 },
};

ItemProbabilities battleProbabilityCurve[] = { { .none = 0,
                                                 .banana = 10,
                                                 .bananaBunch = 5,
                                                 .greenShell = 5,
                                                 .tripleGreenShell = 20,
                                                 .redShell = 20,
                                                 .tripleRedShell = 0,
                                                 .blueSpinyShell = 0,
                                                 .thunderbolt = 0,
                                                 .fakeItemBox = 15,
                                                 .star = 20,
                                                 .boo = 5,
                                                 .mushroom = 0,
                                                 .doubleMushroom = 0,
                                                 .tripleMushroom = 0,
                                                 .superMushroom = 0 } };

void getProbabilityArray(const ItemProbabilities* probStruct, u8* probArray) {
    probArray[ITEM_NONE] = probStruct->none;
    probArray[ITEM_BANANA] = probStruct->banana;
    probArray[ITEM_BANANA_BUNCH] = probStruct->bananaBunch;
    probArray[ITEM_GREEN_SHELL] = probStruct->greenShell;
    probArray[ITEM_TRIPLE_GREEN_SHELL] = probStruct->tripleGreenShell;
    probArray[ITEM_RED_SHELL] = probStruct->redShell;
    probArray[ITEM_TRIPLE_RED_SHELL] = probStruct->tripleRedShell;
    probArray[ITEM_BLUE_SPINY_SHELL] = probStruct->blueSpinyShell;
    probArray[ITEM_THUNDERBOLT] = probStruct->thunderbolt;
    probArray[ITEM_FAKE_ITEM_BOX] = probStruct->fakeItemBox;
    probArray[ITEM_STAR] = probStruct->star;
    probArray[ITEM_BOO] = probStruct->boo;
    probArray[ITEM_MUSHROOM] = probStruct->mushroom;
    probArray[ITEM_DOUBLE_MUSHROOM] = probStruct->doubleMushroom;
    probArray[ITEM_TRIPLE_MUSHROOM] = probStruct->tripleMushroom;
    probArray[ITEM_SUPER_MUSHROOM] = probStruct->superMushroom;
}

// Output a warning if a probability table does not add up to 100
void verify_probability_table(char* str, const ItemProbabilities* probs, int16_t rank) {
#ifndef _DEBUG
    return;
#endif
    u8 itemProbabilities[ITEM_MAX];

    getProbabilityArray(probs, itemProbabilities);
    size_t count = 0;
    for (size_t i = 0; i < ITEM_MAX; i++) {
        // printf("prob %d ", itemProbabilities[i]);
        count += itemProbabilities[i];
    }
    // printf("\n");

    if (count != 100) {
        printf("update_objects.c::verify_probability_table\n  %s table for rank %d is imba %d/100\n", str, rank, count);
    }
}

enum RandomItemOption {
    HUMAN_TABLE,
    CPU_TABLE,
    HARD_CPU_TABLE,
};

/**
 * New random item system uses chance based on percent
 * Likely functionally equivallent to the old system but easier to modify
 */
u8 gen_random_item(s16 rank, s16 option) {
#define PERCENTAGE_BASE 100
    u16 rand = random_int(PERCENTAGE_BASE);
#undef PERCENTAGE_BASE
    ItemProbabilities* distributionTable;
    u8 randomItem = 0;
    u8 cumulativeProbability = 0;

    switch (gModeSelection) {
        case GRAND_PRIX:
            switch (option) {
                case HUMAN_TABLE:
                    distributionTable = &grandPrixHumanProbabilityTable[rank];
                    verify_probability_table("Human", distributionTable, rank);
                    break;
                case CPU_TABLE:
                    distributionTable = &grandPrixCPUProbabilityTable[rank];
                    verify_probability_table("CPU", distributionTable, rank);
                    break;
                case HARD_CPU_TABLE:
                    distributionTable = &grandPrixHardCPUProbabilityTable[rank];
                    verify_probability_table("Hard CPU", distributionTable, rank);
                    break;
            }
            break;
        case VERSUS:
            switch (gPlayerCountSelection1) {
                case TWO_PLAYERS_SELECTED:
                    distributionTable = &versus2PlayerProbabilityTable[rank];
                    verify_probability_table("Versus 2P", distributionTable, rank);
                    break;
                case THREE_PLAYERS_SELECTED:
                    distributionTable = &versus3PlayerProbabilityTable[rank];
                    verify_probability_table("Versus 3P", distributionTable, rank);
                    break;
                case FOUR_PLAYERS_SELECTED:
                    distributionTable = &versus4PlayerProbabilityTable[rank];
                    verify_probability_table("Versus 4P", distributionTable, rank);
                    break;
            }
            break;
        case BATTLE:
            distributionTable = &battleProbabilityCurve[0];
            verify_probability_table("Battle", distributionTable, rank);
            break;
    }

    u8 itemProbabilities[ITEM_MAX];
    getProbabilityArray(distributionTable, itemProbabilities);

    for (size_t i = 0; i < ITEM_MAX; i++) {
        cumulativeProbability += itemProbabilities[i];
        if (rand < cumulativeProbability) {
            randomItem = i;
            break;
        }
    }
    return randomItem;
}

u8 gen_random_item_human(UNUSED s16 arg0, s16 rank) {
    if (CVarGetInteger("gHarderCPU", 0) == true) {
        return gen_random_item(rank, HARD_CPU_TABLE);
    } else {
        return gen_random_item(rank, HUMAN_TABLE);
    }
}

u8 cpu_gen_random_item(UNUSED s32 arg0, s16 rank) {
    return gen_random_item(rank, CPU_TABLE);
}

u8 hard_cpu_gen_random_item(UNUSED s32 arg0, s16 rank) {
    return gen_random_item(rank, HARD_CPU_TABLE);
}

s16 func_8007AFB0(s32 objectIndex, s32 playerId) {
    UNUSED s32 pad[3];
    s16 randomItem;

    randomItem = gen_random_item_human(gLapCountByPlayerId[playerId], gGPCurrentRaceRankByPlayerId[playerId]);

    if (randomItem == ITEM_NONE) {
        play_sound2(SOUND_MENU_FILE_NOT_FOUND);
    }

    if (playerHUD[playerId].itemOverride != 0) {
        randomItem = (s16) playerHUD[playerId].itemOverride;
        playerHUD[playerId].itemOverride = 0;
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
    s32 item;
    s32 sp50[4];
    s32 sp40[4];
    s32 var_v1_2;
    Player* sp38;
    s16 temp_a1;

    var_a3 = 0;
    var_t3 = 0;
    if (gModeSelection == GRAND_PRIX) {
        // Boo item
        if (random_int(0x0064U) < 0x51) {
            item = gen_random_item_human(gLapCountByPlayerId[playerId], gGPCurrentRaceRankByPlayerId[playerId]);
            // ITEM_NONE is not a valid item for ghost, randomize again
            size_t attempts = 0;
            while (item == ITEM_NONE && attempts < 200) {
                item = gen_random_item_human(gLapCountByPlayerId[playerId], gGPCurrentRaceRankByPlayerId[playerId]);
                attempts++;
            }
            if (attempts >= 200) {
                printf("[update_objects.c] [func_8007B040]: Item table has ITEM_NONE set to 100 percent.\n");
            }
        } else {
            item = 0;
            func_800C9060(playerId, 0x1900A058U);
        }
        var_t3 = 1;
        gObjectList[objectIndex].textureListIndex = gObjectList[objectIndex].unk_0A2 = item;
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
            item = random_int(var_a3);
            temp_a1 = sp40[item];
            gObjectList[objectIndex].unk_0A2 = temp_a1;
            gObjectList[objectIndex].textureListIndex = temp_a1;
            temp_v1 = sp50[item];
            gObjectList[objectIndex].unk_0D1 = temp_v1;
            temp_a0 = gItemWindowObjectByPlayerId[temp_v1];
            sp38 = &gPlayerOne[temp_v1];
            func_800722A4(temp_a0, 1);
            gObjectList[temp_a0].type = 0;
            sp38->currentItemCopy = 0;
            if (func_80072320(temp_a0, 2) != 0) {
                func_800722CC(temp_a0, 2);
                gObjectList[temp_a0].animationTimer = 0;
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
    gObjectList[objectIndex].tlutList = (u8*) common_tlut_item_window_none;
    gObjectList[objectIndex].textureList = common_texture_item_window_none;
    gObjectList[objectIndex].textureWidth = 0x28;
    gObjectList[objectIndex].textureHeight = 0x20;
    gObjectList[objectIndex].unk_04C = 0x00000032;
    gObjectList[objectIndex].unk_0D6 = 1;
    gObjectList[objectIndex].animationTimer = 0;
    func_800722A4(objectIndex, 4);
    if (D_80165888 != 0) {
        func_800726CC(objectIndex, 8);
        gObjectList[objectIndex].unk_0D6 = 2;
        gObjectList[objectIndex].type = 0x000E;
        test = gObjectList[objectIndex].type;
        gObjectList[objectIndex].textureListIndex = test;
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
    Object* object;
    Player* sp38;
    struct Controller* new_var;

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
                    object_next_state(temp_s0);
                }
            } else if (gActiveScreenMode == 3) {
                if ((playerId == 0) || (playerId == 2)) {
                    s16_step_up_towards(&playerHUD[playerId].slideItemBoxX, 0x0080, 8);
                    if (playerHUD[playerId].slideItemBoxX == 0x0080) {
                        object_next_state(temp_s0);
                    }
                } else {
                    s16_step_down_towards(&playerHUD[playerId].slideItemBoxX, -0x0080, 8);
                    if (playerHUD[playerId].slideItemBoxX == -0x0080) {
                        object_next_state(temp_s0);
                    }
                }
            } else {
                s16_step_up_towards(&playerHUD[playerId].slideItemBoxX, 0x0080, 8);
                if (playerHUD[playerId].slideItemBoxX == 0x0080) {
                    object_next_state(temp_s0);
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
            object->textureListIndex = func_8007AFB0(temp_s0, playerId);
            object->unk_04C = 8;
            object->unk_0D6 = 2;
            object_next_state(temp_s0);
            func_800C9018((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x1C));
            func_800C8F80((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x47));
            break;
        case 7:
            func_80072D3C(temp_s0, (s32) object->unk_0A2, 0, 8, 0x0000000A);
            break;
        case 9:
            func_800722CC(temp_s0, 4);
            func_80073600(temp_s0);
            object->textureListIndex = 0;
            object_next_state(temp_s0);
            break;
        case 10:
            set_and_run_timer_object(temp_s0, 0x00000014);
            break;
        case 11:
            if (gActiveScreenMode == 0) {
                if (s16_step_down_towards(&playerHUD[playerId].slideItemBoxY, 0, 4) != 0) {
                    object_next_state(temp_s0);
                }
            } else if (gActiveScreenMode == 3) {
                if ((playerId == 0) || (playerId == 2)) {
                    s16_step_down_towards(&playerHUD[playerId].slideItemBoxX, 0, 8);
                    if (playerHUD[playerId].slideItemBoxX == 0) {
                        object_next_state(temp_s0);
                    }
                } else {
                    s16_step_up_towards(&playerHUD[playerId].slideItemBoxX, 0, 8);
                    if (playerHUD[playerId].slideItemBoxX == 0) {
                        object_next_state(temp_s0);
                    }
                }
            } else if (s16_step_down_towards(&playerHUD[playerId].slideItemBoxX, 0, 8) != 0) {
                object_next_state(temp_s0);
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
        if (object->animationTimer == 0) {
            consume_item(playerId);
        } else {
            object->animationTimer--;
        }
    }
    if (object->unk_04C >= 0) {
        if (object->unk_04C > 0) {
            object->unk_04C--;
        } else {
            switch (object->unk_0D6) { /* switch 1; irregular */
                case 1:                /* switch 1 */
                    if (sp40 != 0) {
                        func_80073600(temp_s0);
                        func_800726CC(temp_s0, 6);
                    }
                    break;
                case 2: /* switch 1 */
                    set_type_object(temp_s0, (s32) object->unk_0A2);
                    object->unk_0D6 = 3;
                    break;
                case 3: /* switch 1 */
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
    object->activeTLUT = (u8*) gItemWindowTLUTs[object->textureListIndex];
    object->activeTexture = gItemWindowTextures[object->textureListIndex];
    sp38->currentItemCopy = object->type;
}

void func_8007BB9C(s32 arg0) {
    func_8007B34C(arg0);
}

void wrapper_update_boos(void) {
    // update_boos();
}

// Updates the display status on an object based on its relative direction to the camera
void func_8007C360(s32 objectIndex, Camera* camera) {
    u16 rot = camera->rot[1];
    u16 temp = ((u16) (gObjectList[objectIndex].direction_angle[1] - rot + 0x8000) * 0x24) / 0x10000;

    if (temp < 0x13) {
        clear_object_flag(objectIndex, 0x80);
        gObjectList[objectIndex].textureListIndex = temp;
    } else {
        set_object_flag(objectIndex, 0x80);
        gObjectList[objectIndex].textureListIndex = 0x24 - temp;
    }
}

void func_8007C420(s32 objectIndex, Player* player, Camera* camera) {
    f32 x;
    f32 z;

    x = player->pos[0] - gObjectList[objectIndex].pos[0];
    z = player->pos[2] - gObjectList[objectIndex].pos[2];
    gObjectList[objectIndex].direction_angle[1] =
        func_800417B4(gObjectList[objectIndex].direction_angle[1], atan2s(x, z));
    func_8007C360(objectIndex, camera);
}

void func_8007CE0C(s32 objectIndex) {
    Object* object;

    init_texture_object(objectIndex, d_course_banshee_boardwalk_boo_tlut, gTextureGhosts, 0x30U, (u16) 0x00000028);
    object = &gObjectList[objectIndex];
    object->textureListIndex = 0x1C;
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    object->sizeScaling = 0.15f;
    object_next_state(objectIndex);
    object->primAlpha = 0;
    func_80073844(objectIndex);
    func_80086EF0(objectIndex);
    object->direction_angle[2] = 0x8000;
    object->direction_angle[1] =
        atan2s(D_8018CF1C->pos[0] - object->origin_pos[0], D_8018CF1C->pos[2] - object->origin_pos[2]);
}

void func_8007CEDC(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) { /* switch 1 */
        case 0:              /* switch 1 */
            break;
        case 1: /* switch 1 */
            func_8007CE0C(objectIndex);
            break;
        case 2: /* switch 1 */
            set_and_run_timer_object(objectIndex, 0x0000001E);
            break;
        case 3: /* switch 1 */
            set_and_run_timer_object(objectIndex, 0x000003E8);
            break;
        case 4: /* switch 1 */
            func_80072428(objectIndex);
            break;
        default: /* switch 2 */
            break;
    }
    if (object->state >= 2) {
        func_80072950(objectIndex, (s32) object->unk_0DC, 0, 4);
        func_80073514(objectIndex);
        switch (object->unk_0DC) { /* switch 2 */
            case 1:                /* switch 2 */
                func_80073998(objectIndex, &object->primAlpha, 0x00000028, 0x00000050, 4, 0, 0);
                break;
            case 2: /* switch 2 */
                func_80073CB0(objectIndex, &object->primAlpha, 0x00000050, 0x000000B4, 2, 0, -1);
                if ((object->unk_0AE == 0) || (object->state == 3)) {
                    object->unk_0DC += 1;
                }
                break;
            case 3: /* switch 2 */
                func_80073DC0(objectIndex, &object->primAlpha, 0, 4);
                break;
            case 4: /* switch 2 */
                func_80073884(objectIndex);
                break;
            case 0:
            default: /* switch 2 */
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
            if (var_v0 == 30) {
                break;
            }
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
            gObjectList[objectIndex].pos[0] =
                gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0];
            gObjectList[objectIndex].pos[1] =
                D_8018CF1C->unk_074 + gObjectList[objectIndex].origin_pos[1] + gObjectList[objectIndex].offset[1];
            gObjectList[objectIndex].pos[2] =
                gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2];
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
    gObjectList[objectIndex].direction_angle[1] =
        get_angle_between_xy(gObjectList[objectIndex].origin_pos[0], gObjectList[objectIndex].unk_01C[0],
                             gObjectList[objectIndex].origin_pos[2], gObjectList[objectIndex].unk_01C[2]);
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
    Object* object;

    object = &gObjectList[objectIndex];
    object->unk_0D5 = 0x0D;
    func_8007D360(objectIndex, arg1);
    object->sizeScaling = 0.1f;
    set_object_flag(objectIndex, 0x00000200);
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

void func_8007DDC0(s32 objectIndex) {
    f32 sp2C;
    Object* object;

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
        if ((gCCSelection == CC_50) || (gCCSelection == CC_100) || (gCCSelection == CC_150) ||
            (gCCSelection == CC_EXTRA)) {
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
    if (are_players_in_course_section(0x000F, 0x0012) == 0) {
        object->type = 2;
    }
}

void func_8007E1F4(s32 objectIndex) {
    f32 sp2C;
    Object* object;

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
        if ((gCCSelection == CC_50) || (gCCSelection == CC_100) || (gCCSelection == CC_150) ||
            (gCCSelection == CC_EXTRA)) {
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
    if (are_players_in_course_section(0x000F, 0x0013) == 0) {
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
    object_next_state(objectIndex);
}

void func_8007E3EC(s32 objectIndex) {

    switch (gObjectList[objectIndex].state) {
        case 1:
            func_8007E358(objectIndex);
            break;
        case 3:
            D_8018CFE8 = 1;
            object_next_state(objectIndex);
            break;
        case 4:
            set_and_run_timer_object(objectIndex, 0x000000D2);
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
            object_next_state(objectIndex);
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

void func_80083F18(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.12f,
                              gObjectList[objectIndex].unk_034, gObjectList[objectIndex].direction_angle[1],
                              0x00000064) != 0) {
                object_next_state(objectIndex);
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
    Object* object;
    Player* sp20;

    object = &gObjectList[objectIndex];
    sp20 = &gPlayerOne[playerId];
    object->unk_084[7] = playerId;
    init_object(objectIndex, 0);
    object->activeTLUT = d_course_sherbet_land_ice;
    object->tlutList = d_course_sherbet_land_ice;
    object->sizeScaling = ((f32) random_int(0x01F4U) * 0.0002) + 0.04;
    object->velocity[1] = ((f32) random_int(0x0032U) * 0.05) + 1.0;
    object->unk_034 = ((f32) random_int(0x000AU) * 0.1) + 1.0;
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
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
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

void func_80085BB4(s32 objectIndex) {
    gObjectList[objectIndex].sizeScaling = 8.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    object_next_state(objectIndex);
}

const char* sNeonMushroomList[] = { d_course_rainbow_road_neon_mushroom1, d_course_rainbow_road_neon_mushroom2,
                                    d_course_rainbow_road_neon_mushroom3, d_course_rainbow_road_neon_mushroom4,
                                    d_course_rainbow_road_neon_mushroom5 };

void init_obj_neon_mushroom(s32 objectIndex) {
    set_obj_origin_pos(objectIndex, xOrientation * -1431.0, 827.0f, -2957.0f);
    init_texture_object(objectIndex,
                        load_lakitu_tlut_x64(d_course_rainbow_road_neon_mushroom_tlut_list,
                                             ARRAY_COUNT(d_course_rainbow_road_neon_mushroom_tlut_list)),
                        sNeonMushroomList, 0x40U, (u16) 0x00000040);
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
            set_and_run_timer_object(objectIndex, 0x00000014);
            break;
        case 5:
            func_80072E54(objectIndex, 0, 4, 1, 0x0000000C, 5);
            break;
        case 6:
            set_and_run_timer_object(objectIndex, 0x00000014);
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

const char* sNeonMarioList[] = { d_course_rainbow_road_neon_mario1, d_course_rainbow_road_neon_mario2,
                            d_course_rainbow_road_neon_mario3, d_course_rainbow_road_neon_mario4,
                            d_course_rainbow_road_neon_mario5 };

void func_80085DB8(s32 objectIndex) {
    set_obj_origin_pos(objectIndex, xOrientation * 799.0, 1193.0f, -5891.0f);
    init_texture_object(objectIndex,
                        load_lakitu_tlut_x64(d_course_rainbow_road_neon_mario_tlut_list,
                                             ARRAY_COUNT(d_course_rainbow_road_neon_mario_tlut_list)),
                        sNeonMarioList, 0x40U, (u16) 0x00000040);
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

const char* sNeonBooList[] = { d_course_rainbow_road_neon_boo1, d_course_rainbow_road_neon_boo2,
                               d_course_rainbow_road_neon_boo3, d_course_rainbow_road_neon_boo4,
                               d_course_rainbow_road_neon_boo5 };

void func_80085EF8(s32 objectIndex) {
    set_obj_origin_pos(objectIndex, xOrientation * -2013.0, 555.0f, 0.0f);
    init_texture_object(objectIndex,
                        load_lakitu_tlut_x64(d_course_rainbow_road_neon_boo_tlut_list,
                                             ARRAY_COUNT(d_course_rainbow_road_neon_boo_tlut_list)),
                        sNeonBooList, 0x40U, (u16) 0x00000040);
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
            set_and_run_timer_object(objectIndex, 0x0000001E);
            break;
        case 4:
            func_80072C00(objectIndex, 4, 0, 7);
            break;
        case 5:
            set_and_run_timer_object(objectIndex, 0x0000001E);
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
    init_texture_object(objectIndex, d_course_rainbow_road_static_tluts[arg1],
                        &d_course_rainbow_road_static_textures[arg1], 64, 64);
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
    switch (id) { /* irregular */
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
