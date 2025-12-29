#include <libultraship.h>
#include <libultra/gbi.h>
#include "Lakitu.h"
#include <vector>
#include "port/interpolation/FrameInterpolation.h"

#include "port/Game.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "code_800029B0.h"
#include "actors.h"
#include "math_util.h"
#include "math_util_2.h"
#include "sounds.h"
#include "update_objects.h"
#include "render_player.h"
#include "external.h"
#include "bomb_kart.h"
#include "collision.h"
#include "code_80086E70.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "defines.h"
#include "code_80005FD0.h"
#include "collision.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
#include "ceremony_and_credits.h"
#include "objects.h"
#include "update_objects.h"
#include "render_objects.h"
#include "course_offsets.h"
#include "data/some_data.h"
#include "race_logic.h"
#include "effects.h"
#include "memory.h"
#include <assets/models/common_data.h>
extern s8 gPlayerCount;
}

size_t OLakitu::_count = 0; 

OLakitu::OLakitu(s32 playerId, LakituType type) {
    Name = "Lakitu";
    mPlayerId = playerId;

    mCameraId = -1;
    // Find the camera for this player
    for (size_t i = 0; i < NUM_CAMERAS; i++) {
        if (cameras[i].playerId == playerId ) {
            mCameraId = cameras[i].cameraId;
            break;
        }
    }

    _idx = _count;
    find_unused_obj_index(&gIndexLakituList[_count]);
    init_object(gIndexLakituList[_count], (s32) type);
    mLakituId = gIndexLakituList[_count];

    _count += 1;
}

void OLakitu::Activate(LakituType type) {
    init_object(mLakituId, (s32) type);
}

void OLakitu::Tick() {
    OLakitu::func_8007AA44(mPlayerId);
}

void OLakitu::Tick60fps() { // update_object_lakitu
    s32 playerId = mPlayerId;
    s32 objectIndex = mLakituId;

    switch (gObjectList[objectIndex].unk_0D8) {
        case 0:
            break;
        case 1:
            OLakitu::update_object_lakitu_starter(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 2:
            OLakitu::update_object_lakitu_checkered_flag(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 3:
            OLakitu::update_object_lakitu_fishing(objectIndex, playerId);
            break;
        case 4:
            OLakitu::update_object_lakitu_second_lap(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 5:
            OLakitu::update_object_lakitu_final_lap(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 6:
            OLakitu::update_object_lakitu_reverse(objectIndex, playerId);
            func_8008BFFC(objectIndex);
            break;
        case 7:
            OLakitu::update_object_lakitu_fishing2(objectIndex, playerId);
            break;
    }
}

void OLakitu::Draw(s32 cameraId) {
    UNUSED s32 stackPadding;
    Camera* camera;
    f32 var_f0;
    f32 var_f2;
    s32 objectIndex;
    Object* object;

    if (cameraId != mCameraId) {
        return;
    }

    FrameInterpolation_RecordOpenChild("Lakitu", (uintptr_t) this);

    objectIndex = mLakituId;
    camera = &cameras[cameraId];
    if (is_obj_flag_status_active(objectIndex, 0x00000010) != 0) {
        object = &gObjectList[objectIndex];
        object->orientation[0] = 0;
        object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], camera->pos);
        object->orientation[2] = 0x8000;
        if (func_80072354(objectIndex, 2) != 0) {
            s32 width = object->textureWidth;
            s32 height = object->textureHeight;
            rsp_set_matrix_transformation(object->pos, object->orientation, object->sizeScaling);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007D78);
            s32 heightIndex;

            gDPLoadTLUT_pal256(gDisplayListHead++, object->activeTLUT);
            gDPLoadTextureTile(gDisplayListHead++, object->activeTexture, G_IM_FMT_CI, G_IM_SIZ_8b, width, height, 0, 0,
                               width - 1, height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP,
                               G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, (uintptr_t) object->vertex, 4, 0);
            gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
            gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
        } else {
            s32 width = object->textureWidth;
            s32 height = object->textureHeight;
            rsp_set_matrix_transformation(object->pos, object->orientation, object->sizeScaling);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007E98);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

            set_transparency(object->primAlpha);
            s32 heightIndex;

            gDPLoadTLUT_pal256(gDisplayListHead++, object->activeTLUT);
            rsp_load_texture((u8*) object->activeTexture, width, height);
            gSPVertex(gDisplayListHead++, (uintptr_t) object->vertex, 4, 0);
            gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
            gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);

            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
        }
        if (gScreenModeSelection == SCREEN_MODE_1P) {
            var_f0 = object->pos[0] - camera->pos[0];
            var_f2 = object->pos[2] - camera->pos[2];
            if (var_f0 < 0.0f) {
                var_f0 = -var_f0;
            }
            if (var_f2 < 0.0f) {
                var_f2 = -var_f2;
            }
            if ((var_f0 + var_f2) <= 200.0) {
                func_8004A630(&D_8018C0B0[cameraId], object->pos, 0.35f);
            }
        }
    }
    FrameInterpolation_RecordCloseChild();
}

