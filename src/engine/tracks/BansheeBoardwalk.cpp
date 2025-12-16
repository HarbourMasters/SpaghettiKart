#include <cstdint>
#include <libultraship.h>
#include <libultra/gbi.h>
#include <unordered_map>
#include <vector>
#include <memory>

#include "BansheeBoardwalk.h"
#include "World.h"
#include "align_asset_macro.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/CheepCheep.h"
#include "engine/objects/TrashBin.h"
#include "engine/objects/Bat.h"
#include "engine/objects/Boos.h"
#include "assets/models/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "assets/other/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "port/Game.h"
#include "assets/textures/boo_frames.h"

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
extern const char *banshee_boardwalk_dls[100];
}

BansheeBoardwalk::BansheeBoardwalk() {
    Props.Minimap.Texture = minimap_banshee_boardwalk;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 262;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 55;
    Props.Minimap.PlayerY = 39;
    Props.Minimap.PlayerScaleFactor = 0.016f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    ResourceName = "mk:banshee_boardwalk";
    Props.SetText(Props.Name, "banshee boardwalk", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "ghost", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "747m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D009058;
    Props.AIMaximumSeparation = 40.0f;
    Props.AIMinimumSeparation = 0.4f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

    Props.PathSizes = { 0x2EE, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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

    Props.PathTable[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_banshee_boardwalk_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_banshee_boardwalk_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;

    Props.Skybox.TopRight = { 0, 0, 0 };
    Props.Skybox.BottomRight = { 0, 0, 0 };
    Props.Skybox.BottomLeft = { 0, 0, 0 };
    Props.Skybox.TopLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 0, 0, 0 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_BANSHEE_BOARDWALK;

    Props.WaterLevel = -80.0f;
}

void BansheeBoardwalk::Load() {
    Track::Load();

    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(banshee_boardwalk_dls); i++) {
            InvertTriangleWindingByName(banshee_boardwalk_dls[i]);
        }
        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_7228);

        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_5CD0);
        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_4E60);
        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_69B0);

        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_580);
        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_60);
        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_540);

        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_6310);

        InvertTriangleWindingByName(d_course_banshee_boardwalk_dl_B278);

        // transparent DLs
        InvertTriangleWindingByName(d_course_banshee_boardwalk_packed_dl_878);
    }

    bFog = true;
    gFogColour.r = 0;
    gFogColour.g = 0;
    gFogColour.b = 0;
    gFogColour.a = 255;
    parse_track_displaylists((TrackSections*) LOAD_ASSET_RAW(d_course_banshee_boardwalk_track_sections));
    func_80295C6C();
    find_vtx_and_set_colours((Gfx*) d_course_banshee_boardwalk_packed_dl_878, 128, 0, 0, 0);
}

void BansheeBoardwalk::BeginPlay() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_banshee_boardwalk_item_box_spawns));

    OCheepCheep::Spawn(FVector(xOrientation * -1650.0, -200.0f, -1650.0f),
                                             OCheepCheep::Behaviour::RACE, IPathSpan(160, 170));

    OTrashBin::Behaviour bhv;
    if (gModeSelection == TIME_TRIALS) {
        bhv = OTrashBin::Behaviour::STATIC;
    } else {
        bhv = OTrashBin::Behaviour::MUNCHING;
    }

    if (gIsMirrorMode) {
        SpawnActor<OTrashBin>(FVector(-1765.0f, 45.0f, 195.0f), IRotator(0, 180, 0), 1.0f, bhv);
    } else {
        SpawnActor<OTrashBin>(FVector(-1765.0f, 45.0f, 70.0f), IRotator(0, 0, 0), 1.0f, bhv);
    }

    if ((gGamestate != CREDITS_SEQUENCE) && (gModeSelection != TIME_TRIALS)) {
        SpawnActor<OBat>(FVector(0,0,0), IRotator(0, 0, 90));
        SpawnActor<OBoos>(5, IPathSpan(180, 190), IPathSpan(200, 210), IPathSpan(280, 290));
        SpawnActor<OBoos>(5, IPathSpan(490, 500), IPathSpan(510, 520), IPathSpan(620, 630));
    }

    if (gModeSelection == VERSUS) {
        SpawnActor<OBombKart>(0, 110, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 190, 1, 0.8333333f);
        SpawnActor<OBombKart>(0, 250, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 475, 1, 0.8333333f);
        SpawnActor<OBombKart>(0, 610, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
    }
}

