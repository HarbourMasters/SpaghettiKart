#include "GrandPrixBalloons.h"

#include "port/Game.h"
#include "assets/textures/other_textures.h"
#include "assets/textures/some_data.h"
#include "assets/models/common_data.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "update_objects.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "code_80086E70.h"
#include "math_util.h"
#include "math_util_2.h"
#include "menus.h"
}

size_t OGrandPrixBalloons::_count = 0;

OGrandPrixBalloons::OGrandPrixBalloons(const SpawnParams& params) : OObject(params) {
    Name = "Grand Prix Balloons";
    ResourceName = "mk:grand_prix_balloons";
    Pos = params.Location.value_or(FVector(0, 0, 0));

    _active = 1;
    if (gPlayerCount == 1) {
        _numBalloons = 0x64;
        _numBalloons2 = 0x3C;
        _numBalloons3 = 0x1E;
    } else {
        _numBalloons = 0x32;
        _numBalloons2 = 0x1E;
        _numBalloons3 = 0xA;
    }

    for (size_t i = 0; i < _numBalloons; i++) {
        find_unused_obj_index(&gObjectParticle3[i]);
        init_object(gObjectParticle3[i], 0);
    }
}

void OGrandPrixBalloons::Tick() {
    s32 someIndex;
    s32 objectIndex;
    s32 someCount;
    Object* object;

    if (!_active) {
        return;
    }
    someCount = 0;
    for (someIndex = 0; someIndex < _numBalloons; someIndex++) {
        objectIndex = gObjectParticle3[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state != 0) {
                OGrandPrixBalloons::func_80074E28(objectIndex);
                OGrandPrixBalloons::func_80074D94(objectIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&gObjectParticle3[someIndex]);
                    this->Destroy();
                }
                someCount += 1;
            }
        }
    }
    if (someCount == 0) {
        _active = 0;
    }
}

void OGrandPrixBalloons::Draw(s32 cameraId) {
    s32 var_s1;
    s32 objectIndex;
    if (!_active) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007E98);
    gDPLoadTLUT_pal256(gDisplayListHead++, gTLUTOnomatopoeia);
    func_8004B614(0, 0, 0, 0, 0, 0, 0);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    D_80183E80[0] = 0;
    D_80183E80[1] = 0x8000;
    rsp_load_texture((uint8_t*) gTextureBalloon1, 64, 32);
    for (var_s1 = 0; var_s1 < _numBalloons; var_s1++) {
        objectIndex = gObjectParticle3[var_s1];
        if ((objectIndex != NULL_OBJECT_ID) && (gObjectList[objectIndex].state >= 2)) {
            OGrandPrixBalloons::func_80053D74(objectIndex, cameraId, 0);
        }
    }
    rsp_load_texture((uint8_t*) gTextureBalloon2, 64, 32);
    for (var_s1 = 0; var_s1 < _numBalloons; var_s1++) {
        objectIndex = gObjectParticle3[var_s1];
        if ((objectIndex != NULL_OBJECT_ID) && (gObjectList[objectIndex].state >= 2)) {
            OGrandPrixBalloons::func_80053D74(objectIndex, cameraId, 4);
        }
    }
}

