#include <libultraship.h>
#include <stdio.h>
#include <macros.h>
#include <common_structs.h>
#include <defines.h>
#include <libc/math.h>
#include <mk64.h>

#include "camera.h"
#include "code_800029B0.h"
#include "racing/math_util.h"
#include "racing/memory.h"
#include "waypoints.h"
#include "render_player.h"
#include "collision.h"
#include "code_80057C60.h"
#include "code_80005FD0.h"
#include "main.h"
#include "spawn_players.h"
#include "enhancements/freecam/freecam_engine.h"
#include "enhancements/freecam/freecam.h"
#include "port/interpolation/FrameInterpolation.h"

#include "engine/GameAPI.h"
#include "port/Game.h"

f32 D_800DDB30[] = { 0.4f, 0.6f, 0.275f, 0.3f };

Camera cameras[NUM_CAMERAS]; // This size should be 5 but there is an overflow somewhere in Bowser's Castle, so we allocate 8 cameras to avoid it.
Camera* camera1 = &cameras[0];
Camera* camera2 = &cameras[1];
Camera* camera3 = &cameras[2];
Camera* camera4 = &cameras[3];
Camera* gFreecamCamera = &cameras[4];

UNUSED s32 D_801649D0[2];

f32 D_801649D8[NUM_CAMERAS];
f32 D_801649E8[NUM_CAMERAS];
f32 D_801649F8[NUM_CAMERAS];
s32 D_80164A08[4];
s32 D_80164A18[NUM_CAMERAS];
s32 D_80164A28;
s32 D_80164A2C;
static s32 sStagingTimer[NUM_CAMERAS];
f32 D_80164A30;
f32 D_80164A38[NUM_CAMERAS];
f32 D_80164A48[NUM_CAMERAS];
f32 D_80164A78[NUM_CAMERAS];
s8 D_80164A88;
s8 D_80164A89;
// UNUSED s8 D_80164A8C[3];
f32 D_80164A90[NUM_CAMERAS];
f32 D_80164AA0[NUM_CAMERAS];

void camera_init(Vec3f pos, s16 rot, u32 mode, s32 cameraId) {
    Camera* camera = &cameras[cameraId];

    if (cameraId >= NUM_CAMERAS) {
        return;
    }

    camera->mode = mode;
    sStagingTimer[cameraId] = 0;
    switch (mode) {
        case 0:
        case 1:
        case 3:
        case 8:
        case 9:
        case 10:
            D_80164A89 = 0;
            camera->pos[0] = pos[0];
            camera->pos[1] = pos[1];
            camera->pos[2] = pos[2];
            camera->someBitFlags = 0;
            camera->lookAt[0] = 0.0f;
            camera->lookAt[2] = 150.0f;
            camera->lookAt[1] = pos[1] - 3.0;
            camera->up[0] = 0.0f;
            camera->up[1] = 1.0f;
            camera->up[2] = 0.0f;

            camera->unk_B0 = 0;
            camera->unk_A0 = 0.0f;

            D_801649D8[cameraId] = 20.0f;
            D_801649E8[cameraId] = 10.0f;
            D_801649F8[cameraId] = 7.0f;
            D_80164A2C = 0;
            D_80164A30 = 30.0f;
            D_80164A38[cameraId] = 0.0f;
            D_80164A48[cameraId] = 0.0f;

            D_80164A90[cameraId] = 0.0f;
            D_80164AA0[cameraId] = 0.0f;
            D_80164A78[cameraId] = D_800DDB30[gActiveScreenMode];
            D_80164A18[cameraId] = 0;
            //D_80164A08[cameraId] = 0; // Now reset in spawn_players_and_cameras
            //D_80164498[cameraId] = 0.0f;
            camera->unk_94.unk_8 = 0;
            camera->unk_94.unk_0 = 0.0f;

            camera->unk_2C = rot;
            camera->unk_AC = rot;
            switch (gActiveScreenMode) {
                case SCREEN_MODE_1P:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    if (gModeSelection == BATTLE) {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 11.6f;
                        camera->unk_30[2] = -38.5f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 19.2f;
                        D_80164A88 = 0;
                    } else {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 9.5f;
                        camera->unk_30[2] = -50.0f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 70.0f;
                    }
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                    if (gModeSelection == BATTLE) {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 11.6f;
                        camera->unk_30[2] = -38.5f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 19.2f;
                    } else {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 9.6f;
                        camera->unk_30[2] = -35.0f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 30.0f;
                    }
                    break;
                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    if (gModeSelection == BATTLE) {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 11.6f;
                        camera->unk_30[2] = -38.5f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 19.2f;
                    } else {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 9.0f;
                        camera->unk_30[2] = -40.0f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 18.0f;
                    }
                    break;
            }

            func_80014DE4(cameraId);

            if (D_80164678[cameraId] == 0) {
                if (D_80164A28 == 1) {
                    gCameraFOV[cameraId] = 80.0f;
                } else {
                    gCameraFOV[cameraId] = 40.0f;
                }
                camera->unk_B4 = gCameraFOV[cameraId];
            }
            if (D_80164678[cameraId] == 1) {
                if (D_80164A28 == 1) {
                    gCameraFOV[cameraId] = 100.0f;
                } else {
                    gCameraFOV[cameraId] = 60.0f;
                }
                camera->unk_B4 = gCameraFOV[cameraId];
            }
            if (D_80164678[cameraId] == 2) {
                if (D_80164A28 == 1) {
                    gCameraFOV[cameraId] = 100.0f;
                } else {
                    gCameraFOV[cameraId] = 60.0f;
                }
                camera->unk_B4 = gCameraFOV[cameraId];
                D_80164A38[cameraId] = 20.0f;
                D_80164A48[cameraId] = 1.5f;
                D_80164A78[cameraId] = 1.0f;
            }
            break;
    }

    func_802B7F7C(camera->pos, camera->lookAt, camera->rot);
}

