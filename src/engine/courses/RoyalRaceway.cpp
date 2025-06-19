#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "RoyalRaceway.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "assets/royal_raceway_data.h"
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
    #include "assets/common_data.h"
    #include "save.h"
    #include "staff_ghosts.h"
    #include "actors.h"
    #include "collision.h"
    #include "memory.h"
    #include "courses/staff_ghost_data.h"
    #include "course.h"
    extern const char *royal_raceway_dls[];
}

const course_texture royal_raceway_textures[] = {
    { gTexture64619C, 0x0124, 0x0800, 0x0 },
    { gTexture645134, 0x052C, 0x0800, 0x0 },
    { gTextureWoodDoor1, 0x0903, 0x1000, 0x0 },
    { gTexture64BB60, 0x0169, 0x0800, 0x0 },
    { gTextureGrass3, 0x0372, 0x0800, 0x0 },
    { gTexture64F9E8, 0x020B, 0x1000, 0x0 },
    { gTextureFlagRed, 0x019E, 0x0800, 0x0 },
    { gTextureCrownJewelBlue, 0x0301, 0x0800, 0x0 },
    { gTextureCrown, 0x0106, 0x0800, 0x0 },
    { gTextureCrownJewelPink, 0x0313, 0x0800, 0x0 },
    { gTextureSignKoopaAir0, 0x0360, 0x1000, 0x0 },
    { gTextureSignKoopaAir1, 0x0304, 0x1000, 0x0 },
    { gTexture6684F8, 0x010D, 0x0800, 0x0 },
    { gTextureSignLuigis0, 0x0287, 0x1000, 0x0 },
    { gTextureSignLuigis1, 0x02AF, 0x1000, 0x0 },
    { gTextureSignMarioStar0, 0x02D2, 0x1000, 0x0 },
    { gTextureSignMarioStar1, 0x02B1, 0x1000, 0x0 },
    { gTexture66CA98, 0x02C9, 0x0800, 0x0 },
    { gTextureCheckerboardPink, 0x0157, 0x0800, 0x0 },
    { gTexture670AC8, 0x0FBF, 0x1000, 0x0 },
    { gTextureRoad0, 0x0300, 0x1000, 0x0 },
    { gTextureRoadFinish0, 0x0338, 0x1000, 0x0 },
    { gTextureSignYoshi, 0x04DF, 0x1000, 0x0 },
    { gTextureCheckerboardBlueGray, 0x04A1, 0x1000, 0x0 },
    { gTextureCastleBricks, 0x0B33, 0x1000, 0x0 },
    { gTextureCastleBridge, 0x0428, 0x0800, 0x0 },
    { gTextureGrass8, 0x02CB, 0x0800, 0x0 },
    { gTextureGrass9, 0x0421, 0x0800, 0x0 },
    { gTexture6646B8, 0x0298, 0x1000, 0x0 },
    { gTexture664408, 0x02AE, 0x1000, 0x0 },
    { gTextureBricksRed, 0x0C55, 0x1000, 0x0 },
    { gTexture665C0C, 0x059D, 0x0800, 0x0 },
    { gTexture6661AC, 0x01F7, 0x0800, 0x0 },
    { gTexture6663A4, 0x05F6, 0x0800, 0x0 },
    { gTexture667BAC, 0x067A, 0x0800, 0x0 },
    { gTextureFlagRed2, 0x0186, 0x0800, 0x0 },
    { gTexture66DB60, 0x01D6, 0x0800, 0x0 },
    { gTextureStainglassPeach0, 0x0ED1, 0x1000, 0x0 },
    { gTextureStainglassPeach1, 0x0DA9, 0x1000, 0x0 },
    { gTextureFencePostWooden, 0x083D, 0x1000, 0x0 },
    { gTexture648508, 0x01FE, 0x1000, 0x0 },
    { gTexture6449D4, 0x075D, 0x0800, 0x0 },
    { gTexture67FE0C, 0x02DE, 0x0800, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

RoyalRaceway::RoyalRaceway() {
    this->vtx = d_course_royal_raceway_vertex;
    this->gfx = d_course_royal_raceway_packed_dls;
    this->gfxSize = 5670;
    Props.textures = royal_raceway_textures;
    Props.Minimap.Texture = gTextureCourseOutlineRoyalRaceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 262;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 37;
    Props.Minimap.PlayerY = 50;
    Props.Minimap.PlayerScaleFactor = 0.014f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

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

    Props.CloudTexture = (u8*) LOAD_ASSET_RAW(gTextureExhaust4);
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
    Props.Sequence = MusicSeq::MUSIC_SEQ_RACEWAYS_WARIO_STADIUM;

    Props.WaterLevel = -60.0f;
}

void RoyalRaceway::Load() {
    Course::Load();

    parse_course_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_royal_raceway_addr));
    func_80295C6C();
}

void RoyalRaceway::LoadTextures() {
    dma_textures(gTextureTrees3, 0x000003E8U, 0x00000800U);
    dma_textures(gTextureTrees7, 0x000003E8U, 0x00000800U);
    D_802BA058 = dma_textures(gTexturePiranhaPlant1, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant2, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant3, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant4, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant5, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant6, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant7, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant8, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant9, 0x000003E8U, 0x00000800U);
}

void RoyalRaceway::BeginPlay() {
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_item_box_spawns));
    spawn_piranha_plants((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_royal_raceway_piranha_plant_spawn));

    if (gModeSelection == VERSUS) {
        FVector pos = { 0, 0, 0 };

        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][50], 50, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][100], 100, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][296], 296, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][400], 400, 1, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][746], 746, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][0], 0, 0, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][0], 0, 0, 0.8333333f));
    }
    if (gGamestate != CREDITS_SEQUENCE) {
        gWorldInstance.AddObject(new OGrandPrixBalloons(FVector(-64, 5, -330)));
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
        D_80162DD6 = 0;
        D_80162DF4 = 0;
    } else {
        D_80162DD6 = 1;
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
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x0700B030))));
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_royal_raceway_packed_dl_A648
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x0700A648))));

    render_course_segments(royal_raceway_dls, arg0);

    // d_course_royal_raceway_packed_dl_11A8
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070011A8))));
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_royal_raceway_packed_dl_8A0
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070008A0))));
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
    find_and_set_tile_size((uintptr_t) segmented_gfx_to_virtual((void*)0x0700A6A8), 0, D_802B87BC);
    // d_course_royal_raceway_packed_dl_A648
    find_and_set_tile_size((uintptr_t) segmented_gfx_to_virtual((void*)0x0700A648), 0, D_802B87BC);
}