void OGrandPrixBalloons::func_80053D74(s32 objectIndex, UNUSED s32 arg1, s32 vertexIndex) {
    Object* object;

    Vtx* vtx = (Vtx*) LOAD_ASSET_RAW(common_vtx_hedgehog);

    size_t i = 0;
    if (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX) {
        object = &gObjectList[objectIndex];

        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("Balloon",
                                           TAG_ITEM_ADDR((objectIndex << 32) + i++)); // Not working properly just yet

        D_80183E80[2] = (s16) (object->unk_084[6] + 0x8000);
        rsp_set_matrix_transformation(object->pos, (u16*) D_80183E80, object->sizeScaling);
        set_color_render((s32) object->unk_084[0], (s32) object->unk_084[1], (s32) object->unk_084[2],
                         (s32) object->unk_084[3], (s32) object->unk_084[4], (s32) object->unk_084[5],
                         (s32) object->primAlpha);
        gSPVertex(gDisplayListHead++, (uintptr_t) &vtx[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);

        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
    }
}

void OGrandPrixBalloons::func_80074924(s32 objectIndex) {
    s32 sp2C = 0;
    s32 sp28;
    s32 sp24;
    s32 sp20 = 0;
    s32 temp_a0;
    Object* object;

    object = &gObjectList[objectIndex];
    object->sizeScaling = 0.15f;

    if (IsMarioRaceway()) {
        sp2C = random_int(0x00C8U);
        sp28 = random_int(_numBalloons3);
        sp24 = random_int(0x0096U);
        sp20 = random_int(0x2000U);
        object->origin_pos[0] = (f32) ((((f64) Pos.x + 100.0) - (f64) sp2C) * (f64) xOrientation);
        object->origin_pos[1] = (f32) (Pos.y + sp28);
        object->origin_pos[2] = (f32) (((f64) Pos.z + 200.0) - (f64) sp24);
    } else if (IsRoyalRaceway()) {
        sp2C = random_int(0x0168U);
        sp28 = random_int(_numBalloons3);
        sp24 = random_int(0x00B4U);
        sp20 = random_int(0x2000U);
        object->origin_pos[0] = (f32) ((((f64) Pos.x + 180.0) - (f64) sp2C) * (f64) xOrientation);
        object->origin_pos[1] = (f32) (Pos.y + sp28);
        object->origin_pos[2] = (f32) (((f64) Pos.z + 200.0) - (f64) sp24);
    } else if (IsLuigiRaceway()) {
        sp2C = random_int(0x012CU);
        sp28 = random_int(_numBalloons3);
        sp24 = random_int(0x0096U);
        sp20 = random_int(0x2000U);
        object->origin_pos[0] = (f32) ((((f64) Pos.x + 150.0) - (f64) sp2C) * (f64) xOrientation);
        object->origin_pos[1] = (f32) (Pos.y + sp28);
        object->origin_pos[2] = (f32) (((f64) Pos.z + 200.0) - (f64) sp24);
    } else { // any track
        sp2C = random_int(0x00C8U);
        sp28 = random_int(_numBalloons3);
        sp24 = random_int(0x0096U);
        sp20 = random_int(0x2000U);
        object->origin_pos[0] = (f32) ((((f64) Pos.x + 100.0) - (f64) sp2C) * (f64) xOrientation);
        object->origin_pos[1] = (f32) (Pos.y + sp28);
        object->origin_pos[2] = (f32) (((f64) Pos.z + 200.0) - (f64) sp24);
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
    object_next_state(objectIndex);
}

void OGrandPrixBalloons::func_80074D94(s32 objectIndex) {
    if (gObjectList[objectIndex].unk_0AE == 1) {
        //! @warning this fades out the balloons. Original game uses _numBalloons3 here but they disappear before
        //! off-screen.
        // So _numBalloons replaces it for now.
        if ((_numBalloons <= gObjectList[objectIndex].offset[1]) &&
            (s16_step_down_towards(&gObjectList[objectIndex].primAlpha, 0, 8) != 0)) {
            func_80086F60(objectIndex);
        }
        object_add_velocity_offset_y(objectIndex);
    }
    object_calculate_new_pos_offset(objectIndex);
}

void OGrandPrixBalloons::func_80074E28(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            OGrandPrixBalloons::func_80074924(objectIndex);
            break;
        case 2:
            if (set_and_run_timer_object(objectIndex, 1) != false) {
                func_80086E70(objectIndex);
                break;
            }
        case 0:
            break;
        case 3:
            OGrandPrixBalloons::func_80041480(&gObjectList[objectIndex].unk_084[6], -0x1000, 0x1000,
                                              &gObjectList[objectIndex].unk_084[7]);
            if (gObjectList[objectIndex].unk_0AE == 0) {
                func_80072428(objectIndex);
            }
            break;
    }
}

void OGrandPrixBalloons::func_80041480(s16* arg0, s16 arg1, s16 arg2, s16* arg3) {
    *arg0 += *arg3;
    if (*arg3 >= 0) {
        if (*arg0 >= arg2) {
            *arg0 = arg2;
            *arg3 = -*arg3;
        }
    } else if (arg1 >= *arg0) {
        *arg0 = arg1;
        *arg3 = -*arg3;
    }
}
