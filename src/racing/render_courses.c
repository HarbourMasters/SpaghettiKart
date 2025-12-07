#include <libultraship.h>
#include <libultra/gbi.h>
#include <macros.h>
#include <mk64.h>
#include <common_structs.h>
#include <defines.h>
#include <course.h>
#include <stdio.h>
#include "../camera.h"
#include "framebuffer_effects.h"
#include "port/interpolation/FrameInterpolation.h"

#include "render_courses.h"
#include "code_800029B0.h"
#include "main.h"
#include "actors.h"
#include "math_util.h"
#include "memory.h"
#include "code_80281780.h"
#include "collision.h"
#include "resourcebridge.h"
#include "skybox_and_splitscreen.h"
#include "courses/all_course_data.h"
#include "courses/all_course_packed.h"
#include "courses/all_course_offsets.h"
#include "port/Game.h"
#include "engine/Matrix.h"
#include "engine/courses/Course.h"

#include "enhancements/collision_viewer.h"

s16 D_802B87B0 = 995;
s16 D_802B87B4 = 1000;
UNUSED s32 D_802B87B8 = 0;
s32 D_802B87BC = 0;
UNUSED s32 D_802B87C0 = 0;
s32 D_802B87C4 = 0;
s32 D_802B87C8 = 0;
s32 D_802B87CC = 0;
s16 D_802B87D0 = 0;
s16 D_802B87D4 = 0;
s16 currentScreenSection = 0;

s32 func_80290C20(Camera* camera) {
    if (camera->collision.unk34 == 0) {
        return 1;
    }
    if ((camera->collision.unk30 == 1) && (camera->collision.surfaceDistance[0] < 3.0f)) {
        return 1;
    }
    if ((camera->collision.unk32 == 1) && (camera->collision.surfaceDistance[1] < 3.0f)) {
        return 1;
    }
    return 0;
}

void parse_course_displaylists(TrackSections* asset) {
    TrackSections* section = (TrackSections*) asset;

    while (section->crc != 0) {
        if (section->flags & 0x8000) {
            D_8015F59C = 1;
        } else {
            D_8015F59C = 0;
        }
        if (section->flags & 0x2000) {
            D_8015F5A0 = 1;
        } else {
            D_8015F5A0 = 0;
        }
        if (section->flags & 0x4000) {
            D_8015F5A4 = 1;
        } else {
            D_8015F5A4 = 0;
        }
        // char* name = ResourceGetNameByCrc(section->crc);
        // printf("Generating collision mesh for section %d: %s\n", section->sectionId, name != NULL ? name :
        // "Unknown");
        void* addr = ResourceGetDataByCrc(section->crc);
        if (addr == NULL) {
            printf("Warning: Could not find resource for section %d with crc 0x%llX\n", section->sectionId,
                   section->crc);
            addr = (void*) section->crc;
        }
        generate_collision_mesh(addr, section->surfaceType, section->sectionId);
        section++;
    }
}

extern u32 isFlycam;

void render_course_segments(const char* addr[], struct UnkStruct_800DC5EC* arg1) {
    Player* player = arg1->player;
    Camera* camera = arg1->camera;
    s16 direction;
    s16 index;
    s16 sp1E;
    s16 temp_v0_3;
    u16 rot;
    if (gIsMirrorMode) {
        rot = (u16) camera->rot[1];
        if (rot < 0x2000) {
            direction = SOUTH;
        } else if (rot < 0x6000) {
            direction = WEST;
        } else if (rot < 0xA000) {
            direction = NORTH;
        } else if (rot < 0xE000) {
            direction = EAST;
        } else {
            direction = SOUTH;
        }
    } else {
        rot = (u16) camera->rot[1];
        if (rot < 0x2000) {
            direction = SOUTH;
        } else if (rot < 0x6000) {
            direction = EAST;
        } else if (rot < 0xA000) {
            direction = NORTH;
        } else if (rot < 0xE000) {
            direction = WEST;
        } else {
            direction = SOUTH;
        }
    }
    arg1->playerDirection = direction;

    if (camera->mode == 1) {
        sp1E = get_track_section_id(camera->collision.meshIndexZX);
        temp_v0_3 = get_track_section_id(player->collision.meshIndexZX);
        index = sp1E - temp_v0_3;
        if ((index < 2) && (index >= -1)) {
            if (sp1E == 255) {
                if (temp_v0_3 == 255) {
                    index = arg1->pathCounter;
                } else if (player->collision.surfaceDistance[2] > 30.0f) {
                    index = arg1->pathCounter;
                } else {
                    index = temp_v0_3;
                }
            } else if (camera->collision.surfaceDistance[2] > 30.0f) {
                index = arg1->pathCounter;
            } else {
                index = sp1E;
            }
        } else {
            if (IsBowsersCastle()) {
                if ((temp_v0_3 >= 0x11) && (temp_v0_3 < 0x18)) {
                    index = temp_v0_3;
                } else if ((temp_v0_3 == 255) && (sp1E != 255)) {
                    index = sp1E;
                } else if ((temp_v0_3 != 255) && (sp1E == 255)) {
                    index = temp_v0_3;
                } else {
                    index = arg1->pathCounter;
                }
            } else if (IsChocoMountain()) {
                if ((temp_v0_3 >= 0xE) && (temp_v0_3 < 0x16)) {
                    index = temp_v0_3;
                } else if ((temp_v0_3 == 255) && (sp1E != 255)) {
                    index = sp1E;
                } else if ((temp_v0_3 != 255) && (sp1E == 255)) {
                    index = temp_v0_3;
                } else {
                    index = arg1->pathCounter;
                }
            } else {
                if (temp_v0_3 == 255) {
                    index = arg1->pathCounter;
                } else if (player->collision.surfaceDistance[2] > 30.0f) {
                    index = arg1->pathCounter;
                } else {
                    index = temp_v0_3;
                }
            }
        }
    } else {
        index = get_track_section_id(camera->collision.meshIndexZX);
        if (camera->collision.surfaceDistance[2] > 30.0f) {
            index = arg1->pathCounter;
        } else if (index == 255) {
            index = arg1->pathCounter;
        }
    }

    arg1->pathCounter = index;
    index = ((index - 1) * 4) + direction;
    gSPDisplayList(gDisplayListHead++, addr[index]);

    if (CVarGetInteger("gDisableLod", 1) == 1 && (IsBowsersCastle()) &&
        (index < 20 || index > 99)) { // always render higher version of bowser statue
        gDisplayListHead--;
        gSPDisplayList(gDisplayListHead++, d_course_bowsers_castle_dl_9148); // use credit version of the course
    }
}

