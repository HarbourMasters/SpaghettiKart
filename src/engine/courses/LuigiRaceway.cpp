#include <cstdint>
#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "LuigiRaceway.h"
#include "World.h"
#include "align_asset_macro.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/luigi_raceway/luigi_raceway_data.h"
#include "assets/other/tracks/luigi_raceway/luigi_raceway_data.h"
#include "engine/objects/HotAirBalloon.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/GrandPrixBalloons.h"

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
#include "courses/staff_ghost_data.h"
#include "framebuffer_effects.h"
#include "skybox_and_splitscreen.h"
#include "course.h"
extern const char* luigi_raceway_dls[120];
extern s16 currentScreenSection;
}

LuigiRaceway::LuigiRaceway() {
    Props.Minimap.Texture = minimap_luigi_raceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 271;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 45;
    Props.Minimap.PlayerY = 60;
    Props.Minimap.PlayerScaleFactor = 0.0155f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    ResizeMinimap(&Props.Minimap);

    Id = "mk:luigi_raceway";
    Props.SetText(Props.Name, "luigi raceway", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "l circuit", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "717m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D0091E8;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.7f;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = { 0x2DA, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

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

    Props.PathTable[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_luigi_raceway_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*) LOAD_ASSET_RAW(d_course_luigi_raceway_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*)gTextureExhaust2;
    Props.Clouds = gLuigiRacewayClouds;
    Props.CloudList = gLuigiRacewayClouds;

    Props.Skybox.TopRight = { 128, 184, 248 };
    Props.Skybox.BottomRight = { 216, 232, 248 };
    Props.Skybox.BottomLeft = { 216, 232, 248 };
    Props.Skybox.TopLeft = { 128, 184, 248 };
    Props.Skybox.FloorTopRight = { 216, 232, 248 };
    Props.Skybox.FloorBottomRight = { 0, 0, 0 };
    Props.Skybox.FloorBottomLeft = { 0, 0, 0 };
    Props.Skybox.FloorTopLeft = { 216, 232, 248 };
    Props.Sequence = MusicSeq::MUSIC_SEQ_LUIGI_RACEWAY;
}

void LuigiRaceway::Load() {
    Course::Load();
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(luigi_raceway_dls); i++) {
            InvertTriangleWindingByName(luigi_raceway_dls[i]);
        }
        InvertTriangleWindingByName(d_course_luigi_raceway_packed_dl_9EC0);
        InvertTriangleWindingByName(d_course_luigi_raceway_packed_dl_E0);
        InvertTriangleWindingByName(d_course_luigi_raceway_packed_dl_68);
    }
    parse_course_displaylists((TrackSections*) LOAD_ASSET_RAW(d_course_luigi_raceway_addr));
    func_80295C6C();
    Props.WaterLevel = gCourseMinY - 10.0f;
}

void LuigiRaceway::UnLoad() {
    RestoreTriangleWinding();
}

void LuigiRaceway::BeginPlay() {
    spawn_foliage((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_luigi_raceway_tree_spawn));
    spawn_all_item_boxes((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_luigi_raceway_item_box_spawns));

    if (gGamestate == CREDITS_SEQUENCE) {
        OHotAirBalloon::Spawn(FVector(-1250.0f, 0.0f, 1110.0f));
    } else { // Normal gameplay
        OHotAirBalloon::Spawn(FVector(-176.0, 0.0f, -2323.0f));
        OGrandPrixBalloons::Spawn(FVector(-140, -44, -215));
    }

    if (gModeSelection == VERSUS) {
        OBombKart::Spawn(0, 50, 1, 0.8333333f);
        OBombKart::Spawn(0, 200, 3, 0.8333333f);
        OBombKart::Spawn(0, 305, 1, 0.8333333f);
        OBombKart::Spawn(0, 440, 3, 0.8333333f);
        OBombKart::Spawn(0, 515, 3, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
    }
}

void LuigiRaceway::InitCourseObjects() {
    size_t i;
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
    }
}

void LuigiRaceway::SomeSounds() {
    vec3f_set(D_8015F748, 85.0f, 21.0f, -219.0f);
    func_800C9D80(D_8015F748, D_802B91C8, 0x5103700B);
}

void LuigiRaceway::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) && ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
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

void LuigiRaceway::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) && ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
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

void LuigiRaceway::SetStaffGhost() {
    u32 temp_v0 = func_800B4E24(0) & 0xfffff;
    if (temp_v0 <= 11200) {
        bCourseGhostDisabled = 0;
        D_80162DF4 = 0;
    } else {
        bCourseGhostDisabled = 1;
        D_80162DF4 = 1;
    }
    D_80162DC4 = d_luigi_raceway_staff_ghost;
    D_80162DE4 = 1;
}

void LuigiRaceway::CopyJumbotron(s32 ulx, s32 uly, s16 portionToDraw, u16* source) {
    // Add CVar if we want to expose a user toggle for only updating 1/6 of the jumbotron per frame
    bool updateWholeJumbo = true;

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

void LuigiRaceway::Render(struct UnkStruct_800DC5EC* arg0) {
    UNUSED s32 pad;
    u16 sp22 = (u16) arg0->pathCounter;
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
        // d_course_luigi_raceway_packed_dl_9EC0
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_luigi_raceway_packed_dl_9EC0);
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    render_course_segments(luigi_raceway_dls, arg0);

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_luigi_raceway_packed_dl_E0
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_luigi_raceway_packed_dl_E0);
    // d_course_luigi_raceway_packed_dl_68
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_luigi_raceway_packed_dl_68);

    D_800DC5DC = 88;
    D_800DC5E0 = 72;

    // Render only the first player camera onto the television billboard. Screen agnostic screens of other players).
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
        CopyJumbotron(D_800DC5DC, D_800DC5E0, currentScreenSection,
                                            (u16*) gPortFramebuffers[prevFrame]);
    }
}

void LuigiRaceway::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*) (d_course_luigi_raceway_dl_FD40));
}

void LuigiRaceway::SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5,
                                      f32* arg6, f32* arg7) {
    func_8003E9EC(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
