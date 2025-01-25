#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "BigDonut.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/big_donut_data.h"

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
extern const char* big_donut_dls[];
extern s16 currentScreenSection;
}

const course_texture big_donut_textures[] = {
    { gTexture66ABA4, 0x0312, 0x0800, 0x0 }, { gTexture6747C4, 0x0145, 0x0800, 0x0 },
    { gTexture67490C, 0x021C, 0x0800, 0x0 }, { gTexture64BA50, 0x0110, 0x0800, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

BigDonut::BigDonut() {
    this->vtx = d_course_big_donut_vertex;
    this->gfx = d_course_big_donut_packed_dls;
    this->gfxSize = 528;
    Props.textures = big_donut_textures;
    Props.MinimapTexture = gTextureCourseOutlineBigDonut;
    Props.MinimapDimensions =
        IVector2D(ResourceGetTexWidthByName(Props.MinimapTexture), ResourceGetTexHeightByName(Props.MinimapTexture));

    Props.Name = "big donut";
    Props.DebugName = "doughnut";
    Props.CourseLength = "";
    Props.AIBehaviour = D_0D008F18;
    Props.AIMaximumSeparation = -1.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 40;

    Props.PathSizes = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = { 0, 0, 0 };
    Props.Skybox.BottomRight = { 0, 0, 0 };
    Props.Skybox.BottomLeft = { 0, 0, 0 };
    Props.Skybox.TopLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 0, 0, 0 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_BATTLE_ARENAS;
}

void BigDonut::Load() {
    Course::Load();

    // d_course_big_donut_packed_dl_1018
    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*) 0x07001018), 6);
    // d_course_big_donut_packed_dl_450
    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*) 0x07000450), 6);
    // d_course_big_donut_packed_dl_AC0
    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*) 0x07000AC0), 6);
    // d_course_big_donut_packed_dl_B58
    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*) 0x07000B58), 6);
    // d_course_big_donut_packed_dl_230
    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*) 0x07000230), 6);
    func_80295C6C();
    D_8015F8E4 = 100.0f;
}

void BigDonut::LoadTextures() {
}

void BigDonut::BeginPlay() {
    spawn_all_item_boxes((ActorSpawnData*) LOAD_ASSET_RAW(d_course_big_donut_item_box_spawns));

    if (gModeSelection == VERSUS) {
        Vec3f pos = { 0, 0, 0 };

        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][20], 20, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][40], 40, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][60], 60, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][80], 80, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][100], 100, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][120], 120, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][140], 140, 0, 1.0f));
    }
}

// Likely sets minimap boundaries
void BigDonut::MinimapSettings() {
    D_8018D2A0 = 0.0257f;
    D_8018D2E0 = 32;
    D_8018D2E8 = 31;
}

void BigDonut::InitCourseObjects() {
}

void BigDonut::SomeSounds() {
}

void BigDonut::WhatDoesThisDo(Player* player, int8_t playerId) {
}

void BigDonut::WhatDoesThisDoAI(Player* player, int8_t playerId) {
}

// Positions the finishline on the minimap
void BigDonut::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY,
                            (u8*) common_texture_minimap_finish_line);
}

void BigDonut::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    func_802B5D64(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_big_donut_packed_dl_DE8
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07000DE8)));
    }
    // d_course_big_donut_packed_dl_450
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07000450)));
    // d_course_big_donut_packed_dl_AC0
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07000AC0)));
    // d_course_big_donut_packed_dl_D20
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07000D20)));
    // d_course_big_donut_packed_dl_230
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x07000230)));
}

void BigDonut::RenderCredits() {
}

void BigDonut::Collision() {
}

void BigDonut::Waypoints(Player* player, int8_t playerId) {
    player->nearestWaypointId = 0;
}

void BigDonut::Destroy() {
}