// Many arrays are hard-coded to 4. Skip those.
void freecam_init(Vec3f pos, s16 rot, u32 mode, s32 cameraId) {
    Camera* camera = &cameras[cameraId];

    if (cameraId >= NUM_CAMERAS) {
        return;
    }

    camera->mode = mode;
    sStagingTimer[cameraId] = 0;
    switch (mode) {
        case 0:
        case 1:
        case 3:
        case 8:
        case 9:
        case 10:
            D_80164A89 = 0;
            camera->pos[0] = pos[0];
            camera->pos[1] = pos[1];
            camera->pos[2] = pos[2];
            camera->someBitFlags = 0;
            camera->lookAt[0] = 0.0f;
            camera->lookAt[2] = 150.0f;
            camera->lookAt[1] = pos[1] - 3.0;
            camera->up[0] = 0.0f;
            camera->up[1] = 1.0f;
            camera->up[2] = 0.0f;
            //camera->playerId = (s16) cameraId;
            camera->unk_B0 = 0;
            camera->unk_A0 = 0.0f;

            // D_801649D8[cameraId] = 20.0f;
            // D_801649E8[cameraId] = 10.0f;
            // D_801649F8[cameraId] = 7.0f;
            // D_80164A2C = 0;
            // D_80164A30 = 30.0f;
            // D_80164A38[cameraId] = 0.0f;
            // D_80164A48[cameraId] = 0.0f;

            // D_80164A90[cameraId] = 0.0f;
            // D_80164AA0[cameraId] = 0.0f;
            // D_80164A78[cameraId] = D_800DDB30[gActiveScreenMode];
            // D_80164A18[cameraId] = 0;
            // D_80164A08[cameraId] = 0;
            // D_80164498[cameraId] = 0.0f;
            camera->unk_94.unk_8 = 0;
            camera->unk_94.unk_0 = 0.0f;

            camera->unk_2C = rot;
            camera->unk_AC = rot;
            switch (gActiveScreenMode) {
                case SCREEN_MODE_1P:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    if (gModeSelection == BATTLE) {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 11.6f;
                        camera->unk_30[2] = -38.5f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 19.2f;
                        D_80164A88 = 0;
                    } else {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 9.5f;
                        camera->unk_30[2] = -50.0f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 70.0f;
                    }
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                    if (gModeSelection == BATTLE) {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 11.6f;
                        camera->unk_30[2] = -38.5f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 19.2f;
                    } else {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 9.6f;
                        camera->unk_30[2] = -35.0f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 30.0f;
                    }
                    break;
                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    if (gModeSelection == BATTLE) {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 11.6f;
                        camera->unk_30[2] = -38.5f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 19.2f;
                    } else {
                        camera->unk_30[0] = 0.0f;
                        camera->unk_30[1] = 9.0f;
                        camera->unk_30[2] = -40.0f;
                        camera->unk_3C[0] = 0.0f;
                        camera->unk_3C[1] = 0.0f;
                        camera->unk_3C[2] = 18.0f;
                    }
                    break;
            }

            //func_80014DE4(cameraId);

           // if (D_80164678[cameraId] == 0) {
                if (D_80164A28 == 1) {
                  //  gCameraFOV[cameraId] = 80.0f;
                } else {
                   // gCameraFOV[cameraId] = 40.0f;
                }
                camera->unk_B4 = gCameraFOV[0];
           // }
            // if (D_80164678[cameraId] == 1) {
            //     if (D_80164A28 == 1) {
            //         gCameraFOV[cameraId] = 100.0f;
            //     } else {
            //         gCameraFOV[cameraId] = 60.0f;
            //     }
            //     camera->unk_B4 = gCameraFOV[cameraId];
            // // }
            // if (D_80164678[cameraId] == 2) {
            //     if (D_80164A28 == 1) {
            //         gCameraFOV[cameraId] = 100.0f;
            //     } else {
            //         gCameraFOV[cameraId] = 60.0f;
            //     }
            //     camera->unk_B4 = gCameraFOV[cameraId];
            //     D_80164A38[cameraId] = 20.0f;
            //     D_80164A48[cameraId] = 1.5f;
            //     D_80164A78[cameraId] = 1.0f;
            // }
            break;
    }
    func_802B7F7C(camera->pos, camera->lookAt, camera->rot);
}

// Thwomp related
void func_8001CA10(Camera* camera) {
    camera->unk_94.unk_8 = 0;
    camera->unk_94.unk_0 = 6.0f;
}

void func_8001CA24(Player* player, f32 arg1) {
    Camera* camera = CM_GetPlayerCamera(player - gPlayerOne);

    if (NULL == camera) {
        printf("[camera.c][func_8001CA24] Could not find a camera using GetPlayerCamera()\n");
        return;
    }

    camera->unk_94.unk_8 = 0;
    camera->unk_94.unk_0 = arg1;
}

