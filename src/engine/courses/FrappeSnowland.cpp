#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "FrappeSnowland.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/frappe_snowland_data.h"
#include "assets/boo_frames.h"

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
    #include "update_objects.h"
    extern const char *d_course_frappe_snowland_dl_list[];
    extern s8 gPlayerCount;
}

FrappeSnowland::FrappeSnowland() {
    this->vtx = d_course_frappe_snowland_vertex;
    this->gfx = d_course_frappe_snowland_packed_dls;
    this->gfxSize = 4140;
    this->textures = frappe_snowland_textures;
    Props.MinimapTexture = gTextureCourseOutlineFrappeSnowland;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 64;

    Props.Name = "frappe snowland";
    Props.DebugName = "snow";
    Props.CourseLength = "734m";
    Props.AIBehaviour = D_0D0090F8;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x2EE, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_frappe_snowland_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_frappe_snowland_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // not used for frappe
    Props.CloudList = NULL;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {28, 11, 90};
    Props.Skybox.BottomRight = {0, 99, 164};
    Props.Skybox.BottomLeft = {0, 9, 164};
    Props.Skybox.TopLeft = {28, 11, 90};
    Props.Skybox.FloorTopRight = {0, 99, 164};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 99, 164};
}

void FrappeSnowland::LoadTextures() {
    dma_textures(gTextureFrappeSnowlandTreeLeft, 0x00000454U, 0x00000800U);
    dma_textures(gTextureFrappeSnowlandTreeRight, 0x00000432U, 0x00000800U);
}

void FrappeSnowland::SpawnActors() {
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_frappe_snowland_tree_spawns));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_frappe_snowland_item_box_spawns));
}

void FrappeSnowland::Init() {  }
void FrappeSnowland::InitClouds() {
    s32 var_s0;
    s32 var_s4;

    if (gPlayerCount == 1) {
        var_s4 = 0x32;
    } else {
        var_s4 = 0x19;
    }
    for (var_s0 = 0; var_s0 < var_s4; var_s0++) {
        find_unused_obj_index(&D_8018CC80[D_8018D1F8 + var_s0]);
    }
    D_8018D1F8 += var_s0;
    D_8018D1F0 = var_s0;
}

void FrappeSnowland::UpdateClouds(s32 sp1C, Camera* camera) {
    func_80078170(sp1C, camera);
}

// Likely sets minimap boundaries
void FrappeSnowland::MinimapSettings() {
    D_8018D2C0[0] = 262;
    D_8018D2A0 = 0.016f;
    D_8018D2E0 = 36;
    D_8018D2E8 = 40;
    D_8018D300 = 72;
    D_8018D308 = 100;
    D_8018D310 = 255;
}

void FrappeSnowland::InitCourseObjects() {
    size_t objectId;
    size_t i;
    for (i = 0; i < NUM_SNOWFLAKES; i++) {
        find_unused_obj_index(&gObjectParticle1[i]);
    }
    if (gGamestate != CREDITS_SEQUENCE) {
        for (i = 0; i < NUM_SNOWMEN; i++) {
            objectId = indexObjectList2[i];
            init_object(objectId, 0);
            gObjectList[objectId].origin_pos[0] = gSnowmanSpawns[i].pos[0] * xOrientation;
            gObjectList[objectId].origin_pos[1] = gSnowmanSpawns[i].pos[1] + 5.0 + 3.0;
            gObjectList[objectId].origin_pos[2] = gSnowmanSpawns[i].pos[2];
            objectId = indexObjectList1[i];
            init_object(objectId, 0);
            gObjectList[objectId].origin_pos[0] = gSnowmanSpawns[i].pos[0] * xOrientation;
            gObjectList[objectId].origin_pos[1] = gSnowmanSpawns[i].pos[1] + 3.0;
            gObjectList[objectId].origin_pos[2] = gSnowmanSpawns[i].pos[2];
            gObjectList[objectId].unk_0D5 = gSnowmanSpawns[i].unk_6;
        }
    }
}

void FrappeSnowland::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        update_snowmen();
    }
    update_snowflakes();
}

void FrappeSnowland::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        render_object_snowmans(cameraId);
    }
}

void FrappeSnowland::SomeSounds() {
}

void FrappeSnowland::WhatDoesThisDo(Player* player, int8_t playerId) {}

void FrappeSnowland::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void FrappeSnowland::SpawnBombKarts() {
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(140, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(165, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(330, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(550, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(595, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void FrappeSnowland::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void FrappeSnowland::SetStaffGhost() {
}

void FrappeSnowland::BeginPlay() {  }

void FrappeSnowland::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_frappe_snowland_packed_dl_65E0
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070065E0)));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_course_segments(d_course_frappe_snowland_dl_list, arg0);
}

void FrappeSnowland::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_frappe_snowland_dl_76A0));
}

void FrappeSnowland::Collision() {}

void FrappeSnowland::GenerateCollision() {
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_frappe_snowland_addr));
    func_80295C6C();
    D_8015F8E4 = -50.0f;
}

void FrappeSnowland::Waypoints(Player* player, int8_t playerId) {
    s16 waypoint = gNearestWaypointByPlayerId[playerId];

    if ((waypoint >= 0xF0) && (waypoint < 0x105)) {
        player->nearestWaypointId = 0xF0U;
    } else {
        player->nearestWaypointId = gCopyNearestWaypointByPlayerId[playerId];
        if (player->nearestWaypointId < 0) {
            player->nearestWaypointId = gWaypointCountByPathIndex[0] + player->nearestWaypointId;
        }
    }
}

void FrappeSnowland::Water() {}

void FrappeSnowland::Destroy() {}
