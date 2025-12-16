#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "PodiumCeremony.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/royal_raceway/royal_raceway_data.h"
#include "assets/other/tracks/royal_raceway/royal_raceway_data.h"
#include "assets/models/ceremony_data.h"
#include "assets/other/ceremony_data.h"
#include "engine/objects/Trophy.h"
#include "engine/objects/Podium.h"
#include "engine/objects/CheepCheep.h"
#include "engine/particles/StarEmitter.h"
#include "engine/objects/GrandPrixBalloons.h"

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
    #include "courses/staff_ghost_data.h"
    #include "podium_ceremony_actors.h"
    extern const char *royal_raceway_dls[];
}

PodiumCeremony::PodiumCeremony() {
    Props.Minimap.Width = 0;
    Props.Minimap.Height = 0;
    Props.Minimap.Pos[0].X = 262;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 37;
    Props.Minimap.PlayerY = 50;
    Props.Minimap.PlayerScaleFactor = 0.014f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

    Props.SetText(Props.Name, "royal raceway", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "p circuit", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "1025m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D009188;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.4f;
    Props.AISteeringSensitivity = 53;

    Props.PathSizes = {500, 500, 500, 500, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path);
    Props.PathTable[1] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path_2);
    Props.PathTable[2] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path_3);
    Props.PathTable[3] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path_4);

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path);
    Props.PathTable2[1] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path_2);
    Props.PathTable2[2] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path_3);
    Props.PathTable2[3] = (TrackPathPoint*)LOAD_ASSET_RAW(podium_ceremony_path_4);

    Props.CloudTexture = (u8*) gTextureExhaust4;
    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;

    Props.Skybox.TopRight = {238, 144, 255};
    Props.Skybox.BottomRight = {255, 224, 240};
    Props.Skybox.BottomLeft = {255, 224, 240};
    Props.Skybox.TopLeft = {238, 144, 255};
    Props.Skybox.FloorTopRight = {255, 224, 240};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 224, 240};
    Props.Sequence = MusicSeq::MUSIC_SEQ_UNKNOWN;

    Props.WaterLevel = -60.0f;
}

void PodiumCeremony::Load() {
    Track::Load();

    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_royal_raceway_addr));
    func_80295C6C();
}

void PodiumCeremony::BeginPlay() {
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_item_box_spawns));
    spawn_piranha_plants((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_piranha_plant_spawn));

    OCheepCheep::Spawn(FVector((f32)-3202, (f32)19, (f32)-478), OCheepCheep::Behaviour::PODIUM_CEREMONY, IPathSpan(0, 0));
    OPodium::Spawn(FVector((f32)-3202, (f32)19, (f32)-478));

    FVector pos = {0, 90.0f, 0};

    OTrophy::TrophyType type = OTrophy::TrophyType::BRONZE;
    switch(D_802874D8.unk1D) {
        case 2: // Bronze
            type = gCCSelection == CC_150 ? OTrophy::TrophyType::BRONZE_150 : OTrophy::TrophyType::BRONZE;
            break;
        case 1: // Silver
            pos.x -= 3.0;
            pos.z += 15.0;
            type = gCCSelection == CC_150 ? OTrophy::TrophyType::SILVER_150 : OTrophy::TrophyType::SILVER;
            break;
        case 0: // Gold
            pos.x -= 2.0;
            pos.z -= 15.0;
            type = gCCSelection == CC_150 ? OTrophy::TrophyType::GOLD_150 : OTrophy::TrophyType::GOLD;
            break;
    }

    SpawnActor<OTrophy>(pos, type, OTrophy::Behaviour::PODIUM_CEREMONY);

    SpawnActor<OBombKart>(3, 3, OBombKart::States::PODIUM_CEREMONY, 1.25f);
    SpawnActor<OBombKart>(3, 40, 0, 1.0f);
    SpawnActor<OBombKart>(3, 60, 0, 1.0f);
    SpawnActor<OBombKart>(3, 80, 0, 1.0f);
    SpawnActor<OBombKart>(3, 100, 0, 1.0f);
    SpawnActor<OBombKart>(3, 120, 0, 1.0f);
    SpawnActor<OBombKart>(3, 140, 0, 1.0f);

    if (gGamestate != CREDITS_SEQUENCE) {
        SpawnActor<OGrandPrixBalloons>(FVector(-64, 5, -330));
    }
}

void PodiumCeremony::InitTrackObjects() {
    size_t i;
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
    }
}

void PodiumCeremony::SomeSounds() {
}

void PodiumCeremony::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x180) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1E1)) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x41);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void PodiumCeremony::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x180) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1E1)) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x41);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void PodiumCeremony::Draw(ScreenContext* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_royal_raceway_packed_dl_B030
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_royal_raceway_packed_dl_B030);
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_royal_raceway_packed_dl_A648
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_royal_raceway_packed_dl_A648);

    render_track_sections(royal_raceway_dls, arg0);

    // d_course_royal_raceway_packed_dl_11A8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_royal_raceway_packed_dl_11A8);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_royal_raceway_packed_dl_8A0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_royal_raceway_packed_dl_8A0);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void PodiumCeremony::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_royal_raceway_dl_D8E8));
}

void PodiumCeremony::Destroy() { }
