#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "KalimariDesert.h"
#include "engine/World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/kalimari_desert/kalimari_desert_data.h"
#include "assets/other/tracks/kalimari_desert/kalimari_desert_data.h"
#include "engine/vehicles/Utils.h"

#include "engine/vehicles/Train.h"

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
    extern const char *kalimari_desert_dls[80];
}

KalimariDesert::KalimariDesert() {
    Props.Minimap.Texture = minimap_kalimari_desert;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 263;
    Props.Minimap.Pos[0].Y = 165;
    Props.Minimap.PlayerX = 55;
    Props.Minimap.PlayerY = 27;
    Props.Minimap.PlayerScaleFactor = 0.015f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 4.0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "kalimari desert", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "desert", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "753m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D009260;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 10.0f;
    Props.FarPersp = 7000.0f;

    Props.PathSizes = {0x2BC, 1, 1, 1, 0x226, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_kalimari_desert_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_kalimari_desert_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) gTextureExhaust5;
    Props.Clouds = gKalimariDesertClouds;
    Props.CloudList = gKalimariDesertClouds;

    Props.Skybox.TopRight = {195, 231, 255};
    Props.Skybox.BottomRight = {255, 192, 0};
    Props.Skybox.BottomLeft = {255, 192, 0};
    Props.Skybox.TopLeft = {195, 231, 255};
    Props.Skybox.FloorTopRight = {255, 192, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 192, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_KALIMARI_DESERT;
}

void KalimariDesert::Load() {
    Track::Load();

    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(kalimari_desert_dls); i++) {
            InvertTriangleWindingByName(kalimari_desert_dls[i]);
        }
        InvertTriangleWindingByName(d_course_kalimari_desert_packed_dl_71C8);

        InvertTriangleWindingByName(d_course_kalimari_desert_packed_dl_1ED8);
        InvertTriangleWindingByName(d_course_kalimari_desert_packed_dl_1B18);
        InvertTriangleWindingByName(d_course_kalimari_desert_packed_dl_8330);
        InvertTriangleWindingByName(d_course_kalimari_desert_packed_dl_998);
        InvertTriangleWindingByName(d_course_kalimari_desert_packed_dl_270);
    }

    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_kalimari_desert_addr));
    func_80295C6C();
    Props.WaterLevel = gTrackMinY - 10.0f;
}

