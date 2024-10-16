#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "YoshiValley.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/yoshi_valley_data.h"
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
    extern const char *d_course_yoshi_valley_dl_list[];
}

YoshiValley::YoshiValley() {
    this->vtx = d_course_yoshi_valley_vertex;
    this->gfx = d_course_yoshi_valley_packed_dls;
    this->gfxSize = 4140;
    this->textures = yoshi_valley_textures;
    Props.MinimapTexture = gTextureCourseOutlineYoshiValley;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 64;

    Props.Name = "yoshi valley";
    Props.DebugName = "maze";
    Props.CourseLength = "772m";
    Props.AIBehaviour = D_0D0090B8;
    Props.AIMaximumSeparation = 35.0f;
    Props.AIMinimumSeparation = 0.0f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x02B2, 0x02A8, 0x02B2, 0x0320, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints);
    Props.PathTable[1] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints_2);
    Props.PathTable[2] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints_3);
    Props.PathTable[3] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints_4);

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints);
    Props.PathTable2[1] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints_2);
    Props.PathTable2[2] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints_3);
    Props.PathTable2[3] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints_4);

    Props.Clouds = gYoshiValleyMooMooFarmClouds;
    Props.CloudList = gYoshiValleyMooMooFarmClouds;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {113, 70, 255};
    Props.Skybox.BottomRight = {255, 184, 99};
    Props.Skybox.BottomLeft = {255, 184, 99};
    Props.Skybox.TopLeft = {113, 70, 255};
    Props.Skybox.FloorTopRight = {95, 40, 15};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {95, 40, 15};
}

void YoshiValley::LoadTextures() {
    dma_textures(gTextureTrees2, 0x000003E8U, 0x00000800U);
}

void YoshiValley::SpawnActors() {
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };

    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_yoshi_valley_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_yoshi_valley_item_box_spawns));
    vec3f_set(position, -2300.0f, 0.0f, 634.0f);
    position[0] *= gCourseDirection;
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_YOSHI_EGG);
}

// Likely sets minimap boundaries
void YoshiValley::MinimapSettings() {
    D_8018D220 = reinterpret_cast<uint8_t (*)[1024]>(dma_textures(gTextureExhaust0, 0x479, 0xC00));
    D_8018D2A0 = 0.018f;
    D_8018D2E0 = 61;
    D_8018D2E8 = 38;
}

void YoshiValley::InitCourseObjects() {
    size_t objectId;
    size_t i;
    for (i = 0; i < NUM_YV_FLAG_POLES; i++) {
        init_object(indexObjectList1[i], 0);
    }
    if (gGamestate != CREDITS_SEQUENCE) {
        for (i = 0; i < NUM_HEDGEHOGS; i++) {
            objectId = indexObjectList2[i];
            init_object(objectId, 0);
            gObjectList[objectId].pos[0] = gObjectList[objectId].origin_pos[0] =
                gHedgehogSpawns[i].pos[0] * xOrientation;
            gObjectList[objectId].pos[1] = gObjectList[objectId].surfaceHeight =
                gHedgehogSpawns[i].pos[1] + 6.0;
            gObjectList[objectId].pos[2] = gObjectList[objectId].origin_pos[2] = gHedgehogSpawns[i].pos[2];
            gObjectList[objectId].unk_0D5 = gHedgehogSpawns[i].unk_06;
            gObjectList[objectId].unk_09C = gHedgehogPatrolPoints[i][0] * xOrientation;
            gObjectList[objectId].unk_09E = gHedgehogPatrolPoints[i][2];
        }
    }
}

void YoshiValley::UpdateCourseObjects() {
    func_80083080();
    if (gGamestate != CREDITS_SEQUENCE) {
        update_hedgehogs();
    }
}

void YoshiValley::RenderCourseObjects(s32 cameraId) {
    func_80055228(cameraId);
    if (gGamestate != CREDITS_SEQUENCE) {
        render_object_hedgehogs(cameraId);
    }
}

void YoshiValley::SomeSounds() {
}

void YoshiValley::WhatDoesThisDo(Player* player, int8_t playerId) {}

void YoshiValley::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void YoshiValley::SpawnBombKarts() {
    gWorldInstance.AddObject(std::make_unique<OBombKart>(140, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(165, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(330, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(550, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(595, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void YoshiValley::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void YoshiValley::SetStaffGhost() {
}

void YoshiValley::BeginPlay() {  }

void YoshiValley::Render(struct UnkStruct_800DC5EC* arg0) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    render_course_segments(d_course_yoshi_valley_dl_list, arg0);
    gDPPipeSync(gDisplayListHead++);
}

void YoshiValley::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_yoshi_valley_dl_18020));
}

void YoshiValley::Collision() {}

void YoshiValley::GenerateCollision() {
    Lights1 lights4 = gdSPDefLights1(100, 100, 100, 255, 254, 254, 0, 0, 120);
    func_802B5D64(&lights4, -0x38F0, 0x1C70, 1);
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_yoshi_valley_addr));
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void YoshiValley::Waypoints(Player* player, int8_t playerId) {
    player->nearestWaypointId = gCopyNearestWaypointByPlayerId[playerId];
}

void YoshiValley::Water() {}

void YoshiValley::Destroy() {}
