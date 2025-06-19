#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "LuigiRaceway.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/luigi_raceway_data.h"
#include "engine/objects/HotAirBalloon.h"
#include "engine/actors/Finishline.h"
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
#include "code_8003DC40.h"
#include "memory.h"
#include "courses/staff_ghost_data.h"
#include "framebuffer_effects.h"
#include "skybox_and_splitscreen.h"
#include "course.h"
extern const char* luigi_raceway_dls[];
extern s16 currentScreenSection;
}

const course_texture luigi_raceway_textures[] = {
    { gTextureSignShellShot0, 0x038C, 0x1000, 0x0 },
    { gTextureSignShellShot1, 0x0247, 0x1000, 0x0 },
    { gTextureCheckerboardYellowBlue, 0x013A, 0x0800, 0x0 },
    { gTexture64619C, 0x0124, 0x0800, 0x0 },
    { gTextureCheckerboardBlueGreen, 0x0139, 0x0800, 0x0 },
    { gTextureGrass3, 0x0372, 0x0800, 0x0 },
    { gTextureFlagRed, 0x019E, 0x0800, 0x0 },
    { gTexture65100C, 0x0120, 0x0800, 0x0 },
    { gTexture65112C, 0x0150, 0x0800, 0x0 },
    { gTexture653608, 0x07A7, 0x0800, 0x0 },
    { gTextureGrass11, 0x01F8, 0x0800, 0x0 },
    { gTextureSignLuigiFace0, 0x05C9, 0x1000, 0x0 },
    { gTextureSignLuigiFace1, 0x065F, 0x1000, 0x0 },
    { gTexture66C7A8, 0x0149, 0x0800, 0x0 },
    { gTexture670AC8, 0x0FBF, 0x1000, 0x0 },
    { gTexture671A88, 0x012D, 0x0800, 0x0 },
    { gTexture6735DC, 0x03B1, 0x0800, 0x0 },
    { gTexture673C68, 0x038D, 0x0800, 0x0 },
    { gTexture6747C4, 0x0145, 0x0800, 0x0 },
    { gTextureRoad1, 0x02D2, 0x1000, 0x0 },
    { gTextureRoad2, 0x02AE, 0x1000, 0x0 },
    { gTextureRoadFinish1, 0x026B, 0x1000, 0x0 },
    { gTexture67BBD8, 0x0310, 0x0800, 0x0 },
    { gTexture68272C, 0x01F9, 0x1000, 0x0 },
    { gTexture682928, 0x01F9, 0x1000, 0x0 },
    { gTexture682B24, 0x01F9, 0x1000, 0x0 },
    { gTexture682D20, 0x01F9, 0x1000, 0x0 },
    { gTexture682F1C, 0x01F9, 0x1000, 0x0 },
    { gTexture683118, 0x01F9, 0x1000, 0x0 },
    { gTextureSignBlue64, 0x0567, 0x1000, 0x0 },
    { gTextureSignKoopaAir0, 0x0360, 0x1000, 0x0 },
    { gTextureSignKoopaAir1, 0x0304, 0x1000, 0x0 },
    { gTextureSignLuigis0, 0x0287, 0x1000, 0x0 },
    { gTextureSignLuigis1, 0x02AF, 0x1000, 0x0 },
    { gTextureSignMarioStar0, 0x02D2, 0x1000, 0x0 },
    { gTextureSignMarioStar1, 0x02B1, 0x1000, 0x0 },
    { gTextureSignNintendoRed0, 0x02A6, 0x1000, 0x0 },
    { gTextureSignNintendoRed1, 0x02F7, 0x1000, 0x0 },
    { gTextureSignYoshi, 0x04DF, 0x1000, 0x0 },
    { gTextureCheckerboardBlueGray, 0x04A1, 0x1000, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

LuigiRaceway::LuigiRaceway() {
    this->vtx = d_course_luigi_raceway_vertex;
    this->gfx = d_course_luigi_raceway_packed_dls;
    this->gfxSize = 6377;
    Props.textures = luigi_raceway_textures;
    Props.Minimap.Texture = gTextureCourseOutlineLuigiRaceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 271;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 45;
    Props.Minimap.PlayerY = 60;
    Props.Minimap.PlayerScaleFactor = 0.0155f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

    Id = "mk:luigi_raceway";
    Props.SetText(Props.Name, "luigi raceway", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "l circuit", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "717m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D0091E8;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.7f;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = { 0x2DA, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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

    Props.PathTable[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_luigi_raceway_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_luigi_raceway_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*)LOAD_ASSET_RAW(gTextureExhaust2);
    Props.Clouds = gLuigiRacewayClouds;
    Props.CloudList = gLuigiRacewayClouds;

    Props.Skybox.TopRight = { 128, 184, 248 };
    Props.Skybox.BottomRight = { 216, 232, 248 };
    Props.Skybox.BottomLeft = { 216, 232, 248 };
    Props.Skybox.TopLeft = { 128, 184, 248 };
    Props.Skybox.FloorTopRight = { 216, 232, 248 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 216, 232, 248 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_RACEWAYS_WARIO_STADIUM;
}

void LuigiRaceway::Load() {
    Course::Load();

    parse_course_displaylists((TrackSections*) LOAD_ASSET_RAW(d_course_luigi_raceway_addr));
    func_80295C6C();
    Props.WaterLevel = gCourseMinY - 10.0f;
}

void LuigiRaceway::LoadTextures() {
    dma_textures(gTextureTrees5Left, 0x000003E8U, 0x00000800U);
    dma_textures(gTextureTrees5Right, 0x000003E8U, 0x00000800U);
}

void LuigiRaceway::BeginPlay() {
    spawn_foliage((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_luigi_raceway_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_luigi_raceway_item_box_spawns));

    if (gGamestate == CREDITS_SEQUENCE) {
        gWorldInstance.AddObject(new OHotAirBalloon(FVector(-1250.0f, 0.0f, 1110.0f)));
    } else { // Normal gameplay
        gWorldInstance.AddObject(new OHotAirBalloon(FVector(-176.0, 0.0f, -2323.0f)));
        gWorldInstance.AddObject(new OGrandPrixBalloons(FVector(-140, -44, -215)));
    }

    if (gModeSelection == VERSUS) {
        FVector pos = { 0, 0, 0 };

        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][50], 50, 1, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][200], 200, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][305], 305, 1, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][440], 440, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][515], 515, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][0], 0, 0, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][0], 0, 0, 0.8333333f));
    }
}

