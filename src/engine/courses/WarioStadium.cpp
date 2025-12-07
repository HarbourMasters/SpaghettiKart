#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "WarioStadium.h"
#include "World.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/wario_stadium/wario_stadium_data.h"
#include "assets/other/tracks/wario_stadium/wario_stadium_data.h"
#include "engine/actors/WarioSign.h"
#include "engine/actors/Finishline.h"

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
#include "assets/models/common_data.h"
#include "save.h"
#include "replays.h"
#include "actors.h"
#include "collision.h"
#include "code_8003DC40.h"
#include "memory.h"
#include "skybox_and_splitscreen.h"
#include "course.h"
extern const char* wario_stadium_dls[108];
extern s16 currentScreenSection;
}

WarioStadium::WarioStadium() {
    Props.Minimap.Texture = minimap_wario_stadium;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 262;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 53;
    Props.Minimap.PlayerY = 35;
    Props.Minimap.PlayerScaleFactor = 0.0155f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Props.SetText(Props.Name, "wario stadium", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "stadium", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "1591m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D009310;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.6f;
    Props.AISteeringSensitivity = 53;

    Props.NearPersp = 10.0f;
    Props.FarPersp = 4800.0f;

    Props.PathSizes = { 0x640, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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

    Props.PathTable[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_wario_stadium_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_wario_stadium_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = gWarioStadiumStars;
    Props.CloudList = gWarioStadiumStars;

    FVector finish;
    finish.x = (gIsMirrorMode != 0) ? 13 + 12.0f : 13 - 12.0f;
    finish.y = (f32) (0 - 15);
    finish.z = -9;

    this->FinishlineSpawnPoint = finish;

    Props.Skybox.TopRight = { 20, 30, 56 };
    Props.Skybox.BottomRight = { 40, 60, 110 };
    Props.Skybox.BottomLeft = { 40, 60, 110 };
    Props.Skybox.TopLeft = { 20, 30, 56 };
    Props.Skybox.FloorTopRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 0, 0, 0 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_WARIO_STADIUM;
}

void WarioStadium::Load() {
    Course::Load();

    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(wario_stadium_dls); i++) {
            InvertTriangleWindingByName(wario_stadium_dls[i]);
        }
        InvertTriangleWindingByName(d_course_wario_stadium_packed_dl_A0C8);
        InvertTriangleWindingByName(d_course_wario_stadium_packed_dl_A228);
        InvertTriangleWindingByName(d_course_wario_stadium_packed_dl_A88);
        InvertTriangleWindingByName(d_course_wario_stadium_packed_dl_EC0);
    }

    parse_course_displaylists((TrackSections*) LOAD_ASSET_RAW(d_course_wario_stadium_addr));
    func_80295C6C();
    Props.WaterLevel = gCourseMinY - 10.0f;
    // d_course_wario_stadium_packed_dl_C50
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_C50, 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_BD8
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_BD8, 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_B60
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_B60, 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_AE8
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_AE8, 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_CC8
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_CC8, 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_D50
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_D50, 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_DD0
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_DD0, 100, 255, 255, 255);
    // d_course_wario_stadium_packed_dl_E48
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_E48, 100, 255, 255, 255);
}

void WarioStadium::UnLoad() {
    RestoreTriangleWinding();
}

void WarioStadium::BeginPlay() {
    spawn_all_item_boxes((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_wario_stadium_item_box_spawns));

    AWarioSign::Spawn(FVector(-131.0f, 83.0f, 286.0f), IRotator(0, 0, 0), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f));
    AWarioSign::Spawn(FVector(-2353.0f, 72.0f, -1608.0f), IRotator(0, 0, 0), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f));
    AWarioSign::Spawn(FVector(-2622.0f, 79.0f, 739.0f), IRotator(0, 0, 0), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f));

    if (gModeSelection == VERSUS) {
        OBombKart::Spawn(0, 50, 3, 0.8333333f);
        OBombKart::Spawn(0, 100, 1, 0.8333333f);
        OBombKart::Spawn(0, 150, 3, 0.8333333f);
        OBombKart::Spawn(0, 200, 1, 0.8333333f);
        OBombKart::Spawn(0, 250, 3, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
    }
}

void WarioStadium::InitClouds() {
    init_stars(this->Props.Clouds);
}

void WarioStadium::UpdateClouds(s32 sp1C, Camera* camera) {
    update_stars(sp1C, camera, this->Props.CloudList);
}

void WarioStadium::InitCourseObjects() {
}

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

void WarioStadium::WhatDoesThisDo(Player* player, int8_t playerId) {
}

void WarioStadium::WhatDoesThisDoAI(Player* player, int8_t playerId) {
}

