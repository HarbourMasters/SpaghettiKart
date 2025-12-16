#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "BowsersCastle.h"
#include "align_asset_macro.h"
#include "engine/World.h"
#include "engine/tracks/Track.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Thwomp.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
#include "assets/other/tracks/bowsers_castle/bowsers_castle_data.h"

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
    #include "code_8003DC40.h"
    #include "memory.h"
    extern const char *bowsers_castle_dls[108];
}

BowsersCastle::BowsersCastle() {
    Props.Minimap.Texture = minimap_bowsers_castle;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 265;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 12;
    Props.Minimap.PlayerY = 48;
    Props.Minimap.PlayerScaleFactor = 0.0174f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    ResourceName = "mk:bowsers_castle";
    Props.SetText(Props.Name, "bowser's castle", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "castle", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "777m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D008FB8;
    Props.AIMaximumSeparation = 35.0f;
    Props.AIMinimumSeparation = 0.2f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

    Props.PathSizes = { 0x30C, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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

    Props.PathTable[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_bowsers_castle_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_bowsers_castle_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;

    Props.Skybox.TopRight = { 48, 8, 120 };
    Props.Skybox.BottomRight = { 0, 0, 0 };
    Props.Skybox.BottomLeft = { 0, 0, 0 };
    Props.Skybox.TopLeft = { 48, 8, 120 };
    Props.Skybox.FloorTopRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 0, 0, 0 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_BOWSERS_CASTLE;

    Props.WaterLevel = -50.0f;
    WaterVolumes.push_back({20.0f, 1549.0f, 1859.0f, -1402.0f, -1102.0f});
}

void BowsersCastle::Load() {
    Track::Load();
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(bowsers_castle_dls); i++) {
            InvertTriangleWindingByName(bowsers_castle_dls[i]);
        }
        InvertTriangleWindingByName(d_course_bowsers_castle_packed_dl_6A80);

        InvertTriangleWindingByName(d_course_bowsers_castle_packed_dl_248);

        InvertTriangleWindingByName(d_course_bowsers_castle_dl_9228);
    }

    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_bowsers_castle_addr));
    func_80295C6C();
    find_vtx_and_set_colours((Gfx*) d_course_bowsers_castle_packed_dl_1350, 0x32, 0, 0, 0);
}

void BowsersCastle::BeginPlay() {
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_bowsers_castle_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_bowsers_castle_item_box_spawns));

    switch (gCCSelection) {
        case CC_100:
        case CC_EXTRA:
            SpawnActor<OThwomp>(0x0320, 0xf92a, 0xC000, 1.0f, OThwomp::States::STATIONARY, 0);
            SpawnActor<OThwomp>(0x044c, 0xf92a, 0xC000, 1.0f, OThwomp::States::STATIONARY, 1);
            SpawnActor<OThwomp>(0x02bc, 0xf95c, 0xC000, 1.0f, OThwomp::States::MOVE_AND_ROTATE, 0);
            SpawnActor<OThwomp>(0x04b0, 0xf8f8, 0xC000, 1.0f, OThwomp::States::MOVE_AND_ROTATE, 1);
            SpawnActor<OThwomp>(0x04b0, 0xf5ba, 0xC000, 1.0f, OThwomp::States::MOVE_FAR, 0);
            SpawnActor<OThwomp>(0x04b0, 0xf592, 0xC000, 1.0f, OThwomp::States::MOVE_FAR, 1);
            SpawnActor<OThwomp>(0x091a, 0xf5bf, 0xC000, 1.0f, OThwomp::States::STATIONARY_FAST, 0);
            SpawnActor<OThwomp>(0x091a, 0xf597, 0xC000, 1.0f, OThwomp::States::STATIONARY_FAST, 1);
            SpawnActor<OThwomp>(0x0596, 0xf92f, 0xC000, 1.5f, OThwomp::States::JAILED, 0);
            SpawnActor<OThwomp>(0x082a, 0xf9f2, 0x4000, 1.0f, OThwomp::States::SLIDE, 0);
            SpawnActor<OThwomp>(0x073a, 0xf9f2, 0x4000, 1.0f, OThwomp::States::SLIDE, 1);
            break;
        case CC_50:
            SpawnActor<OThwomp>(0x3B6, 0xF92A, 0xC000, 1.0f, OThwomp::States::STATIONARY, 0);
            SpawnActor<OThwomp>(0x0352, 0xf95c, 0xC000, 1.0f, OThwomp::States::MOVE_AND_ROTATE, 0);
            SpawnActor<OThwomp>(0x04b0, 0xf5ba, 0xC000, 1.0f, OThwomp::States::MOVE_FAR, 0);
            SpawnActor<OThwomp>(0x04b0, 0xf592, 0xC000, 1.0f, OThwomp::States::MOVE_FAR, 1);
            SpawnActor<OThwomp>(0x091a, 0xf5b0, 0xC000, 1.0f, OThwomp::States::STATIONARY_FAST, 0);
            SpawnActor<OThwomp>(0x0596, 0xf92f, 0xC000, 1.5f, OThwomp::States::JAILED, 0);
            SpawnActor<OThwomp>(0x082a, 0xf9f2, 0x4000, 1.0f, OThwomp::States::SLIDE , 0);
            SpawnActor<OThwomp>(0x073a, 0xf9f2, 0x4000, 1.0f, OThwomp::States::SLIDE, 1);
            break;
        case CC_150:
            SpawnActor<OThwomp>(0x0320, 0xf92a, 0xC000, 1.0f, OThwomp::States::STATIONARY, 0);
            SpawnActor<OThwomp>(0x044c, 0xf92a, 0xC000, 1.0f, OThwomp::States::STATIONARY, 1);
            SpawnActor<OThwomp>(0x02bc, 0xf95c, 0xC000, 1.0f, OThwomp::States::MOVE_AND_ROTATE, 0);
            SpawnActor<OThwomp>(0x04b0, 0xf8f8, 0xC000, 1.0f, OThwomp::States::MOVE_AND_ROTATE, 1);
            SpawnActor<OThwomp>(0x04b0, 0xf5ba, 0xC000, 1.0f, OThwomp::States::MOVE_FAR, 0);
            SpawnActor<OThwomp>(0x04b0, 0xf592, 0xC000, 1.0f, OThwomp::States::MOVE_FAR, 1);
            SpawnActor<OThwomp>(0x091a, 0xf5c9, 0xC000, 1.0f, OThwomp::States::STATIONARY_FAST, 0);
            SpawnActor<OThwomp>(0x091a, 0xf5ab, 0xC000, 1.0f, OThwomp::States::STATIONARY_FAST, 1);
            SpawnActor<OThwomp>(0x091a, 0xf58d, 0xC000, 1.0f, OThwomp::States::STATIONARY_FAST, 2);
            SpawnActor<OThwomp>(0x0596, 0xf92f, 0xC000, 1.5f, OThwomp::States::JAILED, 0);
            SpawnActor<OThwomp>(0x082a, 0xf9f2, 0x4000, 1.0f, OThwomp::States::SLIDE, 0);
            SpawnActor<OThwomp>(0x073a, 0xf9f2, 0x4000, 1.0f, OThwomp::States::SLIDE, 1);
            break;
    }

    if (gModeSelection == VERSUS) {
        SpawnActor<OBombKart>(0, 50, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 150, 1, 0.8333333f);
        SpawnActor<OBombKart>(0, 200, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 260, 1, 0.8333333f);
        SpawnActor<OBombKart>(0, 435, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
    }
}