void func_80291198(void) {
    // d_course_mario_raceway_packed_dl_1140
    gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_mario_raceway_packed_dl_1140); //
}

void func_8029122C(struct UnkStruct_800DC5EC* screen, s32 playerId) {
    UNUSED s32 pad;
    Player* player = screen->player;
    Mat4 matrix;
    Vec3f vector;
    u16 pathCounter;
    u16 cameraRot;
    s16 playerDirection;

    init_rdp();
    pathCounter = (u16) screen->pathCounter;
    cameraRot = (u16) screen->camera->rot[1];
    playerDirection = screen->playerDirection;

    // This pushes the camera matrices to the top of the stack.
    // It does not appear to really do anything.
    // Perhaps they thought it was necessary to set the camera back to projection mode since rainbow road uses model mode.
    // But that issue should be cleared up in render_screens() already.
    gSPMatrix(gDisplayListHead++, screen->camera->perspectiveMatrix,
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, screen->camera->lookAtMatrix,
                G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    FrameInterpolation_RecordOpenChild("track_water", screen->camera->cameraId);

    mtxf_identity(matrix);
    if (gIsMirrorMode != 0) {
        matrix[0][0] = -1.0f;
        // matrix[1][1] = -1.0f;
        // matrix[2][2] = -1.0f;
    }
    render_set_position(matrix, 0);

    CM_DrawWater(screen, pathCounter, cameraRot, playerDirection);
    FrameInterpolation_RecordCloseChild();
}

void render_course(struct UnkStruct_800DC5EC* screen) {
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    CM_RenderCourse(screen);
}

void func_80295BF8(s32 playerIndex) {
    Player* player = &gPlayers[playerIndex];
    func_802AAAAC(&player->collision);
    player->tyres[FRONT_RIGHT].surfaceFlags = 0;
    player->tyres[FRONT_LEFT].surfaceFlags = 0;
    player->tyres[BACK_RIGHT].surfaceFlags = 0;
    player->tyres[BACK_LEFT].surfaceFlags = 0;

    player->tyres[FRONT_RIGHT].collisionMeshIndex = 0x1388;
    player->tyres[FRONT_LEFT].collisionMeshIndex = 0x1388;
    player->tyres[BACK_RIGHT].collisionMeshIndex = 0x1388;
    player->tyres[BACK_LEFT].collisionMeshIndex = 0x1388;
}

void func_80295C6C(void) {
    gNextFreeMemoryAddress += ALIGN16(gCollisionMeshCount * sizeof(CollisionTriangle));
    gCourseMaxX += 20;
    gCourseMaxZ += 20;
    gCourseMinX += -20;
    gCourseMinZ += -20;
    gCourseMinY += -20;

    gCollisionIndices = (u16*) gNextFreeMemoryAddress;
    generate_collision_grid();
    gNextFreeMemoryAddress += ALIGN16(gNumCollisionTriangles * sizeof(u16));
}

UNUSED void func_80295D50(s16 arg0, s16 arg1) {
    D_8015F6F4 = arg1;
    D_8015F6F6 = arg0;
}

void func_80295D6C(void) {
    D_8015F6F4 = 3000;
    D_8015F6F6 = -3000;
}

void course_init(void) {
}

void func_802966A0(void) {
    CM_ScrollingTextures();
}