void LuigiRaceway::InitCourseObjects() {
    size_t i;
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
    }
}

void LuigiRaceway::SomeSounds() {
    vec3f_set(D_8015F748, 85.0f, 21.0f, -219.0f);
    func_800C9D80(D_8015F748, D_802B91C8, 0x5103700B);
}

void LuigiRaceway::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) && ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
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

void LuigiRaceway::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) && ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
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

void LuigiRaceway::SetStaffGhost() {
    u32 temp_v0 = func_800B4E24(0) & 0xfffff;
    if (temp_v0 <= 11200) {
        D_80162DD6 = 0;
        D_80162DF4 = 0;
    } else {
        D_80162DD6 = 1;
        D_80162DF4 = 1;
    }
    D_80162DC4 = d_luigi_raceway_staff_ghost;
    D_80162DE4 = 1;
}

void LuigiRaceway::Jumbotron() {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
               G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0x5800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xB710), 3, 0);
    gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0xB800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xB740), 32, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSP2Triangles(gDisplayListHead++, 4, 5, 6, 0, 4, 6, 7, 0);
    gSP2Triangles(gDisplayListHead++, 8, 9, 10, 0, 9, 11, 12, 0);
    gSP2Triangles(gDisplayListHead++, 9, 12, 10, 0, 13, 14, 15, 0);
    gSP2Triangles(gDisplayListHead++, 13, 15, 16, 0, 17, 18, 19, 0);
    gSP2Triangles(gDisplayListHead++, 17, 19, 20, 0, 11, 21, 12, 0);
    gSP2Triangles(gDisplayListHead++, 10, 22, 23, 0, 10, 23, 24, 0);
    gSP2Triangles(gDisplayListHead++, 25, 22, 26, 0, 25, 26, 27, 0);
    gSP2Triangles(gDisplayListHead++, 28, 23, 29, 0, 28, 29, 30, 0);
    gSP1Triangle(gDisplayListHead++, 27, 26, 31, 0);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xB940), 14, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 3, 4, 5, 0);
    gSP2Triangles(gDisplayListHead++, 3, 5, 6, 0, 1, 7, 8, 0);
    gSP2Triangles(gDisplayListHead++, 1, 8, 9, 0, 10, 11, 12, 0);
    gSP1Triangle(gDisplayListHead++, 10, 12, 13, 0);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0x0000, G_TX_RENDERTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x00FC, 0x007C);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0xF800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 2047, 128);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xBA20), 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0x10800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 2047, 128);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xBA60), 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0x11800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 2047, 128);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xBAA0), 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0x12800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 2047, 128);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xBAE0), 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0x13800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 2047, 128);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xBB20), 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0x14800);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 2047, 128);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xBB60), 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
               G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSegmentTable[5] + 0xB000);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
    gSPVertex(gDisplayListHead++, (uintptr_t) segment_vtx_to_virtual(0xBBA0), 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
}