void WarioStadium::CopyJumbotron(s32 ulx, s32 uly, s16 portionToDraw, u16* source) {
    // Add CVar if we want to expose a user toggle for only updating 1/6 of the jumbotron per frame
    u8 updateWholeJumbo = true;

    if (portionToDraw == -1 || updateWholeJumbo) {
        copy_framebuffer(ulx, uly, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture68272C));
        copy_framebuffer(ulx + 64, uly, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682928));
        copy_framebuffer(ulx, uly + 32, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682B24));
        copy_framebuffer(ulx + 64, uly + 32, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682D20));
        copy_framebuffer(ulx, uly + 64, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682F1C));
        copy_framebuffer(ulx + 64, uly + 64, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture683118));
    } else {
        switch (portionToDraw) {
            case 0:
                copy_framebuffer(ulx, uly, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture68272C));
                break;
            case 1:
                copy_framebuffer(ulx + 64, uly, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682928));
                break;
            case 2:
                copy_framebuffer(ulx, uly + 32, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682B24));
                break;
            case 3:
                copy_framebuffer(ulx + 64, uly + 32, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682D20));
                break;
            case 4:
                copy_framebuffer(ulx, uly + 64, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture682F1C));
                break;
            case 5:
                copy_framebuffer(ulx + 64, uly + 64, 64, 32, source, (u16*) LOAD_ASSET_RAW(gTexture683118));
                break;
            default:
                break;
        }
    }
}

void WarioStadium::Render(struct UnkStruct_800DC5EC* arg0) {
    s16 prevFrame;

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    // Invalidate Jumbotron textures so they update each frame
    // This could be more efficient if we exposed the non-opcode based invalidation to be called
    // inside copy_framebuffers_port
    gSPInvalidateTexCache(gDisplayListHead++, (uintptr_t) gTexture68272C);
    gSPInvalidateTexCache(gDisplayListHead++, (uintptr_t) gTexture682928);
    gSPInvalidateTexCache(gDisplayListHead++, (uintptr_t) gTexture682B24);
    gSPInvalidateTexCache(gDisplayListHead++, (uintptr_t) gTexture682D20);
    gSPInvalidateTexCache(gDisplayListHead++, (uintptr_t) gTexture682F1C);
    gSPInvalidateTexCache(gDisplayListHead++, (uintptr_t) gTexture683118);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_wario_stadium_packed_dl_A0C8
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_wario_stadium_packed_dl_A0C8);
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    render_course_segments(wario_stadium_dls, arg0);

    // d_course_wario_stadium_packed_dl_A228
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_wario_stadium_packed_dl_A228);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_wario_stadium_packed_dl_A88
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_wario_stadium_packed_dl_A88);
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
        if (currentScreenSection >= 6) {
            currentScreenSection = 0;
        }

        /**
         * The jumbo television screen used to be split into six sections to fit into the n64's texture size
         * restrictions It isn't split into six sections anymore
         */
        CopyJumbotron(D_800DC5DC, D_800DC5E0, currentScreenSection, (u16*) gPortFramebuffers[prevFrame]);
    }
}

void WarioStadium::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*) (d_course_wario_stadium_dl_CA78));
}

void WarioStadium::SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5,
                                      f32* arg6, f32* arg7) {
    func_8003EE2C(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void WarioStadium::DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot,
                             uint16_t playerDirection) {

    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0x00, 0xFF);
    // d_course_wario_stadium_packed_dl_EC0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_wario_stadium_packed_dl_EC0);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    gDPPipeSync(gDisplayListHead++);
}

void WarioStadium::CreditsSpawnActors() {
    Vec3f position;
    Vec3f velocity = { 0, 0, 0 };
    Vec3s rotation = { 0, 0, 0 };

    vec3f_set(position, -131.0f, 83.0f, 286.0f);
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_WARIO_SIGN);
    vec3f_set(position, -2353.0f, 72.0f, -1608.0f);
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_WARIO_SIGN);
    vec3f_set(position, -2622.0f, 79.0f, 739.0f);
    add_actor_to_empty_slot(position, rotation, velocity, ACTOR_WARIO_SIGN);
    // d_course_wario_stadium_packed_dl_C50
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_C50, 0x64, 0xFF, 0xFF, 0xFF);
    // d_course_wario_stadium_packed_dl_BD8
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_BD8, 0x64, 0xFF, 0xFF, 0xFF);
    // d_course_wario_stadium_packed_dl_B60
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_B60, 0x64, 0xFF, 0xFF, 0xFF);
    // d_course_wario_stadium_packed_dl_AE8
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_AE8, 0x64, 0xFF, 0xFF, 0xFF);
    // d_course_wario_stadium_packed_dl_CC8
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_CC8, 0x64, 0xFF, 0xFF, 0xFF);
    // d_course_wario_stadium_packed_dl_D50
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_D50, 0x64, 0xFF, 0xFF, 0xFF);
    // d_course_wario_stadium_packed_dl_DD0
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_DD0, 0x64, 0xFF, 0xFF, 0xFF);
    // d_course_wario_stadium_packed_dl_E48
    find_vtx_and_set_colours((Gfx*) d_course_wario_stadium_packed_dl_E48, 0x64, 0xFF, 0xFF, 0xFF);
}

void WarioStadium::Destroy() {
}
