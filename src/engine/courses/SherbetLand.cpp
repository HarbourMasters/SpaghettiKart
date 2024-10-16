#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "SherbetLand.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/sherbet_land_data.h"

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
    extern const char *sherbet_land_dls[];
}

SherbetLand::SherbetLand() {
    this->vtx = d_course_sherbet_land_vertex;
    this->gfx = d_course_sherbet_land_packed_dls;
    this->gfxSize = 1803;
    this->textures = sherbet_land_textures;
    Props.MinimapTexture = gTextureCourseOutlineSherbetLand;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 64;

    Props.Name = "sherbet land";
    Props.DebugName = "sherbet";
    Props.CourseLength = "756m";
    Props.AIBehaviour = D_0D009280;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x2BC, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_sherbet_land_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_sherbet_land_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gSherbetLandClouds;
    Props.CloudList = gSherbetLandClouds;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {128, 184, 248};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {128, 184, 248};
    Props.Skybox.FloorTopRight = {216, 232, 248};
    Props.Skybox.FloorBottomRight = {128, 184, 248};
    Props.Skybox.FloorBottomLeft = {128, 184, 248};
    Props.Skybox.FloorTopLeft = {216, 232, 248};
}

void SherbetLand::LoadTextures() {
}

void SherbetLand::SpawnActors() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_sherbet_land_item_box_spawns));
}

// Likely sets minimap boundaries
void SherbetLand::MinimapSettings() {
    D_8018D220 = reinterpret_cast<uint8_t (*)[1024]>(dma_textures(gTextureExhaust1, 0x485, 0xC00));
    D_8018D2A0 = 0.015f;
    D_8018D2C0[0] = 262;
    D_8018D2E0 = 52;
    D_8018D2E8 = 33;
    D_8018D300 = 72;
    D_8018D308 = 100;
    D_8018D310 = 255;
}

void SherbetLand::InitCourseObjects() {
    for (size_t i = 0; i < NUM_PENGUINS; i++) {
        init_object(indexObjectList1[i], 0);
    }
}

void SherbetLand::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        func_800842C8();
    }
    update_penguins();
}

void SherbetLand::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        func_80052E30(cameraId);
    }
    render_object_train_penguins(cameraId);
}

void SherbetLand::SomeSounds() {
}

void SherbetLand::WhatDoesThisDo(Player* player, int8_t playerId) {}

void SherbetLand::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void SherbetLand::SpawnBombKarts() {
    gWorldInstance.AddObject(std::make_unique<OBombKart>(40, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(100, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(265, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(285, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(420, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void SherbetLand::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void SherbetLand::SetStaffGhost() {}

void SherbetLand::BeginPlay() {  }
void SherbetLand::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_course_segments(sherbet_land_dls, arg0);
}

void SherbetLand::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_sherbet_land_dl_9AE8));
}

void SherbetLand::Collision() {}

void SherbetLand::GenerateCollision() {
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_sherbet_land_addr));
    func_80295C6C();
    D_8015F8E4 = -18.0f;
    // d_course_sherbet_land_packed_dl_1EB8
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07001EB8), -0x4C, 255, 255, 255);
    // d_course_sherbet_land_packed_dl_2308
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07002308), -0x6A, 255, 255, 255);
}

void SherbetLand::Destroy() { }
