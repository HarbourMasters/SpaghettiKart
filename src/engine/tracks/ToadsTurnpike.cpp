#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "ToadsTurnpike.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/toads_turnpike/toads_turnpike_data.h"
#include "assets/other/tracks/toads_turnpike/toads_turnpike_data.h"
#include "engine/actors/Finishline.h"
#include "engine/vehicles/Bus.h"
#include "engine/vehicles/Car.h"
#include "engine/vehicles/Truck.h"
#include "engine/vehicles/TankerTruck.h"

#include "engine/vehicles/Utils.h"

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
    #include "code_80086E70.h"
    extern const char *d_course_toads_turnpike_dl_list[81];
    extern s16 currentScreenSection;
    extern s8 gPlayerCount;
}

ToadsTurnpike::ToadsTurnpike() {
    Props.Minimap.Texture = minimap_toads_turnpike;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 252;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 57;
    Props.Minimap.PlayerY = 44;
    Props.Minimap.PlayerScaleFactor = 0.013f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    ResourceName = "mk:toads_turnpike";
    Props.SetText(Props.Name, "toad's turnpike", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "highway", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "1036m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D009238;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 40;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {1000, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_toads_turnpike_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_toads_turnpike_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gToadsTurnpikeRainbowRoadStars;
    Props.CloudList = gToadsTurnpikeRainbowRoadStars;

    FVector finish;
    finish.x = -38.0f * xOrientation;
    finish.y = (f32) (0 - 15);
    finish.z = 16;

    this->FinishlineSpawnPoint = finish;

    Props.Skybox.TopRight = {0, 2, 94};
    Props.Skybox.BottomRight = {209, 65, 23};
    Props.Skybox.BottomLeft = {209, 65, 23};
    Props.Skybox.TopLeft = {0, 2, 94};
    Props.Skybox.FloorTopRight = {209, 65, 23};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {209, 65, 23};
    Props.Sequence = MusicSeq::MUSIC_SEQ_TOADS_TURNPIKE;
}

void ToadsTurnpike::Load() {
    Track::Load();

    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(d_course_toads_turnpike_dl_list); i++) {
            InvertTriangleWindingByName(d_course_toads_turnpike_dl_list[i]);
        }
        InvertTriangleWindingByName(d_course_toads_turnpike_packed_dl_0);
        InvertTriangleWindingByName(d_course_toads_turnpike_packed_dl_68);
        InvertTriangleWindingByName(d_course_toads_turnpike_packed_dl_D8);
    }

    gFogColour.r = 43;
    gFogColour.g = 13;
    gFogColour.b = 4;
    gFogColour.a = 255;
    gFogMin = 993;
    gFogMax = 1000;
    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_toads_turnpike_addr));
    func_80295C6C();
    Props.WaterLevel = gTrackMinY - 10.0f;
}

void ToadsTurnpike::BeginPlay() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_toads_turnpike_item_box_spawns));

    if (gGamestate != CREDITS_SEQUENCE) {
        uint32_t pathPoint;
        f32 a = ((gCCSelection * 90.0) / 216.0f) + 4.583333333333333;
        f32 b = ((gCCSelection * 90.0) / 216.0f) + 2.9166666666666665;
        a /= 2; // Normally vehicle logic is only ran every 2 frames. This slows the vehicles down to match.
        b /= 2;

        _numTrucks = CVarGetInteger("gNumTrucks", 7);
        _numBuses = CVarGetInteger("gNumBuses", 7);
        _numTankerTrucks = CVarGetInteger("gNumTankerTrucks", 7);
        _numCars = CVarGetInteger("gNumCars", 7);

        // Other game modes spawn seven of each vehicle
        if (gModeSelection == TIME_TRIALS) {
            _numTrucks = 8;
            _numBuses = 8;
            _numTankerTrucks = 8;
            _numCars = 8;
        }

        for (size_t i = 0; i < _numTrucks; i++) {
            pathPoint = CalculateWaypointDistribution(i, _numTrucks, gPathCountByPathIndex[0], 0);
            SpawnActor<ATruck>(a, b, 0, pathPoint, ATruck::SpawnMode::POINT);
        }

        for (size_t i = 0; i < _numBuses; i++) {
            pathPoint = CalculateWaypointDistribution(i, _numBuses, gPathCountByPathIndex[0], 75);
            SpawnActor<ABus>(a, b, 0, pathPoint, ABus::SpawnMode::POINT);
        }

        for (size_t i = 0; i < _numTankerTrucks; i++) {
            pathPoint = CalculateWaypointDistribution(i, _numTankerTrucks, gPathCountByPathIndex[0], 50);
            SpawnActor<ATankerTruck>(a, b, 0, pathPoint, ATankerTruck::SpawnMode::POINT);
        }

        for (size_t i = 0; i < _numCars; i++) {
            pathPoint = CalculateWaypointDistribution(i, _numCars, gPathCountByPathIndex[0], 25);
            SpawnActor<ACar>(a, b, 0, pathPoint, ACar::SpawnMode::POINT);
        }

        if (gModeSelection == VERSUS) {
            SpawnActor<OBombKart>(0, 50, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 100, 1, 0.8333333f);
            SpawnActor<OBombKart>(0, 150, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 200, 1, 0.8333333f);
            SpawnActor<OBombKart>(0, 250, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
            SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
        }
    }
}

void ToadsTurnpike::InitClouds() {
    init_stars(this->Props.Clouds);
}

void ToadsTurnpike::TickClouds(s32 sp1C, Camera* camera) {
    update_stars(sp1C, camera, this->Props.CloudList);
}

void ToadsTurnpike::SomeSounds() {}

void ToadsTurnpike::WhatDoesThisDo(Player* player, int8_t playerId) {
    if ((player->type & PLAYER_CINEMATIC_MODE) != PLAYER_CINEMATIC_MODE) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x1e);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void ToadsTurnpike::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if ((player->type & PLAYER_CINEMATIC_MODE) != PLAYER_CINEMATIC_MODE) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x1E);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void ToadsTurnpike::Draw(ScreenContext* arg0) {
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetFogColor(gDisplayListHead++, gFogColour.r, gFogColour.g, gFogColour.b, gFogColour.a);
    gDPSetCycleType(gDisplayListHead++, G_CYC_2CYCLE);
    gSPFogPosition(gDisplayListHead++, gFogMin, gFogMax);
    gSPSetGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_PASS2);
    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);

    render_track_sections(d_course_toads_turnpike_dl_list, arg0);

    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_PASS2);
    // d_course_toads_turnpike_packed_dl_0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_toads_turnpike_packed_dl_0);
    // d_course_toads_turnpike_packed_dl_68
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_toads_turnpike_packed_dl_68);
    // d_course_toads_turnpike_packed_dl_D8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_toads_turnpike_packed_dl_D8);
    gSPClearGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void ToadsTurnpike::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_toads_turnpike_dl_23930));
}

void ToadsTurnpike::Destroy() { }
