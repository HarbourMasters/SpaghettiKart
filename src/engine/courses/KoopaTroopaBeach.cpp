#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "KoopaTroopaBeach.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/koopa_troopa_beach_data.h"

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
    #include "code_8003DC40.h"
    #include "memory.h"
    extern const char *koopa_troopa_beach_dls[];
    extern s8 gPlayerCount;
}

KoopaTroopaBeach::KoopaTroopaBeach() {
    this->vtx = d_course_koopa_troopa_beach_vertex;
    this->gfx = d_course_koopa_troopa_beach_packed_dls;
    this->gfxSize = 5720;
    this->textures = koopa_troopa_beach_textures;
    Props.MinimapTexture = gTextureCourseOutlineKoopaTroopaBeach;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 64;

    Props.Id = "mk:koopa_beach";
    Props.Name = "koopa troopa beach";
    Props.DebugName = "beach";
    Props.CourseLength = "691m";
    Props.AIBehaviour = D_0D009158;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 1.0f;
    Props.FarPersp = 5000.0f;

    Props.PathSizes = {0x2BC, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_track_waypoints);
    Props.PathTable2[1] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_track_waypoints_2);
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gKoopaTroopaBeachClouds;
    Props.CloudList = gKoopaTroopaBeachClouds;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {48, 152, 120};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {48, 152, 120};
    Props.Skybox.FloorTopRight = {48, 152, 120};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {48, 152, 120};
}

void KoopaTroopaBeach::LoadTextures() {
}

void KoopaTroopaBeach::SpawnActors() {
    init_actor_hot_air_balloon_item_box(328.0f * gCourseDirection, 70.0f, 2541.0f);
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_item_box_spawns));
    spawn_palm_trees((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_tree_spawn));
}

void KoopaTroopaBeach::Init() {}

// Likely sets minimap boundaries
void KoopaTroopaBeach::MinimapSettings() {
    D_8018D220 = reinterpret_cast<uint8_t (*)[1024]>(dma_textures(gTextureExhaust3, 0x3C8U, 0x1000));
    D_8018D2A0 = 0.014f;
    D_8018D2C0[0] = 268;
    D_8018D2E0 = 40;
    D_8018D2E8 = 21;
}

void KoopaTroopaBeach::InitCourseObjects() {
    size_t objectId;
    size_t i;

    if (gGamestate != CREDITS_SEQUENCE) {
        for (i = 0; i < NUM_CRABS; i++) {
            objectId = indexObjectList1[i];
            init_object(objectId, 0);
            gObjectList[objectId].pos[0] = gObjectList[objectId].origin_pos[0] =
                gCrabSpawns[i].startX * xOrientation;
            gObjectList[objectId].unk_01C[0] = gCrabSpawns[i].patrolX * xOrientation;

            gObjectList[objectId].pos[2] = gObjectList[objectId].origin_pos[2] = gCrabSpawns[i].startZ;
            gObjectList[objectId].unk_01C[2] = gCrabSpawns[i].patrolZ;
        }
    }
    for (i = 0; i < NUM_SEAGULLS; i++) {
        objectId = indexObjectList2[i];
        init_object(objectId, 0);
        if (i < (NUM_SEAGULLS / 2)) {
            gObjectList[objectId].unk_0D5 = 0;
        } else {
            gObjectList[objectId].unk_0D5 = 1;
        }
    }
}

void KoopaTroopaBeach::UpdateCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        update_crabs();
    }
    if ((gPlayerCount == 1) || (gPlayerCount == 2) || (gGamestate == CREDITS_SEQUENCE)) {
        update_seagulls();
    }
}

void KoopaTroopaBeach::RenderCourseObjects(s32 cameraId) {
    if (gGamestate != CREDITS_SEQUENCE) {
        render_object_crabs(cameraId);
    }
    if (gGamestate != CREDITS_SEQUENCE) {

        if ((gPlayerCount == 1) || (gPlayerCount == 2)) {
            render_object_seagulls(cameraId);
        }
    } else {
        render_object_seagulls(cameraId);
    }
}

void KoopaTroopaBeach::SomeSounds() {
    vec3f_set(D_8015F738, 153.0f, 0.0f, 2319.0f);
    func_800C9D80(D_8015F738, D_802B91C8, 0x51028001);
}

void KoopaTroopaBeach::WhatDoesThisDo(Player* player, int8_t playerId) {}

void KoopaTroopaBeach::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void KoopaTroopaBeach::SpawnBombKarts() {
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(140, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(165, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(330, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(550, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(595, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.SpawnObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void KoopaTroopaBeach::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void KoopaTroopaBeach::SetStaffGhost() {
}

void KoopaTroopaBeach::BeginPlay() {  }

void KoopaTroopaBeach::Render(struct UnkStruct_800DC5EC* arg0) {
    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_koopa_troopa_beach_packed_dl_9CC0
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07009CC0)));
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_koopa_troopa_beach_packed_dl_9688
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07009688)));
    render_course_segments((const char**)d_course_koopa_troopa_beach_dl_list1, arg0);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_koopa_troopa_beach_packed_dl_2C0
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070002C0)));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPPipeSync(gDisplayListHead++);
}

void KoopaTroopaBeach::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_koopa_troopa_beach_dl_18D68));
}

void KoopaTroopaBeach::Collision() {}

void KoopaTroopaBeach::SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    func_8003E37C(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void KoopaTroopaBeach::GenerateCollision() {
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_koopa_troopa_beach_addr));
    func_80295C6C();
    find_vtx_and_set_colours(segmented_gfx_to_virtual(reinterpret_cast<void*>(0x0700ADE0)), -0x6A, 255, 255, 255);
    find_vtx_and_set_colours(segmented_gfx_to_virtual(reinterpret_cast<void*>(0x0700A540)), -0x6A, 255, 255, 255);
    find_vtx_and_set_colours(segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07009E70)), -0x6A, 255, 255, 255);
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000358), -0x6A, 255, 255, 255);
}

void KoopaTroopaBeach::Water() {
    // clang-format off
    if (D_8015F8E8 < 0.0f) {
        if (D_8015F8E4 < -20.0f) { D_8015F8E8 *= -1.0f; }
    } else {
        if (D_8015F8E4 > 0.0f) { D_8015F8E8 *= -1.0f; }
    }
    // clang-format on
    D_8015F8E4 += D_8015F8E8;

    D_802B87BC += 9;
    if (D_802B87BC > 255) {
        D_802B87BC = 0;
    }
    D_802B87C4 += 3;
    if (D_802B87C4 > 255) {
        D_802B87C4 = 0;
    }
    // waterfall animation
    // d_course_koopa_troopa_beach_packed_dl_9D58
    find_and_set_tile_size((uintptr_t)segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07009D58)), 0, D_802B87BC);
    // d_course_koopa_troopa_beach_packed_dl_9CD0
    find_and_set_tile_size((uintptr_t)segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07009CD0)), 0, D_802B87C4);
    D_802B87CC = random_int(300) / 40;
    if (D_802B87C8 < 0) {
        D_802B87C8 = random_int(300) / 40;
    } else {
        D_802B87C8 = -(random_int(300) / 40);
    }
    // Waterfall bubbling effect? (unused)
    // d_course_koopa_troopa_beach_packed_dl_2E8
    find_and_set_tile_size((uintptr_t)segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070002E8)), D_802B87C8, D_802B87CC);

}

void KoopaTroopaBeach::Destroy() {}