void func_8001CA78(UNUSED Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, UNUSED s32 huh,
                   UNUSED s32 wut) {
    Mat3 sp74;
    Vec3f sp68;
    Vec3f sp5C;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 var_f14;
    f32 temp_f18;
    f32 temp_f16;
    UNUSED s32 pad;
    TrackPathPoint* temp_s2;

    temp_s2 = &gTrackPaths[0][gPathCountByPathIndex[0] - 10];
    sp68[0] = camera->unk_30[0];
    sp68[1] = camera->unk_30[1];
    sp68[2] = camera->unk_30[2];
    sp5C[0] = camera->unk_3C[0];
    sp5C[1] = camera->unk_3C[1];
    sp5C[2] = camera->unk_3C[2];
    arg2[0] = camera->lookAt[0];
    arg2[1] = camera->lookAt[1];
    arg2[2] = camera->lookAt[2];
    calculate_orientation_matrix(sp74, 0, 1, 0, -0x00008000);
    mtxf_translate_vec3f_mat3(sp5C, sp74);
    if (IsToadsTurnpike()) {
        var_f14 = sp5C[0];
    } else {
        var_f14 = sp5C[0] + temp_s2->x;
    }
    temp_f16 = D_80165230[7] + sp5C[2];
    temp_f18 = sp5C[1] + (temp_s2->y + D_80164A30);
    arg2[0] += (var_f14 - camera->lookAt[0]) * 1;
    arg2[1] += (temp_f18 - camera->lookAt[1]) * 1;
    arg2[2] += (temp_f16 - camera->lookAt[2]) * 1;
    mtxf_translate_vec3f_mat3(sp68, sp74);
    if (IsToadsTurnpike()) {
        var_f14 = sp68[0];
    } else {
        var_f14 = sp68[0] + temp_s2->x;
    }
    temp_f16 = D_80165230[7] + sp68[2];
    temp_f18 = sp68[1] + (temp_s2->y + D_80164A30 + 6.0f);
    move_f32_towards(&D_80164A30, 0, 0.02f);
    posX = camera->pos[0];
    *arg3 = ((var_f14 - posX) * 1) + posX;
    posY = camera->pos[1];
    *arg4 = ((temp_f18 - posY) * 1) + posY;
    posZ = camera->pos[2];
    *arg5 = ((temp_f16 - posZ) * 1) + posZ;
}

void func_8001CCEC(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, UNUSED s32* arg6,
                   s16 arg7, s16 index) {
    Mat3 sp9C;
    Vec3f sp90;
    Vec3f sp84;
    UNUSED s32 pad[3];
    f32 x;
    f32 y;
    f32 z;
    UNUSED s32 pad2;
    f32 var_f2;
    s16 var_v1;
    f32 temp_f0;
    f32 var_f0;
    s16 var_v0;
    f32 temp_f12;

    var_v1 = player->unk_DB4.unk0;
    var_f2 = player->unk_DB4.unk8;

    var_v1++;
    temp_f0 = ((var_f2 * var_v1) - (0.7 * (var_v1 * var_v1)));
    if ((var_v1 != 0) && (temp_f0 < 0)) {
        var_v1 = 0;
        var_f2 *= 0.8;
        if (var_f2 <= 0.1) {
            var_f2 = 0;
        }
    }
    if (temp_f0 <= 0) {
        temp_f0 = 0;
    }
    player->unk_DB4.unk0 = var_v1;
    player->unk_DB4.unk8 = var_f2;
    var_v0 = camera->unk_94.unk_8;
    var_f0 = camera->unk_94.unk_0;
    var_v0++;
    temp_f12 = (var_v0 * var_f0) - (1.25 * (var_v0 * var_v0));
    if ((var_v0 != 0) && (temp_f12 < 0)) {
        var_v0 = 0;
        var_f0 *= 0.9;
        if (var_f0 <= 0.1) {
            var_f0 = 0;
        }
    }
    if (temp_f12 <= 0) {
        temp_f12 = 0;
        // fakematch
        if (!var_v0) {}
    }
    camera->unk_94.unk_8 = var_v0;
    camera->unk_94.unk_0 = var_f0;
    if (D_80164678[index] == 2) {
        move_f32_towards(&D_80164A38[index], 20.0f, 0.1f);
        move_f32_towards(&D_80164A48[index], 1.5f, 0.1f);
        D_80164A78[index] += 0.1;
        if (D_80164A78[index] >= 1) {
            D_80164A78[index] = 1;
        }

    } else {
        move_f32_towards(&D_80164A38[index], 0, 0.1f);
        move_f32_towards(&D_80164A48[index], 0, 0.1f);
        D_80164A78[index] -= 0.1;
        if (D_800DDB30[gActiveScreenMode] >= D_80164A78[index]) {
            D_80164A78[index] = D_800DDB30[gActiveScreenMode];
        }
    }
    if ((player->unk_0CA & 0x100) == 0x100) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                move_f32_towards(&D_80164A90[index], 20, 0.02f);
                move_f32_towards(&D_80164AA0[index], 10, 0.02f);
                break;
            default:
                if (IsYoshiValley()) {
                    move_f32_towards(&D_80164A90[index], 50, 0.04f);
                    move_f32_towards(&D_80164AA0[index], 35, 0.04f);
                } else {
                    move_f32_towards(&D_80164A90[index], 40, 0.02f);
                    move_f32_towards(&D_80164AA0[index], 20, 0.02f);
                }
                break;
        }
    } else {
        move_f32_towards(&D_80164A90[index], 0, 0.04f);
        move_f32_towards(&D_80164AA0[index], 0, 0.04f);
    }
    sp90[0] = camera->unk_30[0];
    sp90[1] =
        camera->unk_30[1] + (player->unk_DB4.unk1E * 0.85) - D_80164A48[index] + D_80164AA0[index] + (temp_f12 / 2);
    sp90[2] = camera->unk_30[2] + temp_f0 + D_80164A38[index];
    sp84[0] = camera->unk_3C[0];
    sp84[1] = camera->unk_3C[1] + (player->unk_DB4.unk1E * 0.85) + temp_f12;
    sp84[2] = camera->unk_3C[2] + temp_f0 - D_80164A90[index];
    arg2[0] = camera->lookAt[0];
    arg2[1] = camera->lookAt[1];
    arg2[2] = camera->lookAt[2];
    if ((player->effects & 0x01000000) == 0x01000000) {
        sp84[2] /= 3.0f;
    }
    calculate_orientation_matrix(sp9C, 0, 1, 0, arg7);
    mtxf_translate_vec3f_mat3(sp84, sp9C);

    x = player->pos[0] + sp84[0];
    z = player->pos[2] + sp84[2];
    y = player->pos[1] + sp84[1];

    arg2[0] += (x - camera->lookAt[0]) * D_80164A78[index];
    arg2[2] += ((z - camera->lookAt[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->effects & 2) == 2)) {
        arg2[1] += ((y - camera->lookAt[1]) * 0.02);
    } else {
        arg2[1] += ((y - camera->lookAt[1]) * 0.5);
    }
    mtxf_translate_vec3f_mat3(sp90, sp9C);
    x = player->pos[0] + sp90[0];
    z = player->pos[2] + sp90[2];
    if ((player->effects & 0x01000000) != 0x01000000) {
        var_f0 = player->pos[1] + sp90[1];
        // permute
        y = var_f0;
    } else {
        y = player->unk_074 + player->boundingBoxSize + sp90[1];
    }

    *arg3 = camera->pos[0] + ((x - camera->pos[0]) * D_80164A78[index]);
    *arg5 = camera->pos[2] + ((z - camera->pos[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->effects & 2) == 2)) {
        *arg4 = camera->pos[1] + (((y - camera->pos[1]) * 0.01));
    } else {
        *arg4 = camera->pos[1] + (((y - camera->pos[1]) * 0.15));
    }

    if ((player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) != 0) {
        *arg4 = gPlayerWaterLevel[index];
    }
}

