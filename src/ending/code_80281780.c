#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <segments.h>
#include <mk64.h>
#include <assets/models/tracks/royal_raceway/royal_raceway_displaylists.h>

#include "code_80281780.h"
#include "memory.h"
#include "camera.h"
#include "camera_junk.h"
#include "spawn_players.h"
#include "skybox_and_splitscreen.h"
#include "code_8006E9C0.h"
#include "podium_ceremony_actors.h"
#include "code_80005FD0.h"
#include "collision.h"
#include "code_80281C40.h"
#include "code_800029B0.h"
#include "menu_items.h"
#include "main.h"
#include "menus.h"
#include "render_courses.h"
#include <string.h>
#include "port/Game.h"

// For init podium ceremony
#include "ceremony_and_credits.h"

#define bcopy memcpy

u8 defaultCharacterIds[] = { 1, 2, 3, 4, 5, 6, 7, 0 };

void debug_switch_character_ceremony_cutscene(void) {
    if (gEnableDebugMode) {
        if (gControllerOne->button & HOLD_ALL_DPAD_AND_C_BUTTONS) {
            // Allows to switch character in debug mode?
            if (gControllerOne->button & U_CBUTTONS) {
                gCharacterSelections[0] = LUIGI;
            } else if (gControllerOne->button & L_CBUTTONS) {
                gCharacterSelections[0] = YOSHI;
            } else if (gControllerOne->button & R_CBUTTONS) {
                gCharacterSelections[0] = TOAD;
            } else if (gControllerOne->button & D_CBUTTONS) {
                gCharacterSelections[0] = DK;
            } else if (gControllerOne->button & U_JPAD) {
                gCharacterSelections[0] = WARIO;
            } else if (gControllerOne->button & L_JPAD) {
                gCharacterSelections[0] = PEACH;
            } else if (gControllerOne->button & R_JPAD) {
                gCharacterSelections[0] = BOWSER;
            } else {
                gCharacterSelections[0] = MARIO;
            }
            //! @todo confirm this.
            // Resets gCharacterIdByGPOverallRank to default?
            bcopy(&defaultCharacterIds, &gCharacterIdByGPOverallRank, 8);
        }
    }
}

s32 func_80281880(s32 arg0) {
    s32 i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (gCharacterIdByGPOverallRank[i] == gCharacterSelections[arg0]) {
            break;
        }
    }
    return i;
}

void func_802818BC(void) {
    s32 temp_v0;
    UNUSED s32 pad;
    s32 sp1C;
    s32 temp_v0_2;

    if (gPlayerCount != TWO_PLAYERS_SELECTED) {
        D_802874D8.unk1D = func_80281880(0);
        D_802874D8.unk1E = gCharacterSelections[0];
        return;
    }
    // weird pattern but if it matches it matches
    temp_v0 = sp1C = func_80281880(0);
    temp_v0_2 = func_80281880(1);
    if (sp1C < temp_v0_2) {
        D_802874D8.unk1E = gCharacterSelections[0];
        D_802874D8.unk1D = temp_v0;
    } else {
        D_802874D8.unk1E = gCharacterSelections[1];
        D_802874D8.unk1D = temp_v0_2;
    }
}

void setup_podium_ceremony(void) {
    Vec3f spawn = {0, 0, 0};

    clear_D_802874D8_actors();

    gCurrentCourseId = COURSE_ROYAL_RACEWAY;
    SelectPodiumCeremony();
    D_800DC5B4 = (u16) 1;
    set_mirror_mode(0);
    gGotoMenu = 0xFFFF;
    D_80287554 = 0;
    func_802A4D18();
    set_screen();
    D_800DC5EC->screenWidth = SCREEN_WIDTH;
    D_800DC5EC->screenHeight = SCREEN_HEIGHT;
    D_800DC5EC->screenStartX = SCREEN_WIDTH / 2;
    D_800DC5EC->screenStartY = SCREEN_HEIGHT / 2;
    gScreenModeSelection = SCREEN_MODE_1P;
    gNextFreeMemoryAddress = gFreeMemoryResetAnchor;
    gActiveScreenMode = SCREEN_MODE_1P;
    gModeSelection = GRAND_PRIX;
    load_course(gCurrentCourseId);
    gFreeMemoryCourseAnchor = (s32) gNextFreeMemoryAddress;
    gWaterLevel = -2000.0f;

    gCourseMinX = -0x15A1;
    gCourseMinY = -0x15A1;
    gCourseMinZ = -0x15A1;

    gCourseMaxX = 0x15A1;
    gCourseMaxY = 0x15A1;
    gCourseMaxZ = 0x15A1;

    D_8015F59C = 0;
    D_8015F5A0 = 0;
    D_8015F58C = 0;
    gCollisionMeshCount = (u16) 0;
    D_800DC5BC = (u16) 0;
    D_800DC5C8 = (u16) 0;
    gCollisionMesh = (CollisionTriangle*) gNextFreeMemoryAddress;
    //! @bug these segmented addresses need to be symbols for mobility
    // d_course_royal_raceway_packed_dl_67E8
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_67E8, -1);
    // d_course_royal_raceway_packed_dl_AEF8
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_AEF8, -1);
    // d_course_royal_raceway_packed_dl_A970
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_A970, 8);
    // d_course_royal_raceway_packed_dl_AC30
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_AC30, 8);
    // d_course_royal_raceway_packed_dl_CE0
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_CE0, 0x10);
    // d_course_royal_raceway_packed_dl_E88
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_E88, 0x10);
    // d_course_royal_raceway_packed_dl_A618
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_A618, -1);
    // d_course_royal_raceway_packed_dl_23F8
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_23F8, 1);
    // d_course_royal_raceway_packed_dl_2478
    generate_collision_mesh_with_default_section_id((Gfx*) d_course_royal_raceway_packed_dl_2478, 1);
    func_80295C6C();
    debug_switch_character_ceremony_cutscene();
    func_802818BC();
    CM_CleanCameras();
    spawn_players_and_cameras();
    D_800DC5EC->camera->renderMode = RENDER_FULL_SCENE;
    D_800DC5EC->camera->unk_B4 = 60.0f;
    gCameraFOV[0] = 60.0f;
    load_kart_textures();
    init_hud();
    func_8001C05C();
    balloons_and_fireworks_init();
    init_camera_podium_ceremony();
    func_80093E60();
    CM_BeginPlay();
    D_801625F8 = (uintptr_t) gHeapEndPtr - gNextFreeMemoryAddress;
    D_801625FC = ((f32) D_801625F8 / 1000.0f);
}
