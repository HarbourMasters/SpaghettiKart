#include <libultraship.h>
#include <libultra/gbi.h>
#include "CoreMath.h"
#include <vector>
#include <memory>

#include "YoshiValley.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Hedgehog.h"
#include "engine/objects/Flagpole.h"
#include "assets/models/tracks/yoshi_valley/yoshi_valley_data.h"
#include "assets/other/tracks/yoshi_valley/yoshi_valley_data.h"

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
    extern const char *d_course_yoshi_valley_dl_list[124];
}

YoshiValley::YoshiValley() {
    Props.Minimap.Texture = minimap_yoshi_valley;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.PlayerX = 61;
    Props.Minimap.PlayerY = 38;
    Props.Minimap.PlayerScaleFactor = 0.018f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "yoshi valley", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "maze", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "772m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D0090B8;
    Props.AIMaximumSeparation = 35.0f;
    Props.AIMinimumSeparation = 0.0f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x02B2, 0x02A8, 0x02B2, 0x0320, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints);
    Props.PathTable[1] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints_2);
    Props.PathTable[2] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints_3);
    Props.PathTable[3] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_unknown_waypoints_4);

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints);
    Props.PathTable2[1] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints_2);
    Props.PathTable2[2] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints_3);
    Props.PathTable2[3] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_yoshi_valley_track_waypoints_4);

    Props.CloudTexture = (u8*) gTextureExhaust0;
    Props.Clouds = gYoshiValleyMooMooFarmClouds;
    Props.CloudList = gYoshiValleyMooMooFarmClouds;

    Props.Skybox.TopRight = {113, 70, 255};
    Props.Skybox.BottomRight = {255, 184, 99};
    Props.Skybox.BottomLeft = {255, 184, 99};
    Props.Skybox.TopLeft = {113, 70, 255};
    Props.Skybox.FloorTopRight = {95, 40, 15};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {95, 40, 15};
    Props.Sequence = MusicSeq::MUSIC_SEQ_YOSHI_VALLEY;
}

void YoshiValley::Load() {
    Track::Load();

    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(d_course_yoshi_valley_dl_list); i++) {
            InvertTriangleWindingByName(d_course_yoshi_valley_dl_list[i]);
        }
    }

    Lights1 lights4 = gdSPDefLights1(100, 100, 100, 255, 254, 254, 0, 0, 120);
    set_track_light_direction(&lights4, -0x38F0, 0x1C70, 1);
    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_yoshi_valley_addr));
    func_80295C6C();
    Props.WaterLevel = gTrackMinY - 10.0f;
}

void YoshiValley::BeginPlay() {
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };

    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_yoshi_valley_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_yoshi_valley_item_box_spawns));
    vec3f_set(position, -2300.0f, 0.0f, 634.0f);
    position[0] *= gTrackDirection;
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_YOSHI_EGG);

    if (gGamestate != CREDITS_SEQUENCE) {
        //! @bug Skip spawning in credits due to animation crash for now
        SpawnActor<OFlagpole>(FVector(-902, 70, -1406), 0x3800);
        SpawnActor<OFlagpole>(FVector(-948, 70, -1533), 0x3800);
        SpawnActor<OFlagpole>(FVector(-2170, 0, 723), 0x400);
        SpawnActor<OFlagpole>(FVector(-2193, 0, 761), 0x400);

        SpawnActor<OHedgehog>(FVector(-1683, -80, -88), FVector2D(-1650, -114), 9);
        SpawnActor<OHedgehog>(FVector(-1636, -93, -147), FVector2D(-1661, -151), 9);
        SpawnActor<OHedgehog>(FVector(-1628, -86, -108), FVector2D(-1666, -58), 9);
        SpawnActor<OHedgehog>(FVector(-1676, -69, -30), FVector2D(-1651, -26), 9);
        SpawnActor<OHedgehog>(FVector(-1227, -27, -989), FVector2D(-1194, -999), 26);
        SpawnActor<OHedgehog>(FVector(-1261, -41, -880), FVector2D(-1213, -864), 26);
        SpawnActor<OHedgehog>(FVector(-1342, -60, -830), FVector2D(-1249, -927), 26);
        SpawnActor<OHedgehog>(FVector(-1429, -78, -849), FVector2D(-1347, -866), 26);
        SpawnActor<OHedgehog>(FVector(-1492, -94, -774), FVector2D(-1427, -891), 26);
        SpawnActor<OHedgehog>(FVector(-1453, -87, -784), FVector2D(-1509, -809), 26);
        SpawnActor<OHedgehog>(FVector(-1488, 89, -852), FVector2D(-1464, -822), 26);
        SpawnActor<OHedgehog>(FVector(-1301, 47, -904), FVector2D(-1537, -854), 26);
        SpawnActor<OHedgehog>(FVector(-2587, 56, -259), FVector2D(-2624, -241), 28);
        SpawnActor<OHedgehog>(FVector(-2493, 94, -454), FVector2D(-2505, -397), 28);
        SpawnActor<OHedgehog>(FVector(-2477, 3, -57), FVector2D(-2539, -66), 28);
    }

    if (gModeSelection == VERSUS) {
        // Note that the waypoint values might be unused for Yoshi's Valley. Entered them because
        // the original data has values here.

        // Note that the Y height is calculated automatically to place the kart on the surface
        SpawnActor<OBombKart>(FVector(-1533, 0, -682), 0, 0.8333333f);
        SpawnActor<OBombKart>(FVector(-1565, 0, -619), 0, 0.8333333f);
        SpawnActor<OBombKart>(FVector(-1529, 0, -579), 0, 0.8333333f);
        SpawnActor<OBombKart>(FVector(-1588, 0, -534), 0, 0.8333333f);
        SpawnActor<OBombKart>(FVector(-1598, 0, -207), 0, 0.8333333f);
        SpawnActor<OBombKart>(FVector(-1646, 0, -147), 0, 0.8333333f);
        SpawnActor<OBombKart>(FVector(-2532, 0, -445), 0, 0.8333333f);
    }
}

void YoshiValley::InitTrackObjects() {
}

void YoshiValley::TickTrackObjects() {
}

void YoshiValley::DrawTrackObjects(s32 cameraId) {
}

void YoshiValley::SomeSounds() {
}

void YoshiValley::WhatDoesThisDo(Player* player, int8_t playerId) {}

void YoshiValley::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void YoshiValley::Draw(ScreenContext* arg0) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    render_track_sections(d_course_yoshi_valley_dl_list, arg0);
    gDPPipeSync(gDisplayListHead++);
}

void YoshiValley::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_yoshi_valley_dl_18020));
}

void YoshiValley::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = gCopyNearestWaypointByPlayerId[playerId];
}

void YoshiValley::ScrollingTextures() {}

void YoshiValley::CreditsSpawnActors() {
    Vec3f position;
    Vec3f velocity = { 0, 0, 0 };
    Vec3s rotation = { 0, 0, 0 };

    vec3f_set(position, -2300.0f, 0.0f, 634.0f);
    position[0] *= gTrackDirection;
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_YOSHI_EGG);
}

void YoshiValley::Destroy() {}