void func_8001D53C(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, s16 arg6, s16 arg7) {
    Mat3 sp74;
    Vec3f sp68;
    Vec3f sp5C;
    f32 stackPadding0;
    f32 stackPadding1;
    f32 stackPadding2;
    UNUSED f32 pad[4];
    f32 thing;

    if (((u16) player->unk_222 == 0) && (camera->unk_A0 == 0.0f)) {
        camera->unk_A0 = 0.0f;
    }
    if ((u16) player->unk_222 != 4) {
        move_f32_towards(&camera->unk_A0, 20.0f, 0.06f);
    } else {
        move_f32_towards(&camera->unk_A0, 0.0f, 0.06f);
    }
    thing = gPlayerWaterLevel[arg7];
    sp68[0] = camera->unk_30[0];
    sp68[1] = camera->unk_30[1];
    sp68[2] = camera->unk_30[2];
    sp5C[0] = camera->unk_3C[0];
    sp5C[1] = camera->unk_3C[1] + camera->unk_A0;
    sp5C[2] = camera->unk_3C[2];
    arg2[0] = camera->lookAt[0];
    arg2[1] = camera->lookAt[1];
    arg2[2] = camera->lookAt[2];
    calculate_orientation_matrix(sp74, 0.0f, 1.0f, 0.0f, arg6);
    mtxf_translate_vec3f_mat3(sp5C, sp74);
    stackPadding0 = player->pos[0] + sp5C[0];
    stackPadding2 = player->pos[2] + sp5C[2];
    stackPadding1 = player->pos[1] + sp5C[1];
    arg2[0] += (stackPadding0 - camera->lookAt[0]) * 1;
    arg2[2] += (stackPadding2 - camera->lookAt[2]) * 1;
    arg2[1] += (stackPadding1 - camera->lookAt[1]) * 1;
    mtxf_translate_vec3f_mat3(sp68, sp74);
    stackPadding0 = player->pos[0] + sp68[0];
    stackPadding2 = player->pos[2] + sp68[2];
    stackPadding1 = sp68[1] + (player->unk_074 + 1.5);
    if ((player->unk_0CA & 1) == 1) {
        stackPadding1 = sp68[1] + (thing + 10.0f);
    }
    *arg3 = stackPadding0;
    *arg4 = stackPadding1;
    *arg5 = stackPadding2;
    D_80164A90[arg7] = 0.0f;
    D_80164AA0[arg7] = 0.0f;
}

void func_8001D794(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, s16 arg6) {
    Mat3 sp6C;
    Vec3f sp60;
    Vec3f sp54;
    UNUSED f32 stackPadding[4];
    f32 test1;
    f32 test2;
    f32 test3;

    sp60[0] = camera->unk_30[0];
    sp60[1] = camera->unk_30[1];
    sp60[2] = camera->unk_30[2] - 6;

    sp54[0] = camera->unk_3C[0];
    sp54[1] = camera->unk_3C[1];
    sp54[2] = camera->unk_3C[2];

    arg2[0] = camera->lookAt[0];
    arg2[1] = camera->lookAt[1];
    arg2[2] = camera->lookAt[2];

    calculate_orientation_matrix(sp6C, 0, 1, 0, arg6);
    mtxf_translate_vec3f_mat3(sp54, sp6C);

    test1 = player->pos[0] + sp54[0];
    test3 = player->pos[2] + sp54[2];
    test2 = player->pos[1] + sp54[1];
    arg2[0] += (test1 - camera->lookAt[0]) * 1;
    arg2[1] += (test2 - camera->lookAt[1]) * 1;
    arg2[2] += (test3 - camera->lookAt[2]) * 1;

    mtxf_translate_vec3f_mat3(sp60, sp6C);

    test1 = player->pos[0] + sp60[0];
    test3 = player->pos[2] + sp60[2];
    test2 = player->pos[1] + sp60[1];
    *arg3 = camera->pos[0] + ((test1 - camera->pos[0]) * 1);
    *arg4 = camera->pos[1] + ((test2 - camera->pos[1]) * 1);
    *arg5 = camera->pos[2] + ((test3 - camera->pos[2]) * 1);
}

