#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "Skyscraper.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/skyscraper_data.h"

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
    extern const char *skyscraper_dls[];
    extern s16 currentScreenSection;
}

const course_texture skyscraper_textures[] = {
    { gTexture6457D8, 0x0160, 0x0800, 0x0 },
    { gTexture6462C0, 0x01B9, 0x0800, 0x0 },
    { gTexture6864E8, 0x0807, 0x1000, 0x0 },
    { gTexture686CF0, 0x08B7, 0x1000, 0x0 },
    { gTexture6875A8, 0x093F, 0x1000, 0x0 },
    { gTexture687EE8, 0x0883, 0x1000, 0x0 },
    { gTexture68876C, 0x0AC2, 0x1000, 0x0 },
    { gTexture689230, 0x09CE, 0x1000, 0x0 },
    { gTexture689C00, 0x0884, 0x1000, 0x0 },
    { gTexture68A484, 0x07D6, 0x1000, 0x0 },
    { gTexture68AC5C, 0x0A47, 0x1000, 0x0 },
    { gTexture68B6A4, 0x07C5, 0x1000, 0x0 },
    { gTexture68BE6C, 0x04A1, 0x1000, 0x0 },
    { gTexture68C310, 0x030D, 0x1000, 0x0 },
    { gTexture64B8D8, 0x0177, 0x0800, 0x0 },
    { gTexture645660, 0x0176, 0x0800, 0x0 },
    { gTextureNumberYellowBlue1, 0x01AE, 0x0800, 0x0 },
    { gTextureNumberYellowBlue2, 0x01C6, 0x0800, 0x0 },
    { gTextureNumberYellowBlue3, 0x01CC, 0x0800, 0x0 },
    { gTextureNumberYellowBlue4, 0x01E4, 0x0800, 0x0 },
    { gTexture668608, 0x0120, 0x0800, 0x0 },
    { gTexture67B75C, 0x0108, 0x0800, 0x0 },
    { gTexture6835F0, 0x0252, 0x1000, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

Skyscraper::Skyscraper() {
    this->vtx = d_course_skyscraper_vertex;
    this->gfx = d_course_skyscraper_packed_dls;
    this->gfxSize = 548;
    Props.textures = skyscraper_textures;
    Props.Minimap.Texture = gTextureCourseOutlineSkyscraper;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 257;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 32;
    Props.Minimap.PlayerY = 32;
    Props.Minimap.PlayerScaleFactor = 0.0445f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

    Props.SetText(Props.Name, "skyscraper", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "skyscraper", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D008F18;
    Props.AIMaximumSeparation = -1.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

    Props.PathSizes = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = NULL;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = NULL;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;

    Props.Skybox.TopRight = {0, 0, 0};
    Props.Skybox.BottomRight = {0, 0, 0};
    Props.Skybox.BottomLeft = {0, 0, 0};
    Props.Skybox.TopLeft = {0, 0, 0};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_BATTLE_ARENAS;

    Props.WaterLevel = -480.0f;
}

void Skyscraper::Load() {
    Course::Load();

    // d_course_skyscraper_packed_dl_1110
    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*)0x07001110), 1);
    // d_course_skyscraper_packed_dl_258
    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*)0x07000258), 1);
    func_80295C6C();
}

void Skyscraper::LoadTextures() {
}

void Skyscraper::BeginPlay() {
    spawn_all_item_boxes((ActorSpawnData*)LOAD_ASSET_RAW(d_course_skyscraper_item_box_spawns));

    if (gModeSelection == VERSUS) {
        FVector pos = { 0, 0, 0 };

        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][20], 20, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][40], 40, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][60], 60, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][80], 80, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][100], 100, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][120], 120, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][140], 140, 0, 1.0f));
    }
}

void Skyscraper::InitCourseObjects() {}

void Skyscraper::SomeSounds() {}

void Skyscraper::WhatDoesThisDo(Player* player, int8_t playerId) {}

void Skyscraper::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void Skyscraper::Render(struct UnkStruct_800DC5EC* arg0) {
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_skyscraper_packed_dl_FE8
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07000FE8)));
    // d_course_skyscraper_packed_dl_C60
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07000C60)));
    // d_course_skyscraper_packed_dl_B70
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07000B70)));
    // d_course_skyscraper_packed_dl_6B8
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x070006B8)));
    // d_course_skyscraper_packed_dl_570
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07000570)));
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_10C8
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x070010C8)));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_258
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x07000258)));
}

void Skyscraper::RenderCredits() {}

void Skyscraper::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = 0;
}

void Skyscraper::Destroy() { }
