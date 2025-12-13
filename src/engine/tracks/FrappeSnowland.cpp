#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "FrappeSnowland.h"
#include "World.h"
#include "CoreMath.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Snowman.h"
#include "assets/models/tracks/frappe_snowland/frappe_snowland_data.h"
#include "assets/other/tracks/frappe_snowland/frappe_snowland_data.h"
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
    #include "update_objects.h"
    #include "course_offsets.h"
    extern const char *d_course_frappe_snowland_dl_list[68];
    extern s8 gPlayerCount;
}

FrappeSnowland::FrappeSnowland() {
    Props.Minimap.Texture = minimap_frappe_snowland;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 262;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 36;
    Props.Minimap.PlayerY = 40;
    Props.Minimap.PlayerScaleFactor = 0.016f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    Props.Minimap.Colour = {72, 100, 255};
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "frappe snowland", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "snow", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "734m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D0090F8;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x2EE, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_frappe_snowland_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_frappe_snowland_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // not used for frappe
    Props.CloudList = NULL;

    Props.Skybox.TopRight = {28, 11, 90};
    Props.Skybox.BottomRight = {0, 99, 164};
    Props.Skybox.BottomLeft = {0, 9, 164};
    Props.Skybox.TopLeft = {28, 11, 90};
    Props.Skybox.FloorTopRight = {0, 99, 164};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 99, 164};
    Props.Sequence = MusicSeq::MUSIC_SEQ_FRAPPE_SNOWLAND;

    Props.WaterLevel = -50.0f;
}

void FrappeSnowland::Load() {
    Track::Load();

    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(d_course_frappe_snowland_dl_list); i++) {
            InvertTriangleWindingByName(d_course_frappe_snowland_dl_list[i]);
        }

        InvertTriangleWindingByName(d_course_frappe_snowland_packed_dl_65E0);
    }
    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_frappe_snowland_addr));
    func_80295C6C();
}

void FrappeSnowland::BeginPlay() {
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_frappe_snowland_tree_spawns));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_frappe_snowland_item_box_spawns));

    if (gGamestate != CREDITS_SEQUENCE) {
        SpawnActor<OSnowman>(FVector(697, 0, -1684));
        SpawnActor<OSnowman>(FVector(82, 0, -2245));
        SpawnActor<OSnowman>(FVector(27, 5, -2067));
        SpawnActor<OSnowman>(FVector(-656, 0, -1735));
        SpawnActor<OSnowman>(FVector(-1497, 0, -83));
        SpawnActor<OSnowman>(FVector(-1643, 0, -25));
        SpawnActor<OSnowman>(FVector(-1547, 0, -20));
        SpawnActor<OSnowman>(FVector(-1445, 0, -10));
        SpawnActor<OSnowman>(FVector(-1502, 0, 61));
        SpawnActor<OSnowman>(FVector(-1429, 0, 79));
        SpawnActor<OSnowman>(FVector(-1586, 0, 71));
        SpawnActor<OSnowman>(FVector(-1471, 0, 157));
        SpawnActor<OSnowman>(FVector(-1539, 0, 175));
        SpawnActor<OSnowman>(FVector(-1484, 0, 303));
        SpawnActor<OSnowman>(FVector(-1442, 0, 358));
        SpawnActor<OSnowman>(FVector(-1510, 0, 426));
        SpawnActor<OSnowman>(FVector(-665, 0, 830));
        SpawnActor<OSnowman>(FVector(-701, 3, 853));
        SpawnActor<OSnowman>(FVector(-602, 0, 929));
    }

    if (gModeSelection == VERSUS) {
        SpawnActor<OBombKart>(0, 50, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 100, 1, 0.8333333f);
        SpawnActor<OBombKart>(0, 150, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 290, 1, 0.8333333f);
        SpawnActor<OBombKart>(0, 350, 3, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
        SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
    }
}

void FrappeSnowland::InitClouds() {
    s32 var_s0;
    s32 var_s4;
    if (gPlayerCount == 1) {
        var_s4 = 0x32;
    } else {
        var_s4 = 0x19;
    }
    for (var_s0 = 0; var_s0 < var_s4; var_s0++) {
        find_unused_obj_index(&D_8018CC80[D_8018D1F8 + var_s0]);
    }
    D_8018D1F8 += var_s0;
    D_8018D1F0 = var_s0;
    D_8018D230 = 0; // This must be turned off or mayhem ensues
}

void FrappeSnowland::TickClouds(s32 sp1C, Camera* camera) {
    func_80078170(sp1C, camera);
}

void FrappeSnowland::InitTrackObjects() {
    size_t objectId;
    size_t i;
    for (i = 0; i < NUM_SNOWFLAKES; i++) {
        find_unused_obj_index(&gObjectParticle1[i]);
    }
}

void FrappeSnowland::TickTrackObjects() {
    update_snowflakes();
}

void FrappeSnowland::Draw(ScreenContext* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_frappe_snowland_packed_dl_65E0
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_frappe_snowland_packed_dl_65E0);
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_track_sections(d_course_frappe_snowland_dl_list, arg0);
}

void FrappeSnowland::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_frappe_snowland_dl_76A0));
}

void FrappeSnowland::Waypoints(Player* player, int8_t playerId) {
    s16 waypoint = gNearestPathPointByPlayerId[playerId];

    if ((waypoint >= 0xF0) && (waypoint < 0x105)) {
        player->nearestPathPointId = 0xF0U;
    } else {
        player->nearestPathPointId = gCopyNearestWaypointByPlayerId[playerId];
        if (player->nearestPathPointId < 0) {
            player->nearestPathPointId = gPathCountByPathIndex[0] + player->nearestPathPointId;
        }
    }
}
