#include <libultraship.h>
#include <libultra/gbi.h>
#include "CoreMath.h"
#include <vector>
#include <memory>

#include "TestCourse.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/actors/BowserStatue.h"
#include "engine/actors/Ship.h"
#include "engine/actors/SpaghettiShip.h"
#include "engine/actors/Starship.h"
#include "engine/objects/Object.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/mario_raceway/mario_raceway_data.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_displaylists.h"
#include "engine/actors/Tree.h"
#include "engine/actors/Cloud.h"
#include "engine/vehicles/Train.h"
#include "engine/objects/Trophy.h"
#include "engine/objects/CheepCheep.h"
#include "engine/objects/Snowman.h"
#include "engine/objects/TrashBin.h"
#include "engine/objects/Hedgehog.h"
#include "engine/objects/Flagpole.h"
#include "engine/objects/HotAirBalloon.h"
#include "engine/objects/Crab.h"
#include "engine/objects/Boos.h"
#include "engine/objects/GrandPrixBalloons.h"
#include "engine/objects/Thwomp.h"

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
    #include "course.h"
    extern Gfx test_course_dls[];
    extern Vtx mario_Plane_001_mesh_vtx_1[];
    extern Gfx mario_Plane_001_mesh[];
    extern TrackPathPoint test_course_path[];
    extern TrackSections test_course_addr[];
}

TestCourse::TestCourse() {
    Props.Minimap.Texture = minimap_mario_raceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 260;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 6;
    Props.Minimap.PlayerY = 28;
    Props.Minimap.PlayerScaleFactor = 0.022f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    Props.Minimap.Colour = {255, 255, 255};
    ResizeMinimap(&Props.Minimap);

    Id = "mk:test_course";

    Props.SetText(Props.Name, "Test Course", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "test track", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "100m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D008F28;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {51, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = test_course_path;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = test_course_path;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) gTextureExhaust5;
    Props.Clouds = gKalimariDesertClouds;
    Props.CloudList = gLuigiRacewayClouds;

    Props.Skybox.TopRight = {120, 140, 188};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {40, 184, 208};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_WARIO_STADIUM;
}

void TestCourse::Load() {
    Course::Load(mario_Plane_001_mesh_vtx_1, NULL);

    if (gIsMirrorMode != 0) {
        InvertTriangleWinding(mario_Plane_001_mesh);
    }

    generate_collision_mesh_with_defaults(mario_Plane_001_mesh);

    parse_course_displaylists((TrackSections*)test_course_addr);
    func_80295C6C();
    Props.WaterLevel = gCourseMinY - 10.0f;
}

void TestCourse::UnLoad() {
}

void TestCourse::BeginPlay() {
    struct ActorSpawnData itemboxes[] = {
        {   200, 1500, 200 , 0},
        {   350, 2500, 300 , 1},
        {   400, 2000, 350 , 2},
        {    40, 0, -800, 0},
        {    -40, 0, -800, 0},
        {    0, 0, -800, 0},
        {    999, 6, 482, 0},
        {    1064, 8, 275, {0}},
        {   1028, 5, -39 , {0}},
        {    320, 0, 1020, {0}},
        {   293, 0, 950, {0}},
        {{ -32768, 0,    0 }, {0}},
    };

    struct ActorSpawnData rocks[] = {
        {{   200, 1500, 200 }, {0}},
        {{   350, 2500, 300 }, {1}},
        {{   400, 2000, 350 }, {2}},
        {{ -32768,   0,   0 }, {0}},
    };

    spawn_all_item_boxes(itemboxes);
    spawn_falling_rocks(rocks);

    Vec3f test = {-100, 0, -150};
    Vec3s rot = {0, 0, 0};
    Vec3f vel = {0, 0, 0};

    add_actor_to_empty_slot(test, rot, vel, ACTOR_TREE_MARIO_RACEWAY);

    struct RailroadCrossing* rrxing;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };
    vec3f_set(position, 50.0f, 2.0f, 50.0f);

    Vec3f crossingPos = {0, 2, 0};
    uintptr_t* crossing1 = (uintptr_t*) gWorldInstance.AddCrossing(crossingPos, 0, 2, 900.0f, 650.0f);

    position[0] *= gCourseDirection;
    rrxing = (struct RailroadCrossing*) GET_ACTOR(add_actor_to_empty_slot(position, rotation, velocity,
                                                                            ACTOR_RAILROAD_CROSSING));
    rrxing->crossingTrigger = crossing1;

    Vec3f pos = {0, 80, 0};
    // gWorldInstance.AddActor(new ACloud(pos));

    // gWorldInstance.AddActor(new OSeagull(0, pos));
    // gWorldInstance.AddActor(new OSeagull(1, pos));
    // gWorldInstance.AddActor(new OSeagull(2, pos));
    // gWorldInstance.AddActor(new OSeagull(3, pos));
    // gWorldInstance.AddObject(new OCheepCheep(FVector(0, 40, 0), OCheepCheep::CheepType::RACE, IPathSpan(0, 10)));
    OTrophy::Spawn(FVector(0,0,0), OTrophy::TrophyType::GOLD, OTrophy::Behaviour::GO_FISH);
    //gWorldInstance.AddObject(new OSnowman(FVector(0, 0, 0)));
    //gWorldInstance.AddObject(new OTrashBin(FVector(0.0f, 0.0f, 0.0f), IRotator(0, 90, 0), 1.0f, OTrashBin::Behaviour::MUNCHING));

    //gWorldInstance.AddObject(new OHedgehog(FVector(0, 0, 0), FVector2D(0, -200), 9));
    //gWorldInstance.AddObject(new OFlagpole(FVector(0, 0, -200), 0x400));
//    gWorldInstance.AddObject(new OHotAirBalloon(FVector(0.0, 20.0f, -200.0f)));

    //gWorldInstance.AddObject(new OCrab(FVector2D(0, 0), FVector2D(0, -200)));
//    gWorldInstance.AddActor(new ABowserStatue(FVector(-200, 0, 0), ABowserStatue::Behaviour::CRUSH));

//    gWorldInstance.AddObject(new OBoos(10, IPathSpan(0, 5), IPathSpan(18, 23), IPathSpan(25, 50)));
    //OThwomp::Spawn(0, 0, 0, 1.0f, 0, 1, 7);

    //gWorldInstance.AddTrain(ATrain::TenderStatus::HAS_TENDER, 5, 2.5f, 0);
    //gWorldInstance.AddTrain(ATrain::TenderStatus::HAS_TENDER, 5, 2.5f, 8);

    OBombKart::Spawn(0, 25, 4, 0.8333333f);
    OBombKart::Spawn(0, 45, 4, 0.8333333f);

   // gWorldInstance.AddActor(new AShip(FVector(0, 0, 0), AShip::Skin::SHIP3));

//    OGrandPrixBalloons::Spawn(FVector(0, 0, 0));
}

void TestCourse::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
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

void TestCourse::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
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

void TestCourse::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_big_donut_packed_dl_DE8
    }
    gSPDisplayList(gDisplayListHead++, mario_Plane_001_mesh);
}

bool TestCourse::IsMod() {
    return true;
}
