#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "KalimariDesert.h"
#include "World.h"
#include "engine/actors/AFinishline.h"
#include "engine/objects/BombKart.h"
#include "kalimari_desert_data.h"
#include "engine/vehicles/Utils.h"

#include "engine/vehicles/Train.h"
#include "engine/vehicles/Vehicle.h"

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
extern const char* kalimari_desert_dls[];
}

const course_texture kalimari_desert_textures[] = {
    { gTexture6684F8, 0x010D, 0x0800, 0x0 },           { gTextureSignLuigis0, 0x0287, 0x1000, 0x0 },
    { gTextureSignLuigis1, 0x02AF, 0x1000, 0x0 },      { gTextureSignMarioStar0, 0x02D2, 0x1000, 0x0 },
    { gTextureSignMarioStar1, 0x02B1, 0x1000, 0x0 },   { gTextureSignNintendoRed0, 0x02A6, 0x1000, 0x0 },
    { gTextureSignNintendoRed1, 0x02F7, 0x1000, 0x0 }, { gTexture67490C, 0x021C, 0x0800, 0x0 },
    { gTextureSignYoshi, 0x04DF, 0x1000, 0x0 },        { gTextureCheckerboardBlueGray, 0x04A1, 0x1000, 0x0 },
    { gTexture646CA8, 0x073A, 0x1000, 0x0 },           { gTexture6473E4, 0x05AD, 0x1000, 0x0 },
    { gTexture647994, 0x05B5, 0x1000, 0x0 },           { gTexture668920, 0x03D9, 0x0800, 0x0 },
    { gTextureRailroadTrack, 0x0B5B, 0x1000, 0x0 },    { gTextureRailroadCrossingTrack, 0x0208, 0x1000, 0x0 },
    { gTexture67291C, 0x059C, 0x0800, 0x0 },           { gTextureFenceBarbedWire, 0x021E, 0x1000, 0x0 },
    { gTexture67D304, 0x091C, 0x1000, 0x0 },           { gTexture67E010, 0x0415, 0x0800, 0x0 },
    { gTexture67EEAC, 0x0140, 0x0800, 0x0 },           { gTextureSignShellShot0, 0x038C, 0x1000, 0x0 },
    { gTextureSignShellShot1, 0x0247, 0x1000, 0x0 },   { gTextureSignKoopaAir0, 0x0360, 0x1000, 0x0 },
    { gTextureSignKoopaAir1, 0x0304, 0x1000, 0x0 },    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

KalimariDesert::KalimariDesert() {
    this->vtx = d_course_kalimari_desert_vertex;
    this->gfx = d_course_kalimari_desert_packed_dls;
    this->gfxSize = 5328;
    Props.textures = kalimari_desert_textures;
    Props.MinimapTexture = gTextureCourseOutlineKalimariDesert;
    Props.MinimapDimensions =
        IVector2D(ResourceGetTexWidthByName(Props.MinimapTexture), ResourceGetTexHeightByName(Props.MinimapTexture));

    Props.Name = "kalimari desert";
    Props.DebugName = "desert";
    Props.CourseLength = "753m";
    Props.AIBehaviour = D_0D009260;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 10.0f;
    Props.FarPersp = 7000.0f;

    Props.PathSizes = { 0x2BC, 1, 1, 1, 0x226, 0, 0, 0, 0, 0, 0 };

    Props.D_0D009418[0] = 4.1666665f;
    Props.D_0D009418[1] = 5.5833334f;
    Props.D_0D009418[2] = 6.1666665f;
    Props.D_0D009418[3] = 6.75f;

    Props.D_0D009568[0] = 3.75f;
    Props.D_0D009568[1] = 5.1666665f;
    Props.D_0D009568[2] = 5.75f;
    Props.D_0D009568[3] = 6.3333334f;

    Props.D_0D0096B8[0] = 3.3333332f;
    Props.D_0D0096B8[1] = 3.9166667f;
    Props.D_0D0096B8[2] = 4.5f;
    Props.D_0D0096B8[3] = 5.0833334f;

    Props.D_0D009808[0] = 3.75f;
    Props.D_0D009808[1] = 5.1666665f;
    Props.D_0D009808[2] = 5.75f;
    Props.D_0D009808[3] = 6.3333334f;

    Props.PathTable[0] = (TrackWaypoint*) LOAD_ASSET_RAW(d_course_kalimari_desert_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*) LOAD_ASSET_RAW(d_course_kalimari_desert_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gKalimariDesertClouds;
    Props.CloudList = gKalimariDesertClouds;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = { 195, 231, 255 };
    Props.Skybox.BottomRight = { 255, 192, 0 };
    Props.Skybox.BottomLeft = { 255, 192, 0 };
    Props.Skybox.TopLeft = { 195, 231, 255 };
    Props.Skybox.FloorTopRight = { 255, 192, 0 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 255, 192, 0 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_KALIMARI_DESERT;
}

void KalimariDesert::Load() {
    Course::Load();

    parse_course_displaylists((TrackSectionsI*) LOAD_ASSET_RAW(d_course_kalimari_desert_addr));
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void KalimariDesert::LoadTextures() {
    dma_textures(gTextureCactus1Left, 0x0000033EU, 0x00000800U);
    dma_textures(gTextureCactus1Right, 0x000002FBU, 0x00000800U);
    dma_textures(gTextureCactus2Left, 0x000002A8U, 0x00000800U);
    dma_textures(gTextureCactus2Right, 0x00000374U, 0x00000800U);
    dma_textures(gTextureCactus3, 0x000003AFU, 0x00000800U);
}

void KalimariDesert::SpawnActors() {
    struct RailroadCrossing* rrxing;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };

    gWorldInstance.AddActor(new AFinishline());

    spawn_foliage((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_kalimari_desert_cactus_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_kalimari_desert_item_box_spawns));

    Vec3f crossingPos = { -2500, 2, 2355 };
    Vec3f crossingPos2 = { -1639, 2, 68 };
    uintptr_t* crossing1 = (uintptr_t*) gWorldInstance.AddCrossing(crossingPos, 306, 310, 900.0f, 650.0f);
    uintptr_t* crossing2 = (uintptr_t*) gWorldInstance.AddCrossing(crossingPos2, 176, 182, 900.0f, 650.0f);

    vec3f_set(position, -1680.0f, 2.0f, 35.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) GET_ACTOR(
        add_actor_to_empty_slot(position, rotation, velocity, ACTOR_RAILROAD_CROSSING));
    rrxing->crossingTrigger = crossing2;
    vec3f_set(position, -1600.0f, 2.0f, 35.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) GET_ACTOR(
        add_actor_to_empty_slot(position, rotation, velocity, ACTOR_RAILROAD_CROSSING));
    rrxing->crossingTrigger = crossing2;
    vec3s_set(rotation, 0, -0x2000, 0);
    vec3f_set(position, -2459.0f, 2.0f, 2263.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) GET_ACTOR(
        add_actor_to_empty_slot(position, rotation, velocity, ACTOR_RAILROAD_CROSSING));
    rrxing->crossingTrigger = crossing1;
    vec3f_set(position, -2467.0f, 2.0f, 2375.0f);
    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) GET_ACTOR(
        add_actor_to_empty_slot(position, rotation, velocity, ACTOR_RAILROAD_CROSSING));
    rrxing->crossingTrigger = crossing1;
}

// Likely sets minimap boundaries
void KalimariDesert::MinimapSettings() {
    D_8018D2C0[0] = 263;
    D_8018D2D8[0] = 165;
    D_8018D220 = reinterpret_cast<uint8_t(*)[1024]>(dma_textures(gTextureExhaust5, 0x443, 0x1000));
    D_8018D2A0 = 0.015f;
    D_8018D2E0 = 55;
    D_8018D2E8 = 27;
}

void KalimariDesert::InitCourseObjects() {
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

void KalimariDesert::SomeSounds() {
}

void KalimariDesert::WhatDoesThisDo(Player* player, int8_t playerId) {
}

void KalimariDesert::WhatDoesThisDoAI(Player* player, int8_t playerId) {
}

// Positions the finishline on the minimap
void KalimariDesert::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY,
                            (u8*) common_texture_minimap_finish_line);
}

void KalimariDesert::SpawnVehicles() {
    generate_train_waypoints();

    s32 centerWaypoint = 160;

    // Spawn two trains
    for (size_t i = 0; i < _numTrains; ++i) {
        uint32_t waypoint = CalculateWaypointDistribution(i, _numTrains, gVehicle2DWaypointLength, centerWaypoint);

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

        gWorldInstance.AddVehicle(new ATrain(_tender, _numCarriages, 2.5f, waypoint));
    }

    if (gModeSelection == VERSUS) {
        Vec3f pos = { 0, 0, 0 };

        gWorldInstance.AddBombKart(pos, &D_80164550[0][50], 50, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][138], 138, 1, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][280], 280, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][404], 404, 1, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][510], 510, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f);
    }
}

void KalimariDesert::Render(struct UnkStruct_800DC5EC* arg0) {
    func_802B5D64(D_800DC610, D_802B87D4, 0, 1);

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_kalimari_desert_packed_dl_71C8
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x070071C8)));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_course_segments(kalimari_desert_dls, arg0);
    // d_course_kalimari_desert_packed_dl_1ED8
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07001ED8)));
    // d_course_kalimari_desert_packed_dl_1B18
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07001B18)));
    // d_course_kalimari_desert_packed_dl_8330
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07008330)));
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_kalimari_desert_packed_dl_998
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07000998)));
    // d_course_kalimari_desert_packed_dl_270
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07000270)));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void KalimariDesert::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*) (d_course_kalimari_desert_dl_22E00));
}

void KalimariDesert::Collision() {
}

void KalimariDesert::Destroy() {
}
