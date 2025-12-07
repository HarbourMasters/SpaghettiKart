#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "MarioRaceway.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/actors/MarioSign.h"
#include "engine/objects/Object.h"
#include "engine/objects/BombKart.h"
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
    #include "assets/models/tracks/mario_raceway/mario_raceway_data.h"
    #include "assets/other/tracks/mario_raceway/mario_raceway_data.h"
    #include "collision.h"
    #include "memory.h"
    #include "courses/staff_ghost_data.h"
    #include "course.h"
    extern const char *mario_raceway_dls[68];
}

MarioRaceway::MarioRaceway() {
    Props.Minimap.Texture = minimap_mario_raceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 260;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 6;
    Props.Minimap.PlayerY = 28;
    Props.Minimap.PlayerScaleFactor = 0.022f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = -2.0;
    ResizeMinimap(&Props.Minimap);

    Id = "mk:mario_raceway";
    Props.SetText(Props.Name, "mario raceway", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "m circuit", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "567m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D008F28;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.AIDistance = gMarioRacewayAIDistances;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {600, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

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

    Props.PathTable[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_mario_raceway_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackPathPoint*)LOAD_ASSET_RAW(d_course_mario_raceway_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (uint8_t*) gTextureExhaust5;
    Props.Clouds = gKalimariDesertClouds;
    Props.CloudList = gLuigiRacewayClouds;

    Props.Skybox.TopRight = {0, 184, 248};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {0, 184, 248};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_MARIO_RACEWAY;
}

void MarioRaceway::Load() {
    Course::Load();

    // Invert winding in mirror mode before generating collision meshes
    if (gIsMirrorMode != 0) {
        for (size_t i = 0; i < ARRAY_COUNT(mario_raceway_dls); i++) {
            InvertTriangleWindingByName(mario_raceway_dls[i]);
        }
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_1140);

        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_3508);
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_3240);
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_14A0);
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_450);
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_240);
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_E0);
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_160);

        // pipe collision mesh
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_8E8);
        InvertTriangleWindingByName(d_course_mario_raceway_packed_dl_2D68);
    }

    generate_collision_mesh_with_defaults((Gfx*) d_course_mario_raceway_packed_dl_1140);
    
    // Generate pipe collision mesh
    if (gScreenModeSelection == SCREEN_MODE_1P) {
        // d_course_mario_raceway_packed_dl_8E8
        generate_collision_mesh_with_defaults((Gfx*) d_course_mario_raceway_packed_dl_8E8);
    } else {
        if (CVarGetInteger("gDisableLod", 1) == true) {
            generate_collision_mesh_with_defaults((Gfx*) d_course_mario_raceway_packed_dl_8E8);
        } else {
            // d_course_mario_raceway_packed_dl_2D68
            generate_collision_mesh_with_defaults((Gfx*) d_course_mario_raceway_packed_dl_2D68);
        }
    }

    parse_course_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_mario_raceway_addr));
    func_80295C6C();
    Props.WaterLevel = gCourseMinY - 10.0f;
}

void MarioRaceway::UnLoad() {
    RestoreTriangleWinding();
}

void MarioRaceway::BeginPlay() {
    struct Actor* actor;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };

    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_tree_spawns));
    spawn_piranha_plants((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_piranha_plant_spawns));
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_item_box_spawns));

    AMarioSign::Spawn(FVector(150.0f, 40.0f, -1300.0f), IRotator(0, 0, 0), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f));
    AMarioSign::Spawn(FVector(2520.0f, 0.0f, 1240.0f), IRotator(0, 0, 0), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f));

    if (gModeSelection == VERSUS) {
        OBombKart::Spawn(0, 40, 3, 0.8333333f);
        OBombKart::Spawn(0, 100, 3, 0.8333333f);
        OBombKart::Spawn(0, 265, 3, 0.8333333f);
        OBombKart::Spawn(0, 285, 1, 0.8333333f);
        OBombKart::Spawn(0, 420, 1, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
        OBombKart::Spawn(0, 0, 0, 0.8333333f);
    }

    if (gGamestate != CREDITS_SEQUENCE) {
        OGrandPrixBalloons::Spawn(FVector(0, 5, -240));
    }
}

void MarioRaceway::InitCourseObjects() {
    if (gGamestate != CREDITS_SEQUENCE) {
        if (gModeSelection == GRAND_PRIX) {
            func_80070714();
        }
    }
}

void MarioRaceway::SomeSounds() {
    vec3f_set(D_8015F748, -223.0f, 94.0f, -155.0f);
    func_800C9D80(D_8015F748, D_802B91C8, 0x5103700B);
}

void MarioRaceway::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
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

void MarioRaceway::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
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

void MarioRaceway::SetStaffGhost() {
    u32 temp_v0 = func_800B4E24(0) & 0xfffff;
    if (temp_v0 <= 9000) {
        bCourseGhostDisabled = 0;
        D_80162DF4 = 0;
    } else {
        bCourseGhostDisabled = 1;
        D_80162DF4 = 1;
    }
    D_80162DC4 = d_mario_raceway_staff_ghost;
    D_80162DE4 = 0;
}

void render_mario_raceway_pipe(void) {
    if (gScreenModeSelection == SCREEN_MODE_1P) {
        // d_course_mario_raceway_packed_dl_8E8
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_8E8);
    } else {
        if (CVarGetInteger("gDisableLod", 1) == true) {
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_8E8);
            return;
        }
        // d_course_mario_raceway_packed_dl_2D68
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_2D68);
    }
}

void MarioRaceway::Render(struct UnkStruct_800DC5EC* arg0) {
    u16 sp22 = arg0->pathCounter;
    u16 temp_t0 = arg0->playerDirection;

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_mario_raceway_packed_dl_3050
        gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_3050);
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    switch (sp22) {
        case 1:
        case 2:
        case 17:
            if ((temp_t0 == 2) || (temp_t0 == 1)) {
                render_mario_raceway_pipe();
            }
            break;
        case 3:
            if (temp_t0 != 0) {
                render_mario_raceway_pipe();
            }
            break;
        case 4:
            if (temp_t0 == 0) {
                func_80291198();
            } else {
                if (temp_t0 == 1) {
                    func_80291198();
                }
                render_mario_raceway_pipe();
            }
            break;
        case 5:
        case 6:
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                render_mario_raceway_pipe();
            } else {
                func_80291198();
            }
            break;
        case 7:
            func_80291198();
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                render_mario_raceway_pipe();
            }
            break;
        case 8:
        case 9:
            if (temp_t0 != 1) {
                render_mario_raceway_pipe();
            }
            /* fallthrough */
        case 10:
            if (temp_t0 != 2) {
                func_80291198();
            }
            break;
        case 11:
            if (temp_t0 == 0) {
                render_mario_raceway_pipe();
                func_80291198();
            } else if (temp_t0 == 3) {
                render_mario_raceway_pipe();
            }
            break;
        case 12:
            if ((temp_t0 == 0) || (temp_t0 == 3)) {
                render_mario_raceway_pipe();
            }
            break;
        case 13:
        case 14:
            if (temp_t0 != 1) {
                case 15:
                case 16:
                    render_mario_raceway_pipe();
            }
            break;
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_3508);
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_3240);
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_14A0);
    render_course_segments(mario_raceway_dls, arg0);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_450);
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_240);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_E0);
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_160);
}

void MarioRaceway::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_mario_raceway_dl_9348));
}

void MarioRaceway::CreditsSpawnActors() {
    spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_mario_raceway_tree_spawns));
}

void MarioRaceway::Destroy() { }
