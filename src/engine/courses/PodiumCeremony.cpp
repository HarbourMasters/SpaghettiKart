#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "PodiumCeremony.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/royal_raceway_data.h"

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
    extern const char *royal_raceway_dls[];
}

PodiumCeremony::PodiumCeremony() {
    this->vtx = d_course_royal_raceway_vertex;
    this->gfx = d_course_royal_raceway_packed_dls;
    this->gfxSize = 5670;
    this->textures = royal_raceway_textures;

    Props.Name = "royal raceway";
    Props.DebugName = "p circuit";
    Props.CourseLength = "1025m";
    Props.AIBehaviour = D_0D009188;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.4f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.PathSizes = {0x3E8, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = d_course_royal_raceway_unknown_waypoints;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = d_course_royal_raceway_track_waypoints;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {238, 144, 255};
    Props.Skybox.BottomRight = {255, 224, 240};
    Props.Skybox.BottomLeft = {255, 224, 240};
    Props.Skybox.TopLeft = {238, 144, 255};
    Props.Skybox.FloorTopRight = {255, 224, 240};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 224, 240};
}

void PodiumCeremony::LoadTextures() {
}

void PodiumCeremony::SpawnActors() {
    spawn_foliage(d_course_royal_raceway_tree_spawn);
    spawn_all_item_boxes(d_course_royal_raceway_item_box_spawns);
    spawn_piranha_plants(d_course_royal_raceway_piranha_plant_spawn);
}

void PodiumCeremony::Init() {  }

// Likely sets minimap boundaries
void PodiumCeremony::MinimapSettings() {
    D_8018D220 = reinterpret_cast<uint8_t (*)[1024]>(dma_textures(gTextureExhaust4, 0x3F8, 0x1000));
    D_8018D2C0[0] = 262;
    D_8018D2A0 = 0.014f;
    D_8018D2E0 = 37;
    D_8018D2E8 = 50;
    D_80165718 = -64;
    D_80165720 = 5;
    D_80165728 = -330;
}

void PodiumCeremony::InitCourseObjects() {
    size_t i;
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
        for (i = 0; i < D_80165738; i++) {
            find_unused_obj_index(&gObjectParticle3[i]);
            init_object(gObjectParticle3[i], 0);
        }
    }
}

void PodiumCeremony::SomeSounds() {
}

void PodiumCeremony::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x180) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x1E1)) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x41);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void PodiumCeremony::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x180) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x1E1)) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x41);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void PodiumCeremony::SpawnBombKarts() {
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
void PodiumCeremony::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void PodiumCeremony::SetStaffGhost() {
    u32 temp_v0 = func_800B4E24(0) & 0xfffff;
    if (temp_v0 <= 16000) {
        D_80162DD6 = 0;
        D_80162DF4 = 0;
    } else {
        D_80162DD6 = 1;
        D_80162DF4 = 1;
    }
    D_80162DC4 = d_royal_raceway_staff_ghost;
    D_80162DE4 = 6;
}

void PodiumCeremony::BeginPlay() {  }
void PodiumCeremony::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_royal_raceway_packed_dl_B030
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x0700B030))));
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_royal_raceway_packed_dl_A648
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x0700A648))));

    render_course_segments(royal_raceway_dls, arg0);

    // d_course_royal_raceway_packed_dl_11A8
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070011A8))));
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_royal_raceway_packed_dl_8A0
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070008A0))));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void PodiumCeremony::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_royal_raceway_dl_D8E8));
}

void PodiumCeremony::Collision() {}

void PodiumCeremony::GenerateCollision() {
    parse_course_displaylists(d_course_royal_raceway_addr);
    func_80295C6C();
    D_8015F8E4 = -60.0f;
}

void PodiumCeremony::Destroy() { }
