#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "WarioStadium.h"
#include "GameObject.h"
#include "World.h"
#include "BombKart.h"
#include "assets/wario_stadium_data.h"
#include "engine/actors/AWarioSign.h"

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
    extern const char *wario_stadium_dls[];
    extern s16 currentScreenSection;
}

WarioStadium::WarioStadium() {
    this->vtx = d_course_wario_stadium_vertex;
    this->gfx = d_course_wario_stadium_packed_dls;
    this->gfxSize = 5272;
    this->textures = wario_stadium_textures;
    Props.MinimapTexture = gTextureCourseOutlineWarioStadium;
    Props.D_800E5548[0] = 64;
    Props.D_800E5548[1] = 64;

    Props.Name = "wario stadium";
    Props.DebugName = "stadium";
    Props.CourseLength = "1591m";
    Props.AIBehaviour = D_0D009310;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.6f;
    Props.SomePtr = D_800DCAF4;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 10.0f;
    Props.FarPersp = 4800.0f;

    Props.PathSizes = {0x640, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_wario_stadium_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_wario_stadium_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gWarioStadiumStars;
    Props.CloudList = gWarioStadiumStars;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {20, 30, 56};
    Props.Skybox.BottomRight = {40, 60, 110};
    Props.Skybox.BottomLeft = {40, 60, 110};
    Props.Skybox.TopLeft = {20, 30, 56};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
}

void WarioStadium::LoadTextures() {
}

void WarioStadium::SpawnActors() {
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_wario_stadium_item_box_spawns));

    Vec3f pos = {-131.0f, 83.0f, 286.0f};
    pos[0] *= gCourseDirection;
    gWorldInstance.AddActor(std::make_unique<AWarioSign>(pos));

    Vec3f pos2 = {-2353.0f, 72.0f, -1608.0f};
    pos2[0] *= gCourseDirection;
    gWorldInstance.AddActor(std::make_unique<AWarioSign>(pos2));

    Vec3f pos3 = {-2622.0f, 79.0f, 739.0f};
    pos3[0] *= gCourseDirection;
    gWorldInstance.AddActor(std::make_unique<AWarioSign>(pos3));
}

void WarioStadium::InitClouds() {
    init_stars(this->Props.Clouds);
}

void WarioStadium::UpdateClouds(s32 sp1C, Camera* camera) {
    update_stars(sp1C, camera, this->Props.CloudList);
}

// Likely sets minimap boundaries
void WarioStadium::MinimapSettings() {
    D_8018D2A0 = 0.0155f;
    D_8018D2C0[0] = 0x0106;
    D_8018D2E0 = 53;
    D_8018D2E8 = 35;
}

void WarioStadium::InitCourseObjects() {}

void WarioStadium::SomeSounds() {
    vec3f_set(D_8015F748, 298.0f, 202.0f, -850.0f);
    func_800C9D80(D_8015F748, D_802B91C8, 0x5103700B);
    vec3f_set(D_8015F758, -1600.0f, 202.0f, -2430.0f);
    func_800C9D80(D_8015F758, D_802B91C8, 0x5103700B);
    vec3f_set(D_8015F768, -2708.0f, 202.0f, 1762.0f);
    func_800C9D80(D_8015F768, D_802B91C8, 0x5103700B);
    vec3f_set(D_8015F778, -775.0f, 202.0f, 1930.0f);
    func_800C9D80(D_8015F778, D_802B91C8, 0x5103700B);
}

void WarioStadium::WhatDoesThisDo(Player* player, int8_t playerId) {}

void WarioStadium::WhatDoesThisDoAI(Player* player, int8_t playerId) {}

void WarioStadium::SpawnBombKarts() {
    gWorldInstance.AddObject(std::make_unique<OBombKart>(40, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(100, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(265, 3, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(285, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(420, 1, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
    gWorldInstance.AddObject(std::make_unique<OBombKart>(0, 0, 0.8333333, 0, 0, 0, 0));
}

// Positions the finishline on the minimap
void WarioStadium::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void WarioStadium::SetStaffGhost() {}

void WarioStadium::BeginPlay() {  }
void WarioStadium::Render(struct UnkStruct_800DC5EC* arg0) {
    s16 prevFrame;

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {

        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_wario_stadium_packed_dl_A0C8
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x0700A0C8));
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    render_course_segments(wario_stadium_dls, arg0);

    // d_course_wario_stadium_packed_dl_A228
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x0700A228));
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_wario_stadium_packed_dl_A88
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07000A88));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);

    D_800DC5DC = 88;
    D_800DC5E0 = 72;
    if (gActiveScreenMode == SCREEN_MODE_1P) {
        prevFrame = (s16) sRenderedFramebuffer - 1;
        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }
        currentScreenSection++;
        if (currentScreenSection > 5) {
            currentScreenSection = 0;
        }
        switch (currentScreenSection) {
            case 0:
#ifdef TARGET_N64
                copy_framebuffer(D_800DC5DC, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x8800));
#endif
                break;
            case 1:
#ifdef TARGET_N64
                copy_framebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x9800));
#endif
                break;
            case 2:
#ifdef TARGET_N64
                copy_framebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xA800));
#endif
                break;
            case 3:
#ifdef TARGET_N64
                copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xB800));
#endif
                break;
            case 4:
#ifdef TARGET_N64
                copy_framebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xC800));
#endif
                break;
            case 5:
#ifdef TARGET_N64
                copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xD800));
#endif
                break;
        }
    }
}

void WarioStadium::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_wario_stadium_dl_CA78));
}

void WarioStadium::Collision() {}

void WarioStadium::SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    func_8003EE2C(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void WarioStadium::GenerateCollision() {
    parse_course_displaylists((TrackSectionsI*)LOAD_ASSET_RAW(d_course_wario_stadium_addr));
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
    // d_course_wario_stadium_packed_dl_C50
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000C50), 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_BD8
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000BD8), 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_B60
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000B60), 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_AE8
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000AE8), 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_CC8
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000CC8), 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_D50
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000D50), 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_DD0
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000DD0), 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_E48
    find_vtx_and_set_colours(segmented_gfx_to_virtual((void*)0x07000E48), 100, 255, 255, 255);
}

void WarioStadium::Destroy() { }
