#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "BlockFort.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/block_fort_data.h"

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
#include "course_offsets.h"
extern const char* block_fort_dls[];
extern s16 currentScreenSection;
}

const course_texture block_fort_textures[] = {
    { gTexture64286C, 0x010A, 0x0800, 0x0 },          { gTextureGrayCheckerboard, 0x010C, 0x0800, 0x0 },
    { gTextureGrayCobblestone, 0x010C, 0x0800, 0x0 }, { gTexture64275C, 0x0110, 0x0800, 0x0 },
    { gTexture642978, 0x010D, 0x0800, 0x0 },          { gTexture6747C4, 0x0145, 0x0800, 0x0 },
    { gTexture6442D4, 0x0138, 0x0800, 0x0 },          { 0x00000000, 0x0000, 0x0000, 0x0 },
};

BlockFort::BlockFort() {
    this->vtx = d_course_block_fort_vertex;
    this->gfx = d_course_block_fort_packed_dls;
    this->gfxSize = 699;
    Props.textures = block_fort_textures;
    Props.Minimap.Texture = gTextureCourseOutlineBlockFort;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 257;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 32;
    Props.Minimap.PlayerY = 32;
    Props.Minimap.PlayerScaleFactor = 0.0335f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

    Props.SetText(Props.Name, "block fort", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "block", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D008F18;
    Props.AIMaximumSeparation = -1.0f;
    Props.AIMinimumSeparation = 0.1f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

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

    Props.Skybox.TopRight = { 128, 184, 248 };
    Props.Skybox.BottomRight = { 216, 232, 248 };
    Props.Skybox.BottomLeft = { 216, 232, 248 };
    Props.Skybox.TopLeft = { 128, 184, 248 };
    Props.Skybox.FloorTopRight = { 216, 232, 248 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 216, 232, 248 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_CHOCO_MOUNTAIN;
}

void BlockFort::Load() {
    Course::Load();

    generate_collision_mesh_with_default_section_id((Gfx*) segmented_gfx_to_virtual((void*) 0x070015C0), 1);
    func_80295C6C();
    Props.WaterLevel = gCourseMinY - 10.0f;
}

void BlockFort::BeginPlay() {
    spawn_all_item_boxes((ActorSpawnData*) LOAD_ASSET_RAW(d_course_block_fort_item_box_spawns));

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

void BlockFort::Render(struct UnkStruct_800DC5EC* arg0) {
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_block_fort_packed_dl_15C0
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*) 0x070015C0)));
}

void BlockFort::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = 0;
}
