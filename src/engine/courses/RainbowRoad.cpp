#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "RainbowRoad.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/ChainChomp.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/rainbow_road/rainbow_road_data.h"
#include "assets/other/tracks/rainbow_road/rainbow_road_data.h"

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
    #include "assets/models/common_data.h"
    #include "save.h"
    #include "replays.h"
    #include "actors.h"
    #include "collision.h"
    #include "memory.h"
    #include "course.h"
    extern const char *rainbow_road_dls[48];
}

RainbowRoad::RainbowRoad() {
    Props.Minimap.Texture = minimap_rainbow_road;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 261;
    Props.Minimap.Pos[0].Y = 166;
    Props.Minimap.PlayerX = 39;
    Props.Minimap.PlayerY = 55;
    Props.Minimap.PlayerScaleFactor = 0.0103f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "rainbow road", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "rainbow", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "2000m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D0092C8;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.4f;
    Props.AISteeringSensitivity = 38;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

    Props.PathSizes = {0x76C, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    Props.CurveTargetSpeed[0] = 4.1666665f;
    Props.CurveTargetSpeed[1] = 5.5833334f;
    Props.CurveTargetSpeed[2] = 6.1666665f;
    Props.CurveTargetSpeed[3] = 6.75f;

    Props.NormalTargetSpeed[0] = 3.75f;
    Props.NormalTargetSpeed[1] = 5.1666665f;
    Props.NormalTargetSpeed[2] = 5.75f;
    Props.NormalTargetSpeed[3] = 6.3333334f;

    Props.D_0D0096B8[0] = 3.3333332f;
    Props.D_0D0096B8[1] = 3.9166667f;
    Props.D_0D0096B8[2] = 4.5f;
    Props.D_0D0096B8[3] = 5.0833334f;

    Props.OffTrackTargetSpeed[0] = 3.75f;
    Props.OffTrackTargetSpeed[1] = 5.1666665f;
    Props.OffTrackTargetSpeed[2] = 5.75f;
    Props.OffTrackTargetSpeed[3] = 6.3333334f;

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_rainbow_road_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_rainbow_road_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gToadsTurnpikeRainbowRoadStars;
    Props.CloudList = gToadsTurnpikeRainbowRoadStars;

    Props.Skybox.TopRight = {0, 0, 0};
    Props.Skybox.BottomRight = {0, 0, 0};
    Props.Skybox.BottomLeft = {0, 0, 0};
    Props.Skybox.TopLeft = {0, 0, 0};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_RAINBOW_ROAD;
    
    Props.WaterLevel = 0.0f;
}

void RainbowRoad::Load() {
    Course::Load();
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(rainbow_road_dls); i++) {
            InvertTriangleWindingByName(rainbow_road_dls[i]);
        }
    }
    D_800DC5C8 = 1;
    parse_course_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_rainbow_road_addr));
    func_80295C6C();
    // d_course_rainbow_road_packed_dl_2068
    find_vtx_and_set_colours((Gfx*) d_course_rainbow_road_packed_dl_2068, 150, 255, 255, 255);
    // d_course_rainbow_road_packed_dl_1E18
    find_vtx_and_set_colours((Gfx*) d_course_rainbow_road_packed_dl_1E18, 150, 255, 255, 255);
    // d_course_rainbow_road_packed_dl_1318
    find_vtx_and_set_colours((Gfx*) d_course_rainbow_road_packed_dl_1318, 255, 255, 255, 0);
    if (gGamestate != CREDITS_SEQUENCE) {
        // d_course_rainbow_road_packed_dl_1FB8
        find_vtx_and_set_colours((Gfx*) d_course_rainbow_road_packed_dl_1FB8, 150, 255, 255, 255);
    }
}

void RainbowRoad::UnLoad() {
    RestoreTriangleWinding();
}

void RainbowRoad::BeginPlay() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_rainbow_road_item_box_spawns));

    if (gGamestate != CREDITS_SEQUENCE) {
        gWorldInstance.AddObject(new OChainChomp());
        gWorldInstance.AddObject(new OChainChomp());
        gWorldInstance.AddObject(new OChainChomp());
    }

    if (gModeSelection == VERSUS) {
        OBombKart::Spawn(0, 50, 3, 0.8333333f);
        OBombKart::Spawn(0, 100, 1, 0.8333333f);
        OBombKart::Spawn(0, 150, 3, 0.8333333f);
        OBombKart::Spawn(0, 200, 1, 0.8333333f);
        OBombKart::Spawn(0, 250, 3, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
    }
}

void RainbowRoad::InitClouds() {
    init_stars(this->Props.Clouds);
}

void RainbowRoad::UpdateClouds(s32 sp1C, Camera* camera) {
    update_stars(sp1C, camera, this->Props.CloudList);
}

void RainbowRoad::InitCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        size_t i;
        for (i = 0; i < NUM_NEON_SIGNS; i++) {
            init_object(indexObjectList1[i], 0);
        }
    }
}

void RainbowRoad::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        update_neon();
        //update_chain_chomps();
    }
}

void RainbowRoad::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        render_object_neon(cameraId);
        //render_object_chain_chomps(cameraId);
    }
}

void RainbowRoad::SomeSounds() {
}

void RainbowRoad::WhatDoesThisDo(Player* player, int8_t playerId) {}

void RainbowRoad::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

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

void RainbowRoad::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = gCopyNearestWaypointByPlayerId[playerId];
}

void RainbowRoad::DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) {

    gDPPipeSync(gDisplayListHead++);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    render_course_segments(rainbow_road_dls, screen);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    gDPPipeSync(gDisplayListHead++);
}

void RainbowRoad::CreditsSpawnActors() {
    // d_course_rainbow_road_packed_dl_2068
    find_vtx_and_set_colours((Gfx*) d_course_rainbow_road_packed_dl_2068, 150, 0xFF, 0xFF, 0xFF);
    // d_course_rainbow_road_packed_dl_1E18
    find_vtx_and_set_colours((Gfx*) d_course_rainbow_road_packed_dl_1E18, 150, 0xFF, 0xFF, 0xFF);
    // d_course_rainbow_road_packed_dl_1318
    find_vtx_and_set_colours((Gfx*) d_course_rainbow_road_packed_dl_1318, 255, 0xFF, 0xFF, 0);
}

void RainbowRoad::Destroy() {}
