#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "DoubleDeck.h"
#include "World.h"
#include "align_asset_macro.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/double_deck/double_deck_data.h"
#include "assets/other/tracks/double_deck/double_deck_data.h"

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

DoubleDeck::DoubleDeck() {
    Props.Minimap.Texture = minimap_double_deck;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 257;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 32;
    Props.Minimap.PlayerY = 32;
    Props.Minimap.PlayerScaleFactor = 0.0285f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "double deck", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "deck", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "", sizeof(Props.TrackLength));


    Props.AIBehaviour = D_0D008F18;
    Props.AIMaximumSeparation = -1.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 1500.0f;

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

    Props.Skybox.TopRight = {113, 70, 255};
    Props.Skybox.BottomRight = {255, 184, 99};
    Props.Skybox.BottomLeft = {255, 184, 99};
    Props.Skybox.TopLeft = {113, 70, 255};
    Props.Skybox.FloorTopRight = {255, 224, 240};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 224, 240};
    Props.Sequence = MusicSeq::MUSIC_SEQ_DOUBLE_DECK;
}

void DoubleDeck::Load() {
    Track::Load();
    if (gIsMirrorMode != 0) {
        InvertTriangleWindingByName(d_course_double_deck_packed_dl_738);
    }
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_double_deck_packed_dl_738, 1);
    func_80295C6C();
    Props.WaterLevel = gTrackMinY - 10.0f;
}

void DoubleDeck::BeginPlay() {
    spawn_all_item_boxes((ActorSpawnData*)LOAD_ASSET_RAW(d_course_double_deck_item_box_spawns));

    if (gModeSelection == VERSUS) {
        SpawnActor<OBombKart>(0, 20, 0, 1.0f);
        SpawnActor<OBombKart>(0, 40, 0, 1.0f);
        SpawnActor<OBombKart>(0, 60, 0, 1.0f);
        SpawnActor<OBombKart>(0, 80, 0, 1.0f);
        SpawnActor<OBombKart>(0, 100, 0, 1.0f);
        SpawnActor<OBombKart>(0, 120, 0, 1.0f);
        SpawnActor<OBombKart>(0, 140, 0, 1.0f);
    }
}

void DoubleDeck::InitTrackObjects() {}

void DoubleDeck::SomeSounds() {}

void DoubleDeck::WhatDoesThisDo(Player* player, int8_t playerId) {}

void DoubleDeck::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void DoubleDeck::Draw(ScreenContext* arg0) {
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_double_deck_packed_dl_738
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_double_deck_packed_dl_738);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void DoubleDeck::DrawCredits() {}

void DoubleDeck::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = 0;
}

void DoubleDeck::Destroy() { }
