#include <libultraship.h>
#include <libultra/gbi.h>

#include "MarioRaceway.h"

extern "C" {
    #include "main.h"
    #include "camera.h"
    #include "course_offsets.h"
    #include "code_800029B0.h"
    #include "render_courses.h"
    #include "code_8006E9C0.h"
    #include "code_80057C60.h"
    #include "defines.h"
    #include "math_util.h"
    #include "external.h"
    #include "code_80005FD0.h"
    #include "spawn_players.h"
    #include "render_objects.h"
    #include "assets/common_data.h"
    #include "save.h"
    #include "staff_ghosts.h"
    #include "actors.h"
    #include "mario_raceway_data.h"
    //#include "all_course_offsets.h"
    #include "memory.h"
    extern const char *mario_raceway_dls[];
}

CourseMarioRaceway::CourseMarioRaceway() {}

    // Virtual functions to be overridden by derived classes
// void CourseMarioRaceway::Load(const char* courseVtx, 
//                   course_texture* textures, const char* displaylists, size_t dlSize) {
//     StockCourse::Load(courseVtx, textures, displaylists, dlSize);
// }
void CourseMarioRaceway::LoadTextures() {
    dma_textures(gTextureTrees1, 0x0000035BU, 0x00000800U);
    D_802BA058 = dma_textures(gTexturePiranhaPlant1, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant2, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant3, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant4, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant5, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant6, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant7, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant8, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant9, 0x000003E8U, 0x00000800U);
}

void CourseMarioRaceway::SpawnActors() {
    struct Actor* actor;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };
    spawn_foliage(d_course_mario_raceway_tree_spawns);
    spawn_piranha_plants(d_course_mario_raceway_piranha_plant_spawns);
    spawn_all_item_boxes(d_course_mario_raceway_item_box_spawns);
    vec3f_set(position, 150.0f, 40.0f, -1300.0f);
    position[0] *= gCourseDirection;
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_MARIO_SIGN);
    vec3f_set(position, 2520.0f, 0.0f, 1240.0f);
    position[0] *= gCourseDirection;
    actor = &gActorList[add_actor_to_empty_slot(position, rotation, velocity, ACTOR_MARIO_SIGN)];
    actor->flags |= 0x4000;
}

void CourseMarioRaceway::Init() {  }
void CourseMarioRaceway::InitClouds() {
    if (this->clouds) {
        init_clouds(this->clouds);
    }
}
void CourseMarioRaceway::UpdateClouds(s32 arg0, Camera* camera) {
    s32 cloudIndex;
    s32 objectIndex;
    CloudData* cloud;

    for (cloudIndex = 0; cloudIndex < D_8018D1F0; cloudIndex++) {
        cloud = &this->cloudList[cloudIndex];
        objectIndex = D_8018CC80[arg0 + cloudIndex];
        func_800788F8(objectIndex, cloud->rotY, camera);
    }
}

// Likely sets minimap boundaries
void CourseMarioRaceway::MinimapSettings() {
    D_8018D220 = reinterpret_cast<uint8_t (*)[1024]>(dma_textures(gTextureExhaust5, 0x443, 0x1000));
    D_8018D2A0 = 0.022f;
    D_8018D2E0 = 6;
    D_8018D2E8 = 28;
    D_8018D2C0[0] = 260;
    D_8018D2D8[0] = 170;
    D_80165718 = 0;
    D_80165720 = 5;
    D_80165728 = -240;
}

void CourseMarioRaceway::InitCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
        for (size_t i = 0; i < D_80165738; i++) {
            find_unused_obj_index(&gObjectParticle3[i]);
            init_object(gObjectParticle3[i], 0);
        }
    }
}

void CourseMarioRaceway::SomeSounds() {
    vec3f_set(D_8015F748, -223.0f, 94.0f, -155.0f);
    func_800C9D80(D_8015F748, D_802B91C8, 0x5103700B);
}

void CourseMarioRaceway::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x1B9)) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void CourseMarioRaceway::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x1B9)) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

// Positions the finishline on the minimap
void CourseMarioRaceway::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->MinimapFinishlineX, this->MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void CourseMarioRaceway::SetStaffGhost() {
    u32 temp_v0;
    if (temp_v0 <= 9000) {
        D_80162DD6 = 0;
        D_80162DF4 = 0;
    } else {
        D_80162DD6 = 1;
        D_80162DF4 = 1;
    }
    D_80162DC4 = &d_mario_raceway_staff_ghost[0];
    D_80162DE4 = 0;
}

void CourseMarioRaceway::BeginPlay() {  }
void CourseMarioRaceway::Render(struct UnkStruct_800DC5EC* arg0) {
    u16 sp22 = arg0->pathCounter;
    u16 temp_t0 = arg0->playerDirection;

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_mario_raceway_packed_dl_3050
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07003050))));
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    switch (sp22) {
        case 1:
        case 2:
        case 17:
            if ((temp_t0 == 2) || (temp_t0 == 1)) {
                func_802911C4();
            }
            break;
        case 3:
            if (temp_t0 != 0) {
                func_802911C4();
            }
            break;
        case 4:
            if (temp_t0 == 0) {
                func_80291198();
            } else {
                if (temp_t0 == 1) {
                    func_80291198();
                }
                func_802911C4();
            }
            break;
        case 5:
        case 6:
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                func_802911C4();
            } else {
                func_80291198();
            }
            break;
        case 7:
            func_80291198();
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                func_802911C4();
            }
            break;
        case 8:
        case 9:
            if (temp_t0 != 1) {
                func_802911C4();
            }
            /* fallthrough */
        case 10:
            if (temp_t0 != 2) {
                func_80291198();
            }
            break;
        case 11:
            if (temp_t0 == 0) {
                func_802911C4();
                func_80291198();
            } else if (temp_t0 == 3) {
                func_802911C4();
            }
            break;
        case 12:
            if ((temp_t0 == 0) || (temp_t0 == 3)) {
                func_802911C4();
            }
            break;
        case 13:
        case 14:
            if (temp_t0 != 1) {
                case 15:
                case 16:
                    func_802911C4();
            }
            break;
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07003508))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07003240))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070014A0))));
    render_course_segments(mario_raceway_dls, arg0);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000450))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000240))));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070000E0))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000160))));
}
void CourseMarioRaceway::Collision() {}
void CourseMarioRaceway::Destroy() { }
