#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "KalimariDesert.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "kalimari_desert_data.h"
#include "engine/vehicles/Utils.h"

#include "engine/vehicles/Vehicle.h"

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
    #include "collision.h"
    #include "memory.h"
    extern const char *kalimari_desert_dls[];
}

KalimariDesert::KalimariDesert() {
    this->vtx = d_course_kalimari_desert_vertex;
    this->gfx = d_course_kalimari_desert_packed_dls;
    this->gfxSize = 5328;
    this->textures = kalimari_desert_textures;
    Props.MinimapTexture = gTextureCourseOutlineKalimariDesert;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 96;

    Props.Name = "kalimari desert";
    Props.DebugName = "desert";
    Props.CourseLength = "753m";
    Props.AIBehaviour = D_0D009260;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 10.0f;
    Props.FarPersp = 7000.0f;

    Props.PathSizes = {0x2BC, 1, 1, 1, 0x226, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_kalimari_desert_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_kalimari_desert_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gKalimariDesertClouds;
    Props.CloudList = gKalimariDesertClouds;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {195, 231, 255};
    Props.Skybox.BottomRight = {255, 192, 0};
    Props.Skybox.BottomLeft = {255, 192, 0};
    Props.Skybox.TopLeft = {195, 231, 255};
    Props.Skybox.FloorTopRight = {255, 192, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 192, 0};
}

void KalimariDesert::LoadTextures() {
    dma_textures(gTextureCactus1Left, 0x0000033EU, 0x00000800U);
    dma_textures(gTextureCactus1Right, 0x000002FBU, 0x00000800U);
    dma_textures(gTextureCactus2Left, 0x000002A8U, 0x00000800U);
    dma_textures(gTextureCactus2Right, 0x00000374U, 0x00000800U);
    dma_textures(gTextureCactus3, 0x000003AFU, 0x00000800U);
}

void KalimariDesert::SpawnActors() {
    struct RailroadCrossing* rrxing;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };

    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_kalimari_desert_cactus_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_kalimari_desert_item_box_spawns));

    Vec3f crossingPos = {-2500, 2, 2355};
    Vec3f crossingPos2 = {-1639, 2, 68};
    uintptr_t* crossing1 = (uintptr_t*) gWorldInstance.AddCrossing(crossingPos, 306, 310, 900.0f, 650.0f);
    uintptr_t* crossing2 = (uintptr_t*) gWorldInstance.AddCrossing(crossingPos2, 176, 182, 900.0f, 650.0f);

    vec3f_set(position, -1680.0f, 2.0f, 35.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) &gActorList[add_actor_to_empty_slot(position, rotation, velocity,
                                                                            ACTOR_RAILROAD_CROSSING)];
    rrxing->crossingTrigger = crossing2;
    vec3f_set(position, -1600.0f, 2.0f, 35.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) &gActorList[add_actor_to_empty_slot(position, rotation, velocity,
                                                                            ACTOR_RAILROAD_CROSSING)];
    rrxing->crossingTrigger = crossing2;
    vec3s_set(rotation, 0, -0x2000, 0);
    vec3f_set(position, -2459.0f, 2.0f, 2263.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) &gActorList[add_actor_to_empty_slot(position, rotation, velocity,
                                                                            ACTOR_RAILROAD_CROSSING)];
    rrxing->crossingTrigger = crossing1;
    vec3f_set(position, -2467.0f, 2.0f, 2375.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) &gActorList[add_actor_to_empty_slot(position, rotation, velocity,
                                                                            ACTOR_RAILROAD_CROSSING)];
    rrxing->crossingTrigger = crossing1;
}

void KalimariDesert::Init() {}

// Likely sets minimap boundaries
void KalimariDesert::MinimapSettings() {
    D_8018D2C0[0] = 263;
    D_8018D2D8[0] = 165;
    D_8018D220 = reinterpret_cast<uint8_t (*)[1024]>(dma_textures(gTextureExhaust5, 0x443, 0x1000));
    D_8018D2A0 = 0.015f;
    D_8018D2E0 = 55;
    D_8018D2E8 = 27;
}

void KalimariDesert::InitCourseObjects() {
    size_t i;
    if (gGamestate != CREDITS_SEQUENCE) {
        find_unused_obj_index(&D_8018CF10);
        init_object(D_8018CF10, 0);
        for (i = 0; i < 50; i++) {
            find_unused_obj_index(&gObjectParticle1[i]);
        }
        for (i = 0; i < 5; i++) {
            find_unused_obj_index(&gObjectParticle2[i]);
        }
        for (i = 0; i < 32; i++) {
            find_unused_obj_index(&gObjectParticle3[i]);
        }
    }
}

void KalimariDesert::SomeSounds() {}

void KalimariDesert::WhatDoesThisDo(Player* player, int8_t playerId) {}

void KalimariDesert::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void KalimariDesert::SpawnBombKarts() {
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(140, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(165, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(330, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(550, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(595, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void KalimariDesert::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void KalimariDesert::SetStaffGhost() {}

void KalimariDesert::SpawnVehicles() {
    generate_train_waypoints();

    s32 numTrains = 2;
    s32 centerWaypoint = 160;

    // Spawn two trains
    for (size_t i = 0; i < numTrains; ++i) {
        uint32_t waypoint = CalculateWaypointDistribution(i, numTrains, gVehicle2DWaypointLength, centerWaypoint);
        
        gWorldInstance.AddTrain(5, 5.0f, waypoint);
    }
}

void KalimariDesert::BeginPlay() {  }
void KalimariDesert::Render(struct UnkStruct_800DC5EC* arg0) {
    func_802B5D64(D_800DC610, D_802B87D4, 0, 1);

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_kalimari_desert_packed_dl_71C8
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x070071C8)));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_course_segments(kalimari_desert_dls, arg0);
    // d_course_kalimari_desert_packed_dl_1ED8
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07001ED8)));
    // d_course_kalimari_desert_packed_dl_1B18
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07001B18)));
    // d_course_kalimari_desert_packed_dl_8330
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07008330)));
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_kalimari_desert_packed_dl_998
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07000998)));
    // d_course_kalimari_desert_packed_dl_270
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07000270)));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void KalimariDesert::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_kalimari_desert_dl_22E00));
}

void KalimariDesert::Collision() {}

void KalimariDesert::GenerateCollision() {
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_kalimari_desert_addr));
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void KalimariDesert::Destroy() { }