void func_8001D944(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, UNUSED s32* arg6,
                   s16 arg7, s16 index) {
    Mat3 sp9C;
    Vec3f sp90;
    Vec3f sp84;
    UNUSED s32 pad[3];
    f32 x;
    f32 y;
    f32 z;
    UNUSED s32 pad2;
    f32 var_f2;
    s16 var_v1;
    f32 temp_f0;
    f32 var_f0;
    s16 var_v0;
    f32 temp_f12;

    var_v1 = player->unk_DB4.unk0;
    var_f2 = player->unk_DB4.unk8;

    var_v1++;
    temp_f0 = ((var_f2 * var_v1) - (0.7 * (var_v1 * var_v1)));
    if ((var_v1 != 0) && (temp_f0 < 0)) {
        var_v1 = 0;
        var_f2 *= 0.8;
        if (var_f2 <= 0.1) {
            var_f2 = 0;
        }
    }
    if (temp_f0 <= 0) {
        temp_f0 = 0;
    }
    player->unk_DB4.unk0 = var_v1;
    player->unk_DB4.unk8 = var_f2;
    var_v0 = camera->unk_94.unk_8;
    var_f0 = camera->unk_94.unk_0;
    var_v0++;
    temp_f12 = (var_v0 * var_f0) - (1.25 * (var_v0 * var_v0));
    if ((var_v0 != 0) && (temp_f12 < 0)) {
        var_v0 = 0;
        var_f0 *= 0.9;
        if (var_f0 <= 0.1) {
            var_f0 = 0;
        }
    }
    if (temp_f12 <= 0) {
        temp_f12 = 0;
        // fakematch
        if (!var_v0) {}
    }
    camera->unk_94.unk_8 = var_v0;
    camera->unk_94.unk_0 = var_f0;
    if (D_80164678[index] == 2) {
        move_f32_towards(&D_80164A38[index], 20.0f, 0.1f);
        move_f32_towards(&D_80164A48[index], 1.5f, 0.1f);
        D_80164A78[index] += 0.1;
        if (D_80164A78[index] >= 1) {
            D_80164A78[index] = 1;
        }

    } else {
        move_f32_towards(&D_80164A38[index], 0, 0.1f);
        move_f32_towards(&D_80164A48[index], 0, 0.1f);
        D_80164A78[index] -= 0.1;
        if (D_800DDB30[gActiveScreenMode] >= D_80164A78[index]) {
            D_80164A78[index] = D_800DDB30[gActiveScreenMode];
        }
    }
    if ((player->unk_0CA & 0x100) == 0x100) {

        move_f32_towards(&D_80164A90[index], 15, 0.02f);
        move_f32_towards(&D_80164AA0[index], 20, 0.02f);
    } else {
        move_f32_towards(&D_80164A90[index], 0, 0.02f);
        move_f32_towards(&D_80164AA0[index], 0, 0.02f);
    }
    sp90[0] = camera->unk_30[0];
    sp90[1] =
        camera->unk_30[1] + (player->unk_DB4.unk1E * 0.85) - D_80164A48[index] + D_80164AA0[index] + (temp_f12 / 2);
    sp90[2] = camera->unk_30[2] + temp_f0 + D_80164A38[index] + D_80164AA0[index];
    sp84[0] = camera->unk_3C[0];
    sp84[1] = camera->unk_3C[1] + (player->unk_DB4.unk1E * 0.85) + temp_f12;
    sp84[2] = camera->unk_3C[2] + temp_f0 - D_80164A90[index];
    arg2[0] = camera->lookAt[0];
    arg2[1] = camera->lookAt[1];
    arg2[2] = camera->lookAt[2];
    if ((player->effects & 0x01000000) == 0x01000000) {
        sp84[2] /= 3.0f;
    }
    calculate_orientation_matrix(sp9C, 0, 1, 0, arg7);
    mtxf_translate_vec3f_mat3(sp84, sp9C);

    x = player->pos[0] + sp84[0];
    z = player->pos[2] + sp84[2];
    y = player->pos[1] + sp84[1];

    arg2[0] += (x - camera->lookAt[0]) * D_80164A78[index];
    arg2[2] += ((z - camera->lookAt[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->effects & 2) == 2)) {
        arg2[1] += ((y - camera->lookAt[1]) * 0.02);
    } else {
        arg2[1] += ((y - camera->lookAt[1]) * 0.5);
    }
    mtxf_translate_vec3f_mat3(sp90, sp9C);
    x = player->pos[0] + sp90[0];
    z = player->pos[2] + sp90[2];
    if ((player->effects & 0x01000000) != 0x01000000) {
        var_f0 = player->pos[1] + sp90[1];
        // permute
        y = var_f0;
    } else {
        y = player->unk_074 + player->boundingBoxSize + sp90[1];
    }

    *arg3 = camera->pos[0] + ((x - camera->pos[0]) * D_80164A78[index]);
    *arg5 = camera->pos[2] + ((z - camera->pos[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->effects & 2) == 2)) {
        *arg4 = camera->pos[1] + (((y - camera->pos[1]) * 0.01));
    } else {
        *arg4 = camera->pos[1] + (((y - camera->pos[1]) * 0.15));
    }

    if ((player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) != 0) {
        *arg4 = gPlayerWaterLevel[index];
    }
}

