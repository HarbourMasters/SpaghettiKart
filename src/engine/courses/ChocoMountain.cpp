#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "ChocoMountain.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "choco_mountain_data.h"
#include "engine/actors/AFinishline.h"

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
#include "course_offsets.h"
extern const char* choco_mountain_dls[];
}

const course_texture choco_mountain_textures[] = {
    { gTexture64619C, 0x0124, 0x0800, 0x0 },
    { gTexture64647C, 0x0829, 0x1000, 0x0 },
    { gTexture647F4C, 0x05BC, 0x1000, 0x0 },
    { gTexture64FBF4, 0x0274, 0x0800, 0x0 },
    { gTexture653DB0, 0x06AE, 0x0800, 0x0 },
    { gTexture652B54, 0x0606, 0x0800, 0x0 },
    { gTexture65315C, 0x04A9, 0x0800, 0x0 },
    { gTexture6684F8, 0x010D, 0x0800, 0x0 },
    { gTextureSignLuigis0, 0x0287, 0x1000, 0x0 },
    { gTextureSignLuigis1, 0x02AF, 0x1000, 0x0 },
    { gTextureSignNintendoRed0, 0x02A6, 0x1000, 0x0 },
    { gTextureSignNintendoRed1, 0x02F7, 0x1000, 0x0 },
    { gTexture6774D8, 0x0113, 0x0800, 0x0 },
    { gTextureSignFallingRocks, 0x012C, 0x0800, 0x0 },
    { gTextureSignBackside, 0x011E, 0x0800, 0x0 },
    { gTexture679C04, 0x012F, 0x0800, 0x0 },
    { gTexture67B864, 0x014C, 0x0800, 0x0 },
    { gTexture67DC20, 0x03EF, 0x0800, 0x0 },
    { gTextureSignYoshi, 0x04DF, 0x1000, 0x0 },
    { gTextureCheckerboardBlueGray, 0x04A1, 0x1000, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

ChocoMountain::ChocoMountain() {
    this->vtx = d_course_choco_mountain_vertex;
    this->gfx = d_course_choco_mountain_packed_dls;
    this->gfxSize = 2910;
    Props.textures = choco_mountain_textures;
    Props.MinimapTexture = gTextureCourseOutlineChocoMountain;
    Props.MinimapDimensions =
        IVector2D(ResourceGetTexWidthByName(Props.MinimapTexture), ResourceGetTexHeightByName(Props.MinimapTexture));

    Props.Id = "mk:choco_mountain";
    Props.Name = "choco mountain";
    Props.DebugName = "mountain";
    Props.CourseLength = "687m";
    Props.AIBehaviour = D_0D008F80;
    Props.AIMaximumSeparation = 35.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 2.0f;
    Props.FarPersp = 1500.0f;

    Props.PathSizes = { 0x2BC, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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

    Props.PathTable[0] = (TrackWaypoint*) LOAD_ASSET_RAW(d_course_choco_mountain_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*) LOAD_ASSET_RAW(d_course_choco_mountain_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL; // no clouds
    Props.CloudList = NULL;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = { 255, 255, 255 };
    Props.Skybox.BottomRight = { 255, 255, 255 };
    Props.Skybox.BottomLeft = { 255, 255, 255 };
    Props.Skybox.TopLeft = { 255, 255, 255 };
    Props.Skybox.FloorTopRight = { 255, 255, 255 };
    Props.Skybox.FloorBottomRight = { 255, 255, 255 };
    Props.Skybox.FloorBottomLeft = { 255, 255, 255 };
    Props.Skybox.FloorTopLeft = { 255, 255, 255 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_CHOCO_MOUNTAIN;
}

void ChocoMountain::Load() {
    Course::Load();
    D_800DC5BC = 1;
    D_801625EC = 255;
    D_801625F4 = 255;
    D_801625F0 = 255;
    D_802B87B0 = 0x3E3;
    D_802B87B4 = 0x3E8;
    D_802B87D4 = 0x71C;
    D_802B87D0 = 0xE38;

    // Spawn guardrail only for CC_50 and time trials.
    if ((gCCSelection != CC_50) && (gModeSelection != TIME_TRIALS)) {
        // d_course_choco_mountain_packed_dl_0
        nullify_displaylist((uintptr_t) segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000000)));
        // d_course_choco_mountain_packed_dl_98
        nullify_displaylist((uintptr_t) segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000098)));
        // d_course_choco_mountain_packed_dl_178
        nullify_displaylist((uintptr_t) segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000178)));
        // d_course_choco_mountain_packed_dl_280
        nullify_displaylist((uintptr_t) segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000280)));
        // d_course_choco_mountain_packed_dl_340
        nullify_displaylist((uintptr_t) segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000340)));
        // d_course_choco_mountain_packed_dl_3C8
        nullify_displaylist((uintptr_t) segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070003C8)));
    }

    parse_course_displaylists((TrackSectionsI*) LOAD_ASSET_RAW(d_course_choco_mountain_addr));
    func_802B5CAC(0x238E, 0x31C7, D_8015F590);
    func_80295C6C();
    D_8015F8E4 = -80.0f;
}

