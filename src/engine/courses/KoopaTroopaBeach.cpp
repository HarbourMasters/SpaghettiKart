#include <libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <vector>
#include <memory>

#include "KoopaTroopaBeach.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Crab.h"
#include "assets/models/tracks/koopa_troopa_beach/koopa_troopa_beach_data.h"
#include "assets/other/tracks/koopa_troopa_beach/koopa_troopa_beach_data.h"
#include "engine/objects/Seagull.h"

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
    #include "course.h"
    extern const char *d_course_koopa_troopa_beach_dl_list1[148];
    extern const char *koopa_troopa_beach_dls2[148];
    extern s8 gPlayerCount;
}

KoopaTroopaBeach::KoopaTroopaBeach() {
    Props.Minimap.Texture = minimap_koopa_troopa_beach;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 268;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 40;
    Props.Minimap.PlayerY = 21;
    Props.Minimap.PlayerScaleFactor = 0.014f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Id = "mk:koopa_beach";
    Props.SetText(Props.Name, "koopa troopa beach", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "beach", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "691m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D009158;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 1.0f;
    Props.FarPersp = 5000.0f;

    Props.PathSizes = {0x2BC, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_track_waypoints);
    Props.PathTable2[1] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_track_waypoints_2);
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) gTextureExhaust3;
    Props.Clouds = gKoopaTroopaBeachClouds;
    Props.CloudList = gKoopaTroopaBeachClouds;

    Props.Skybox.TopRight = {48, 152, 120};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {48, 152, 120};
    Props.Skybox.FloorTopRight = {48, 152, 120};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {48, 152, 120};
    Props.Sequence = MusicSeq::MUSIC_SEQ_KOOPA_TROOPA_BEACH;

    Props.WaterLevel = 0.0f;
    gWaterVelocity = -0.1f;
    WaterVolumes.push_back({0.8f, 67.0f, 239.0f, 2233.0f, 2405.0f});
}

void KoopaTroopaBeach::Load() {
    Course::Load();
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(d_course_koopa_troopa_beach_dl_list1); i++) {
            InvertTriangleWindingByName(d_course_koopa_troopa_beach_dl_list1[i]);
        }
        for (size_t i = 0; i < ARRAY_COUNT(koopa_troopa_beach_dls2); i++) {
            InvertTriangleWindingByName(koopa_troopa_beach_dls2[i]);
        }
        InvertTriangleWindingByName(d_course_koopa_troopa_beach_packed_dl_9CC0);
        InvertTriangleWindingByName(d_course_koopa_troopa_beach_packed_dl_9688);
        InvertTriangleWindingByName(d_course_koopa_troopa_beach_packed_dl_2C0);
        InvertTriangleWindingByName(d_course_koopa_troopa_beach_packed_dl_9E70);
    }
    parse_course_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_addr));
    func_80295C6C();
    find_vtx_and_set_colours((Gfx*) d_course_koopa_troopa_beach_packed_dl_ADE0, 150, 255, 255, 255);
    find_vtx_and_set_colours((Gfx*) d_course_koopa_troopa_beach_packed_dl_A540, 150, 255, 255, 255);
    find_vtx_and_set_colours((Gfx*) d_course_koopa_troopa_beach_packed_dl_9E70, 150, 255, 255, 255);
    find_vtx_and_set_colours((Gfx*) d_course_koopa_troopa_beach_packed_dl_358, 150, 255, 255, 255);
}

void KoopaTroopaBeach::UnLoad() {
    RestoreTriangleWinding();
}

void KoopaTroopaBeach::BeginPlay() {
    init_actor_hot_air_balloon_item_box(328.0f * gCourseDirection, 70.0f, 2541.0f);
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_item_box_spawns));
    spawn_palm_trees((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_tree_spawn));

    if (gGamestate != CREDITS_SEQUENCE) {
        OCrab::Spawn(FVector2D(-1809, 625), FVector2D(-1666, 594));
        OCrab::Spawn(FVector2D(-1852, 757), FVector2D(-1620, 740));
        OCrab::Spawn(FVector2D(-1478, 1842), FVector2D(-1453, 1833));
        OCrab::Spawn(FVector2D(-1418, 1967), FVector2D(-1455, 1962));
        OCrab::Spawn(FVector2D(-1472, 2112), FVector2D(-1417, 2100));
        OCrab::Spawn(FVector2D(-1389, 2152), FVector2D(-1335, 2136));
        OCrab::Spawn(FVector2D(218, 693), FVector2D(69, 696));
        OCrab::Spawn(FVector2D(235, 528), FVector2D(24, 501));
        OCrab::Spawn(FVector2D(268, 406), FVector2D(101, 394));
        OCrab::Spawn(FVector2D(223, 318), FVector2D(86, 308));
    }

    if (gGamestate == CREDITS_SEQUENCE) {
        for (size_t i = 0; i < NUM_SEAGULLS; i++) {
            OSeagull::Spawn(FVector(-360.0f, 60.0f, -1300.0f));
        }
    } else { // Normal gameplay
        for (size_t i = 0; i < 4; i++) {
            OSeagull::Spawn(FVector(-985.0f, 15.0f, 1200.0f));
        }

        for (size_t i = 0; i < 6; i++) {
            OSeagull::Spawn(FVector(328.0f, 20.0f, 2541.0f));
        }
    }

    if (gModeSelection == VERSUS) {
        OBombKart::Spawn(0, 60, 1, 0.8333333f);
        OBombKart::Spawn(0, 120, 1, 0.8333333f);
        OBombKart::Spawn(0, 200, 3, 0.8333333f);
        OBombKart::Spawn(0, 280, 1, 0.8333333f);
        OBombKart::Spawn(0, 435, 3, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
    }
}