void func_8001E0C4(Camera* camera, Player* player, s8 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp80;
    f32 temp_f14;
    UNUSED s32 pad2;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    Vec3f sp60;
    s16 temp_t7;
    s16 var_a2;
    UNUSED s32 pad3[8];
    s32 test = 3;

    if (player->unk_078 == 0) {
        var_a2 = 0x0064;
    } else if (player->unk_078 < 0) {
        var_a2 = 0x87 - (player->unk_078 / 3);
    } else {
        var_a2 = (player->unk_078 / 3) + 0x87;
    }
    adjust_angle(&camera->unk_2C, player->rotation[1], var_a2);
    func_8001CA78(player, camera, sp60, &sp74, &sp70, &sp6C, camera->unk_2C, arg2);
    camera->someBitFlags &= ~0x0004;
    temp_t7 = check_bounding_collision(&camera->collision, test, sp74, sp70, sp6C);
    if (camera->collision.surfaceDistance[2] < 0.0f) {
        sp74 += -camera->collision.orientationVector[0] * camera->collision.surfaceDistance[2] * 1;
        sp70 += -camera->collision.orientationVector[1] * camera->collision.surfaceDistance[2] * 0.5;
        sp6C += -camera->collision.orientationVector[2] * camera->collision.surfaceDistance[2] * 1;
    }
    if (camera->collision.surfaceDistance[0] < 0.0f) {
        camera->someBitFlags = camera->someBitFlags | 4 | 2;
        sp74 += -camera->collision.unk48[0] * camera->collision.surfaceDistance[0] * 1.5;
        sp70 += -camera->collision.unk48[1] * camera->collision.surfaceDistance[0] * 1;
        sp6C += -camera->collision.unk48[2] * camera->collision.surfaceDistance[0] * 1.5;
    }
    if (camera->collision.surfaceDistance[1] < 0.0f) {
        camera->someBitFlags = camera->someBitFlags | 4 | 2;
        sp74 += -camera->collision.unk54[0] * camera->collision.surfaceDistance[1] * 1.5;
        sp70 += -camera->collision.unk54[1] * camera->collision.surfaceDistance[1] * 1;
        sp6C += -camera->collision.unk54[2] * camera->collision.surfaceDistance[1] * 1.5;
    }
    if ((temp_t7 == 0) && ((camera->someBitFlags & 2) != 2)) {
        camera->unk_AC = camera->unk_2C;
    }
    camera->lookAt[0] = sp60[0];
    camera->lookAt[1] = sp60[1];
    camera->lookAt[2] = sp60[2];
    camera->pos[0] = sp74;
    camera->pos[1] = sp70;
    camera->pos[2] = sp6C;
    temp_f12 = camera->lookAt[0] - camera->pos[0];
    sp80 = camera->lookAt[1] - camera->pos[1];
    temp_f14 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(temp_f12, temp_f14);
    camera->rot[0] = atan2s(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp80);
    camera->rot[2] = 0;
}

// This function has a few stack variables.
void func_8001E45C(Camera* camera, Player* player, s8 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    UNUSED s32 pad2;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED s32 pad3[3];
    Vec3f sp64;
    UNUSED s32 pad4[2];
    s32 sp58;
    UNUSED s16 pad5[4];
    s16 var_a3;
    UNUSED s16 pad6;
    s16 temp;

    if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
        var_a3 = 100;
        if (player->unk_078 == 0) {
            camera->unk_B0 = 0;
        } else {
            if (player->unk_078 < 0) {
                var_a3 = 0xA5 - (player->unk_078 / 2);
                if ((player->effects & 0x20000000) == 0x20000000) {
                    move_s16_towards(&camera->unk_B0, -0x0B60, 0.1f);
                } else {
                    move_s16_towards(&camera->unk_B0, -0x0888, 0.1f);
                }
            } else {
                var_a3 = (player->unk_078 / 2) + 0xA5;
                if ((player->effects & 0x20000000) == 0x20000000) {
                    move_s16_towards(&camera->unk_B0, 0x0B60, 0.1f);
                } else {
                    move_s16_towards(&camera->unk_B0, 0x0888, 0.1f);
                }
            }
        }
    } else {
        move_s16_towards(&camera->unk_B0, 0, 0.05f);
        var_a3 = ((s16) camera->unk_2C / 182) - ((s16) player->rotation[1] / 182);
        if (player->unk_078 == 0) {
            if ((player->effects & 0x20) == 0x20) {
                var_a3 = 0x02D8;
            } else {
                var_a3 = 0x01F4;
            }
        } else if (player->unk_078 < 0) {
            if ((var_a3 <= -70) || (var_a3 >= 70)) {
                var_a3 = 0xB4 - player->unk_078;
            } else {
                var_a3 = 0xA5 - (player->unk_078 / 2);
            }
        } else if ((var_a3 <= -70) || (var_a3 >= 0x46)) {
            var_a3 = player->unk_078 + 0xB4;
        } else {
            var_a3 = (player->unk_078 / 2) + 0xA5;
        }
    }
    if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40) ||
        ((player->effects & 0x4000) == 0x4000) || ((player->effects & 0x80000) == 0x80000) ||
        ((player->effects & 0x800000) == 0x800000) || (((player->effects & 0x20) == 0x20) && (player->unk_078 != 0)) ||
        (player->collision.surfaceDistance[0] <= 0.0f) || (player->collision.surfaceDistance[1] <= 0.0f) ||
        ((player->effects & 0x20000) == 0x20000)) {
        func_8001CCEC(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->unk_2C, (s32) arg2);
    } else {
        adjust_angle(&camera->unk_2C, (s16) (player->rotation[1] + camera->unk_B0), var_a3);
        func_8001CCEC(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->unk_2C, (s32) arg2);
    }
    temp = 3;
    camera->someBitFlags &= 0xFFFB;
    check_bounding_collision(&camera->collision, temp, sp84, sp80, sp7C);

    camera->pos[0] = sp84;
    camera->pos[1] = sp80;
    camera->pos[2] = sp7C;

    camera->lookAt[0] = sp64[0];
    camera->lookAt[1] = sp64[1];
    camera->lookAt[2] = sp64[2];

    temp_f12 = camera->lookAt[0] - camera->pos[0];
    sp90 = camera->lookAt[1] - camera->pos[1];
    temp_f14 = camera->lookAt[2] - camera->pos[2];

    camera->rot[1] = atan2s(temp_f12, temp_f14);
    camera->rot[0] = atan2s(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    camera->rot[2] = 0;
}

void func_8001E8E8(Camera* camera, Player* player, s8 arg2) {
    UNUSED f32 pad[6];
    f32 temp_f12;
    f32 sp88;
    f32 temp_f14;
    UNUSED f32 pad2;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    UNUSED Vec3f pad3;
    Vec3f sp5C;
    UNUSED f32 pad4[10];

    camera->unk_B0 = 0;
    camera->unk_2C = player->rotation[1];
    func_8001D53C(player, camera, sp5C, &sp7C, &sp78, &sp74, (s16) (s32) player->rotation[1], (s16) (s32) arg2);
    check_bounding_collision(&camera->collision, 5.0f, sp7C, sp78, sp74);
    camera->lookAt[0] = sp5C[0];
    camera->lookAt[1] = sp5C[1];
    camera->lookAt[2] = sp5C[2];
    camera->pos[0] = sp7C;
    camera->pos[1] = sp78;
    camera->pos[2] = sp74;
    temp_f12 = camera->lookAt[0] - camera->pos[0];
    sp88 = camera->lookAt[1] - camera->pos[1];
    temp_f14 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(temp_f12, temp_f14);
    camera->rot[0] = atan2s(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp88);
    camera->rot[2] = 0;
}