void BowsersCastle::InitTrackObjects() {
    size_t objectId;
    size_t i;

    // Handle the big statue's fire breath
    objectId = indexObjectList2[0];
    init_object(objectId, 0);
    gObjectList[objectId].pos[0] = -68.0 * xOrientation;
    gObjectList[objectId].pos[1] = 80.0f;
    gObjectList[objectId].pos[2] = -1840.0f;
    // Handle the smaller statues' fire breath
    for (i = 0; i < NUM_FIRE_BREATHS; i++) {
        objectId = indexObjectList3[i];
        init_object(objectId, 0);
        gObjectList[objectId].pos[0] = gFireBreathsSpawns[i][0] * xOrientation;
        gObjectList[objectId].pos[1] = gFireBreathsSpawns[i][1];
        gObjectList[objectId].pos[2] = gFireBreathsSpawns[i][2];
        gObjectList[objectId].direction_angle[1] = 0;
        if (i % 2U) {
            gObjectList[objectId].direction_angle[1] += 0x8000;
        }
    }
    for (i = 0; i < 32; i++) {
        delete_object(&indexObjectList4[i]);
    }
}

void BowsersCastle::TickTrackObjects() {
    update_flame_particle();
}

void BowsersCastle::DrawTrackObjects(s32 cameraId) {
    // render_object_thwomps(cameraId);
    render_object_bowser_flame(cameraId);
}

void BowsersCastle::SomeSounds() {
}

void BowsersCastle::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x29) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1D2)) {
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

void BowsersCastle::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x29) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1D2)) {
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

void BowsersCastle::Draw(ScreenContext* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_bowsers_castle_packed_dl_6A80
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_bowsers_castle_packed_dl_6A80);
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    D_802B87BC++;
    if (D_802B87BC > 255) {
        D_802B87BC = 0;
    }
    render_track_sections(bowsers_castle_dls, arg0);

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_bowsers_castle_packed_dl_248
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_bowsers_castle_packed_dl_248);
}

void BowsersCastle::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*) (d_course_bowsers_castle_dl_9148));
}

void BowsersCastle::SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    func_8003E6EC(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void BowsersCastle::Waypoints(Player* player, int8_t playerId) {
    s16 waypoint = gNearestPathPointByPlayerId[playerId];
    if ((waypoint >= 0x235) && (waypoint < 0x247)) {
        player->nearestPathPointId = 0x214;
    } else if ((waypoint >= 0x267) && (waypoint < 0x277)) {
        player->nearestPathPointId = 0x25B;
    } else {
        player->nearestPathPointId = gNearestPathPointByPlayerId[playerId];
        if (player->nearestPathPointId < 0) {
            player->nearestPathPointId = gPathCountByPathIndex[0] + player->nearestPathPointId;
        }
    }
}

void BowsersCastle::DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot,
                              uint16_t playerDirection) {
    if (gActiveScreenMode != SCREEN_MODE_1P) {
        return;
    }
    if (pathCounter < 6) {
        return;
    }
    if (pathCounter > 9) {
        return;
    }
    if (pathCounter == 9) {
        if (cameraRot < 0xA000) {
            return;
        }
        if (cameraRot > 0xE000) {
            return;
        }
    }
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_bowsers_castle_dl_9228);
}

void BowsersCastle::CreditsSpawnActors() {
    find_vtx_and_set_colours((Gfx*) d_course_bowsers_castle_packed_dl_1350, 0x32, 0, 0, 0);
}

void BowsersCastle::Destroy() {
}
