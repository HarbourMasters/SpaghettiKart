#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "DKJungle.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/dks_jungle_parkway_data.h"

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
    #include "sounds.h"
    extern const char *dks_jungle_parkway_dls[];
    extern s16 currentScreenSection;
}

DKJungle::DKJungle() {
    this->vtx = d_course_dks_jungle_parkway_vertex;
    this->gfx = d_course_dks_jungle_parkway_packed_dls;
    this->gfxSize = 4997;
    this->textures = dks_jungle_parkway_textures;

    Props.Name = "d.k.'s jungle parkway";
    Props.DebugName = "jungle";
    Props.CourseLength = "893m";
    Props.AIBehaviour = D_0D0093C0;
    Props.AIMaximumSeparation = 40.0f;
    Props.AIMinimumSeparation = 0.1f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.PathSizes = {0x370, 1, 1, 1, 0x1F4, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = d_course_dks_jungle_parkway_unknown_waypoints;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = d_course_dks_jungle_parkway_track_waypoints;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {255, 174, 0};
    Props.Skybox.BottomRight = {255, 229, 124};
    Props.Skybox.BottomLeft = {255, 229, 124};
    Props.Skybox.TopLeft = {255, 174, 0};
    Props.Skybox.FloorTopRight = {22, 145, 22};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {22, 145, 22};
}

void DKJungle::LoadTextures() {
}

void DKJungle::SpawnActors() {
    spawn_all_item_boxes(d_course_dks_jungle_parkway_item_box_spawns);
    init_kiwano_fruit();
    func_80298D10();
}

void DKJungle::Init() {  }

// Likely sets minimap boundaries
void DKJungle::MinimapSettings() {
    D_8018D2A0 = 0.0155f;
    D_8018D2C0[0] = 255;
    D_8018D2E0 = 29;
    D_8018D2E8 = 47;
}

void DKJungle::InitCourseObjects() {}

void DKJungle::SomeSounds() {
    vec3f_set(D_8015F738, -790.0f, -255.0f, -447.0f);
    func_800C9D80(D_8015F738, D_802B91C8, 0x51028001);
}

void DKJungle::WhatDoesThisDo(Player* player, int8_t playerId) {
    if ((((s16) gNearestWaypointByPlayerId[playerId] >= 0) &&
            ((s16) gNearestWaypointByPlayerId[playerId] < 0x65)) ||
        (((s16) gNearestWaypointByPlayerId[playerId] >= 0x14A) &&
            ((s16) gNearestWaypointByPlayerId[playerId] < 0x21F))) {
        if (D_80165300[playerId] != 2) {
            func_800C8F80(playerId, 0x0170802D);
        }
        D_80165300[playerId] = 2;
    } else {
        if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x288) &&
            ((s16) gNearestWaypointByPlayerId[playerId] < 0x305)) {
            if (D_80165300[playerId] != 1) {
                func_800CA288(playerId, 0x55);
            }
            D_80165300[playerId] = 1;
        } else {
            if (D_80165300[playerId] != 0) {
                if (D_80165300[playerId] == 1) {
                    func_800CA2B8(playerId);
                }
                if (D_80165300[playerId] == 2) {
                    func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x70, 0x80, 0x2D));
                }
                D_80165300[playerId] = 0;
            }
        }
    }
}

void DKJungle::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x288) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x305)) {
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

void DKJungle::SpawnBombKarts() {
    World* world = GetWorld();

    if (world) {
        world->SpawnObject(std::make_unique<OBombKart>(40, 3, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(100, 3, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(265, 3, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(285, 1, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(420, 1, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
        world->SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    }
}

// Positions the finishline on the minimap
void DKJungle::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void DKJungle::SetStaffGhost() {}

void DKJungle::BeginPlay() {  }
void DKJungle::Render(struct UnkStruct_800DC5EC* arg0) {
    func_802B5D64(D_800DC610, D_802B87D4, 0, 1);
    func_802B5D64(&D_800DC610[1], D_802B87D4, D_802B87D0, 1);

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK | G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_dks_jungle_parkway_packed_dl_92D8
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual((void*)0x070092D8)));
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    render_course_segments(dks_jungle_parkway_dls, arg0);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void DKJungle::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_dks_jungle_parkway_dl_13C30));
}

void DKJungle::Collision() {}

void DKJungle::GenerateCollision() {
    parse_course_displaylists(d_course_dks_jungle_parkway_addr);
    func_80295C6C();
    D_8015F8E4 = -475.0f;
    // d_course_dks_jungle_parkway_packed_dl_3FA8
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07003FA8), 120, 255, 255, 255);
}

void DKJungle::Destroy() { }
