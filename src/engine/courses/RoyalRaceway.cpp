#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "RoyalRaceway.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/royal_raceway/royal_raceway_data.h"
#include "assets/other/tracks/royal_raceway/royal_raceway_data.h"
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
    #include "course.h"
    extern const char *royal_raceway_dls[132];
}

RoyalRaceway::RoyalRaceway() {
    Props.Minimap.Texture = minimap_royal_raceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 262;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 37;
    Props.Minimap.PlayerY = 50;
    Props.Minimap.PlayerScaleFactor = 0.014f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "royal raceway", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "p circuit", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "1025m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D009188;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.4f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x3E8, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_royal_raceway_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_royal_raceway_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) gTextureExhaust4;
    Props.Clouds = gRoyalRacewayClouds;
    Props.CloudList = gRoyalRacewayClouds;

    Props.Skybox.TopRight = {238, 144, 255};
    Props.Skybox.BottomRight = {255, 224, 240};
    Props.Skybox.BottomLeft = {255, 224, 240};
    Props.Skybox.TopLeft = {238, 144, 255};
    Props.Skybox.FloorTopRight = {255, 224, 240};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 224, 240};
    Props.Sequence = MusicSeq::MUSIC_SEQ_ROYAL_RACEWAY;

    Props.WaterLevel = -60.0f;
}

void RoyalRaceway::Load() {
    Course::Load();
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(royal_raceway_dls); i++) {
            InvertTriangleWindingByName(royal_raceway_dls[i]);
        }
        InvertTriangleWindingByName(d_course_royal_raceway_packed_dl_B030);
        InvertTriangleWindingByName(d_course_royal_raceway_packed_dl_A648);
        InvertTriangleWindingByName(d_course_royal_raceway_packed_dl_11A8);
        InvertTriangleWindingByName(d_course_royal_raceway_packed_dl_8A0);
    }
    parse_course_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_royal_raceway_addr));
    func_80295C6C();
}

void RoyalRaceway::UnLoad() {
    RestoreTriangleWinding();
}

void RoyalRaceway::BeginPlay() {
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_item_box_spawns));
    spawn_piranha_plants((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_piranha_plant_spawn));

    if (gModeSelection == VERSUS) {
        OBombKart::Spawn(0, 50, 3, 0.8333333f);
        OBombKart::Spawn(0, 100, 3, 0.8333333f);
        OBombKart::Spawn(0, 296, 3, 0.8333333f);
        OBombKart::Spawn(0, 400, 1, 0.8333333f);
        OBombKart::Spawn(0, 746, 3, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
    }
    if (gGamestate != CREDITS_SEQUENCE) {
        OGrandPrixBalloons::Spawn(FVector(-64, 5, -330));
    }
}

void RoyalRaceway::InitCourseObjects() {
    size_t i;
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
    }
}

void RoyalRaceway::WhatDoesThisDo(Player* player, int8_t playerId) {
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

void RoyalRaceway::WhatDoesThisDoAI(Player* player, int8_t playerId) {
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

void RoyalRaceway::SetStaffGhost() {
    u32 temp_v0 = func_800B4E24(0) & 0xfffff;
    if (temp_v0 <= 16000) {
        bCourseGhostDisabled = 0;
        D_80162DF4 = 0;
    } else {
        bCourseGhostDisabled = 1;
        D_80162DF4 = 1;
    }
    D_80162DC4 = d_royal_raceway_staff_ghost;
    D_80162DE4 = 6;
}

void RoyalRaceway::Render(struct UnkStruct_800DC5EC* arg0) {
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

    render_course_segments(royal_raceway_dls, arg0);

    // d_course_royal_raceway_packed_dl_11A8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_royal_raceway_packed_dl_11A8);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_royal_raceway_packed_dl_8A0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_royal_raceway_packed_dl_8A0);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void RoyalRaceway::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_royal_raceway_dl_D8E8));
}

void RoyalRaceway::Waypoints(Player* player, int8_t playerId) {
    s16 waypoint = gNearestPathPointByPlayerId[playerId];
    if ((waypoint >= 0x258) && (waypoint < 0x2A4)) {
        player->nearestPathPointId = 0x258U;
    } else {
        player->nearestPathPointId = gCopyNearestWaypointByPlayerId[playerId];
        if (player->nearestPathPointId < 0) {
            player->nearestPathPointId = gPathCountByPathIndex[0] + player->nearestPathPointId;
        }
    }
}

void RoyalRaceway::ScrollingTextures() {
    D_802B87BC -= 20;
    if (D_802B87BC < 0) {
        D_802B87BC = 0xFF;
    }
    // d_course_royal_raceway_packed_dl_A6A8
    find_and_set_tile_size((uintptr_t) d_course_royal_raceway_packed_dl_A6A8, 0, D_802B87BC);
    // d_course_royal_raceway_packed_dl_A648
    find_and_set_tile_size((uintptr_t) d_course_royal_raceway_packed_dl_A648, 0, D_802B87BC);
}
