#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "MarioRaceway.h"
#include "World.h"
#include "engine/actors/AFinishline.h"
#include "engine/objects/Object.h"
#include "engine/objects/BombKart.h"

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
    #include "collision.h"
    #include "memory.h"
    #include "courses/staff_ghost_data.h"
    extern const char *mario_raceway_dls[];
}

const course_texture mario_raceway_textures[] = {
    { gTextureCheckerboardYellowPink, 0x0149, 0x0800, 0x0 },
    { gTexture64619C, 0x0124, 0x0800, 0x0 },
    { gTextureGrass1, 0x0125, 0x0800, 0x0 },
    { gTexture64BB60, 0x0169, 0x0800, 0x0 },
    { gTextureGrass7, 0x05DE, 0x0800, 0x0 },
    { gTextureGrass5, 0x023F, 0x0800, 0x0 },
    { gTextureFlagRed, 0x019E, 0x0800, 0x0 },
    { gTexture663F90, 0x0122, 0x0800, 0x0 },
    { gTexture6642A4, 0x0162, 0x0800, 0x0 },
    { gTexture6640B4, 0x01EF, 0x0800, 0x0 },
    { gTextureGrass10, 0x01F8, 0x0800, 0x0 },
    { gTexture6684F8, 0x010D, 0x0800, 0x0 },
    { gTextureSignLuigis0, 0x0287, 0x1000, 0x0 },
    { gTextureSignLuigis1, 0x02AF, 0x1000, 0x0 },
    { gTextureSignMarioStar0, 0x02D2, 0x1000, 0x0 },
    { gTextureSignMarioStar1, 0x02B1, 0x1000, 0x0 },
    { gTexture66C8F4, 0x01A1, 0x0800, 0x0 },
    { gTextureSignNintendoRed0, 0x02A6, 0x1000, 0x0 },
    { gTextureSignNintendoRed1, 0x02F7, 0x1000, 0x0 },
    { gTexture670AC8, 0x0FBF, 0x1000, 0x0 },
    { gTexture674354, 0x046F, 0x0800, 0x0 },
    { gTextureRoad0, 0x0300, 0x1000, 0x0 },
    { gTextureRoadFinish0, 0x0338, 0x1000, 0x0 },
    { gTexture67B9B0, 0x0225, 0x0800, 0x0 },
    { gTextureSignYoshi, 0x04DF, 0x1000, 0x0 },
    { gTextureCheckerboardBlueGray, 0x04A1, 0x1000, 0x0 },
    { gTextureSignShellShot0, 0x038C, 0x1000, 0x0 },
    { gTextureSignShellShot1, 0x0247, 0x1000, 0x0 },
    { gTextureSignKoopaAir0, 0x0360, 0x1000, 0x0 },
    { gTextureSignKoopaAir1, 0x0304, 0x1000, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

MarioRaceway::MarioRaceway() {
    this->vtx = d_course_mario_raceway_vertex;
    this->gfx = d_course_mario_raceway_packed_dls;
    this->gfxSize = 3367;
    Props.textures = mario_raceway_textures;
    Props.MinimapTexture = gTextureCourseOutlineMarioRaceway;
    Props.MinimapDimensions = IVector2D(ResourceGetTexWidthByName(Props.MinimapTexture), ResourceGetTexHeightByName(Props.MinimapTexture));

    Props.Id = "mk:mario_raceway";
    Props.Name = "Mario Raceway";
    Props.DebugName = "m circuit";
    Props.CourseLength = "567m";

    Props.AIBehaviour = D_0D008F28;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.SomePtr = D_800DCB34;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {600, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    Props.D_0D009418[0] = 4.1666665f;
    Props.D_0D009418[1] = 5.5833334f;
    Props.D_0D009418[2] = 6.1666665f;
    Props.D_0D009418[3] = 6.75f;

    Props.D_0D009568[0] = 3.75f;
    Props.D_0D009568[1] = 5.1666665f;
    Props.D_0D009568[2] = 5.75f;
    Props.D_0D009568[3] = 6.3333334f;

    Props.D_0D0096B8[0] = 3.3333332f;
    Props.D_0D0096B8[1] = 3.9166667f;
    Props.D_0D0096B8[2] = 4.5f;
    Props.D_0D0096B8[3] = 5.0833334f;

    Props.D_0D009808[0] = 3.75f;
    Props.D_0D009808[1] = 5.1666665f;
    Props.D_0D009808[2] = 5.75f;
    Props.D_0D009808[3] = 6.3333334f;

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_mario_raceway_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_mario_raceway_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gKalimariDesertClouds;
    Props.CloudList = gLuigiRacewayClouds;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {0, 184, 248};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {0, 184, 248};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_RACEWAYS_WARIO_STADIUM;
}

void MarioRaceway::Load() {
    Course::Load();

    generate_collision_mesh_with_defaults(segmented_gfx_to_virtual((void*)0x07001140));
    
    // Generate pipe collision mesh
    if (gScreenModeSelection == SCREEN_MODE_1P) {
        // d_course_mario_raceway_packed_dl_8E8
        generate_collision_mesh_with_defaults(segmented_gfx_to_virtual((void*)0x070008E8));
    } else {
        if (CVarGetInteger("gDisableLod", 1) == true) {
            generate_collision_mesh_with_defaults(segmented_gfx_to_virtual((void*)0x070008E8));
        } else {
            // d_course_mario_raceway_packed_dl_2D68
            generate_collision_mesh_with_defaults(segmented_gfx_to_virtual((void*)0x07002D68));
        }
    }

    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_mario_raceway_addr));
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void MarioRaceway::LoadTextures() {
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

void MarioRaceway::BeginPlay() {
    struct Actor* actor;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };

    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_tree_spawns));
    spawn_piranha_plants((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_piranha_plant_spawns));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_item_box_spawns));
    vec3f_set(position, 150.0f, 40.0f, -1300.0f);
    position[0] *= gCourseDirection;
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_MARIO_SIGN);
    vec3f_set(position, 2520.0f, 0.0f, 1240.0f);
    position[0] *= gCourseDirection;
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_MARIO_SIGN);

    if (gModeSelection == VERSUS) {
        Vec3f pos = {0, 0, 0};
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][40], 40, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][100], 100, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][265], 265, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][285], 285, 1, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][420], 420, 1, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f));
    }
}

