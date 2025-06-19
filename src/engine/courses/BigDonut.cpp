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
    Props.Minimap.Texture = gTextureCourseOutlineBigDonut;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 257;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 32;
    Props.Minimap.PlayerY = 31;
    Props.Minimap.PlayerScaleFactor = 0.0257f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

    Props.SetText(Props.Name, "big donut", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "doughnut", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D008F18;
    Props.AIMaximumSeparation = -1.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 40;

    Props.PathSizes = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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

    Props.Skybox.TopRight = { 0, 0, 0 };
    Props.Skybox.BottomRight = { 0, 0, 0 };
    Props.Skybox.BottomLeft = { 0, 0, 0 };
    Props.Skybox.TopLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 0, 0, 0 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_BATTLE_ARENAS;

    Props.WaterLevel = 100.0f;
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
}

void BigDonut::BeginPlay() {
    spawn_all_item_boxes((ActorSpawnData*) LOAD_ASSET_RAW(d_course_big_donut_item_box_spawns));

    if (gModeSelection == VERSUS) {
        FVector pos = {0, 0, 0};

        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][20], 20, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][40], 40, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][60], 60, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][80], 80, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][100], 100, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][120], 120, 0, 1.0f));
        gWorldInstance.AddObject(new OBombKart(pos, &gTrackPaths[0][140], 140, 0, 1.0f));
    }
}

void BigDonut::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
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

void BigDonut::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = 0;
}

void BigDonut::Destroy() {
}
