#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "SherbetLand.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "assets/sherbet_land_data.h"
#include "engine/objects/Penguin.h"

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
    #include "course.h"
    extern const char *sherbet_land_dls[];
    extern const char *sherbet_land_dls_2[];
}

const course_texture sherbet_land_textures[] = {
    { gTexture643B3C, 0x0798, 0x0800, 0x0 }, { gTexture66D024, 0x04EA, 0x0800, 0x0 },
    { gTexture678118, 0x0314, 0x0800, 0x0 }, { gTextureSignWoodRedArrow, 0x04E1, 0x1000, 0x0 },
    { gTexture678CC8, 0x058E, 0x0800, 0x0 }, { gTexture67842C, 0x050E, 0x0800, 0x0 },
    { gTexture67893C, 0x038B, 0x0800, 0x0 }, { gTexture651984, 0x019C, 0x0800, 0x0 },
    { gTexture651428, 0x055B, 0x0800, 0x0 }, { gTexture662924, 0x0110, 0x0800, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

SherbetLand::SherbetLand() {
    this->vtx = d_course_sherbet_land_vertex;
    this->gfx = d_course_sherbet_land_packed_dls;
    this->gfxSize = 1803;
    Props.textures = sherbet_land_textures;
    Props.Minimap.Texture = gTextureCourseOutlineSherbetLand;
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

    Props.SetText(Props.Name, "sherbet land", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "sherbet", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "756m", sizeof(Props.CourseLength));

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

    Props.CloudTexture = (u8*) LOAD_ASSET_RAW(gTextureExhaust1);
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
    Course::Load();

    parse_course_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_sherbet_land_addr));
    func_80295C6C();
    // d_course_sherbet_land_packed_dl_1EB8
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07001EB8), -0x4C, 255, 255, 255);
    // d_course_sherbet_land_packed_dl_2308
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07002308), -0x6A, 255, 255, 255);
}

f32 SherbetLand::GetWaterLevel(FVector pos, Collision* collision) {
    if ((get_surface_type(collision->meshIndexZX) & 0xFF) == SNOW) {
        return (f32) (gCourseMinY - 0xA);
    }
    return Props.WaterLevel;
}

void SherbetLand::BeginPlay() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_sherbet_land_item_box_spawns));

    // Multiplayer does not spawn the big penguin
//  if (gPlayerCountSelection1 == 1) {
        FVector pos = {-383.0f, 2.0f, -690.0f};
        gWorldInstance.AddObject(new OPenguin(pos, 0, OPenguin::PenguinType::EMPEROR, OPenguin::Behaviour::STRUT));
//  }

    FVector pos2 = { -2960.0f, -80.0f, 1521.0f };
    auto penguin = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos2, 0x150, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    auto penguin2 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos2, 0x150, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    penguin->Diameter = penguin2->Diameter = 100.0f;

    FVector pos3 = { -2490.0f, -80.0f, 1612.0f };
    auto penguin3 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos3, 0x100, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    auto penguin4 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos3, 0x100, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    penguin3->Diameter = penguin4->Diameter = 80.0f;

    FVector pos4 = { -2098.0f, -80.0f, 1624.0f };
    auto penguin5 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos4, 0xFF00, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    auto penguin6 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos4, 0xFF00, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    penguin5->Diameter = penguin6->Diameter = 80.0f;


    FVector pos5 = { -2080.0f, -80.0f, 1171.0f };
    auto penguin7 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos5, 0x150, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    auto penguin8 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos5, 0x150, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE)));
    penguin7->Diameter = penguin8->Diameter = 80.0f;


    if (gGamestate == CREDITS_SEQUENCE) {
        FVector pos6 = { 380.0, 0.0f, -535.0f };
        auto penguin9 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos6, 0x9000, OPenguin::PenguinType::CREDITS, OPenguin::Behaviour::SLIDE3)));
        penguin9->MirrorModeAngleOffset = -0x4000;
    } else {
        FVector pos6 = { 146.0f, 0.0f, -380.0f };
        auto penguin9 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos6, 0x9000, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE3)));
        penguin9->MirrorModeAngleOffset = -0x4000;
    }

    FVector pos7 = { 380.0f, 0.0f, -766.0f };
    auto penguin10 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos7, 0x5000, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE4)));
    penguin10->MirrorModeAngleOffset = 0x8000;

    FVector pos8 = { -2300.0f, 0.0f, -210.0f };
    auto penguin11 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos8, 0xC000, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6)));
    penguin11->MirrorModeAngleOffset = 0x8000;

    FVector pos9 = { -2500.0f, 0.0f, -250.0f };
    auto penguin12 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos9, 0x4000, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6)));
    penguin12->MirrorModeAngleOffset = 0x8000;

    FVector pos10 = { -535.0f, 0.0f, 875.0f };
    auto penguin13 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos10, 0x8000, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6)));
    penguin13->MirrorModeAngleOffset = -0x4000;

    FVector pos11 = { -250.0f, 0.0f, 953.0f };
    auto penguin14 = reinterpret_cast<OPenguin*>(gWorldInstance.AddObject(new OPenguin(pos11, 0x9000, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE6)));
    penguin14->MirrorModeAngleOffset = -0x4000;

    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == VERSUS) {
            FVector kart = { 0, 0, 0 };
            gWorldInstance.AddObject(new OBombKart(kart, &gTrackPaths[0][50], 50, 3, 0.8333333f));
            gWorldInstance.AddObject(new OBombKart(kart, &gTrackPaths[0][100], 100, 1, 0.8333333f));
            gWorldInstance.AddObject(new OBombKart(kart, &gTrackPaths[0][150], 150, 3, 0.8333333f));
            gWorldInstance.AddObject(new OBombKart(kart, &gTrackPaths[0][200], 200, 1, 0.8333333f));
            gWorldInstance.AddObject(new OBombKart(kart, &gTrackPaths[0][250], 250, 3, 0.8333333f));
            gWorldInstance.AddObject(new OBombKart(kart, &gTrackPaths[0][0], 0, 0, 0.8333333f));
            gWorldInstance.AddObject(new OBombKart(kart, &gTrackPaths[0][0], 0, 0, 0.8333333f));
        }
    }
}

void SherbetLand::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        func_800842C8();
    }
}

void SherbetLand::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        func_80052E30(cameraId);
    }
}

void SherbetLand::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    render_course_segments(sherbet_land_dls, arg0);
}

void SherbetLand::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_sherbet_land_dl_9AE8));
}

void SherbetLand::DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) {
    Mat4 matrix;

    gDPPipeSync(gDisplayListHead++);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

    mtxf_identity(matrix);
    render_set_position(matrix, 0);
    render_course_segments(sherbet_land_dls_2, screen);

    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    if ((func_80290C20(screen->camera) == 1) && (get_water_level(screen->player) < screen->player->pos[1])) {
        gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_sherbet_land_packed_dl_2B48
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07002B48));
    }
    gDPPipeSync(gDisplayListHead++);
}

void SherbetLand::CreditsSpawnActors() {
    // d_course_sherbet_land_packed_dl_1EB8
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07001EB8), -0x4C, 0xFF, 0xFF, 0xFF);
    // d_course_sherbet_land_packed_dl_2308
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07002308), -0x6A, 0xFF, 0xFF, 0xFF);
}