void OLakitu::func_80079114(s32 objectIndex, s32 playerId, s32 arg2) {
    s32 a;
    if (gObjectList[objectIndex].state >= 2) {
        if ((u8) gObjectList[objectIndex].unk_0D8 == 1) {
            if (playerId == 0) {
                func_80074894(objectIndex, gLakituTexturePtr);
                return;
            }

            // Use the textures from the first lakitu?
            // Perhaps this syncs the animations
            a = gIndexLakituList[0];
            gObjectList[objectIndex].activeTLUT = gObjectList[a].activeTLUT;
            gObjectList[objectIndex].activeTexture = gObjectList[a].activeTexture;
            if (0) {}
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

void OLakitu::func_800791F0(s32 objectIndex, s32 playerId) {
    Player* player = &gPlayerOne[playerId];

    if ((gObjectList[objectIndex].unk_0D8 != 3) && (gObjectList[objectIndex].unk_0D8 != 7)) {
        func_800722CC(objectIndex, 1);
        if (CM_GetProps()->LakituTowType == LakituTowType::ICE) {
            player->lakituProps &= ~FRIGID_EFFECT;
        }
    }

    if (CM_GetProps()->LakituTowType == LakituTowType::ICE) {
        func_800722CC(objectIndex, 0x00000010);
        player->lakituProps &= ~THAWING_EFFECT;
    }
    func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
}

static const char* sLakituTextures[] = {
    gTextureLakituNoLights1,   gTextureLakituNoLights2,   gTextureLakituNoLights3,   gTextureLakituNoLights4,
    gTextureLakituNoLights5,   gTextureLakituNoLights6,   gTextureLakituNoLights7,   gTextureLakituNoLights8,
    gTextureLakituRedLights01, gTextureLakituRedLights02, gTextureLakituRedLights03, gTextureLakituRedLights04,
    gTextureLakituRedLights05, gTextureLakituRedLights06, gTextureLakituRedLights07, gTextureLakituRedLights08,
    gTextureLakituRedLights09, gTextureLakituRedLights10, gTextureLakituRedLights11, gTextureLakituRedLights12,
    gTextureLakituRedLights13, gTextureLakituRedLights14, gTextureLakituRedLights15, gTextureLakituRedLights16,
    gTextureLakituBlueLight1,  gTextureLakituBlueLight2,  gTextureLakituBlueLight3,  gTextureLakituBlueLight4,
    gTextureLakituBlueLight5,  gTextureLakituBlueLight6,  gTextureLakituBlueLight7,  gTextureLakituBlueLight8,
};

Vtx fixed_common_vtx_lakitu[] = {
    { { { -28, -35, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 27, -35, 0 }, 0, { 3520, 0 }, { 255, 255, 255, 255 } } },
    { { { 27, 35, 0 }, 0, { 3520, 4480 }, { 255, 255, 255, 255 } } },
    { { { -28, 35, 0 }, 0, { 0, 4480 }, { 255, 255, 255, 255 } } },
};

void OLakitu::init_obj_lakitu_starter_and_checkered_flag(s32 objectIndex, s32 playerId) {
    if (playerId == 0) {
        D_801656F0 = 0;
        D_8018D168 = 0;
    }

    // u8 *tlut = (u8 *) LOAD_ASSET_RAW(common_tlut_lakitu_countdown);
    // u8 *lights = (u8 *) LOAD_ASSET_RAW(gTextureLakituNoLights1);

    init_texture_object(
        objectIndex,
        (u8*) load_lakitu_tlut_x64(common_tlut_lakitu_countdown, ARRAY_COUNT(common_tlut_lakitu_countdown)),
        sLakituTextures, 56, (u16) 72);
    gObjectList[objectIndex].vertex = fixed_common_vtx_lakitu;
    gObjectList[objectIndex].sizeScaling = 0.15f;
    clear_object_flag(objectIndex, 0x00000010);
    object_next_state(objectIndex);
    gObjectList[objectIndex].unk_048 = D_8018D180;
}

void OLakitu::update_object_lakitu_starter(s32 objectIndex, s32 arg1) {
    UNUSED s32 pad;
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OLakitu::init_obj_lakitu_starter_and_checkered_flag(objectIndex, arg1);
            break;
        case 2:
            set_and_run_timer_object(objectIndex, gObjectList[objectIndex].unk_048);
            if ((gObjectList[objectIndex].timer == 0x00000055) && (gPlayerCount == 3) && (arg1 == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 3:
            set_object_flag(objectIndex, 0x00000010);
            func_80086F10(objectIndex, 1, &D_800E67B8); // set a spline
            object_next_state(objectIndex);
            break;
        case 4:
            if ((set_and_run_timer_object(objectIndex, 0x0000001E) != false) && (gPlayerCount != 3) && (arg1 == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 5:
            set_and_run_timer_object(objectIndex, 0x0000001E);
            break;
        case 6:
            func_80072E54(objectIndex, 1, 7, 1, 2, 0);
            break;
        case 7:
            if (set_and_run_timer_object(objectIndex, 0x00000014) != 0) {
                gObjectList[objectIndex].tlutList += 0x200;
                if (arg1 == 0) {
                    play_sound2(SOUND_ACTION_COUNTDOWN_LIGHT);
                }
            }
            break;
        case 8:
            func_80072E54(objectIndex, 8, 0x0000000F, 1, 6, 0);
            break;
        case 9:
            if ((set_and_run_timer_object(objectIndex, 8) != 0) && (arg1 == 0)) {
                play_sound2(SOUND_ACTION_COUNTDOWN_LIGHT);
            }
            break;
        case 10:
            if ((func_80072E54(objectIndex, 0x00000010, 0x00000017, 1, 6, 0) != 0) && (arg1 == 0)) {
                D_801656F0 = 1;
            }
            break;
        case 11:
            if (set_and_run_timer_object(objectIndex, 8) != 0) {
                gObjectList[objectIndex].tlutList += 0x200;
                if (arg1 == 0) {
                    play_sound2(SOUND_ACTION_GREEN_LIGHT);
                }
            }
            break;
        case 12:
            func_80072E54(objectIndex, 0x00000018, 0x0000001B, 1, 6, 0);
            break;
        case 13:
            if (arg1 == 0) {
                OLakitu::func_800729EC(objectIndex);
                D_8018D160 = 1;
                break;
            }
            object_next_state(objectIndex);
            break;
        case 14:
            set_and_run_timer_object(objectIndex, 0x00000078);
            break;
        case 15:
            func_80072428(objectIndex);
            break;
    }
}

void OLakitu::func_800729EC(s32 objectIndex) {
    u32 temp_v1 = 1;
    s32 i;

    start_race();
    object_next_state(objectIndex);
    D_8018D2BC = 1;
    D_8018D2A4 = 1;

    if (!IsYoshiValley()) {
        for (i = 0; i < gPlayerCount; i++) {
            playerHUD[i].unk_81 = temp_v1;
        }
    }
    func_8005AB20();
}

static const char* sLakituCheckeredList[] = {
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

Vtx fixed_common_vtx_also_lakitu[] = {
    { { { -36, -27, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 35, -27, 0 }, 0, { 4544, 0 }, { 255, 255, 255, 255 } } },
    { { { 35, 27, 0 }, 0, { 4544, 3456 }, { 255, 255, 255, 255 } } },
    { { { -36, 27, 0 }, 0, { 0, 3456 }, { 255, 255, 255, 255 } } },
    { { { -40, -24, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 40, -24, 0 }, 0, { 5056, 0 }, { 255, 255, 255, 255 } } },
    { { { 40, 0, 0 }, 0, { 5056, 1472 }, { 255, 255, 255, 255 } } },
    { { { -40, 0, 0 }, 0, { 0, 1472 }, { 255, 255, 255, 255 } } },
    { { { -40, 0, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 40, 0, 0 }, 0, { 5056, 0 }, { 255, 255, 255, 255 } } },
    { { { 40, 24, 0 }, 0, { 5056, 1472 }, { 255, 255, 255, 255 } } },
    { { { -40, 24, 0 }, 0, { 0, 1472 }, { 255, 255, 255, 255 } } },
    { { { -48, -8, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 47, -8, 0 }, 0, { 6080, 0 }, { 255, 255, 255, 255 } } },
    { { { 47, 7, 0 }, 0, { 6080, 960 }, { 255, 255, 255, 255 } } },
    { { { -48, 7, 0 }, 0, { 0, 960 }, { 255, 255, 255, 255 } } },
    { { { -56, -16, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 55, -16, 0 }, 0, { 7104, 0 }, { 255, 255, 255, 255 } } },
    { { { 55, 15, 0 }, 0, { 7104, 1984 }, { 255, 255, 255, 255 } } },
    { { { -56, 15, 0 }, 0, { 0, 1984 }, { 255, 255, 255, 255 } } },
    { { { -56, -16, 0 }, 0, { 0, 0 }, { 0, 0, 0, 255 } } },
    { { { 55, -16, 0 }, 0, { 7104, 0 }, { 0, 0, 0, 255 } } },
    { { { 55, 15, 0 }, 0, { 7104, 1984 }, { 0, 0, 0, 255 } } },
    { { { -56, 15, 0 }, 0, { 0, 1984 }, { 0, 0, 0, 255 } } },
};

void OLakitu::init_obj_lakitu_checkered_flag(s32 objectIndex, s32 playerIndex) {
    Object* object;

    OLakitu::func_800791F0(objectIndex, playerIndex);

    u8* tex = (u8*) LOAD_ASSET_RAW(common_tlut_lakitu_checkered_flag);

    init_texture_object(objectIndex, (u8*) tex, sLakituCheckeredList, 0x48U, (u16) 0x00000038);
    object = &gObjectList[objectIndex];
    object->activeTexture = *gObjectList[objectIndex].textureList;
    object->vertex = fixed_common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->sizeScaling = 0.15f;
    func_80086F10(objectIndex, 2, &D_800E6834);
    clear_object_flag(objectIndex, 0x00000010);
    object_next_state(objectIndex);
}

void OLakitu::update_object_lakitu_checkered_flag(s32 objectIndex, s32 playerIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OLakitu::init_obj_lakitu_checkered_flag(objectIndex, playerIndex);
            break;
        case 2:
            set_object_flag(objectIndex, 0x00000010);
            object_next_state(objectIndex);
            break;
        case 3:
            func_80072E54(objectIndex, 0, 0x0000001F, 1, 2, -1);
            break;
        case 4:
            func_80072428(objectIndex);
            break;
    }
}

void OLakitu::func_800797AC(s32 playerId) {
    s32 objectIndex;
    Player* player;

    objectIndex = mLakituId;
    player = &gPlayerOne[playerId];
    // if ((IsSherbetLand()) && (player->lakituProps & 1)) {
    if ((CM_GetProps()->LakituTowType == LakituTowType::ICE) && (player->lakituProps & LAKITU_RETRIEVAL)) {
        init_object(objectIndex, 7);
        player->lakituProps |= FRIGID_EFFECT;
    } else {
        init_object(objectIndex, 3);
    }
    func_800722A4(objectIndex, 1);
}

void OLakitu::func_80079860(s32 playerId) {
    s32 objectIndex;
    Player* player;

    objectIndex = mLakituId;
    player = &gPlayerOne[playerId];
    if ((func_80072354(objectIndex, 1) != 0) &&
        (((func_802ABDF4(player->collision.meshIndexZX) != 0) && (player->collision.surfaceDistance[2] <= 3.0f)) ||
         (player->lakituProps & LAKITU_RETRIEVAL) || ((player->surfaceType == OUT_OF_BOUNDS) && !(player->effects & 8)))) {
        func_80090778(player);
        OLakitu::func_800797AC(playerId);
    }
}

void OLakitu::func_8007993C(s32 objectIndex, Player* player) {
    if (player->lakituProps & LAKITU_FIZZLE) {
        func_800722A4(objectIndex, 2);
        gObjectList[objectIndex].primAlpha = player->alpha;
        return;
    }
    func_800722CC(objectIndex, 2);
}

static const char* sLakituFishingTextures[] = { gTextureLakituFishing1, gTextureLakituFishing2, gTextureLakituFishing3,
                                                gTextureLakituFishing4 };

Vtx fixed_D_0D005F30[] = {
    { { { -10, -35, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 45, -35, 0 }, 0, { 3520, 0 }, { 255, 255, 255, 255 } } },
    { { { 45, 35, 0 }, 0, { 3520, 4480 }, { 255, 255, 255, 255 } } },
    { { { -10, 35, 0 }, 0, { 0, 4480 }, { 255, 255, 255, 255 } } },
};

void OLakitu::init_obj_lakitu_red_flag_fishing(s32 objectIndex, s32 arg1) {

    u8* tlut = (u8*) LOAD_ASSET_RAW(common_tlut_lakitu_fishing);

    OLakitu::func_800791F0(objectIndex, arg1);
    init_texture_object(objectIndex, tlut, sLakituFishingTextures, 0x38U, (u16) 0x00000048);
    gObjectList[objectIndex].vertex = fixed_D_0D005F30;
    gObjectList[objectIndex].sizeScaling = 0.15f;
    func_80086E70(objectIndex);
    clear_object_flag(objectIndex, 0x00000010);
    func_80073720(objectIndex);
    object_next_state(objectIndex);
    func_800C8F80((u8) arg1, 0x0100FA28);
}

void OLakitu::func_80079A5C(s32 objectIndex, UNUSED Player* player) {
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

void OLakitu::update_object_lakitu_fishing(s32 objectIndex, s32 playerId) {
    Player* player = &gPlayerOne[playerId];

    switch (gObjectList[objectIndex].state) { /* switch 1; irregular */
        case 0:                               /* switch 1 */
            break;
        case 1: /* switch 1 */
            OLakitu::init_obj_lakitu_red_flag_fishing(objectIndex, playerId);
            break;
        case 2: /* switch 1 */
            set_object_flag(objectIndex, 0x00000010);
            func_800736E0(objectIndex);
            object_next_state(objectIndex);
            break;
        case 3: /* switch 1 */
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
            if (!(player->lakituProps & HELD_BY_LAKITU)) {
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
        OLakitu::func_8007993C(objectIndex, player);
    }
    OLakitu::func_80079A5C(objectIndex, player);
}

void OLakitu::update_object_lakitu_fishing2(s32 objectIndex, s32 playerId) {
    Player* player = &gPlayerOne[playerId];

    switch (gObjectList[objectIndex].state) { /* switch 1; irregular */
        case 0:                               /* switch 1 */
            break;
        case 1: /* switch 1 */
            OLakitu::init_obj_lakitu_red_flag_fishing(objectIndex, playerId);
            break;
        case 2: /* switch 1 */
            set_object_flag(objectIndex, 0x00000010);
            func_800736E0(objectIndex);
            player->lakituProps |= FROZEN_EFFECT;
            object_next_state(objectIndex);
            break;
        case 3: /* switch 1 */
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
            if ((player->surfaceType == ICE) && !(player->lakituProps & LAKITU_RETRIEVAL) &&
                ((f64) player->collision.surfaceDistance[2] <= 30.0)) {
                func_800722A4(objectIndex, 8);
            }
            if (!(player->lakituProps & HELD_BY_LAKITU)) {
                func_80086EAC(objectIndex, 0, 3);
                func_80073654(objectIndex);
            }
            break;
        case 4:
            func_8007375C(objectIndex, 0x0000001E);
            break;
        case 5:
            player->lakituProps &= ~FROZEN_EFFECT;
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
                player->lakituProps &= ~FRIGID_EFFECT;
                player->lakituProps |= THAWING_EFFECT;
            }
            break;
        case 7:
            func_8007375C(objectIndex, 0x0000003C);
            break;
        case 8:
            func_80073720(objectIndex);
            func_80072428(objectIndex);
            player->lakituProps &= ~THAWING_EFFECT;
            func_800722CC(objectIndex, 1);
            func_800C9018((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            break;
    }

    if (gObjectList[objectIndex].state >= 2) {
        OLakitu::func_8007993C(objectIndex, player);
    }
    OLakitu::func_80079A5C(objectIndex, player);
}

static const char* sLakituSecondLapTextures[] = {
    gTextureLakituSecondLap01, gTextureLakituSecondLap02, gTextureLakituSecondLap03, gTextureLakituSecondLap04,
    gTextureLakituSecondLap05, gTextureLakituSecondLap06, gTextureLakituSecondLap07, gTextureLakituSecondLap08,
    gTextureLakituSecondLap09, gTextureLakituSecondLap10, gTextureLakituSecondLap11, gTextureLakituSecondLap12,
    gTextureLakituSecondLap13, gTextureLakituSecondLap14, gTextureLakituSecondLap15, gTextureLakituSecondLap16
};

void OLakitu::func_8007A060(s32 objectIndex, s32 playerIndex) {
    Object* object;

    OLakitu::func_800791F0(objectIndex, playerIndex);

    u8* tlut = (u8*) LOAD_ASSET_RAW(common_tlut_lakitu_second_lap);

    init_texture_object(objectIndex, tlut, sLakituSecondLapTextures, 0x48U, (u16) 0x00000038);
    object = &gObjectList[objectIndex];
    object->activeTexture = *gObjectList[objectIndex].textureList;
    object->vertex = fixed_common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->sizeScaling = 0.15f;
    clear_object_flag(objectIndex, 0x00000010);
    func_80086F10(objectIndex, 5, &D_800E694C);
    object_next_state(objectIndex);
}

void OLakitu::update_object_lakitu_second_lap(s32 objectIndex, s32 playerIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OLakitu::func_8007A060(objectIndex, playerIndex);
            break;
        case 2:
            set_object_flag(objectIndex, 0x00000010);
            object_next_state(objectIndex);
            break;
        case 3:
            set_and_run_timer_object(objectIndex, 0x00000014);
            break;
        case 4:
            func_80072E54(objectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            set_and_run_timer_object(objectIndex, 0x0000003C);
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

void OLakitu::func_8007A228(s32 objectIndex, s32 playerIndex) {
    Object* object;

    OLakitu::func_800791F0(objectIndex, playerIndex);

    u8* tlut = (u8*) LOAD_ASSET_RAW(common_tlut_lakitu_final_lap);

    init_texture_object(objectIndex, tlut, sLakituFinalLapTextures, 0x48U, (u16) 0x00000038);
    object = &gObjectList[objectIndex];
    object->activeTexture = *gObjectList[objectIndex].textureList;
    object->vertex = fixed_common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->sizeScaling = 0.15f;
    clear_object_flag(objectIndex, 0x00000010);
    func_80086F10(objectIndex, 5, &D_800E694C);
    object_next_state(objectIndex);
}

void OLakitu::update_object_lakitu_final_lap(s32 objectIndex, s32 playerIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OLakitu::func_8007A228(objectIndex, playerIndex);
            break;
        case 2:
            set_object_flag(objectIndex, 0x00000010);
            object_next_state(objectIndex);
            break;
        case 3:
            set_and_run_timer_object(objectIndex, 0x00000014);
            break;
        case 4:
            func_80072E54(objectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            set_and_run_timer_object(objectIndex, 0x0000003C);
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

void OLakitu::func_8007A3F0(s32 objectIndex, s32 arg1) {
    f32 var = 5000.0f;
    OLakitu::func_800791F0(objectIndex, arg1);

    u8* tlut = (u8*) LOAD_ASSET_RAW(common_tlut_lakitu_reverse);

    init_texture_object(objectIndex, tlut, sLakituReverseTextures, 72, (u16) 56);
    gObjectList[objectIndex].activeTexture = *gObjectList[objectIndex].textureList;
    gObjectList[objectIndex].vertex = fixed_common_vtx_also_lakitu;
    gObjectList[objectIndex].pos[2] = var;
    gObjectList[objectIndex].pos[1] = var;
    gObjectList[objectIndex].pos[0] = var;
    gObjectList[objectIndex].sizeScaling = 0.15f;
    clear_object_flag(objectIndex, 0x00000010);
    func_80086F10(objectIndex, 6, &D_800E69B0);
    gObjectList[objectIndex].unk_0D6 = 0;
    object_next_state(objectIndex);
    func_800C8F80((u8) arg1, 0x0100FA28);
}

void OLakitu::update_object_lakitu_reverse(s32 objectIndex, s32 playerId) {
    Player* sp2C = &gPlayerOne[playerId];

    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OLakitu::func_8007A3F0(objectIndex, playerId);
            break;
        case 2:
            set_object_flag(objectIndex, 0x00000010);
            gObjectList[objectIndex].unk_0D6 = 1;
            object_next_state(objectIndex);
            break;
        case 3:
            func_800730BC(objectIndex, 0, 0x0000000F, 1, 2, -1);
            break;
        case 4:
            func_80072428(objectIndex);
            break;
    }
    switch (gObjectList[objectIndex].unk_0D6) {
        case 1:
            if ((gObjectList[objectIndex].state >= 3) && (!(sp2C->effects & 0x400000))) {
                func_80086F10(objectIndex, 6, &D_800E69F4);
                gObjectList[objectIndex].unk_0D6 = 2;
                gObjectList[objectIndex].unk_04C = 0x00000050;
                func_800C9018((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
                return;
            }
            return;
        case 2:
            gObjectList[objectIndex].unk_04C--;
            if (gObjectList[objectIndex].unk_04C == 0) {
                object_next_state(objectIndex);
                gObjectList[objectIndex].unk_0D6 = 0;
            }
            break;
    }
}

void OLakitu::func_8007A66C(s32 objectIndex) {
    Player* player = &gPlayers[mPlayerId];

    if (mCameraId == -1) {
        return;
    }

    Camera* camera = &cameras[mCameraId];
    u16 rot = 0x8000 - camera->rot[1];

    gObjectList[objectIndex].pos[0] =
        (player->pos[0] + (coss(rot) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) -
        (sins(rot) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
    gObjectList[objectIndex].pos[1] =
        player->unk_074 + gObjectList[objectIndex].origin_pos[1] + gObjectList[objectIndex].offset[1];
    gObjectList[objectIndex].pos[2] =
        (player->pos[2] + (sins(rot) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) +
        (coss(rot) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
}

void OLakitu::func_8007A778(s32 objectIndex) {
    Player* player = &gPlayers[mPlayerId];
    if (mCameraId == -1) {
        return;
    }
    Camera* camera = &cameras[mCameraId];
    u16 rot = 0x8000 - camera->rot[1];

    gObjectList[objectIndex].pos[0] =
        (player->pos[0] + (coss(rot) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) -
        (sins(rot) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
    gObjectList[objectIndex].pos[1] =
        player->pos[1] + gObjectList[objectIndex].origin_pos[1] + gObjectList[objectIndex].offset[1];
    gObjectList[objectIndex].pos[2] =
        (player->pos[2] + (sins(rot) * (gObjectList[objectIndex].origin_pos[0] + gObjectList[objectIndex].offset[0]))) +
        (coss(rot) * (gObjectList[objectIndex].origin_pos[2] + gObjectList[objectIndex].offset[2]));
}

void OLakitu::func_8007A88C(s32 playerId) {
    s32 objectIndex;
    Player* player;

    objectIndex = mLakituId;
    player = &gPlayerOne[playerId];

    if ((gObjectList[objectIndex].state == 0) && (player->effects & 0x400000)) {
        // func_800790E4(playerId);
        init_object(mLakituId, 6);
    }
}

void OLakitu::func_8007A910(s32 arg0) {
    if (D_801657B4 == 0) {
        OLakitu::func_8007A88C(arg0);
    }
    func_80079860(arg0);
}

// animate lakitu?
void OLakitu::func_8007AA44(s32 playerId) {
    s32 objectIndex;

    OLakitu::func_8007A910(playerId);
    objectIndex = mLakituId;
    gLakituTexturePtr = (const char**) &gLakituTextureBuffer[playerId];
    switch (gObjectList[objectIndex].unk_0D8) {
        case 1:
            OLakitu::func_80079114(objectIndex, playerId, 2);
            OLakitu::func_8007A66C(objectIndex);
            break;
        case 2:
            OLakitu::func_80079114(objectIndex, playerId, 0);
            OLakitu::func_8007A66C(objectIndex);
            break;
        case 3:
            OLakitu::func_80079114(objectIndex, playerId, 0);
            OLakitu::func_8007A778(objectIndex);
            break;
        case 4:
            OLakitu::func_80079114(objectIndex, playerId, 0);
            OLakitu::func_8007A66C(objectIndex);
            break;
        case 5:
            OLakitu::func_80079114(objectIndex, playerId, 0);
            OLakitu::func_8007A66C(objectIndex);
            break;
        case 6:
            OLakitu::func_80079114(objectIndex, playerId, 0);
            OLakitu::func_8007A66C(objectIndex);
            break;
        case 7:
            OLakitu::func_80079114(objectIndex, playerId, 0);
            OLakitu::func_8007A778(objectIndex);
            break;
        case 0:
        default:
            break;
    }
}
