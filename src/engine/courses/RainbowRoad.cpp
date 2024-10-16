#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "RainbowRoad.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/rainbow_road_data.h"

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
    extern const char *rainbow_road_dls[];
}

RainbowRoad::RainbowRoad() {
    this->vtx = d_course_rainbow_road_vertex;
    this->gfx = d_course_rainbow_road_packed_dls;
    this->gfxSize = 5670;
    this->textures = rainbow_road_textures;
    Props.MinimapTexture = gTextureCourseOutlineRainbowRoad;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 96;

    Props.Name = "rainbow road";
    Props.DebugName = "rainbow";
    Props.CourseLength = "2000m";
    Props.AIBehaviour = D_0D0092C8;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.4f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 38;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

    Props.PathSizes = {0x76C, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_rainbow_road_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_rainbow_road_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gToadsTurnpikeRainbowRoadStars;
    Props.CloudList = gToadsTurnpikeRainbowRoadStars;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {0, 0, 0};
    Props.Skybox.BottomRight = {0, 0, 0};
    Props.Skybox.BottomLeft = {0, 0, 0};
    Props.Skybox.TopLeft = {0, 0, 0};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
}

void RainbowRoad::LoadTextures() {
}

void RainbowRoad::SpawnActors() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_rainbow_road_item_box_spawns));
}

void RainbowRoad::Init() {}

void RainbowRoad::InitClouds() {
    init_stars(this->Props.Clouds);
}

void RainbowRoad::UpdateClouds(s32 sp1C, Camera* camera) {
    update_stars(sp1C, camera, this->Props.CloudList);
}

// Likely sets minimap boundaries
void RainbowRoad::MinimapSettings() {
    D_8018D2A0 = 0.0103f;
    D_8018D2C0[0] = 261;
    D_8018D2D8[0] = 166;
    D_8018D2E0 = 39;
    D_8018D2E8 = 55;
}

void RainbowRoad::InitCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        size_t i;
        for (i = 0; i < NUM_NEON_SIGNS; i++) {
            init_object(indexObjectList1[i], 0);
        }
        for (i = 0; i < NUM_CHAIN_CHOMPS; i++) {
            init_object(indexObjectList2[i], 0);
        }
    }
}

void RainbowRoad::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        update_neon();
        update_chain_chomps();
    }
}

void RainbowRoad::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        render_object_neon(cameraId);
        render_object_chain_chomps(cameraId);
    }
}

void RainbowRoad::SomeSounds() {
}

void RainbowRoad::WhatDoesThisDo(Player* player, int8_t playerId) {}

void RainbowRoad::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void RainbowRoad::SpawnBombKarts() {
    gWorldInstance.AddObject(std::make_unique<OBombKart>(40, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(100, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(265, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(285, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(420, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void RainbowRoad::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void RainbowRoad::SetStaffGhost() {}

void RainbowRoad::BeginPlay() {}
void RainbowRoad::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

void RainbowRoad::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_rainbow_road_dl_16220));
}

void RainbowRoad::Collision() {}

void RainbowRoad::GenerateCollision() {
    D_800DC5C8 = 1;
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_rainbow_road_addr));
    func_80295C6C();
    D_8015F8E4 = 0.0f;
    // d_course_rainbow_road_packed_dl_2068
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07002068), -0x6A, 255, 255, 255);
    // d_course_rainbow_road_packed_dl_1E18
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07001E18), -0x6A, 255, 255, 255);
    // d_course_rainbow_road_packed_dl_1318
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07001318), 255, 255, 255, 0);
    if (gGamestate != CREDITS_SEQUENCE) {
        // d_course_rainbow_road_packed_dl_1FB8
        find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07001FB8), -0x6A, 255, 255, 255);
    }
}

void RainbowRoad::Waypoints(Player* player, int8_t playerId) {
    player->nearestWaypointId = gCopyNearestWaypointByPlayerId[playerId];
}

void RainbowRoad::Destroy() {}