void KalimariDesert::BeginPlay() {
    struct RailroadCrossing* rrxing;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };

    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_kalimari_desert_cactus_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_kalimari_desert_item_box_spawns));

    if (gGamestate != CREDITS_SEQUENCE) {

        Vec3f crossingPos = {-2500, 2, 2355};
        Vec3f crossingPos2 = {-1639, 2, 68};
        uintptr_t* crossing1 = (uintptr_t*) GetWorld()->AddCrossing(crossingPos, 306, 310, 900.0f, 650.0f);
        uintptr_t* crossing2 = (uintptr_t*) GetWorld()->AddCrossing(crossingPos2, 176, 182, 900.0f, 650.0f);

        vec3f_set(position, -1680.0f, 2.0f, 35.0f);
        position[0] *= gTrackDirection;
        rrxing = (struct RailroadCrossing*) GET_ACTOR(add_actor_to_empty_slot(position, rotation, velocity,
                                                                                ACTOR_RAILROAD_CROSSING));
        rrxing->crossingTrigger = crossing2;
        vec3f_set(position, -1600.0f, 2.0f, 35.0f);
        position[0] *= gTrackDirection;
        rrxing = (struct RailroadCrossing*) GET_ACTOR(add_actor_to_empty_slot(position, rotation, velocity,
                                                                                ACTOR_RAILROAD_CROSSING));
        rrxing->crossingTrigger = crossing2;

        // Original game forgot to put gTrackDirection to face the crossing the right direction in extra mode
        vec3s_set(rotation, 0, -0x2000 * gTrackDirection, 0);
        vec3f_set(position, -2459.0f, 2.0f, 2263.0f);
        position[0] *= gTrackDirection;
        rrxing = (struct RailroadCrossing*) GET_ACTOR(add_actor_to_empty_slot(position, rotation, velocity,
                                                                                ACTOR_RAILROAD_CROSSING));
        rrxing->crossingTrigger = crossing1;
        vec3f_set(position, -2467.0f, 2.0f, 2375.0f);
        position[0] *= gTrackDirection;
        rrxing = (struct RailroadCrossing*) GET_ACTOR(add_actor_to_empty_slot(position, rotation, velocity,
                                                                                ACTOR_RAILROAD_CROSSING));
        rrxing->crossingTrigger = crossing1;


        generate_train_path();

        s32 centerWaypoint = 160;

        _numTrains = CVarGetInteger("gNumTrains", 2);
        _numCarriages = CVarGetInteger("gNumCarriages", 5);
        _tender = (ATrain::TenderStatus)CVarGetInteger("gHasTender", 1);

        // Spawn two trains
        for (size_t i = 0; i < _numTrains; ++i) {
            // outputs 160 for train 1 and 392 for train 2.
            // If using more trains, it wraps the value to always output a valid waypoint.
            uint32_t waypoint = CalculateWaypointDistribution(i, _numTrains, gVehiclePathSize, centerWaypoint);

            if (CVarGetInteger("gMultiplayerNoFeatureCuts", 0) == false) {
                // Multiplayer modes have no tender and no carriages
                if (gActiveScreenMode != SCREEN_MODE_1P) {
                    _tender = ATrain::TenderStatus::NO_TENDER;
                    _numCarriages = 0;
                }

                // 2 player versus mode has a tender and a carriage
                if ((gModeSelection == VERSUS) && (gPlayerCountSelection1 == 2)) {
                    _tender = ATrain::TenderStatus::HAS_TENDER;
                    _numCarriages = 1;
                }
            }

            SpawnActor<ATrain>(_tender, _numCarriages, 2.5f, 0, waypoint, ATrain::SpawnMode::POINT);
        }

        if (gModeSelection == VERSUS) {
            SpawnActor<OBombKart>(0, 50, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 138, 1, 0.8333333f);
            SpawnActor<OBombKart>(0, 280, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 404, 1, 0.8333333f);
            SpawnActor<OBombKart>(0, 510, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
            SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
        }
    }
}

void KalimariDesert::InitTrackObjects() {
    size_t i;
    if (gGamestate != CREDITS_SEQUENCE) {
        find_unused_obj_index(&D_8018CF10);
        init_object(D_8018CF10, 0);
        for (i = 0; i < 50; i++) {
            find_unused_obj_index(&gObjectParticle1[i]);
        }
        for (i = 0; i < 5; i++) {
            find_unused_obj_index(&gObjectParticle2[i]);
        }
        for (i = 0; i < 32; i++) {
            find_unused_obj_index(&gObjectParticle3[i]);
        }
    }
}

void KalimariDesert::SomeSounds() {}

void KalimariDesert::WhatDoesThisDo(Player* player, int8_t playerId) {}

void KalimariDesert::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void KalimariDesert::Draw(ScreenContext* arg0) {
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_kalimari_desert_packed_dl_71C8
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_kalimari_desert_packed_dl_71C8);
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_track_sections(kalimari_desert_dls, arg0);
    // d_course_kalimari_desert_packed_dl_1ED8
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_kalimari_desert_packed_dl_1ED8);
    // d_course_kalimari_desert_packed_dl_1B18
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_kalimari_desert_packed_dl_1B18);
    // d_course_kalimari_desert_packed_dl_8330
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_kalimari_desert_packed_dl_8330);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_kalimari_desert_packed_dl_998
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_kalimari_desert_packed_dl_998);
    // d_course_kalimari_desert_packed_dl_270
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_kalimari_desert_packed_dl_270);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void KalimariDesert::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_kalimari_desert_dl_22E00));
}

void KalimariDesert::Destroy() { }