void ChocoMountain::LoadTextures() {
}

void ChocoMountain::SpawnActors() {
    gWorldInstance.AddActor(new AFinishline());
    spawn_all_item_boxes((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_choco_mountain_item_box_spawns));
    spawn_falling_rocks(
        (struct ActorSpawnData*) LOAD_ASSET_RAW((const char*) d_course_choco_mountain_falling_rock_spawns));
}

void ChocoMountain::SpawnVehicles() {
    if (gModeSelection == VERSUS) {
        Vec3f pos = { 0, 0, 0 };

        gWorldInstance.AddBombKart(pos, &D_80164550[0][140], 140, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][165], 165, 1, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][330], 330, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][550], 550, 1, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][595], 595, 3, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f);
        gWorldInstance.AddBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f);
    }
}

// Likely sets minimap boundaries
void ChocoMountain::MinimapSettings() {
    D_8018D2A0 = 0.022f;
    D_8018D2C0[0] = 265;
    D_8018D2E0 = 19;
    D_8018D2E8 = 37;
}

void ChocoMountain::InitCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
        for (size_t i = 0; i < D_80165738; i++) {
            find_unused_obj_index(&gObjectParticle3[i]);
            init_object(gObjectParticle3[i], 0);
        }
    }
}

void ChocoMountain::SomeSounds() {
    vec3f_set(D_8015F748, -223.0f, 94.0f, -155.0f);
    func_800C9D80(D_8015F748, D_802B91C8, 0x5103700B);
}

void ChocoMountain::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0xA0) && ((s16) gNearestWaypointByPlayerId[playerId] < 0xB4)) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void ChocoMountain::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0xA0) && ((s16) gNearestWaypointByPlayerId[playerId] < 0xB4)) {
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

// Positions the finishline on the minimap
void ChocoMountain::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY,
                            (u8*) common_texture_minimap_finish_line);
}

void ChocoMountain::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_choco_mountain_packed_dl_4608
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07004608)));
    }
    gDPSetCycleType(gDisplayListHead++, G_CYC_2CYCLE);
    gDPSetFogColor(gDisplayListHead++, D_801625EC, D_801625F4, D_801625F0, 0xFF);
    gSPFogPosition(gDisplayListHead++, D_802B87B0, D_802B87B4);

    gDPPipeSync(gDisplayListHead++);
    gSPSetGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATERGB, G_CC_PASS2);

    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    // d_course_choco_mountain_packed_dl_5A70
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07005A70)));
    // d_course_choco_mountain_packed_dl_828
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000828)));
    // d_course_choco_mountain_packed_dl_8E0
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070008E0)));
    // d_course_choco_mountain_packed_dl_5868
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07005868)));
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    render_course_segments(choco_mountain_dls, arg0);

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_PASS2);
    // d_course_choco_mountain_packed_dl_448
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000448)));
    // d_course_choco_mountain_packed_dl_5D8
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070005D8)));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_choco_mountain_packed_dl_718
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000718)));
    gSPClearGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPPipeSync(gDisplayListHead++);
}

void ChocoMountain::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*) (d_course_choco_mountain_dl_71B8));
}

void ChocoMountain::Collision() {
}

void ChocoMountain::SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5,
                                       f32* arg6, f32* arg7) {
    func_8003E37C(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void ChocoMountain::Destroy() {
}
