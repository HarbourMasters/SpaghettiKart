#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "SherbetLand.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/sherbet_land/sherbet_land_data.h"
#include "assets/other/tracks/sherbet_land/sherbet_land_data.h"
#include "engine/objects/Penguin.h"
#include "libultraship/bridge/resourcebridge.h"

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
    extern const char *sherbet_land_dls[72];
    extern const char *sherbet_land_dls_2[72];
}

SherbetLand::SherbetLand() {
    Props.Minimap.Texture = minimap_sherbet_land;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 262;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 52;
    Props.Minimap.PlayerY = 33;
    Props.Minimap.PlayerScaleFactor = 0.015f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    Props.Minimap.Colour = {72, 100, 255};
    ResizeMinimap(&Props.Minimap);

    ResourceName = "mk:sherbet_land";
    Props.SetText(Props.Name, "sherbet land", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "sherbet", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "756m", sizeof(Props.TrackLength));

    Props.LakituTowType = (s32)OLakitu::LakituTowType::ICE;

    Props.AIBehaviour = D_0D009280;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_sherbet_land_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_sherbet_land_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) gTextureExhaust1;
    Props.Clouds = gSherbetLandClouds;
    Props.CloudList = gSherbetLandClouds;

    Props.Skybox.TopRight = {128, 184, 248};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {128, 184, 248};
    Props.Skybox.FloorTopRight = {216, 232, 248};
    Props.Skybox.FloorBottomRight = {128, 184, 248};
    Props.Skybox.FloorBottomLeft = {128, 184, 248};
    Props.Skybox.FloorTopLeft = {216, 232, 248};
    Props.Sequence = MusicSeq::MUSIC_SEQ_FRAPPE_SNOWLAND;

    Props.WaterLevel = -18.0f;
}

void SherbetLand::Load() {
    Track::Load();
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(sherbet_land_dls); i++) {
            InvertTriangleWindingByName(sherbet_land_dls[i]);
        }
        for (size_t i = 0; i < ARRAY_COUNT(sherbet_land_dls_2); i++) {
            InvertTriangleWindingByName(sherbet_land_dls_2[i]);
        }
    }
    parse_track_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_sherbet_land_addr));
    func_80295C6C();
    // d_course_sherbet_land_packed_dl_1EB8
    find_vtx_and_set_colours((Gfx*) d_course_sherbet_land_packed_dl_1EB8, 180, 255, 255, 255);
    // d_course_sherbet_land_packed_dl_2308
    find_vtx_and_set_colours((Gfx*) d_course_sherbet_land_packed_dl_2308, 150, 255, 255, 255);
}

f32 SherbetLand::GetWaterLevel(FVector pos, Collision* collision) {
    if ((get_surface_type(collision->meshIndexZX) & 0xFF) == SNOW) {
        return (f32) (gTrackMinY - 0xA);
    }
    return Props.WaterLevel;
}

void SherbetLand::BeginPlay() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_sherbet_land_item_box_spawns));

    // Multiplayer does not spawn the big penguin... It does now!
//  if (gPlayerCountSelection1 == 1) {
        SpawnActor<OPenguin>(FVector(-383.0f, 2.0f, -690.0f), 0, 0, 0.0f, OPenguin::PenguinType::EMPEROR, OPenguin::Behaviour::STRUT);
//  }

    SpawnActor<OPenguin>(FVector(-2960.0f, -80.0f, 1521.0f), 0x150, 0, 100.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);
    SpawnActor<OPenguin>(FVector(-2960.0f, -80.0f, 1521.0f), 0x150, 0, 100.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);

    SpawnActor<OPenguin>(FVector(-2490.0f, -80.0f, 1612.0f), 0x100, 0, 80.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);
    SpawnActor<OPenguin>(FVector(-2490.0f, -80.0f, 1612.0f), 0x100, 0, 80.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);

    SpawnActor<OPenguin>(FVector(-2098.0f, -80.0f, 1624.0f), 0xFF00, 0, 80.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);
    SpawnActor<OPenguin>(FVector(-2098.0f, -80.0f, 1624.0f), 0xFF00, 0, 80.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);

    SpawnActor<OPenguin>(FVector(-2080.0f, -80.0f, 1171.0f), 0x150, 0, 80.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);
    SpawnActor<OPenguin>(FVector(-2080.0f, -80.0f, 1171.0f), 0x150, 0, 80.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE);

    if (gGamestate == CREDITS_SEQUENCE) {
        SpawnActor<OPenguin>(FVector(380.0f, 0.0f, -535.0f), 0x9000, -0x4000, 0.0f, OPenguin::PenguinType::CREDITS, OPenguin::Behaviour::SLIDE3);
    } else {
        SpawnActor<OPenguin>(FVector(146.0f, 0.0f, -380.0f), 0x9000, -0x4000, 0.0f, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE3);
    }

    SpawnActor<OPenguin>(FVector(380.0f, 0.0f, -766.0f), 0x5000, 0x8000, 0.0f, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE4);
    SpawnActor<OPenguin>(FVector(-2300.0f, 0.0f, -210.0f), 0xC000, 0x8000, 0.0f, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6);
    SpawnActor<OPenguin>(FVector(-2500.0f, 0.0f, -250.0f), 0x4000, 0x8000, 0.0f, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6);
    SpawnActor<OPenguin>(FVector(-535.0f, 0.0f, 875.0f), 0x8000, -0x4000, 0.0f, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6);
    SpawnActor<OPenguin>(FVector(-250.0f, 0.0f, 953.0f), 0x9000, -0x4000, 0.0f, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6);

    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == VERSUS) {
            SpawnActor<OBombKart>(0, 50, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 100, 1, 0.8333333f);
            SpawnActor<OBombKart>(0, 150, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 200, 1, 0.8333333f);
            SpawnActor<OBombKart>(0, 250, 3, 0.8333333f);
            SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
            SpawnActor<OBombKart>(0, 0, 0, 0.8333333f);
        }
    }
}

void SherbetLand::TickTrackObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        func_800842C8();
    }
}

void SherbetLand::DrawTrackObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        func_80052E30(cameraId);
    }
}

void SherbetLand::Draw(ScreenContext* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_track_sections(sherbet_land_dls, arg0);
}

void SherbetLand::DrawCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_sherbet_land_dl_9AE8));
}

void SherbetLand::DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) {
    gDPPipeSync(gDisplayListHead++);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    render_track_sections(sherbet_land_dls_2, screen);

    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    if ((func_80290C20(screen->camera) == 1) && (get_water_level(screen->player) < screen->player->pos[1])) {
        gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_sherbet_land_packed_dl_2B48
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_sherbet_land_packed_dl_2B48);
    }
    gDPPipeSync(gDisplayListHead++);
}

void SherbetLand::CreditsSpawnActors() {
    // d_course_sherbet_land_packed_dl_1EB8
    find_vtx_and_set_colours((Gfx*) d_course_sherbet_land_packed_dl_1EB8, 180, 0xFF, 0xFF, 0xFF);
    // d_course_sherbet_land_packed_dl_2308
    find_vtx_and_set_colours((Gfx*) d_course_sherbet_land_packed_dl_2308, 150, 0xFF, 0xFF, 0xFF);
}
