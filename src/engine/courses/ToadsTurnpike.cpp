#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "ToadsTurnpike.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/toads_turnpike_data.h"

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

    Props.PathSizes = {0x3E8, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = d_course_toads_turnpike_unknown_waypoints;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = d_course_toads_turnpike_track_waypoints;
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

void ToadsTurnpike::LoadTextures() {
}

void ToadsTurnpike::SpawnActors() {
    D_801625EC = 43;
    D_801625F4 = 13;
    D_801625F0 = 4;
    D_802B87B0 = 993;
    D_802B87B4 = 1000;
    parse_course_displaylists(d_course_toads_turnpike_addr);
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void ToadsTurnpike::Init() {}
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

void ToadsTurnpike::SpawnBombKarts() {
    World* world = GetWorld();

    if (world) {
        world->SpawnObject(std::make_unique<OBombKart>(40, 3, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(100, 3, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(265, 3, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(285, 1, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(420, 1, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    }
}

// Positions the finishline on the minimap
void ToadsTurnpike::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void ToadsTurnpike::SetStaffGhost() {}

void ToadsTurnpike::BeginPlay() {}

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
    s16 trainCarYRot;
    UNUSED Vec3f pad;
    TrainCarStuff* tempLocomotive;
    TrainCarStuff* tempTender;
    TrainCarStuff* tempPassengerCar;
    Vec3s trainCarRot;
    VehicleStuff* tempBoxTruck;
    VehicleStuff* tempSchoolBus;
    VehicleStuff* tempTankerTruck;
    VehicleStuff* tempCar;
    PaddleBoatStuff* tempPaddleWheelBoat;
    Vec3s paddleWheelBoatRot;
    s32 loopIndex;
    s32 loopIndex2;
    f32 origXPos;
    f32 origZPos;

    for (loopIndex = 0; loopIndex < NUM_RACE_BOX_TRUCKS; loopIndex++) {
        tempBoxTruck = &gBoxTruckList[loopIndex];
        spawn_vehicle_on_road(tempBoxTruck);
        tempBoxTruck->actorIndex = add_actor_to_empty_slot(tempBoxTruck->position, tempBoxTruck->rotation,
                                                            tempBoxTruck->velocity, ACTOR_BOX_TRUCK);
    }
    for (loopIndex = 0; loopIndex < NUM_RACE_SCHOOL_BUSES; loopIndex++) {
        tempSchoolBus = &gSchoolBusList[loopIndex];
        spawn_vehicle_on_road(tempSchoolBus);
        tempSchoolBus->actorIndex = add_actor_to_empty_slot(tempSchoolBus->position, tempSchoolBus->rotation,
                                                            tempSchoolBus->velocity, ACTOR_SCHOOL_BUS);
    }
    for (loopIndex = 0; loopIndex < NUM_RACE_TANKER_TRUCKS; loopIndex++) {
        tempTankerTruck = &gTankerTruckList[loopIndex];
        spawn_vehicle_on_road(tempTankerTruck);
        tempTankerTruck->actorIndex =
            add_actor_to_empty_slot(tempTankerTruck->position, tempTankerTruck->rotation,
                                    tempTankerTruck->velocity, ACTOR_TANKER_TRUCK);
    }
    for (loopIndex = 0; loopIndex < NUM_RACE_CARS; loopIndex++) {
        tempCar = &gCarList[loopIndex];
        spawn_vehicle_on_road(tempCar);
        tempCar->actorIndex =
            add_actor_to_empty_slot(tempCar->position, tempCar->rotation, tempCar->velocity, ACTOR_CAR);
    }
}

void ToadsTurnpike::UpdateVehicles() {
    update_vehicle_box_trucks();
    update_vehicle_school_bus();
    update_vehicle_tanker_trucks();
    update_vehicle_cars();
}

void ToadsTurnpike::GenerateCollision() {
    D_801625EC = 43;
    D_801625F4 = 13;
    D_801625F0 = 4;
    D_802B87B0 = 993;
    D_802B87B4 = 1000;
    parse_course_displaylists(d_course_toads_turnpike_addr);
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void ToadsTurnpike::Destroy() { }