void BansheeBoardwalk::InitTrackObjects() {
    size_t objectId = 0;
    if (gGamestate != CREDITS_SEQUENCE) {
        init_object(indexObjectList1[2], 0);
    }
}

void BansheeBoardwalk::TickTrackObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        func_8007E4C4();
        if (gModeSelection != TIME_TRIALS) {
            // update_bat();
        }
        // wrapper_update_boos();
    }
}

void BansheeBoardwalk::DrawTrackObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        // render_object_bat(cameraId);
        // render_object_boos(cameraId);
    }
}

void BansheeBoardwalk::SomeSounds() {
}

void BansheeBoardwalk::WhatDoesThisDo(Player* player, int8_t playerId) {
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

void BansheeBoardwalk::WhatDoesThisDoAI(Player* player, int8_t playerId) {
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

void BansheeBoardwalk::Draw(ScreenContext* arg0) {
    Camera* camera = arg0->camera;
    Mat4 spCC;
    UNUSED s32 pad[6];
    Vec3f spA8;
    UNUSED s32 pad2[6];

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    // d_course_banshee_boardwalk_packed_dl_7228
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_7228);

    gSPFogPosition(gDisplayListHead++, gFogMin, gFogMax);

    gDPPipeSync(gDisplayListHead++);

    gSPClearGeometryMode(gDisplayListHead++,
                         G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_banshee_boardwalk_packed_dl_5CD0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_5CD0);
    // d_course_banshee_boardwalk_packed_dl_4E60
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_4E60);
    // d_course_banshee_boardwalk_packed_dl_69B0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_69B0);

    render_track_sections(banshee_boardwalk_dls, arg0);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH);
    // d_course_banshee_boardwalk_packed_dl_580
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_580);
    // d_course_banshee_boardwalk_packed_dl_60
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_60);
    // d_course_banshee_boardwalk_packed_dl_540
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_540);

    if (camera->pos[1] < -20.0f) {
        // d_course_banshee_boardwalk_packed_dl_6310
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_6310);
    }
    spA8[0] = camera->pos[0];
    spA8[1] = -82.0f;
    spA8[2] = camera->pos[2];
    mtxf_translate(spCC, spA8);
    render_set_position(spCC, 0);

    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_dl_B278);
    gDPPipeSync(gDisplayListHead++);
}

void BansheeBoardwalk::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*) (d_course_banshee_boardwalk_dl_B308));
}

void BansheeBoardwalk::ScrollingTextures() {
    D_802B87BC++;

    if (D_802B87BC >= 0x100) {
        D_802B87BC = 0;
    }
    find_and_set_tile_size((uintptr_t) LOAD_ASSET_RAW(d_course_banshee_boardwalk_dl_B278), 0, D_802B87BC);
}

void BansheeBoardwalk::Waypoints(Player* player, int8_t playerId) {
    s16 waypoint = gNearestPathPointByPlayerId[playerId];
    if ((waypoint >= 0x12C) && (waypoint < 0x13C)) {
        player->nearestPathPointId = 0x12CU;
    } else {
        player->nearestPathPointId = gNearestPathPointByPlayerId[playerId];
        if (player->nearestPathPointId < 0) {
            player->nearestPathPointId = gPathCountByPathIndex[0] + player->nearestPathPointId;
        }
    }
}

void BansheeBoardwalk::DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot,
                                 uint16_t playerDirection) {
    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    // d_course_banshee_boardwalk_packed_dl_878
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_packed_dl_878);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    gDPPipeSync(gDisplayListHead++);
}

void BansheeBoardwalk::CreditsSpawnActors() {
    find_vtx_and_set_colours((Gfx*) d_course_banshee_boardwalk_packed_dl_878, 0x32, 0, 0, 0);
}

void BansheeBoardwalk::Destroy() {
}