void func_8001EA0C(Camera* camera, Player* player, s8 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    UNUSED s32 pad2;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED s32 pad3[3];
    Vec3f sp64;
    UNUSED s32 pad4[2];
    s32 sp58;
    UNUSED s16 pad5[4];
    s16 var_a3;
    UNUSED s16 pad6;
    s16 temp;

    if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
        var_a3 = 100;
        if (player->unk_078 == 0) {
            camera->unk_B0 = 0;
        } else {
            if (player->unk_078 < 0) {
                var_a3 = 0xA5 - (player->unk_078 / 2);

                if ((player->effects & 0x20000000) == 0x20000000) {
                    move_s16_towards(&camera->unk_B0, -0x0B60, 0.1f);
                } else {
                    move_s16_towards(&camera->unk_B0, -0x0888, 0.1f);
                }
            } else {
                var_a3 = (player->unk_078 / 2) + 0xA5;
                if ((player->effects & 0x20000000) == 0x20000000) {
                    move_s16_towards(&camera->unk_B0, 0x0B60, 0.1f);
                } else {
                    move_s16_towards(&camera->unk_B0, 0x0888, 0.1f);
                }
            }
        }
    } else {
        move_s16_towards(&camera->unk_B0, 0, 0.05f);
        var_a3 = ((s16) camera->unk_2C / 182) - ((s16) player->rotation[1] / 182);
        if (player->unk_078 == 0) {
            if ((player->effects & 0x20) == 0x20) {
                var_a3 = 0x02D8;
            } else {
                var_a3 = 0x01F4;
            }
        } else if (player->unk_078 < 0) {
            if ((var_a3 <= -70) || (var_a3 >= 70)) {
                var_a3 = 0xB4 - player->unk_078;
            } else {
                var_a3 = 0xA5 - (player->unk_078 / 2);
            }
        } else if ((var_a3 <= -70) || (var_a3 >= 0x46)) {
            var_a3 = player->unk_078 + 0xB4;
        } else {
            var_a3 = (player->unk_078 / 2) + 0xA5;
        }
    }
    if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40) ||
        ((player->effects & 0x4000) == 0x4000) || ((player->effects & 0x80000) == 0x80000) ||
        ((player->effects & 0x800000) == 0x800000) || (((player->effects & 0x20) == 0x20) && (player->unk_078 != 0)) ||
        (player->collision.surfaceDistance[0] <= 0.0f) || (player->collision.surfaceDistance[1] <= 0.0f) ||
        ((player->effects & 0x20000) == 0x20000)) {
        func_8001D944(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->unk_2C, (s32) arg2);
    } else {
        adjust_angle(&camera->unk_2C, (s16) (player->rotation[1] + camera->unk_B0), var_a3);
        func_8001D944(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->unk_2C, (s32) arg2);
    }
    temp = 3;
    camera->someBitFlags &= 0xFFFB;
    check_bounding_collision(&camera->collision, temp, sp84, sp80, sp7C);

    camera->pos[0] = sp84;
    camera->pos[1] = sp80;
    camera->pos[2] = sp7C;

    camera->lookAt[0] = sp64[0];
    camera->lookAt[1] = sp64[1];
    camera->lookAt[2] = sp64[2];

    temp_f12 = camera->lookAt[0] - camera->pos[0];
    sp90 = camera->lookAt[1] - camera->pos[1];
    temp_f14 = camera->lookAt[2] - camera->pos[2];

    camera->rot[1] = atan2s(temp_f12, temp_f14);
    camera->rot[0] = atan2s(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    camera->rot[2] = 0;
}

void func_8001EE98(Player* player, Camera* camera, s8 index) {
    s32 cameraIndex = camera->cameraId;

    switch (gModeSelection) {
        case GRAND_PRIX:
            // clang-format off
            if (((player->type & PLAYER_CINEMATIC_MODE) == PLAYER_CINEMATIC_MODE) || (gDemoMode == 1)) { camera->mode = 3;
            //             -->                 -->        Scroll right        -->      bit more     -->     ^ Required for matching
                // clang-format on
            } else if (gIsGamePaused == 1) {
                func_8001A0A4(&camera->mode, camera, player, index, cameraIndex);
            } else {
                func_8001A0DC(&camera->mode, camera, player, index, cameraIndex);
            }
            break;
        case BATTLE:
            if ((gDemoMode == 1) || ((D_8015F894 == 2) && (D_80164A89 == 1))) {
                if (D_80164A88 == 0) {
                    func_80019ED0();
                }
                D_80164A88 = 1;
                camera->mode = 3;
            } else {
                D_80164A88 = 0;
                if (gIsGamePaused == 1) {
                    func_8001A0A4(&camera->mode, camera, player, index, cameraIndex);
                } else {
                    func_8001A0DC(&camera->mode, camera, player, index, cameraIndex);
                }
                camera->mode = 9;
            }
            break;
        case TIME_TRIALS:
            if (((gPlayerOne->type & PLAYER_CINEMATIC_MODE) == PLAYER_CINEMATIC_MODE) || (gDemoMode == 1)) {
                camera->mode = 3;
            } else {
                if (gIsGamePaused == 1) {
                    func_8001A0A4(&camera->mode, camera, player, index, cameraIndex);
                } else {
                    func_8001A0DC(&camera->mode, camera, player, index, cameraIndex);
                }
                camera->mode = 1;
            }
            break;
        case VERSUS:
            if (((player->type & PLAYER_CINEMATIC_MODE) == PLAYER_CINEMATIC_MODE) || (gDemoMode == 1) ||
                (D_8015F894 == 2)) {
                camera->mode = 3;
            } else {
                if (gIsGamePaused == 1) {
                    func_8001A0A4(&camera->mode, camera, player, index, cameraIndex);
                } else {
                    func_8001A0DC(&camera->mode, camera, player, index, cameraIndex);
                }
                camera->mode = 1;
            }
            break;
    }
    if (gIsGamePaused == 0) {
        switch (camera->mode) {
            case 3: // end of race
                func_8001A588(&camera->mode, camera, player, index, cameraIndex);
                break;
            case 1: // player camera
                if (((player->unk_0CA & 1) == 1) || ((player->unk_0CA & 2) == 2)) {
                    func_8001E8E8(camera, player, index);
                    break;
                }
                func_8001E45C(camera, player, index);
                break;
            case 8: // Transition start
                func_8001E0C4(camera, player, index);
                func_8001F87C(cameraIndex);
                break;
            case 9:
                if (((player->unk_0CA & 1) == 1) || ((player->unk_0CA & 2) == 2)) {
                    func_8001E8E8(camera, player, index);
                    break;
                }
                func_8001EA0C(camera, player, index);
                break;
        }
    }
}

