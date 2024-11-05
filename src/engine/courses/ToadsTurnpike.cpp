#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "ToadsTurnpike.h"
#include "GameObject.h"
#include "World.h"
#include "engine/vehicles/OBombKart.h"
#include "assets/toads_turnpike_data.h"
#include "engine/actors/AFinishline.h"

#include "engine/vehicles/Utils.h"

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
    #include "code_80086E70.h"
    extern const char *d_course_toads_turnpike_dl_list[];
    extern s16 currentScreenSection;
    extern s8 gPlayerCount;
}

ToadsTurnpike::ToadsTurnpike() {
    this->vtx = d_course_toads_turnpike_vertex;
    this->gfx = d_course_toads_turnpike_packed_dls;
    this->gfxSize = 3427;
    this->textures = toads_turnpike_textures;
    Props.MinimapTexture = gTextureCourseOutlineToadsTurnpike;
    Props.D_800E5548[0] = 128;
    Props.D_800E5548[1] = 64;

    Props.Name = "toad's turnpike";
    Props.DebugName = "highway";
    Props.CourseLength = "1036m";
    Props.AIBehaviour = D_0D009238;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 40;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {1000, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_toads_turnpike_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_toads_turnpike_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gToadsTurnpikeRainbowRoadStars;
    Props.CloudList = gToadsTurnpikeRainbowRoadStars;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {0, 2, 94};
    Props.Skybox.BottomRight = {209, 65, 23};
    Props.Skybox.BottomLeft = {209, 65, 23};
    Props.Skybox.TopLeft = {0, 2, 94};
    Props.Skybox.FloorTopRight = {209, 65, 23};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {209, 65, 23};
}

void ToadsTurnpike::Load() {
    Course::Load();

    D_801625EC = 43;
    D_801625F4 = 13;
    D_801625F0 = 4;
    D_802B87B0 = 993;
    D_802B87B4 = 1000;
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_toads_turnpike_addr));
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void ToadsTurnpike::LoadTextures() {
}

void ToadsTurnpike::SpawnActors() {
    Vec3f pos;
    pos[0] = (gIsMirrorMode != 0) ? D_80164490->posX + 138.0f : D_80164490->posX - 138.0f;
    pos[1] = (f32) (D_80164490->posY - 15);
    pos[2] = D_80164490->posZ;

    gWorldInstance.AddActor(new AFinishline(pos));

    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_toads_turnpike_item_box_spawns));
}

void ToadsTurnpike::InitClouds() {
    init_stars(this->Props.Clouds);
}

void ToadsTurnpike::UpdateClouds(s32 sp1C, Camera* camera) {
    update_stars(sp1C, camera, this->Props.CloudList);
}

// Likely sets minimap boundaries
void ToadsTurnpike::MinimapSettings() {
    D_8018D2A0 = 0.013f;
    D_8018D2C0[0] = 252;
    D_8018D2E0 = 57;
    D_8018D2E8 = 44;
}

void ToadsTurnpike::SomeSounds() {}

void ToadsTurnpike::WhatDoesThisDo(Player* player, int8_t playerId) {
    if ((player->type & PLAYER_CINEMATIC_MODE) != PLAYER_CINEMATIC_MODE) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x1e);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void ToadsTurnpike::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if ((player->type & PLAYER_CINEMATIC_MODE) != PLAYER_CINEMATIC_MODE) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x1E);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

// Positions the finishline on the minimap
void ToadsTurnpike::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void ToadsTurnpike::Render(struct UnkStruct_800DC5EC* arg0) {
    func_802B5D64(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetFogColor(gDisplayListHead++, D_801625EC, D_801625F4, D_801625F0, 0xFF);
    gDPSetCycleType(gDisplayListHead++, G_CYC_2CYCLE);
    gSPFogPosition(gDisplayListHead++, D_802B87B0, D_802B87B4);
    gSPSetGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_PASS2);
    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);

    render_course_segments(d_course_toads_turnpike_dl_list, arg0);

    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_PASS2);
    // d_course_toads_turnpike_packed_dl_0
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07000000));
    // d_course_toads_turnpike_packed_dl_68
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07000068));
    // d_course_toads_turnpike_packed_dl_D8
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x070000D8));
    gSPClearGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void ToadsTurnpike::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_toads_turnpike_dl_23930));
}