// Likely sets minimap boundaries
void MarioRaceway::MinimapSettings() {
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

void MarioRaceway::InitCourseObjects() {
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

void MarioRaceway::SomeSounds() {
    vec3f_set(D_8015F748, -223.0f, 94.0f, -155.0f);
    func_800C9D80(D_8015F748, D_802B91C8, 0x5103700B);
}

void MarioRaceway::WhatDoesThisDo(Player* player, int8_t playerId) {
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

void MarioRaceway::WhatDoesThisDoAI(Player* player, int8_t playerId) {
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
void MarioRaceway::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void MarioRaceway::SetStaffGhost() {
    u32 temp_v0 = func_800B4E24(0) & 0xfffff;
    if (temp_v0 <= 9000) {
        D_80162DD6 = 0;
        D_80162DF4 = 0;
    } else {
        D_80162DD6 = 1;
        D_80162DF4 = 1;
    }
    D_80162DC4 = d_mario_raceway_staff_ghost;
    D_80162DE4 = 0;
}

void MarioRaceway::Render(struct UnkStruct_800DC5EC* arg0) {
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
                render_mario_raceway_pipe();
            }
            break;
        case 3:
            if (temp_t0 != 0) {
                render_mario_raceway_pipe();
            }
            break;
        case 4:
            if (temp_t0 == 0) {
                func_80291198();
            } else {
                if (temp_t0 == 1) {
                    func_80291198();
                }
                render_mario_raceway_pipe();
            }
            break;
        case 5:
        case 6:
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                render_mario_raceway_pipe();
            } else {
                func_80291198();
            }
            break;
        case 7:
            func_80291198();
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                render_mario_raceway_pipe();
            }
            break;
        case 8:
        case 9:
            if (temp_t0 != 1) {
                render_mario_raceway_pipe();
            }
            /* fallthrough */
        case 10:
            if (temp_t0 != 2) {
                func_80291198();
            }
            break;
        case 11:
            if (temp_t0 == 0) {
                render_mario_raceway_pipe();
                func_80291198();
            } else if (temp_t0 == 3) {
                render_mario_raceway_pipe();
            }
            break;
        case 12:
            if ((temp_t0 == 0) || (temp_t0 == 3)) {
                render_mario_raceway_pipe();
            }
            break;
        case 13:
        case 14:
            if (temp_t0 != 1) {
                case 15:
                case 16:
                    render_mario_raceway_pipe();
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

void MarioRaceway::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_mario_raceway_dl_9348));
}

void MarioRaceway::Collision() {}

void MarioRaceway::CreditsSpawnActors() {
    dma_textures(gTextureTrees1, 0x35B, 0x800);
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_tree_spawns));
}

void MarioRaceway::Destroy() { }