void LuigiRaceway::Render(struct UnkStruct_800DC5EC* arg0) {
    UNUSED s32 pad;
    u16 sp22 = (u16) arg0->pathCounter;
    s16 prevFrame;

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    // Invalidate Jumbotron textures so they update each frame
    // This could be more efficient if we exposed the non-opcode based invalidation to be called
    // inside copy_framebuffers_port
    gSPInvalidateTexCache(gDisplayListHead++, gSegmentTable[5] + 0xF800);
    gSPInvalidateTexCache(gDisplayListHead++, gSegmentTable[5] + 0x10800);
    gSPInvalidateTexCache(gDisplayListHead++, gSegmentTable[5] + 0x11800);
    gSPInvalidateTexCache(gDisplayListHead++, gSegmentTable[5] + 0x12800);
    gSPInvalidateTexCache(gDisplayListHead++, gSegmentTable[5] + 0x13800);
    gSPInvalidateTexCache(gDisplayListHead++, gSegmentTable[5] + 0x14800);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_luigi_raceway_packed_dl_9EC0
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07009EC0))));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    LuigiRaceway::Jumbotron();
    render_course_segments(luigi_raceway_dls, arg0);

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_luigi_raceway_packed_dl_E0
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070000E0))));
    // d_course_luigi_raceway_packed_dl_68
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000068))));

    D_800DC5DC = 88;
    D_800DC5E0 = 72;

    // Render only the first player camera onto the television billboard. Screen agnostic screens of other players).
    if (gActiveScreenMode == SCREEN_MODE_1P) {

        prevFrame = (s16) sRenderedFramebuffer - 1;

        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }
        currentScreenSection++;
        if (currentScreenSection >= 6) {
            currentScreenSection = 0;
        }

        /**
         * The jumbo television screen used to be split into six sections to fit into the n64's texture size
         * restrictions It isn't split into six sections anymore
         */
        copy_jumbotron_fb_port(D_800DC5DC, D_800DC5E0, currentScreenSection, (u16*) gPortFramebuffers[prevFrame],
                               (u16*) (gSegmentTable[5] + 0xF800));
    }
}

void LuigiRaceway::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*) (d_course_luigi_raceway_dl_FD40));
}

void LuigiRaceway::SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5,
                                      f32* arg6, f32* arg7) {
    func_8003E9EC(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