void KoopaTroopaBeach::InitCourseObjects() {
}

void KoopaTroopaBeach::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        //update_crabs();
    }
    if ((gPlayerCount == 1) || (gPlayerCount == 2) || (gGamestate == CREDITS_SEQUENCE)) {
        //update_seagulls();
    }
}

void KoopaTroopaBeach::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        //render_object_crabs(cameraId);
    }
    if (gGamestate != CREDITS_SEQUENCE) {

        if ((gPlayerCount == 1) || (gPlayerCount == 2)) {
            //render_object_seagulls(cameraId);
        }
    } else {
        //render_object_seagulls(cameraId);
    }
}

void KoopaTroopaBeach::SomeSounds() {
    vec3f_set(D_8015F738, 153.0f, 0.0f, 2319.0f);
    func_800C9D80(D_8015F738, D_802B91C8, 0x51028001);
}

void KoopaTroopaBeach::WhatDoesThisDo(Player* player, int8_t playerId) {}

void KoopaTroopaBeach::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void KoopaTroopaBeach::Render(struct UnkStruct_800DC5EC* arg0) {
    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_koopa_troopa_beach_packed_dl_9CC0
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_koopa_troopa_beach_packed_dl_9CC0);
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_koopa_troopa_beach_packed_dl_9688
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_koopa_troopa_beach_packed_dl_9688);
    render_course_segments((const char**)d_course_koopa_troopa_beach_dl_list1, arg0);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_koopa_troopa_beach_packed_dl_2C0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_koopa_troopa_beach_packed_dl_2C0);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPPipeSync(gDisplayListHead++);
}

void KoopaTroopaBeach::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_koopa_troopa_beach_dl_18D68));
}

void KoopaTroopaBeach::SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    func_8003E37C(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void KoopaTroopaBeach::ScrollingTextures() {
    // clang-format off
    // This flips the velocity from 0.1f to -0.1f
    if (gWaterVelocity < 0.0f) {
        if (Props.WaterLevel < -20.0f) { gWaterVelocity *= -1.0f; }
    } else {
        if (Props.WaterLevel > 0.0f) { gWaterVelocity *= -1.0f; }
    }
    // clang-format on
    Props.WaterLevel += gWaterVelocity;

    D_802B87BC += 9;
    if (D_802B87BC > 255) {
        D_802B87BC = 0;
    }
    D_802B87C4 += 3;
    if (D_802B87C4 > 255) {
        D_802B87C4 = 0;
    }
    // waterfall animation
    // d_course_koopa_troopa_beach_packed_dl_9D58
    find_and_set_tile_size((uintptr_t) d_course_koopa_troopa_beach_packed_dl_9D58, 0, D_802B87BC);
    // d_course_koopa_troopa_beach_packed_dl_9CD0
    find_and_set_tile_size((uintptr_t) d_course_koopa_troopa_beach_packed_dl_9CD0, 0, D_802B87C4);
    D_802B87CC = random_int(300) / 40;
    if (D_802B87C8 < 0) {
        D_802B87C8 = random_int(300) / 40;
    } else {
        D_802B87C8 = -(random_int(300) / 40);
    }
    // Waterfall bubbling effect? (unused)
    // d_course_koopa_troopa_beach_packed_dl_2E8
    find_and_set_tile_size((uintptr_t) d_course_koopa_troopa_beach_packed_dl_2E8, D_802B87C8, D_802B87CC);

}

void KoopaTroopaBeach::DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) {
    Vec3f vector;

    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);

    switch (pathCounter) {
        case 22:
        case 23:
        case 29:
        case 30:
        case 31:
        case 37:
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            // d_course_koopa_troopa_beach_packed_dl_9E70
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_koopa_troopa_beach_packed_dl_9E70);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
            break;
    }
    vector[0] = 0.0f;
    vector[1] = Props.WaterLevel;
    vector[2] = 0.0f;

    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    render_course_segments((const char**)koopa_troopa_beach_dls2, screen);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    gDPPipeSync(gDisplayListHead++);
}

void KoopaTroopaBeach::Destroy() {}