void ToadsTurnpike::Collision() {}

void ToadsTurnpike::SpawnVehicles() {
    f32 a = ((gCCSelection * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((gCCSelection * 90.0) / 216.0f) + 2.9166666666666665;
    a /= 2; // Normally vehicle logic is only ran every 2 frames. This slows the vehicles down to match.
    b /= 2;
    uint32_t waypoint;

    if (gModeSelection == TIME_TRIALS) {
        for (size_t i = 0; i < NUM_TIME_TRIAL_BOX_TRUCKS; i++) {
            waypoint = CalculateWaypointDistribution(i, NUM_TIME_TRIAL_BOX_TRUCKS, gWaypointCountByPathIndex[0], 0);
            gWorldInstance.AddTruck(a, b, &D_80164550[0][0], waypoint);
        }

        for (size_t i = 0; i < NUM_TIME_TRIAL_SCHOOL_BUSES; i++) {
            waypoint = CalculateWaypointDistribution(i, NUM_TIME_TRIAL_SCHOOL_BUSES, gWaypointCountByPathIndex[0], 75);
            gWorldInstance.AddBus(a, b, &D_80164550[0][0], waypoint);
        }

        for (size_t i = 0; i < NUM_TIME_TRIAL_TANKER_TRUCKS; i++) {
            waypoint = CalculateWaypointDistribution(i, NUM_TIME_TRIAL_TANKER_TRUCKS, gWaypointCountByPathIndex[0], 50);
            gWorldInstance.AddTankerTruck(a, b, &D_80164550[0][0], waypoint);
        }

        for (size_t i = 0; i < NUM_TIME_TRIAL_CARS; i++) {
            waypoint = CalculateWaypointDistribution(i, NUM_TIME_TRIAL_CARS, gWaypointCountByPathIndex[0], 25);
            gWorldInstance.AddCar(a, b, &D_80164550[0][0], waypoint);
        }
    } else { // All other modes
        for (size_t i = 0; i < NUM_RACE_BOX_TRUCKS; i++) {
            uint32_t waypoint = CalculateWaypointDistribution(i, NUM_RACE_BOX_TRUCKS, gWaypointCountByPathIndex[0], 0);
            gWorldInstance.AddTruck(a, b,  &D_80164550[0][0], waypoint);
        }

        for (size_t i = 0; i < NUM_RACE_SCHOOL_BUSES; i++) {
            waypoint = CalculateWaypointDistribution(i, NUM_RACE_SCHOOL_BUSES, gWaypointCountByPathIndex[0], 75);
            gWorldInstance.AddBus(a, b,&D_80164550[0][0], waypoint);
        }

        for (size_t i = 0; i < NUM_RACE_TANKER_TRUCKS; i++) {
            waypoint = CalculateWaypointDistribution(i, NUM_RACE_TANKER_TRUCKS, gWaypointCountByPathIndex[0], 50);
            gWorldInstance.AddTankerTruck(a, b, &D_80164550[0][0], waypoint);
        }

        for (size_t i = 0; i < NUM_RACE_CARS; i++) {
            waypoint = CalculateWaypointDistribution(i, NUM_RACE_CARS, gWaypointCountByPathIndex[0], 25);
            gWorldInstance.AddCar(a, b, &D_80164550[0][0], waypoint);
        }
    }

    if (gModeSelection == VERSUS) {
        Vec3f pos = {0, 0, 0};

        gWorldInstance.AddBombKart(pos, &D_80164550[0][50], 50, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][100], 100, 1, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][150], 150, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][200], 200, 1, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][250], 250, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f);
    }
}

void ToadsTurnpike::Destroy() { }
