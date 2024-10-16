#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "BansheeBoardwalk.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/banshee_boardwalk_data.h"
#include "assets/boo_frames.h"

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
    extern const char *banshee_boardwalk_dls[];
}

BansheeBoardwalk::BansheeBoardwalk() {
    this->vtx = d_course_banshee_boardwalk_vertex;
    this->gfx = d_course_banshee_boardwalk_packed_dls;
    this->gfxSize = 3689;
    this->textures = banshee_boardwalk_textures;
    Props.MinimapTexture = gTextureCourseOutlineBansheeBoardwalk;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 64;

    Props.Id = "mk:banshee_boardwalk";
    Props.Name = "banshee boardwalk";
    Props.DebugName = "ghost";
    Props.CourseLength = "747m";
    Props.AIBehaviour = D_0D009058;
    Props.AIMaximumSeparation = 40.0f;
    Props.AIMinimumSeparation = 0.4f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 2700.0f;

    Props.PathSizes = {0x2EE, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_banshee_boardwalk_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_banshee_boardwalk_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {0, 0, 0};
    Props.Skybox.BottomRight = {0, 0, 0};
    Props.Skybox.BottomLeft = {0, 0, 0};
    Props.Skybox.TopLeft = {0, 0, 0};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
}

void BansheeBoardwalk::LoadTextures() {
}

void BansheeBoardwalk::SpawnActors() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_banshee_boardwalk_item_box_spawns));
}

// Likely sets minimap boundaries
void BansheeBoardwalk::MinimapSettings() {
    D_80165880 = dma_textures((const char*)gTextureGhosts, 0x4CC2, 0xD980);
    D_8018D2A0 = 0.016f;
    D_8018D2C0[0] = 0x0106;
    D_8018D2E0 = 55;
    D_8018D2E8 = 39;
}

const char* sBoardwalkTexList[] = { gTextureBat1, gTextureBat2, gTextureBat3, gTextureBat4 };

void BansheeBoardwalk::InitCourseObjects() {
    size_t objectId = 0;
    if (gGamestate != CREDITS_SEQUENCE) {
        objectId = indexObjectList1[0];
        init_texture_object(objectId, (uint8_t*)d_course_banshee_boardwalk_bat_tlut, sBoardwalkTexList, 0x20U,
                            (u16) 0x00000040);
        gObjectList[objectId].orientation[0] = 0;
        gObjectList[objectId].orientation[1] = 0;
        gObjectList[objectId].orientation[2] = 0x8000;
        init_object(indexObjectList1[1], 0);
        init_object(indexObjectList1[2], 0);
    }
}

void BansheeBoardwalk::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        update_trash_bin();
        func_8007E4C4();
        if (gModeSelection != TIME_TRIALS) {
            update_bat();
        }
        wrapper_update_boos();
        update_cheep_cheep(0);
    }
}

void BansheeBoardwalk::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        render_object_trash_bin(cameraId);
        render_object_bat(cameraId);
        func_8005217C(cameraId);
        render_object_boos(cameraId);
    }
}

void BansheeBoardwalk::SomeSounds() {
}

void BansheeBoardwalk::WhatDoesThisDo(Player* player, int8_t playerId) {
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

void BansheeBoardwalk::WhatDoesThisDoAI(Player* player, int8_t playerId) {
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

void BansheeBoardwalk::SpawnBombKarts() {
    gWorldInstance.AddObject(std::make_unique<OBombKart>(140, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(165, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(330, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(550, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(595, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void BansheeBoardwalk::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void BansheeBoardwalk::SetStaffGhost() {
}

void BansheeBoardwalk::BeginPlay() {  }

void BansheeBoardwalk::Render(struct UnkStruct_800DC5EC* arg0) {
    Camera* camera = arg0->camera;
    Mat4 spCC;
    UNUSED s32 pad[6];
    Vec3f spA8;
    UNUSED s32 pad2[6];

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    // d_course_banshee_boardwalk_packed_dl_7228
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07007228)));

    gSPFogPosition(gDisplayListHead++, D_802B87B0, D_802B87B4);

    gDPPipeSync(gDisplayListHead++);

    gSPClearGeometryMode(gDisplayListHead++,
                         G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_banshee_boardwalk_packed_dl_5CD0
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07005CD0)));
    // d_course_banshee_boardwalk_packed_dl_4E60
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07004E60)));
    // d_course_banshee_boardwalk_packed_dl_69B0
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070069B0)));

    render_course_segments(banshee_boardwalk_dls, arg0);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH);
    // d_course_banshee_boardwalk_packed_dl_580
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000580)));
    // d_course_banshee_boardwalk_packed_dl_60
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000060)));
    // d_course_banshee_boardwalk_packed_dl_540
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000540)));

    if (camera->pos[1] < -20.0f) {
        // d_course_banshee_boardwalk_packed_dl_6310
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07006310)));
    }
    spA8[0] = camera->pos[0];
    spA8[1] = -82.0f;
    spA8[2] = camera->pos[2];
    mtxf_translate(spCC, spA8);
    render_set_position(spCC, 0);

    gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_banshee_boardwalk_dl_B278);
    gDPPipeSync(gDisplayListHead++);
}

void BansheeBoardwalk::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_banshee_boardwalk_dl_B308));
}

void BansheeBoardwalk::Collision() {}

void BansheeBoardwalk::GenerateCollision() {
    D_800DC5BC = 1;
    D_801625EC = 0;
    D_801625F4 = 0;
    D_801625F0 = 0;
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_banshee_boardwalk_track_sections));
    func_80295C6C();
    find_vtx_and_set_colours(segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000878)), 128, 0, 0, 0);
    D_8015F8E4 = -80.0f;
}

void BansheeBoardwalk::Water() {
    D_802B87BC++;

    if (D_802B87BC >= 0x100) {
        D_802B87BC = 0;
    }
    find_and_set_tile_size((uintptr_t) LOAD_ASSET_RAW(d_course_banshee_boardwalk_dl_B278), 0, D_802B87BC);
}

void BansheeBoardwalk::Waypoints(Player* player, int8_t playerId) {
    s16 waypoint = gNearestWaypointByPlayerId[playerId];
    if ((waypoint >= 0x12C) && (waypoint < 0x13C)) {
        player->nearestWaypointId = 0x12CU;
    } else {
        player->nearestWaypointId = gNearestWaypointByPlayerId[playerId];
        if (player->nearestWaypointId < 0) {
            player->nearestWaypointId = gWaypointCountByPathIndex[0] + player->nearestWaypointId;
        }
    }
}

void BansheeBoardwalk::Destroy() { }
