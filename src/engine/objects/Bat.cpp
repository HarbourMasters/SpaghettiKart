#include "Bat.h"
#include "World.h"
#include "CoreMath.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "assets/textures/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "assets/models/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
#include "code_800029B0.h"
}

const char* sBoardwalkTexList[] = { gTextureBat1, gTextureBat2, gTextureBat3, gTextureBat4 };

size_t OBat::_count = 0;

OBat::OBat(const SpawnParams& params) : OObject(params) {
    Name = "Bat";
    ResourceName = "mk:bat";

    //! @warning this likely needs to be rot.Set()
    IRotator rot = params.Rotation.value_or(IRotator(0, 0, 0));

    find_unused_obj_index(&_objectIndex);

    init_texture_object(_objectIndex, (uint8_t*) d_course_banshee_boardwalk_bat_tlut, sBoardwalkTexList, 0x20U,
                        (u16) 0x00000040);
    gObjectList[_objectIndex].orientation[0] = rot.pitch;
    gObjectList[_objectIndex].orientation[1] = rot.roll;
    gObjectList[_objectIndex].orientation[2] = rot.yaw; // 0x8000

    _count++;
}

void OBat::Tick() {
    s32 var_s2;
    s32 objectIndex;
    Object* object;

    if (D_8018CFC8 != 0) {
        D_8018CFC8 -= 1;
    }
    if (D_8018D000 != 0) {
        D_8018D000 -= 1;
    }
    objectIndex = _objectIndex;
    func_80072E54(objectIndex, 0, 3, 1, 0, -1);
    func_80073514(objectIndex);
    object = &gObjectList[objectIndex];
    func_80073CB0(objectIndex, &object->primAlpha, -0x00001000, 0x00001000, 0x00000400, 0, -1);
    object->orientation[2] = object->primAlpha + 0x8000;
    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        D_8018CFD8 = 0;
        for (var_s2 = 0; var_s2 < 40; var_s2++) {
            objectIndex = gObjectParticle2[var_s2];
            if (objectIndex == -1) {
                continue;
            }

            object = &gObjectList[objectIndex];
            if (object->state == 0) {
                continue;
            }

            OBat::func_8007D8D4(objectIndex, 1);
            OBat::func_8007DAF8(objectIndex, 1);
            OBat::func_8007D794(objectIndex);
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
            objectIndex = gObjectParticle3[var_s2];
            if (objectIndex == -1) {
                continue;
            }

            object = &gObjectList[objectIndex];
            if (object->state == 0) {
                continue;
            }

            OBat::func_8007D8D4(objectIndex, 2);
            OBat::func_8007DAF8(objectIndex, 2);
            OBat::func_8007D794(objectIndex);
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

void OBat::Draw(s32 cameraId) {
    s32 i;
    s32 objectIndex = _objectIndex;
    Camera* cam = &camera1[cameraId];

    OBat::func_80046F60((u8*) gObjectList[objectIndex].activeTLUT, (u8*) gObjectList[objectIndex].activeTexture,
                        0x00000020, 0x00000040, 5);
    D_80183E80[0] = gObjectList[objectIndex].orientation[0];
    D_80183E80[2] = gObjectList[objectIndex].orientation[2];

    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        for (i = 0; i < 40; i++) {
            objectIndex = gObjectParticle2[i];
            if (objectIndex == -1) {
                continue;
            }

            if ((gObjectList[objectIndex].state >= 2) && (gMatrixHudCount < 0x2EF)) {
                // @port: Tag the transform.
                FrameInterpolation_RecordOpenChild("Bat set 1", (uintptr_t) &gObjectList[objectIndex]);

                D_80183E80[1] =
                    func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], cam->pos);
                func_800431B0(gObjectList[objectIndex].pos, D_80183E80, gObjectList[objectIndex].sizeScaling,
                              (Vtx*) D_0D0062B0);

                // @port Pop the transform id.
                FrameInterpolation_RecordCloseChild();
            }
        }
    }

    if ((D_8018CFE8 != 0) || (D_8018D000 != 0)) {
        for (i = 0; i < 30; i++) {

            objectIndex = gObjectParticle3[i];
            if (objectIndex == -1) {
                continue;
            }

            if ((gObjectList[objectIndex].state >= 2) && (gMatrixHudCount < 0x2EF)) {
                // @port: Tag the transform.
                FrameInterpolation_RecordOpenChild("Bat set 2", (uintptr_t) &gObjectList[objectIndex]);

                D_80183E80[1] =
                    func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], cam->pos);
                func_800431B0(gObjectList[objectIndex].pos, D_80183E80, gObjectList[objectIndex].sizeScaling,
                              (Vtx*) D_0D0062B0);

                // @port Pop the transform id.
                FrameInterpolation_RecordCloseChild();
            }
        }
    }
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void OBat::func_80046F60(u8* tlut, u8* arg1, s32 arg2, s32 arg3, s32 arg4) {
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007D78);
    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    rsp_load_texture_mask(arg1, arg2, arg3, arg4);
}

void OBat::func_8007D8AC(s32 arg0) {
    object_next_state(arg0);
    func_80086E70(arg0);
}

s32 OBat::func_8007D804(s32 objectIndex) {
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

void OBat::func_8007D8D4(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            OBat::func_8007D8AC(objectIndex);
            break;
        case 2:
            if (OBat::func_8007D804(objectIndex) == 0) {
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

void OBat::func_8007DA74(s32 objectIndex) {
    UNUSED s32 pad;
    if ((gObjectList[objectIndex].unk_0AE != 0) && (gObjectList[objectIndex].unk_0AE == 1)) {
        if (func_80087060(objectIndex, 0x0000001E) != 0) {
            gObjectList[objectIndex].unk_0C6 = 0U;
        }
    }
    gObjectList[objectIndex].direction_angle[0] =
        func_800417B4(gObjectList[objectIndex].direction_angle[0], gObjectList[objectIndex].unk_0C6);
    func_80087844(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void OBat::func_8007DA4C(s32 objectIndex) {
    func_8008781C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void OBat::func_8007DAF8(s32 objectIndex, s32 arg1) {
    switch (arg1) {
        case 1:
            OBat::func_8007DA74(objectIndex);
            return;
        case 2:
            OBat::func_8007DA4C(objectIndex);
            return;
    }
}

void OBat::func_80089A04(s32 objectIndex, f32 arg1, f32 arg2) {
    Player* player;
    s32 var_s1;

    player = gPlayerOne;
    if (is_obj_flag_status_active(objectIndex, 0x00000200) != 0) {
        for (var_s1 = 0; var_s1 < D_8018D158; var_s1++, player++) {
            if ((gObjectList[objectIndex].state != 0) && !(player->effects & (BOO_EFFECT | STAR_EFFECT)) &&
                (has_collided_with_player(objectIndex, player) != 0)) {
                func_8008933C(player, objectIndex, arg1, arg2 * 1.1);
                if (is_obj_flag_status_active(objectIndex, 0x04000000) != 0) {
                    func_80072180();
                }
            }
        }
    }
}

void OBat::func_8007D794(s32 objectIndex) {
    if (gCCSelection == CC_50) {
        OBat::func_80089A04(objectIndex, 0.5f, 0.2f);
    } else if (gCCSelection == CC_100) {
        OBat::func_80089A04(objectIndex, 0.4f, 0.15f);
    } else {
        OBat::func_80089A04(objectIndex, 0.25f, 0.1f);
    }
}
