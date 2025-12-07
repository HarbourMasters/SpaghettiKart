#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "Skyscraper.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/skyscraper/skyscraper_data.h"
#include "assets/other/tracks/skyscraper/skyscraper_data.h"

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
    extern s16 currentScreenSection;
}

Skyscraper::Skyscraper() {
    Props.Minimap.Texture = minimap_skyscraper;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 257;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 32;
    Props.Minimap.PlayerY = 32;
    Props.Minimap.PlayerScaleFactor = 0.0445f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "skyscraper", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "skyscraper", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D008F18;
    Props.AIMaximumSeparation = -1.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

    Props.PathSizes = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = NULL;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = NULL;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;

    Props.Skybox.TopRight = {0, 0, 0};
    Props.Skybox.BottomRight = {0, 0, 0};
    Props.Skybox.BottomLeft = {0, 0, 0};
    Props.Skybox.TopLeft = {0, 0, 0};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_BATTLE_ARENAS;

    Props.WaterLevel = -480.0f;
}

void Skyscraper::Load() {
    Course::Load();
    if (gIsMirrorMode != 0) {
        InvertTriangleWindingByName(d_course_skyscraper_packed_dl_FE8);
        InvertTriangleWindingByName(d_course_skyscraper_packed_dl_C60);
        InvertTriangleWindingByName(d_course_skyscraper_packed_dl_B70);
        InvertTriangleWindingByName(d_course_skyscraper_packed_dl_6B8);
        InvertTriangleWindingByName(d_course_skyscraper_packed_dl_570);
        InvertTriangleWindingByName(d_course_skyscraper_packed_dl_10C8);
        InvertTriangleWindingByName(d_course_skyscraper_packed_dl_258);
    }
    // d_course_skyscraper_packed_dl_1110
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_skyscraper_packed_dl_1110, 1);
    // d_course_skyscraper_packed_dl_258
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_skyscraper_packed_dl_258, 1);
    func_80295C6C();
}

void Skyscraper::UnLoad() {
}

void Skyscraper::BeginPlay() {
    spawn_all_item_boxes((ActorSpawnData*)LOAD_ASSET_RAW(d_course_skyscraper_item_box_spawns));

    if (gModeSelection == VERSUS) {
        OBombKart::Spawn(0, 20, 0, 1.0f);
        OBombKart::Spawn(0, 40, 0, 1.0f);
        OBombKart::Spawn(0, 60, 0, 1.0f);
        OBombKart::Spawn(0, 80, 0, 1.0f);
        OBombKart::Spawn(0, 100, 0, 1.0f);
        OBombKart::Spawn(0, 120, 0, 1.0f);
        OBombKart::Spawn(0, 140, 0, 1.0f);
    }
}

void Skyscraper::InitCourseObjects() {}

void Skyscraper::SomeSounds() {}

void Skyscraper::WhatDoesThisDo(Player* player, int8_t playerId) {}

void Skyscraper::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void Skyscraper::Render(struct UnkStruct_800DC5EC* arg0) {
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_skyscraper_packed_dl_FE8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_skyscraper_packed_dl_FE8);
    // d_course_skyscraper_packed_dl_C60
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_skyscraper_packed_dl_C60);
    // d_course_skyscraper_packed_dl_B70
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_skyscraper_packed_dl_B70);
    // d_course_skyscraper_packed_dl_6B8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_skyscraper_packed_dl_6B8);
    // d_course_skyscraper_packed_dl_570
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_skyscraper_packed_dl_570);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_10C8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_skyscraper_packed_dl_10C8);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_258
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_skyscraper_packed_dl_258);
}

void Skyscraper::RenderCredits() {}

void Skyscraper::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = 0;
}

void Skyscraper::Destroy() { }