void func_8001F394(Player* player, f32* arg1) {
    f32 var_f0;
    UNUSED s32 pad;
    s32 playerIndex = player - gPlayerOne;
    UNUSED s32 pad2;

    Camera* camera = CM_GetPlayerCamera(player - gPlayerOne);

    if (NULL == camera) {
        printf("[camera.c][func_8001F394] Could not find a camera using GetPlayerCamera()\n");
        return;
    }

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }

    if (D_80164A08[playerIndex] == 0) {
        if (player->soundEffects & HOLD_BANANA_SOUND_EFFECT) {
            D_80164A08[playerIndex] = 1;
        }
        if ((player->effects & BOOST_EFFECT) == BOOST_EFFECT) {
            D_80164A08[playerIndex] = 2;
        }
        if ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) {
            D_80164A08[playerIndex] = 3;
        }
        if ((player->soundEffects & 0x100) == 0x100) {
            D_80164A08[playerIndex] = 4;
        }
        if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) {
            D_80164A08[playerIndex] = 5;
        }
        D_80164498[playerIndex] = 0.0f;
    }
    switch (D_80164A08[playerIndex]) {
        case 1:
            if (player->soundEffects & HOLD_BANANA_SOUND_EFFECT) {
                move_f32_towards(&D_80164498[playerIndex], 20.0f, 0.2f);
            } else {
                if (D_80164498[playerIndex] > 1.0f) {
                    D_80164498[playerIndex] -= 1.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    D_80164498[playerIndex] = 0.0f;
                }
            }
            break;
        case 2:
            if ((player->effects & BOOST_EFFECT) == BOOST_EFFECT) {
                if (player->boostTimer != 0) {
                    move_f32_towards(&D_80164498[playerIndex], 8.0f, 0.2f);
                }
            } else {
                if (D_80164498[playerIndex] > 1.0f) {
                    D_80164498[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    D_80164498[playerIndex] = 0.0f;
                }
            }
            break;
        case 3:
            if (((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) &&
                ((player->effects & 8) == 8)) {
                move_f32_towards(&D_80164498[playerIndex], 20.0f, 0.1f);
            } else {
                if (D_80164498[playerIndex] > 1.0f) {
                    D_80164498[playerIndex] -= 1.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    D_80164498[playerIndex] = 0.0f;
                }
            }
            break;
        case 4:
            if ((player->soundEffects & 0x100) == 0x100) {
                move_f32_towards(&D_80164498[playerIndex], 25.0f, 1.0f);
            } else {
                if (D_80164498[playerIndex] > 1.0f) {
                    D_80164498[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    D_80164498[playerIndex] = 0.0f;
                }
            }
            break;
        case 5:
            if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) {
                move_f32_towards(&D_80164498[playerIndex], 18.0f, 0.2f);
            } else {
                if (D_80164498[playerIndex] > 1.0f) {
                    D_80164498[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    D_80164498[playerIndex] = 0.0f;
                }
            }
            break;
    }
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            if (D_80164A28 == 1) {
                D_80164498[playerIndex] = 40.0f;
            }
            if (D_80164A28 == 2) {
                if (D_80164498[playerIndex] >= 0.0f) {
                    D_80164498[playerIndex] -= 0.8;
                }
                if (D_80164498[playerIndex] <= 0.0f) {
                    D_80164A28 = 0;
                    D_80164498[playerIndex] = 0.0f;
                }
            }
            var_f0 = func_80014EE4(*arg1, playerIndex);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            var_f0 = func_80014EE4(*arg1, playerIndex);
            break;
    }
    *arg1 = var_f0;
    camera->unk_B4 = var_f0;
}

void func_8001F87C(s32 cameraId) {
    if (gActiveScreenMode == SCREEN_MODE_1P) {
        if (gModeSelection == GRAND_PRIX) {
            for (size_t i = 0; i < NUM_PLAYERS; i++) {
                if ((gPlayerOne[i].type & 0x200) || (gPlayerOne[i].type & 0x80)) {
                    break;
                }
                if (i == PLAYER_EIGHT) {
                    sStagingTimer[cameraId] += 1;
                }
                if ((i == PLAYER_EIGHT) && (sStagingTimer[cameraId] == 60)) {
                    D_80164A28 = 2;
                    cameras[cameraId].mode = 1;
                    cameras[cameraId].rot[1] = gPlayerOne[i].rotation[1];
                    cameras[cameraId].unk_2C = gPlayerOne[i].rotation[1];
                }
            }
        }
    }
}
