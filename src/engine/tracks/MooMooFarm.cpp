#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "MooMooFarm.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/moo_moo_farm/moo_moo_farm_data.h"
#include "assets/other/tracks/moo_moo_farm/moo_moo_farm_data.h"
#include "engine/objects/MoleGroup.h"
#include "engine/objects/Mole.h"

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
    extern const char *moo_moo_farm_dls[92];
    extern s16 currentScreenSection;
    extern s8 gPlayerCount;
}

MooMooFarm::MooMooFarm() {
    Props.Minimap.Texture = minimap_moo_moo_farm;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 271;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 18;
    Props.Minimap.PlayerY = 36;
    Props.Minimap.PlayerScaleFactor = 0.0155f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "moo moo farm", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "farm", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "527m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D009210;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x230, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_moo_moo_farm_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_moo_moo_farm_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) gTextureExhaust0;
    Props.Clouds = gYoshiValleyMooMooFarmClouds;
    Props.CloudList = gYoshiValleyMooMooFarmClouds;

    Props.Skybox.TopRight = {0, 18, 255};
    Props.Skybox.BottomRight = {197, 211, 255};
    Props.Skybox.BottomLeft = {197, 211, 255};
    Props.Skybox.TopLeft = {0, 18, 255};
    Props.Skybox.FloorTopRight = {255, 184, 99};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 184, 99};
    Props.Sequence = MusicSeq::MUSIC_SEQ_MOO_MOO_FARM;
}

void MooMooFarm::Load() {
    Track::Load();
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(moo_moo_farm_dls); i++) {
            InvertTriangleWindingByName(moo_moo_farm_dls[i]);
        }
        InvertTriangleWindingByName(d_course_moo_moo_farm_packed_dl_4DF8);
        InvertTriangleWindingByName(d_course_moo_moo_farm_packed_dl_5640);
        InvertTriangleWindingByName(d_course_moo_moo_farm_dl_13FF8);
        InvertTriangleWindingByName(d_course_moo_moo_farm_packed_dl_5410);
        InvertTriangleWindingByName(d_course_moo_moo_farm_dl_14060);
        InvertTriangleWindingByName(d_course_moo_moo_farm_packed_dl_10C0);
    }
    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_moo_moo_farm_addr));
    func_80295C6C();
    Props.WaterLevel = gTrackMinY - 10.0f;
}

void MooMooFarm::BeginPlay() {
    if (gPlayerCountSelection1 != 4) {
        spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_moo_moo_farm_tree_spawn));
    }
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_moo_moo_farm_item_box_spawns));

    for (size_t i = 0; i < gObjectParticle2_SIZE; i++) {
        find_unused_obj_index(&gObjectParticle2[i]);
    }

    if (gGamestate != CREDITS_SEQUENCE) {
        std::vector<FVector> moleSpawns1 = {
            { FVector(771, 20, -2022) },
            { FVector(807, 15, -2063) },
            { FVector(847, 18, -2040) },
            { FVector(913, 14, -2054) },
            { FVector(939, 21, -1997) },
            { FVector(941, 17, -2024) },
            { FVector(994, 17, -1994) },
            { FVector(863, 22, -2010) },
        };

        std::vector<FVector> moleSpawns2 = {
            { FVector(1500, 2,  1140) },
            { FVector(1510, 15, 1050) },
            { FVector(1609, 21,  935) },
            { FVector(1289, 3,  1269) },
            { FVector(1468, 22, 1046) },
            { FVector(1380, 12, 1154) },
            { FVector(1297, 19, 1170) },
            { FVector(1589, 11, 1004) },
            { FVector(1414, 3,  1185) },
            { FVector(1405, 4,  1254) },
            { FVector(1463, 8,  1118) },
        };

        std::vector<FVector> moleSpawns3 = {
            { FVector(701, 2,  1279) },
            { FVector(811, 8,  1278) },
            { FVector(791, 16, 1229) },
            { FVector(876, 15, 1266) },
            { FVector(984, 23, 1248) },
            { FVector(891, 20, 1242) },
            { FVector(920, 15, 1304) },
            { FVector(823, 6,  1327) },
            { FVector(717, 8,  1239) },
            { FVector(695, 19, 1176) },
            { FVector(628, 8,  1191) },
            { FVector(724, 4,  1339) },
        };

        // How many moles can appear per tick?
        size_t tick1, tick2, tick3;
        switch(gCCSelection) {
            case CC_50:
                tick1 = 4;
                tick2 = 6;
                tick3 = 6;
                break;
            case CC_100:
                tick1 = 5;
                tick2 = 8;
                tick3 = 8;
                break;
            case CC_150:
                tick1 = 5;
                tick2 = 8;
                tick3 = 10;
                break;
            case CC_EXTRA:
                tick1 = 5;
                tick2 = 8;
                tick3 = 8;
                break;
            default:
                tick1 = 4;
                tick2 = 6;
                tick3 = 6;
                break;
        }

        GetWorld()->AddObject(std::make_unique<OMoleGroup>(moleSpawns1, tick1));
        GetWorld()->AddObject(std::make_unique<OMoleGroup>(moleSpawns2, tick2));
        GetWorld()->AddObject(std::make_unique<OMoleGroup>(moleSpawns3, tick3));
    }

    if (gModeSelection == VERSUS) {
        SpawnActor<OBombKart>(0, 50, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 140, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 225, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 316, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 434, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
    }
}

void MooMooFarm::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void MooMooFarm::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void MooMooFarm::Draw(ScreenContext* arg0) {
    s16 temp_s0 = arg0->pathCounter;
    s16 temp_s1 = arg0->playerDirection;

    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_moo_moo_farm_packed_dl_4DF8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_moo_moo_farm_packed_dl_4DF8);
    // d_course_moo_moo_farm_packed_dl_5640
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_moo_moo_farm_packed_dl_5640);
    gSPFogPosition(gDisplayListHead++, gFogMin, gFogMax);

    render_track_sections(moo_moo_farm_dls, arg0);

    if ((temp_s0 < 14) && (temp_s0 > 10)) {
        if ((temp_s1 == 2) || (temp_s1 == 3) || (temp_s1 == 1)) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
        }

    } else if (temp_s0 < 16) {
        gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
    } else if (temp_s0 < 19) {
        if (temp_s1 != 2) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
        }

    } else if (temp_s0 < 20) {
        if (temp_s1 == 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
        }
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    if ((temp_s0 >= 16) && (temp_s0 < 24)) {
        if ((temp_s1 == 2) || (temp_s1 == 3)) {
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_moo_moo_farm_packed_dl_5410);
        }

    } else if (temp_s0 < 9) {
        if (temp_s1 == 2) {
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_moo_moo_farm_packed_dl_5410);
        }
    }
    if (temp_s0 < 4) {
        if (temp_s1 != 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
        }

    } else if (temp_s0 < 8) {
        if (temp_s1 == 2) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
        }

    } else if (temp_s0 >= 22) {
        gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
    } else if (temp_s0 >= 18) {
        if ((temp_s1 == 0) || (temp_s1 == 3)) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
        }
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_moo_moo_farm_packed_dl_10C0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_moo_moo_farm_packed_dl_10C0);
}

void MooMooFarm::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_moo_moo_farm_dl_14088));
}

void MooMooFarm::CreditsSpawnActors() {
    spawn_foliage((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_moo_moo_farm_tree_spawn));
}

void MooMooFarm::Destroy() { }
