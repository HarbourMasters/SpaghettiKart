#include <stdint.h>
#include <libultraship.h>
#include <macros.h>
#include <libultra/gbi.h>
#include <mk64.h>
#include <common_structs.h>
#include <actor_types.h>
#include "main.h"
#include "memory.h"
#include "collision.h"
#include "math_util.h"
#include "code_800029B0.h"
#include <defines.h>
#include "port/Engine.h"
#include "port/Game.h"
#include "resourcebridge.h"
#include <stdio.h>
#include "RaceManager.h"

#pragma intrinsic(sqrtf)

// Used to delete the choco mountain guard rail
void nullify_displaylist(uintptr_t addr) {
    Gfx* macro;

    macro = (Gfx*) addr;
    macro->words.w0 = (G_ENDDL << 24);
    macro->words.w1 = 0;
}

void func_802AAAAC(Collision* collision) {
    collision->meshIndexYX = 5000;
    collision->meshIndexZY = 5000;
    collision->meshIndexZX = 5000;
    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    collision->surfaceDistance[0] = 0;
    collision->surfaceDistance[1] = 0;
    collision->surfaceDistance[2] = 0;
    vec3f_set(collision->unk48, 0.0f, 0.0f, 1.0f);
    vec3f_set(collision->unk54, 1.0f, 0.0f, 0.0f);
    vec3f_set(collision->orientationVector, 0.0f, 1.0f, 0.0f);
}

f32 get_water_level(Player* player) {
    return CM_GetWaterLevel(player->pos, &player->collision);
}

s32 check_collision_zx(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    UNUSED f32 pad;
    f32 x3;
    UNUSED f32 pad2;
    f32 z3;
    UNUSED f32 pad3[4];
    f32 x2;
    f32 z2;
    f32 x1;
    f32 z1;
    f32 distanceToSurface;
    f32 crossProductZX_1;
    f32 crossProductZX_2;
    f32 crossProductZX_3;
    s32 b = true;

    if (triangle->normalY < -0.9f) {
        return 0;
    }
    if (triangle->minX > posX) {
        return 0;
    }
    if (triangle->minZ > posZ) {
        return 0;
    }
    if (triangle->maxX < posX) {
        return 0;
    }
    if (triangle->maxZ < posZ) {
        return 0;
    }
    if ((triangle->minY - boundingBoxSize * 3.0f) > posY) {
        return 0;
    }

    x1 = triangle->vtx1->v.ob[0];
    z1 = triangle->vtx1->v.ob[2];

    x2 = triangle->vtx2->v.ob[0];
    z2 = triangle->vtx2->v.ob[2];

    x3 = triangle->vtx3->v.ob[0];
    z3 = triangle->vtx3->v.ob[2];

    crossProductZX_1 = (z1 - posZ) * (x2 - posX) - (x1 - posX) * (z2 - posZ);

    if (!crossProductZX_1) {

        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

        if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
            b = false;
        }
    } else {

        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        if (!crossProductZX_2) {
            crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

            if ((crossProductZX_1 * crossProductZX_3) < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductZX_1 * crossProductZX_2) < 0.0f) {
                b = false;
            } else {
                crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);
                if (crossProductZX_3 != 0) {
                    if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    if (!b) {
        return 0;
    }
    distanceToSurface =
        ((triangle->normalX * posX) + (triangle->normalY * posY) + (triangle->normalZ * posZ) + triangle->distance) -
        boundingBoxSize;
    if (distanceToSurface > 0.0f) {
        if (collision->surfaceDistance[2] > distanceToSurface) {
            collision->unk34 = 1;
            collision->meshIndexZX = index;
            collision->surfaceDistance[2] = distanceToSurface;
            collision->orientationVector[0] = triangle->normalX;
            collision->orientationVector[1] = triangle->normalY;
            collision->orientationVector[2] = triangle->normalZ;
        }
        return 0;
    }

    if (distanceToSurface > -16.0f) {
        collision->unk34 = 1;
        collision->meshIndexZX = index;
        collision->surfaceDistance[2] = distanceToSurface;
        collision->orientationVector[0] = triangle->normalX;
        collision->orientationVector[1] = triangle->normalY;
        collision->orientationVector[2] = triangle->normalZ;
        return 1;
    }
    return 0;
}

s32 check_collision_yx(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    UNUSED f32 pad[6];
    f32 x3;
    f32 y3;
    UNUSED f32 pad2[1];

    UNUSED f32 pad3[5];
    f32 x2;
    f32 y2;
    f32 x1;
    f32 y1;
    f32 distanceToSurface;
    f32 crossProductYX_1;
    f32 crossProductYX_2;
    f32 crossProductYX_3;
    s32 b = true;

    if (triangle->minX > posX) {
        return 0;
    }
    if (triangle->maxX < posX) {
        return 0;
    }
    if (triangle->maxY < posY) {
        return 0;
    }
    if (triangle->minY > posY) {
        return 0;
    }

    if ((triangle->minZ - boundingBoxSize * 3.0f) > posZ) {
        return 0;
    }
    if ((triangle->maxZ + boundingBoxSize * 3.0f) < posZ) {
        return 0;
    }

    x1 = triangle->vtx1->v.ob[0];
    y1 = triangle->vtx1->v.ob[1];

    x2 = triangle->vtx2->v.ob[0];
    y2 = triangle->vtx2->v.ob[1];

    x3 = triangle->vtx3->v.ob[0];
    y3 = triangle->vtx3->v.ob[1];

    crossProductYX_1 = (y1 - posY) * (x2 - posX) - (x1 - posX) * (y2 - posY);

    if (!crossProductYX_1) {

        crossProductYX_2 = (y2 - posY) * (x3 - posX) - (x2 - posX) * (y3 - posY);

        crossProductYX_3 = (y3 - posY) * (x1 - posX) - (x3 - posX) * (y1 - posY);

        if ((crossProductYX_2 * crossProductYX_3) < 0.0f) {
            b = false;
        }
    } else {
        crossProductYX_2 = (y2 - posY) * (x3 - posX) - (x2 - posX) * (y3 - posY);
        if (!crossProductYX_2) {
            crossProductYX_3 = (y3 - posY) * (x1 - posX) - (x3 - posX) * (y1 - posY);
            if (crossProductYX_1 * crossProductYX_3 < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductYX_1 * crossProductYX_2) < 0.0f) {
                b = false;
            } else {
                crossProductYX_3 = ((y3 - posY) * (x1 - posX)) - ((x3 - posX) * (y1 - posY));
                if (crossProductYX_3 != 0) {
                    if ((crossProductYX_2 * crossProductYX_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    if (!b) {
        return 0;
    }
    distanceToSurface =
        ((triangle->normalX * posX) + (triangle->normalY * posY) + (triangle->normalZ * posZ) + triangle->distance) -
        boundingBoxSize;
    if (distanceToSurface > 0.0f) {
        if (distanceToSurface < collision->surfaceDistance[0]) {
            collision->unk30 = 1;
            collision->meshIndexYX = index;
            collision->surfaceDistance[0] = distanceToSurface;
            collision->unk48[0] = triangle->normalX;
            collision->unk48[1] = triangle->normalY;
            collision->unk48[2] = triangle->normalZ;
        }
        return 0;
    }

    if (distanceToSurface > -16.0f) {
        collision->unk30 = 1;
        collision->meshIndexYX = index;
        collision->surfaceDistance[0] = distanceToSurface;
        collision->unk48[0] = triangle->normalX;
        collision->unk48[1] = triangle->normalY;
        collision->unk48[2] = triangle->normalZ;
        return 1;
    }
    return 0;
}

s32 check_collision_zy(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    s32 b = true;
    UNUSED f32 pad[7];
    f32 y3;
    f32 z3;
    UNUSED f32 pad2[5];
    f32 y2;
    f32 z2;
    f32 y1;
    f32 z1;
    f32 distanceToSurface;
    f32 crossProductYZ_1;
    f32 crossProductYZ_2;
    f32 crossProductYZ_3;
    if (triangle->minZ > posZ) {
        return 0;
    }
    if (triangle->maxZ < posZ) {
        return 0;
    }
    if (triangle->maxY < posY) {
        return 0;
    }
    if (triangle->minY > posY) {
        return 0;
    }
    if ((triangle->minX - (boundingBoxSize * 3.0f)) > posX) {
        return 0;
    }
    if ((triangle->maxX + (boundingBoxSize * 3.0f)) < posX) {
        return 0;
    }

    z1 = triangle->vtx1->v.ob[2];
    y1 = triangle->vtx1->v.ob[1];

    z2 = triangle->vtx2->v.ob[2];
    y2 = triangle->vtx2->v.ob[1];

    z3 = triangle->vtx3->v.ob[2];
    y3 = triangle->vtx3->v.ob[1];

    crossProductYZ_1 = (y1 - posY) * (z2 - posZ) - (z1 - posZ) * (y2 - posY);

    if (!crossProductYZ_1) {

        crossProductYZ_2 = ((y2 - posY) * (z3 - posZ)) - ((z2 - posZ) * (y3 - posY));

        crossProductYZ_3 = ((y3 - posY) * (z1 - posZ)) - ((z3 - posZ) * (y1 - posY));

        if ((crossProductYZ_2 * crossProductYZ_3) < 0.0f) {
            b = false;
        }
    } else {

        crossProductYZ_2 = ((y2 - posY) * (z3 - posZ)) - ((z2 - posZ) * (y3 - posY));

        if (crossProductYZ_2 == 0) {
            crossProductYZ_3 = ((y3 - posY) * (z1 - posZ)) - ((z3 - posZ) * (y1 - posY));

            if ((crossProductYZ_1 * crossProductYZ_3) < 0.0f) {
                b = false;
            }
        } else {

            if ((crossProductYZ_1 * crossProductYZ_2) < 0.0f) {
                b = false;
            } else {
                crossProductYZ_3 = ((y3 - posY) * (z1 - posZ)) - ((z3 - posZ) * (y1 - posY));
                if (crossProductYZ_3 != 0) {
                    if ((crossProductYZ_2 * crossProductYZ_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    if (!b) {
        return 0;
    }

    distanceToSurface = ((((triangle->normalX * posX) + (triangle->normalY * posY)) + (triangle->normalZ * posZ)) +
                         triangle->distance) -
                        boundingBoxSize;

    if (distanceToSurface > 0.0f) {
        if (distanceToSurface < collision->surfaceDistance[1]) {
            collision->unk32 = 1;
            collision->meshIndexZY = index;
            collision->surfaceDistance[1] = distanceToSurface;
            collision->unk54[0] = triangle->normalX;
            collision->unk54[1] = triangle->normalY;
            collision->unk54[2] = triangle->normalZ;
        }
        return 0;
    }
    if (distanceToSurface > (-16.0f)) {
        collision->unk32 = 1;
        collision->meshIndexZY = index;
        collision->surfaceDistance[1] = distanceToSurface;
        collision->unk54[0] = triangle->normalX;
        collision->unk54[1] = triangle->normalY;
        collision->unk54[2] = triangle->normalZ;
        return 1;
    }
    return 0;
}

s32 check_horizontally_colliding_with_triangle(f32 posX, f32 posZ, u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    UNUSED f32 pad;
    f32 x3;
    UNUSED f32 pad2;
    f32 z3;
    f32 x2;
    UNUSED f32 pad3;
    f32 z2;
    f32 x1;
    f32 z1;
    UNUSED f32 pad4[4];
    f32 crossProductZX_1;
    f32 crossProductZX_3;
    f32 crossProductZX_2;
    s32 b = true;

    x1 = triangle->vtx1->v.ob[0];
    z1 = triangle->vtx1->v.ob[2];

    x2 = triangle->vtx2->v.ob[0];
    z2 = triangle->vtx2->v.ob[2];

    x3 = triangle->vtx3->v.ob[0];
    z3 = triangle->vtx3->v.ob[2];

    crossProductZX_1 = (z1 - posZ) * (x2 - posX) - (x1 - posX) * (z2 - posZ);

    if (!crossProductZX_1) {

        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

        if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
            b = false;
        }
    } else {
        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);
        if (!crossProductZX_2) {
            crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);
            if (crossProductZX_1 * crossProductZX_3 < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductZX_1 * crossProductZX_2) < 0.0f) {
                b = false;
            } else {
                crossProductZX_3 = ((z3 - posZ) * (x1 - posX)) - ((x3 - posX) * (z1 - posZ));
                if (crossProductZX_3 != 0) {
                    if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    return b;
}

s8 get_surface_type(u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    return triangle->surfaceType;
}

s16 get_track_section_id(u16 index) {
    if (index >= gCollisionMeshCount) {
        // printf("[collision.c] [get_track_section_id] Warning: Trying to access a collision triangle index %d\n
        //  Which is overflows gCollisionMesh, as its total size is %d\n", index, gCollisionMeshCount);
        return 0xFF;
    }
    CollisionTriangle* triangle = &gCollisionMesh[index];
    return triangle->flags & 0xFF;
}

s16 func_802ABD7C(u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    return triangle->flags & 0x1000;
}

s16 func_802ABDB8(u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    return triangle->flags & 0x400;
}

s16 func_802ABDF4(u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    return triangle->flags & 0x800;
}

f32 calculate_surface_height(f32 x, f32 y, f32 z, u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    if (triangle->normalY == 0.0f) {
        return y;
    }
    return ((triangle->normalX * x) + (triangle->normalZ * z) + triangle->distance) / -triangle->normalY;
}

f32 func_802ABEAC(Collision* collision, Vec3f pos) {
    if (collision->unk34 == 1) {
        return calculate_surface_height(pos[0], pos[1], pos[2], collision->meshIndexZX);
    }
    if (collision->unk30 == 1) {
        return calculate_surface_height(pos[0], pos[1], pos[2], collision->meshIndexYX);
    }
    if (collision->unk32 == 1) {
        return calculate_surface_height(pos[0], pos[1], pos[2], collision->meshIndexZY);
    }
    return pos[1];
}

void process_shell_collision(Vec3f pos, UNUSED f32 boundingBoxSize, Vec3f velocity, UNUSED f32 unk) {
    f32 magnitude;
    f32 dotProduct;
    f32 x;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 y;
    f32 z;
    UNUSED f32 x2;
    UNUSED f32 y2;
    f32 velocityMagnitude;
    UNUSED f32 z2;
    f32 scaleFactor;

    x = velocity[0];
    y = velocity[1];
    z = velocity[2];
    velocityMagnitude = sqrtf((x * x) + (y * y) + (z * z));

    if ((velocityMagnitude > 4.5) || (velocityMagnitude < 3.5)) {
        velocityMagnitude = 4.0f;
    }

    x2 = pos[0];
    y2 = pos[1];
    z2 = pos[2];

    dotProduct = (pos[0] * x) + (pos[1] * y) + (pos[2] * z);
    scaleX = x - dotProduct * pos[0];
    scaleY = y - dotProduct * pos[1];
    scaleZ = z - dotProduct * pos[2];

    x = scaleX - dotProduct * pos[0];
    y = scaleY - dotProduct * pos[1];
    z = scaleZ - dotProduct * pos[2];

    magnitude = sqrtf((x * x) + (y * y) + (z * z));

    scaleFactor = 1.0f / magnitude * velocityMagnitude;

    velocity[0] = x * scaleFactor;
    velocity[1] = y * scaleFactor;
    velocity[2] = z * scaleFactor;
}

void shell_collision(Collision* collision, Vec3f velocity) {
    if (collision->surfaceDistance[0] < 0.0f) {
        process_shell_collision(collision->unk48, collision->surfaceDistance[0], velocity, 2.0f);
    }

    if (collision->surfaceDistance[1] < 0.0f) {
        process_shell_collision(collision->unk54, collision->surfaceDistance[1], velocity, 2.0f);
    }
}

/**
 * Adjusts the position of pos2 based on pos1 but in the orthogonal direction to pos2.
 */
void adjust_pos_orthogonally(Vec3f pos1, f32 boundingBoxSize, Vec3f pos2, UNUSED f32 unk) {
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 dotProduct;
    f32 orthoX;
    f32 orthoY;
    f32 orthoZ;

    x2 = pos2[0];
    y2 = pos2[1];
    z2 = pos2[2];
    x1 = -pos1[0];
    y1 = -pos1[1];
    z1 = -pos1[2];

    dotProduct = (x1 * x2) + (y1 * y2) + (z1 * z2);

    orthoX = x2 - (dotProduct * x1);
    orthoY = y2 - (dotProduct * y1);
    orthoZ = z2 - (dotProduct * z1);

    if (boundingBoxSize < -3.5) {
        pos2[0] = orthoX - (dotProduct * x1 * 0.5f);
        pos2[1] = orthoY - (dotProduct * y1 * 0.5f);
        pos2[2] = orthoZ - (dotProduct * z1 * 0.5f);
    } else {
        pos2[0] = orthoX;
        pos2[1] = orthoY;
        pos2[2] = orthoZ;
    }
}

UNUSED s32 detect_tyre_collision(KartTyre* tyre) {
    Collision collision;
    UNUSED s32 pad[12];
    s32 courseLengthX;
    s32 courseLengthZ;
    f32 tyreX;
    f32 tyreY;
    f32 tyreZ;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    u16 i;
    u16 numTriangles;
    u16 meshIndex;
    s16 gridIndex;
    u16 sectionIndex;

    collision.unk30 = 0;
    collision.unk32 = 0;
    collision.unk34 = 0;
    collision.surfaceDistance[0] = 1000.0f;
    collision.surfaceDistance[1] = 1000.0f;
    collision.surfaceDistance[2] = 1000.0f;
    tyreX = tyre->pos[0];
    tyreY = tyre->pos[1];
    tyreZ = tyre->pos[2];
    switch (tyre->surfaceFlags) { /* irregular */
        case 0x80:
            if (check_collision_zy(&collision, 5.0f, tyreX, tyreY, tyreZ, (u16) (s32) tyre->collisionMeshIndex) == 1) {
                tyre->baseHeight = calculate_surface_height(tyreX, tyreY, tyreZ, tyre->collisionMeshIndex);
                return 1;
            }
            break;
        case 0x40:
            if (check_collision_zx(&collision, 5.0f, tyreX, tyreY, tyreZ, (u16) (s32) tyre->collisionMeshIndex) == 1) {
                tyre->baseHeight = calculate_surface_height(tyreX, tyreY, tyreZ, tyre->collisionMeshIndex);
                return 1;
            }
            break;
        case 0x20:
            if (check_collision_yx(&collision, 5.0f, tyreX, tyreY, tyreZ, (u16) (s32) tyre->collisionMeshIndex) == 1) {
                tyre->baseHeight = calculate_surface_height(tyreX, tyreY, tyreZ, tyre->collisionMeshIndex);
                return 1;
            }
            break;
        default:
            break;
    }
    courseLengthX = gCourseMaxX - gCourseMinX;
    courseLengthZ = gCourseMaxZ - gCourseMinZ;
    sectionIndexX = (tyreX - gCourseMinX) / (courseLengthX / GRID_SIZE);
    sectionIndexZ = (tyreZ - gCourseMinZ) / (courseLengthZ / GRID_SIZE);
    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = (sectionIndexX + sectionIndexZ * GRID_SIZE);
    numTriangles = gCollisionGrid[gridIndex].numTriangles;
    if (numTriangles == 0) {
        return 0;
    }
    sectionIndex = gCollisionGrid[gridIndex].triangle;
    for (i = 0; i < numTriangles; i++) {
        meshIndex = gCollisionIndices[sectionIndex];
        if (gCollisionMesh[meshIndex].flags & FACING_Y_AXIS) {
            if (meshIndex != tyre->collisionMeshIndex) {
                if (check_collision_zx(&collision, 5.0f, tyreX, tyreY, tyreZ, meshIndex) == 1) {
                    tyre->baseHeight = calculate_surface_height(tyreX, tyreY, tyreZ, meshIndex);
                    tyre->surfaceType = gCollisionMesh[meshIndex].surfaceType;
                    tyre->surfaceFlags = 0x40;
                    tyre->collisionMeshIndex = meshIndex;
                    if (gCollisionMesh[meshIndex].flags & 0x1000) {
                        tyre->unk_14 = 1;
                    } else {
                        tyre->unk_14 = 0;
                    }
                    return 1;
                }
            }
        } else if (gCollisionMesh[meshIndex].flags & FACING_X_AXIS) {
            if ((gCollisionMesh[meshIndex].normalX != 1.0f) && (meshIndex != tyre->collisionMeshIndex)) {
                if (check_collision_zy(&collision, 5.0f, tyreX, tyreY, tyreZ, meshIndex) == 1) {
                    tyre->baseHeight = calculate_surface_height(tyreX, tyreY, tyreZ, meshIndex);
                    tyre->surfaceType = gCollisionMesh[meshIndex].surfaceType;
                    tyre->surfaceFlags = 0x80;
                    tyre->collisionMeshIndex = meshIndex;
                    return 1;
                }
            }
        } else if ((gCollisionMesh[meshIndex].normalZ != 1.0f) && (meshIndex != tyre->collisionMeshIndex)) {
            if (check_collision_yx(&collision, 5.0f, tyreX, tyreY, tyreZ, meshIndex) == 1) {
                tyre->baseHeight = calculate_surface_height(tyreX, tyreY, tyreZ, meshIndex);
                tyre->surfaceType = gCollisionMesh[meshIndex].surfaceType;
                tyre->surfaceFlags = 0x20;
                tyre->collisionMeshIndex = meshIndex;
                return 1;
            }
        }
        sectionIndex++;
    }
    tyre->baseHeight = tyreY;
    tyre->surfaceType = 0;
    //! @bug
    // Another function that has a return value but doesn't have an explicit return statement in one of its codepaths.
    // The return value at this point will be whatever was last returned by func_802AAE4C/func_802AB6C4/func_802AB288
    // depending on which (if any) if statements were entered on the loop's last cycle
}

s32 is_colliding_with_drivable_surface(Collision* collision, f32 boundingBoxSize, f32 newX, f32 newY, f32 newZ,
                                       u16 index, f32 oldX, f32 oldY, f32 oldZ) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    UNUSED s32 pad;
    f32 x4;
    UNUSED f32 y4;
    f32 z4;
    f32 x3;
    UNUSED f32 y3;
    f32 z3;
    f32 x2;
    UNUSED f32 y2;
    f32 z2;
    f32 surfaceDistance;
    f32 temp;
    UNUSED s32 pad2[2];
    f32 area;
    f32 area2;
    f32 area3;
    s32 b = 1;

    if (triangle->minX > newX) {
        return 0;
    }
    if (triangle->minZ > newZ) {
        return 0;
    }
    if (triangle->maxX < newX) {
        return 0;
    }
    if (triangle->maxZ < newZ) {
        return 0;
    }
    if ((triangle->minY - boundingBoxSize * 3.0f) > newY) {
        return 0;
    }

    x2 = (f32) triangle->vtx1->v.ob[0];
    z2 = (f32) triangle->vtx1->v.ob[2];

    x3 = (f32) triangle->vtx2->v.ob[0];
    z3 = (f32) triangle->vtx2->v.ob[2];

    x4 = (f32) triangle->vtx3->v.ob[0];
    z4 = (f32) triangle->vtx3->v.ob[2];

    area = (z2 - newZ) * (x3 - newX) - (x2 - newX) * (z3 - newZ);

    if (area == 0) {
        area2 = (z3 - newZ) * (x4 - newX) - (x3 - newX) * (z4 - newZ);
        area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);
        if (area2 * area3 < 0.0f) {
            b = 0;
        }
    } else {

        area2 = (z3 - newZ) * (x4 - newX) - (x3 - newX) * (z4 - newZ);

        if (area2 == 0) {

            area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);

            if (area * area3 < 0.0f) {
                b = 0;
            }
        } else {
            if ((area * area2) < 0.0f) {
                b = 0;
            } else {
                area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);
                if (area3 != 0) {
                    if (area2 * area3 < 0.0f) {
                        b = 0;
                    }
                }
            }
        }
    }
    if (b == 0) {
        return 0;
    }

    surfaceDistance =
        (triangle->normalX * newX) + (triangle->normalY * newY) + (triangle->normalZ * newZ) + triangle->distance;

    if (surfaceDistance > boundingBoxSize) {
        if (collision->surfaceDistance[2] > surfaceDistance) {
            collision->unk34 = 1;
            collision->meshIndexZX = index;
            collision->surfaceDistance[2] = surfaceDistance - boundingBoxSize;
            collision->orientationVector[0] = triangle->normalX;
            collision->orientationVector[1] = triangle->normalY;
            collision->orientationVector[2] = triangle->normalZ;
        }
        return 0;
    }

    temp = (triangle->normalX * oldX) + (triangle->normalY * oldY) + (triangle->normalZ * oldZ) + triangle->distance;

    if (temp < 0.0f) {
        return 0;
    }

    collision->unk34 = 1;
    collision->meshIndexZX = index;
    collision->surfaceDistance[2] = surfaceDistance - boundingBoxSize;
    collision->orientationVector[0] = triangle->normalX;
    collision->orientationVector[1] = triangle->normalY;
    collision->orientationVector[2] = triangle->normalZ;
    return 1;
}

/**
 * Wall collision
 */
s32 is_colliding_with_wall2(Collision* arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 surfaceIndex, f32 posX,
                            f32 posY, f32 posZ) {
    if (CVarGetInteger("gNoWallColision", 0)) {
        return NO_COLLISION;
    }
    CollisionTriangle* triangle = &gCollisionMesh[surfaceIndex];
    UNUSED s32 pad[6];
    f32 x4;
    f32 y4;
    f32 x3;
    f32 y3;
    f32 x2;
    f32 y2;
    UNUSED s32 pad2[3];
    f32 distanceToSurface;
    f32 distanceToSurface2;
    UNUSED s32 pad3[2];
    f32 area;
    f32 area2;
    f32 area3;
    s32 b = true;
    if (triangle->minX > x1) {
        return NO_COLLISION;
    }
    if (triangle->maxX < x1) {
        return NO_COLLISION;
    }
    if (triangle->maxY < y1) {
        return NO_COLLISION;
    }
    if (triangle->minY > y1) {
        return NO_COLLISION;
    }
    if ((triangle->minZ - boundingBoxSize * 3.0f) > z1) {
        return NO_COLLISION;
    }
    if ((triangle->maxZ + boundingBoxSize * 3.0f) < z1) {
        return NO_COLLISION;
    }

    x2 = (f32) triangle->vtx1->v.ob[0];
    y2 = (f32) triangle->vtx1->v.ob[1];

    x3 = (f32) triangle->vtx2->v.ob[0];
    y3 = (f32) triangle->vtx2->v.ob[1];

    x4 = (f32) triangle->vtx3->v.ob[0];
    y4 = (f32) triangle->vtx3->v.ob[1];

    area = (y2 - y1) * (x3 - x1) - (x2 - x1) * (y3 - y1);

    if (area == 0) {
        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);
        area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

        if (area2 * area3 < 0.0f) {
            b = false;
        }
    } else {

        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

            if ((area * area3) < 0.0f) {
                b = false;
            }
        } else {
            if ((area * area2) < 0.0f) {
                b = false;
            } else {
                area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

                if (area3 != 0) {
                    if ((area2 * area3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }

    if (!b) {
        return NO_COLLISION;
    }

    distanceToSurface =
        ((triangle->normalX * x1) + (triangle->normalY * y1) + (triangle->normalZ * z1)) + triangle->distance;
    if (triangle->flags & 0x200) {
        distanceToSurface2 =
            ((triangle->normalX * posX) + (triangle->normalY * posY) + (triangle->normalZ * posZ)) + triangle->distance;
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 > 0.0f)) {
            if (distanceToSurface < boundingBoxSize) {
                arg->unk30 = 1;
                arg->meshIndexYX = surfaceIndex;
                arg->surfaceDistance[0] = distanceToSurface - boundingBoxSize;
                arg->unk48[0] = triangle->normalX;
                arg->unk48[1] = triangle->normalY;
                arg->unk48[2] = triangle->normalZ;
                return COLLISION;
            }
            return NO_COLLISION;
        }

        if ((distanceToSurface < 0.0f) && (distanceToSurface2 < 0.0f)) {
            distanceToSurface *= -1.0f;
            if (distanceToSurface < boundingBoxSize) {
                arg->unk30 = 1;
                arg->meshIndexYX = surfaceIndex;
                arg->surfaceDistance[0] = distanceToSurface - boundingBoxSize;
                arg->unk48[0] = -triangle->normalX;
                arg->unk48[1] = -triangle->normalY;
                arg->unk48[2] = -triangle->normalZ;
                return 1;
            }
            return NO_COLLISION;
        }
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 < 0.0f)) {
            arg->unk30 = 1;
            arg->meshIndexYX = surfaceIndex;
            arg->surfaceDistance[0] = -(distanceToSurface + boundingBoxSize);
            arg->unk48[0] = -triangle->normalX;
            arg->unk48[1] = -triangle->normalY;
            arg->unk48[2] = -triangle->normalZ;
            return COLLISION;
        }
        if ((distanceToSurface < 0.0f) && (distanceToSurface2 > 0.0f)) {
            arg->unk30 = 1;
            arg->meshIndexYX = surfaceIndex;
            arg->surfaceDistance[0] = distanceToSurface + boundingBoxSize;
            arg->unk48[0] = triangle->normalX;
            arg->unk48[1] = triangle->normalY;
            arg->unk48[2] = triangle->normalZ;
            return COLLISION;
        }
        if (distanceToSurface == 0.0f) {
            if (distanceToSurface2 >= 0.0f) {
                arg->unk30 = 1;
                arg->meshIndexYX = surfaceIndex;
                arg->surfaceDistance[0] = distanceToSurface2 + boundingBoxSize;
                arg->unk48[0] = triangle->normalX;
                arg->unk48[1] = triangle->normalY;
                arg->unk48[2] = triangle->normalZ;
                return COLLISION;
            }
            arg->unk30 = 1;
            arg->meshIndexYX = surfaceIndex;
            arg->surfaceDistance[0] = -(distanceToSurface2 + boundingBoxSize);
            arg->unk48[0] = triangle->normalX;
            arg->unk48[1] = triangle->normalY;
            arg->unk48[2] = triangle->normalZ;
            return COLLISION;
        }
        return NO_COLLISION;
    }
    if (distanceToSurface > boundingBoxSize) {
        if (distanceToSurface < arg->surfaceDistance[0]) {
            arg->unk30 = 1;
            arg->meshIndexYX = surfaceIndex;
            arg->surfaceDistance[0] = distanceToSurface - boundingBoxSize;
            arg->unk48[0] = triangle->normalX;
            arg->unk48[1] = triangle->normalY;
            arg->unk48[2] = triangle->normalZ;
        }
        return NO_COLLISION;
    }

    distanceToSurface2 =
        (triangle->normalX * posX) + (triangle->normalY * posY) + (triangle->normalZ * posZ) + triangle->distance;
    if (distanceToSurface2 < 0.0f) {
        return NO_COLLISION;
    }
    arg->unk30 = 1;
    arg->meshIndexYX = surfaceIndex;
    arg->surfaceDistance[0] = distanceToSurface - boundingBoxSize;
    arg->unk48[0] = triangle->normalX;
    arg->unk48[1] = triangle->normalY;
    arg->unk48[2] = triangle->normalZ;
    return COLLISION;
}

/**
 * This is actually more like colliding with face X/Y/Z
 */
s32 is_colliding_with_wall1(Collision* arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 surfaceIndex, f32 posX,
                            f32 posY, f32 posZ) {
    if (CVarGetInteger("gNoWallColision", 0)) {
        return NO_COLLISION;
    }
    CollisionTriangle* triangle = &gCollisionMesh[surfaceIndex];
    s32 b = 1;
    UNUSED s32 pad[7];
    f32 y4;
    f32 z4;
    f32 y3;
    f32 z3;
    f32 y2;
    f32 z2;
    UNUSED s32 pad3[2];
    f32 distanceToSurface;
    f32 distanceToSurface2;
    UNUSED s32 pad4[2];
    f32 area;
    f32 area2;
    f32 area3;

    if (triangle->minZ > z1) {
        return NO_COLLISION;
    }
    if (triangle->maxZ < z1) {
        return NO_COLLISION;
    }
    if (triangle->maxY < y1) {
        return NO_COLLISION;
    }
    if (triangle->minY > y1) {
        return NO_COLLISION;
    }
    if ((triangle->minX - boundingBoxSize * 3.0f) > x1) {
        return NO_COLLISION;
    }
    if ((triangle->maxX + boundingBoxSize * 3.0f) < x1) {
        return NO_COLLISION;
    }

    z2 = (f32) triangle->vtx1->v.ob[2];
    y2 = (f32) triangle->vtx1->v.ob[1];

    z3 = (f32) triangle->vtx2->v.ob[2];
    y3 = (f32) triangle->vtx2->v.ob[1];

    z4 = (f32) triangle->vtx3->v.ob[2];
    y4 = (f32) triangle->vtx3->v.ob[1];

    area = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);

    if (area == 0) {
        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);
        area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

        if (area2 * area3 < 0.0f) {
            b = 0;
        }
    } else {

        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

            if ((area * area3) < 0.0f) {
                b = 0;
            }
        } else {
            if ((area * area2) < 0.0f) {
                b = 0;
            } else {
                area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

                if (area3 != 0) {
                    if ((area2 * area3) < 0.0f) {
                        b = 0;
                    }
                }
            }
        }
    }
    if (b == 0) {
        return NO_COLLISION;
    }

    distanceToSurface =
        ((triangle->normalX * x1) + (triangle->normalY * y1) + (triangle->normalZ * z1)) + triangle->distance;
    if (triangle->flags & 0x200) {
        distanceToSurface2 =
            ((triangle->normalX * posX) + (triangle->normalY * posY) + (triangle->normalZ * posZ)) + triangle->distance;
        // sp48 = temp_f4_2;
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 > 0.0f)) {
            if (distanceToSurface < boundingBoxSize) {
                arg->unk32 = 1;
                arg->meshIndexZY = surfaceIndex;
                arg->surfaceDistance[1] = distanceToSurface - boundingBoxSize;
                arg->unk54[0] = triangle->normalX;
                arg->unk54[1] = triangle->normalY;
                arg->unk54[2] = triangle->normalZ;
                return COLLISION;
            }
            return NO_COLLISION;
        }

        if ((distanceToSurface < 0.0f) && (distanceToSurface2 < 0.0f)) {
            distanceToSurface *= -1.0f;
            if (distanceToSurface < boundingBoxSize) {
                arg->unk32 = 1;
                arg->meshIndexZY = surfaceIndex;
                arg->surfaceDistance[1] = distanceToSurface - boundingBoxSize;
                arg->unk54[0] = -triangle->normalX;
                arg->unk54[1] = -triangle->normalY;
                arg->unk54[2] = -triangle->normalZ;
                return 1;
            }
            return NO_COLLISION;
        }
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 < 0.0f)) {
            arg->unk32 = 1;
            arg->meshIndexZY = surfaceIndex;
            arg->surfaceDistance[1] = -(distanceToSurface + boundingBoxSize);
            arg->unk54[0] = -triangle->normalX;
            arg->unk54[1] = -triangle->normalY;
            arg->unk54[2] = -triangle->normalZ;
            return COLLISION;
        }
        if ((distanceToSurface < 0.0f) && (distanceToSurface2 > 0.0f)) {
            arg->unk32 = 1;
            arg->meshIndexZY = surfaceIndex;
            arg->surfaceDistance[1] = distanceToSurface + boundingBoxSize;
            arg->unk54[0] = triangle->normalX;
            arg->unk54[1] = triangle->normalY;
            arg->unk54[2] = triangle->normalZ;
            return COLLISION;
        }
        if (distanceToSurface == 0.0f) {
            if (distanceToSurface2 >= 0.0f) {
                arg->unk32 = 1;
                arg->meshIndexZY = surfaceIndex;
                arg->surfaceDistance[1] = distanceToSurface2 + boundingBoxSize;
                arg->unk54[0] = triangle->normalX;
                arg->unk54[1] = triangle->normalY;
                arg->unk54[2] = triangle->normalZ;
                return COLLISION;
            }
            arg->unk32 = 1;
            arg->meshIndexZY = surfaceIndex;
            arg->surfaceDistance[1] = -(distanceToSurface2 + boundingBoxSize);
            arg->unk54[0] = triangle->normalX;
            arg->unk54[1] = triangle->normalY;
            arg->unk54[2] = triangle->normalZ;
            return COLLISION;
        }
        return NO_COLLISION;
    }
    if (distanceToSurface > boundingBoxSize) {
        if (arg->surfaceDistance[1] > distanceToSurface) {
            arg->unk32 = 1;
            arg->meshIndexZY = surfaceIndex;
            arg->surfaceDistance[1] = distanceToSurface - boundingBoxSize;
            arg->unk54[0] = triangle->normalX;
            arg->unk54[1] = triangle->normalY;
            arg->unk54[2] = triangle->normalZ;
        }
        return NO_COLLISION;
    }

    distanceToSurface2 =
        (triangle->normalX * posX) + (triangle->normalY * posY) + (triangle->normalZ * posZ) + triangle->distance;
    if (distanceToSurface2 < 0.0f) {
        return NO_COLLISION;
    }
    arg->unk32 = 1;
    arg->meshIndexZY = surfaceIndex;
    arg->surfaceDistance[1] = distanceToSurface - boundingBoxSize;
    arg->unk54[0] = triangle->normalX;
    arg->unk54[1] = triangle->normalY;
    arg->unk54[2] = triangle->normalZ;
    return COLLISION;
}

u16 actor_terrain_collision(Collision* collision, f32 boundingBoxSize, f32 newX, f32 newY, f32 newZ, f32 oldX, f32 oldY,
                            f32 oldZ) {
    s32 courseLengthX;
    s32 courseLengthZ;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    u16 numTriangles;
    u16 collisionIndex;
    s16 gridIndex;

    u16 sectionIndex;

    u16 flags = 0;
    s32 sectionX;
    s32 sectionZ;
    u16 i;

    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    collision->surfaceDistance[0] = 1000.0f;
    collision->surfaceDistance[1] = 1000.0f;
    collision->surfaceDistance[2] = 1000.0f;

    // Check if an intersection test has already been performed.
    if ((s32) collision->meshIndexZX < (s32) gCollisionMeshCount) {
        if (is_colliding_with_drivable_surface(collision, boundingBoxSize, newX, newY, newZ, collision->meshIndexZX,
                                               oldX, oldY, oldZ) == COLLISION) {
            flags |= FACING_Y_AXIS;
        }
    }

    if ((s32) collision->meshIndexYX < (s32) gCollisionMeshCount) {
        if (is_colliding_with_wall2(collision, boundingBoxSize, newX, newY, newZ, collision->meshIndexYX, oldX, oldY,
                                    oldZ) == COLLISION) {
            flags |= FACING_Z_AXIS;
        }
    }

    if ((s32) collision->meshIndexZY < (s32) gCollisionMeshCount) {
        if (is_colliding_with_wall1(collision, boundingBoxSize, newX, newY, newZ, collision->meshIndexZY, oldX, oldY,
                                    oldZ) == COLLISION) {
            flags |= FACING_X_AXIS;
        }
    }

    if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
        return flags;
    }

    courseLengthX = (s32) gCourseMaxX - gCourseMinX;
    courseLengthZ = (s32) gCourseMaxZ - gCourseMinZ;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (newX - gCourseMinX) / sectionX;
    sectionIndexZ = (newZ - gCourseMinZ) / sectionZ;
    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = (sectionIndexX + sectionIndexZ * GRID_SIZE);
    numTriangles = gCollisionGrid[gridIndex].numTriangles;

    if (numTriangles == 0) {
        return flags;
    }

    sectionIndex = gCollisionGrid[gridIndex].triangle;

    for (i = 0; i < numTriangles; i++) {
        if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
            return flags;
        }

        collisionIndex = gCollisionIndices[sectionIndex];

        if ((gCollisionMesh[collisionIndex].flags & FACING_Y_AXIS)) {
            if ((flags & FACING_Y_AXIS) == 0) {
                if (collisionIndex != collision->meshIndexZX) {
                    if (is_colliding_with_drivable_surface(collision, boundingBoxSize, newX, newY, newZ, collisionIndex,
                                                           oldX, oldY, oldZ) == COLLISION) {
                        flags |= FACING_Y_AXIS;
                    }
                }
            }
        } else if ((gCollisionMesh[collisionIndex].flags & FACING_X_AXIS) != 0) {
            if ((flags & FACING_X_AXIS) == 0) {
                if (collisionIndex != collision->meshIndexZY) {
                    if (is_colliding_with_wall1(collision, boundingBoxSize, newX, newY, newZ, collisionIndex, oldX,
                                                oldY, oldZ) == COLLISION) {
                        flags |= FACING_X_AXIS;
                    }
                }
            }
        } else if ((flags & FACING_Z_AXIS) == 0) {
            if (collisionIndex != collision->meshIndexYX) {
                if (is_colliding_with_wall2(collision, boundingBoxSize, newX, newY, newZ, collisionIndex, oldX, oldY,
                                            oldZ) == COLLISION) {
                    flags |= FACING_Z_AXIS;
                }
            }
        }
        sectionIndex++;
    }
    return flags;
}

u16 check_bounding_collision(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ) {
    u16 numTriangles;
    s32 courseLengthX;
    s32 courseLengthZ;
    u16 meshIndex;
    s32 sectionX;
    s32 sectionZ;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridIndex;
    u16 i;

    u16 sectionIndex;
    u16 flags;

    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    collision->surfaceDistance[0] = 1000.0f;
    collision->surfaceDistance[1] = 1000.0f;
    collision->surfaceDistance[2] = 1000.0f;
    flags = 0;
    if (collision->meshIndexZX < gCollisionMeshCount) {
        if (check_collision_zx(collision, boundingBoxSize, posX, posY, posZ, collision->meshIndexZX) == 1) {
            flags |= FACING_Y_AXIS;
        }
    }
    if (collision->meshIndexYX < gCollisionMeshCount) {
        if (check_collision_yx(collision, boundingBoxSize, posX, posY, posZ, collision->meshIndexYX) == 1) {
            flags |= FACING_Z_AXIS;
        }
    }
    if (collision->meshIndexZY < gCollisionMeshCount) {
        if (check_collision_zy(collision, boundingBoxSize, posX, posY, posZ, collision->meshIndexZY) == 1) {
            flags |= FACING_X_AXIS;
        }
    }
    if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
        return flags;
    }

    courseLengthX = (s32) gCourseMaxX - gCourseMinX;
    courseLengthZ = (s32) gCourseMaxZ - gCourseMinZ;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (posX - gCourseMinX) / sectionX;
    sectionIndexZ = (posZ - gCourseMinZ) / sectionZ;

    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = sectionIndexX + sectionIndexZ * GRID_SIZE;
    numTriangles = gCollisionGrid[gridIndex].numTriangles;
    if (numTriangles == 0) {
        return flags;
    }
    sectionIndex = gCollisionGrid[gridIndex].triangle;

    for (i = 0; i < numTriangles; i++) {
        if (flags == (FACING_X_AXIS | FACING_Y_AXIS | FACING_Z_AXIS)) {
            return flags;
        }
        meshIndex = gCollisionIndices[sectionIndex];
        if (gCollisionMesh[meshIndex].flags & FACING_Y_AXIS) {
            if (!(flags & FACING_Y_AXIS)) {
                if (meshIndex != collision->meshIndexZX) {
                    if (check_collision_zx(collision, boundingBoxSize, posX, posY, posZ, meshIndex) == 1) {
                        flags |= FACING_Y_AXIS;
                    }
                }
            }
        } else if (gCollisionMesh[meshIndex].flags & FACING_X_AXIS) {
            if (!(flags & FACING_X_AXIS)) {
                if (meshIndex != collision->meshIndexZY) {
                    if (check_collision_zy(collision, boundingBoxSize, posX, posY, posZ, meshIndex) == 1) {
                        flags |= FACING_X_AXIS;
                    }
                }
            }
        } else {
            if (!(flags & FACING_Z_AXIS)) {
                if (meshIndex != collision->meshIndexYX) {
                    if (check_collision_yx(collision, boundingBoxSize, posX, posY, posZ, meshIndex) == 1) {
                        flags |= FACING_Z_AXIS;
                    }
                }
            }
        }
        sectionIndex++;
    }
    return flags;
}

extern u8 D_8014F1110;

/**
 * If unable to spawn actor on the surface set to -3000.0f or
 * if outside the collision grid, spawn in the air (3000.0f).
 */
f32 spawn_actor_on_surface(f32 posX, f32 posY, f32 posZ) {
    f32 height;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridSection;

    u16 index;
    u16 numTriangles;
    u16 sectionIndex;
    f32 phi_f20 = -3000.0f;
    u16 i;

    s32 courseLengthX;
    s32 courseLengthZ;
    s32 sectionX;
    s32 sectionZ;

    courseLengthX = (gCourseMaxX - gCourseMinX);
    courseLengthZ = (gCourseMaxZ - gCourseMinZ);
    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (s16) ((posX - gCourseMinX) / sectionX);
    sectionIndexZ = (s16) ((posZ - gCourseMinZ) / sectionZ);
    gridSection = sectionIndexX + (sectionIndexZ * GRID_SIZE);
    numTriangles = gCollisionGrid[gridSection].numTriangles;

    if (sectionIndexX < 0) {
        printf("collision.c: actor outside of -sectionX %d\n", sectionIndexX);
        return 3000.0f;
    }
    if (sectionIndexZ < 0) {
        printf("collision.c: actor outside of -sectionZ %d\n", sectionIndexZ);
        return 3000.0f;
    }
    if (sectionIndexX >= GRID_SIZE) {
        printf("collision.c: actor outside of sectionX %d\n", sectionIndexX);
        return 3000.0f;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        printf("collision.c: actor outside of sectionZ %d\n", sectionIndexZ);
        return 3000.0f;
    }
    if (numTriangles == 0) {
        printf("collision.c: No collision triangles in track!\n  Something is wrong with the tracks geometry\n");
        return 3000.0f;
    }

    sectionIndex = gCollisionGrid[gridSection].triangle;

    for (i = 0; i < numTriangles; i++) {

        index = gCollisionIndices[sectionIndex];

        if ((gCollisionMesh[index].flags & FACING_Y_AXIS) &&
            (check_horizontally_colliding_with_triangle(posX, posZ, index) == 1)) {
            height = calculate_surface_height(posX, posY, posZ, index);
            if ((height <= posY) && (phi_f20 < height)) {
                phi_f20 = height;
            }
        }
        sectionIndex++;
    }
    return phi_f20;
}

#define MAX3(a, b, c, out) \
    if (a >= b) {          \
        if (a >= c) {      \
            out = a;       \
        } else             \
            out = c;       \
                           \
    } else if (b >= c) {   \
        out = b;           \
    } else                 \
        out = c;

#define MIN3(a, b, c, out) \
    if (a <= b) {          \
        if (a <= c) {      \
            out = a;       \
        } else             \
            out = c;       \
                           \
    } else if (b <= c) {   \
        out = b;           \
    } else                 \
        out = c;

void add_collision_triangle(Vtx* vtx1, Vtx* vtx2, Vtx* vtx3, s8 surfaceType, u16 sectionId) {
    CollisionTriangle* triangle = &gCollisionMesh[gCollisionMeshCount];
    s16 x2;
    s16 z2;
    u16 vtx1Flag;
    s16 x3;
    s16 x1;
    s16 y1;
    s16 z1;
    u16 vtx2Flag;
    s16 y2;
    u16 vtx3Flag;
    u16 flags;
    s16 y3;
    s16 z3;

    // printf("triangle index: %d ", gCollisionMeshCount);
    // printf("sectionId: 0x%X ", sectionId);
    // printf("surfaceType: 0x%X ", surfaceType);

    /* Unused variables placed around doubles for dramatic effect */
    UNUSED s32 pad2[7];

    f64 crossProductX;
    f64 crossProductY;
    f64 crossProductZ;
    f64 magnitude;

    UNUSED s32 pad3[3];

    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 distance;

    s16 maxX;
    s16 maxZ;
    s16 minY;
    s16 minX;
    s16 maxY;
    s16 minZ;

    add_triangle_to_collision_mesh(vtx1, vtx2, vtx3, &vtx1, &vtx2, &vtx3);

    triangle->vtx1 = vtx1;
    triangle->vtx2 = vtx2;
    triangle->vtx3 = vtx3;
    if ((triangle->vtx1->v.flag == 4) && (triangle->vtx2->v.flag == 4) && (triangle->vtx3->v.flag == 4)) {

        return;
    }

    x1 = triangle->vtx1->v.ob[0];
    y1 = triangle->vtx1->v.ob[1];
    z1 = triangle->vtx1->v.ob[2];
    x2 = triangle->vtx2->v.ob[0];
    y2 = triangle->vtx2->v.ob[1];
    z2 = triangle->vtx2->v.ob[2];
    x3 = triangle->vtx3->v.ob[0];
    y3 = triangle->vtx3->v.ob[1];
    z3 = triangle->vtx3->v.ob[2];
    if ((x1 == x2) && (z1 == z2)) {
        triangle->vtx1 = vtx1;
        triangle->vtx3 = vtx2;
        triangle->vtx2 = vtx3;
        x1 = triangle->vtx1->v.ob[0];
        y1 = triangle->vtx1->v.ob[1];
        z1 = triangle->vtx1->v.ob[2];
        x2 = triangle->vtx3->v.ob[0];
        y2 = triangle->vtx3->v.ob[1];
        z2 = triangle->vtx3->v.ob[2];
        x3 = triangle->vtx2->v.ob[0];
        y3 = triangle->vtx2->v.ob[1];
        z3 = triangle->vtx2->v.ob[2];
    }
    MAX3(x1, x2, x3, maxX)

    MAX3(z1, z2, z3, maxZ)

    MAX3(y1, y2, y3, maxY)

    MIN3(x1, x2, x3, minX)

    MIN3(y1, y2, y3, minY)

    MIN3(z1, z2, z3, minZ)

    crossProductX = (((y2 - y1) * (z3 - z2)) - ((z2 - z1) * (y3 - y2)));
    crossProductY = (((z2 - z1) * (x3 - x2)) - ((x2 - x1) * (z3 - z2)));
    crossProductZ = (((x2 - x1) * (y3 - y2)) - ((y2 - y1) * (x3 - x2)));

    // length of the cross product
    magnitude =
        sqrtf((crossProductX * crossProductX) + (crossProductY * crossProductY) + (crossProductZ * crossProductZ));

    if (!magnitude) {
        return;
    }

    normalX = (f32) crossProductX / magnitude;
    normalY = (f32) crossProductY / magnitude;
    normalZ = (f32) crossProductZ / magnitude;

    // Distance from x to plane (cross product's normal).
    distance = -((normalX * x1) + (normalY * y1) + (normalZ * z1));

    // Return if normalY is not vertical.
    // Could be checking if the surface is a floor
    if (D_8015F59C) {
        if (normalY < -0.9f) {
            return;
        } else if (normalY > 0.9f) {
            return;
        }
    }

    // Return if normalY is not horizontal
    // Could be checking if the surface is a wall
    if (D_8015F5A0) {
        if ((normalY < 0.1f) && (normalY > -0.1f)) {
            return;
        }
    }

    triangle->maxX = maxX;
    triangle->maxZ = maxZ;
    triangle->minX = minX;
    triangle->minZ = minZ;
    triangle->minY = minY;
    triangle->maxY = maxY;

    // Define the minimum and maximum dimensions of the course.
    if (minX < gCourseMinX) {
        gCourseMinX = minX;
    }
    if (minY < gCourseMinY) {
        gCourseMinY = minY;
    }
    if (minZ < gCourseMinZ) {
        gCourseMinZ = minZ;
    }
    if (maxX > gCourseMaxX) {
        gCourseMaxX = maxX;
    }
    if (maxY > gCourseMaxY) {
        gCourseMaxY = maxY;
    }
    if (maxZ > gCourseMaxZ) {
        gCourseMaxZ = maxZ;
    }

    triangle->normalX = normalX;
    triangle->normalY = normalY;
    triangle->normalZ = normalZ;
    triangle->distance = distance;

    triangle->surfaceType = (u16) surfaceType;

    // Square the crossProduct to produce a magnitude
    crossProductX = crossProductX * crossProductX;
    crossProductY = crossProductY * crossProductY;
    crossProductZ = crossProductZ * crossProductZ;

    D_8015F6FA = 0;
    D_8015F6FC = 0;

    vtx1Flag = triangle->vtx1->v.flag;
    vtx2Flag = triangle->vtx2->v.flag;
    vtx3Flag = triangle->vtx3->v.flag;

    flags = sectionId;

    if ((vtx1Flag == 1) && (vtx2Flag == 1) && (vtx3Flag == 1)) {
        flags |= 0x400;
    } else if ((vtx1Flag == 2) && (vtx2Flag == 2) && (vtx3Flag == 2)) {
        flags |= 0x800;
    } else if ((vtx1Flag == 3) && (vtx2Flag == 3) && (vtx3Flag == 3)) {
        flags |= 0x1000;
    } else if (D_8015F5A4 != 0) {
        flags |= 0x200;
    }

    triangle->flags = flags;

    // Find the axis with the highest magnitude.

    // Y is the significant axis
    if ((crossProductX <= crossProductY) && (crossProductY >= crossProductZ)) {
        triangle->flags |= FACING_Y_AXIS;
        // X is the significant axis
    } else if ((crossProductX > crossProductY) && (crossProductX >= crossProductZ)) {
        triangle->flags |= FACING_X_AXIS;
        // Z is the significant axis
    } else {
        triangle->flags |= FACING_Z_AXIS;
    }
    gCollisionMeshCount++;
}

/**
 * Triangle contains three indices that are used to get the actual vertices.
 */
void set_vtx_from_triangle(u32 triangle, s8 surfaceType, u16 sectionId) {
    u32 vert1 = ((triangle & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((triangle & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (triangle  & 0x000000FF) / 2;

    Vtx* vtx1 = vtxBuffer[vert1];
    Vtx* vtx2 = vtxBuffer[vert2];
    Vtx* vtx3 = vtxBuffer[vert3];

    add_collision_triangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
}

void set_vtx_from_tri2(u32 triangle1, u32 triangle2, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[2];
    u32 vert1 = ((triangle1 & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((triangle1 & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (triangle1 & 0x000000FF) / 2;

    // This is actually triangle 2; vert 1,2,3.
    u32 vert4 = ((triangle2 & 0x00FF0000) >> 16) / 2;
    u32 vert5 = ((triangle2 & 0x0000FF00) >> 8) / 2;
    u32 vert6 = (triangle2 & 0x000000FF) / 2;

    Vtx* vtx1 = vtxBuffer[vert1];
    Vtx* vtx2 = vtxBuffer[vert2];
    Vtx* vtx3 = vtxBuffer[vert3];

    Vtx* vtx4 = vtxBuffer[vert4];
    Vtx* vtx5 = vtxBuffer[vert5];
    Vtx* vtx6 = vtxBuffer[vert6];

    // Triangle 1
    add_collision_triangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    add_collision_triangle(vtx4, vtx5, vtx6, surfaceType, sectionId);
}

void set_vtx_from_quadrangle(u32 line, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[6];
    Vtx* vtx1;
    Vtx* vtx2;
    Vtx* vtx3;
    Vtx* vtx4;

    u32 vert1 = ((line & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((line & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (line & 0x000000FF) / 2;
    u32 vert4 = ((line & 0xFF000000) >> 24) / 2;

    vtx1 = vtxBuffer[vert1];
    vtx2 = vtxBuffer[vert2];
    vtx3 = vtxBuffer[vert3];
    vtx4 = vtxBuffer[vert4];

    // Triangle 1
    add_collision_triangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    add_collision_triangle(vtx1, vtx3, vtx4, surfaceType, sectionId);
}

/**
 * Generates a list of pointers to course vtx.
 */
void set_vtx_buffer(uintptr_t addr, u32 numVertices, u32 bufferIndex) {
    u32 i;
    Vtx* vtx = (Vtx*) addr;
    for (i = 0; i < numVertices; i++) {
        vtxBuffer[bufferIndex] = vtx;
        vtx++;
        bufferIndex++;
    }
}
/**
 * @return 1 intersecting triangle, 0 not intersecting.
 */
s32 is_line_intersecting_rectangle(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 x1, s16 z1, s16 x2, s16 z2) {
    f32 xOffset;
    f32 projectedPoint;
    f32 zOffset;

    xOffset = x2 - x1;
    zOffset = z2 - z1;
    // Line is vertical
    if (xOffset == 0.0f) {
        if (x1 < minX) {
            return 0;
        }
        if (maxX < x1) {
            return 0;
        }
        if (zOffset > 0.0f) {
            if ((z1 < minZ) && (maxZ < z2)) {
                return 1;
            }
        } else if ((z2 < minZ) && (maxZ < z1)) {
            return 1;
        }
    } else {
        // Line is horizontal
        if (zOffset == 0.0f) {
            if (z1 < minZ) {
                return 0;
            }
            if (maxZ < z1) {
                return 0;
            }
            if (xOffset > 0.0f) {
                if ((x1 < minX) && (maxX < x2)) {
                    return 1;
                }
            } else if ((x2 < minX) && (maxX < x1)) {
                return 1;
            }
        } else {
            projectedPoint = ((xOffset / zOffset) * (minZ - z1)) + x1;
            if ((minX <= projectedPoint) && (projectedPoint <= maxX)) {
                return 1;
            }
            projectedPoint = ((xOffset / zOffset) * (maxZ - z1)) + x1;
            if ((minX <= projectedPoint) && (projectedPoint <= maxX)) {
                return 1;
            }
            projectedPoint = ((zOffset / xOffset) * (minX - x1)) + z1;
            if ((minZ <= projectedPoint) && (projectedPoint <= maxZ)) {
                return 1;
            }
            projectedPoint = ((zOffset / xOffset) * (maxX - x1)) + z1;
            if ((minZ <= projectedPoint) && (projectedPoint <= maxZ)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 is_triangle_intersecting_bounding_box(s16 minX, s16 maxX, s16 minZ, s16 maxZ, u16 index) {
    CollisionTriangle* triangle = &gCollisionMesh[index];
    s16 x1;
    s16 z1;
    s16 x2;
    s16 z2;
    s16 x3;
    s16 z3;

    x1 = triangle->vtx1->v.ob[0];
    z1 = triangle->vtx1->v.ob[2];
    x2 = triangle->vtx2->v.ob[0];
    z2 = triangle->vtx2->v.ob[2];
    x3 = triangle->vtx3->v.ob[0];
    z3 = triangle->vtx3->v.ob[2];
    if ((x1 >= minX) && (maxX >= x1) && (z1 >= minZ) && (maxZ >= z1)) {
        return 1;
    }
    if ((x2 >= minX) && (maxX >= x2) && (z2 >= minZ) && (maxZ >= z2)) {
        return 1;
    }
    if ((x3 >= minX) && (maxX >= x3) && (z3 >= minZ) && (maxZ >= z3)) {
        return 1;
    }
    if (is_line_intersecting_rectangle(minX, maxX, minZ, maxZ, x1, z1, x2, z2) == 1) {
        return 1;
    }
    if (is_line_intersecting_rectangle(minX, maxX, minZ, maxZ, x2, z2, x3, z3) == 1) {
        return 1;
    }
    if (is_line_intersecting_rectangle(minX, maxX, minZ, maxZ, x3, z3, x1, z1) == 1) {
        return 1;
    }
    return 0;
}

/**
 * Splits the collision mesh into 32x32 sections. This allows the game to check only
 * nearby geography for a collision rather than checking against the whole collision mesh.
 * (checking against the whole mesh for every actor would be expensive)
 */
void generate_collision_grid(void) {
    CollisionTriangle* triangle;
    s32 i, j, k;
    UNUSED s32 pad[5];
    s16 maxX;
    s16 maxZ;
    s16 minX;
    s16 minZ;
    s32 sectionZ;
    s32 sectionX;
    s32 courseLengthX;
    s32 courseLengthZ;
    s32 index;

    courseLengthX = (s32) gCourseMaxX - gCourseMinX;
    courseLengthZ = (s32) gCourseMaxZ - gCourseMinZ;

    // Separate the course into 32 sections
    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    // Reset the collision grid
    for (i = 0; i < 1024; i++) {
        gCollisionGrid[i].numTriangles = 0;
    }

    gNumCollisionTriangles = 0;

    // 32x32 grid
    for (j = 0; j < GRID_SIZE; j++) {
        for (k = 0; k < GRID_SIZE; k++) {
            index = k + j * GRID_SIZE;

            // Select a section of the course using min/max akin to drawing a bounding-box
            minX = (gCourseMinX + (sectionX * k)) - 20;
            minZ = (gCourseMinZ + (sectionZ * j)) - 20;

            maxX = minX + sectionX + 40;
            maxZ = minZ + sectionZ + 40;

            for (i = 0; i < gCollisionMeshCount; i++) {
                triangle = gCollisionMesh + i;
                if (triangle->maxZ < minZ) {
                    continue;
                }
                if (triangle->minZ > maxZ) {
                    continue;
                }
                if (triangle->maxX < minX) {
                    continue;
                }
                if (triangle->minX > maxX) {
                    continue;
                }

                // Add the collision triangle to the list if it's inside the bounding-box
                if (is_triangle_intersecting_bounding_box(minX, maxX, minZ, maxZ, (u16) i) == 1) {
                    // Point this grid section to the first triangle in the section
                    if (gCollisionGrid[index].numTriangles == 0) {
                        gCollisionGrid[index].triangle = gNumCollisionTriangles;
                    }
                    gCollisionGrid[index].numTriangles++;
                    gCollisionIndices[gNumCollisionTriangles] = (u16) i;
                    gNumCollisionTriangles++;
                }
            }
        }
    }
}

/**
 * Recursive search for vtx and set surfaceTypes to -1 and sectionId's to 0xFF
 */
void generate_collision_mesh_with_defaults(Gfx* gfx) {
    generate_collision_mesh(gfx, SURFACE_DEFAULT, 0xFF);
}

/**
 * Recursive search for vtx and set sectionId's to 0xFF
 */
void generate_collision_mesh_with_default_section_id(Gfx* gfx, s8 surfaceType) {
    generate_collision_mesh(gfx, surfaceType, 0xFF);
}

extern u32 D_8015F58C;
u32 numTimes = 0;

/**
 * Generate via a recursive search and set for vertex data.
 */
bool is_cull_box(const char* filePath);
void generate_collision_mesh(Gfx* addr, s8 surfaceType, u16 sectionId) {
    if (GameEngine_OTRSigCheck((char*)addr)) {
        addr = LOAD_ASSET(addr);
    }
    int8_t opcode;
    uintptr_t lo;
    uintptr_t hi;
    s32 i;
    numTimes++;
    // printf("Initial\n");
    // printf("ptr 0x%llX\n", &addr);
    // printf("w0 0x%llX\n", addr->words.w0);
    // printf("w1 0x%llX\n", addr->words.w1);
    // printf("----loop----\n");
    Gfx* gfx = (Gfx*) addr;
    D_8015F6FA = 0;
    D_8015F6FC = 0;

    // printf("\n\nORIG:\n");
    // for (size_t i = 0; i < 100; i++) {
    //     printf(" 0x%X ", orig[i]);
    // }

    for (i = 0; i < 0x1FFF; i++) {
        lo = gfx->words.w0;
        hi = gfx->words.w1;
        opcode = GFX_GET_OPCODE(lo) >> 24;

        //  printf("ptr 0x%llX\n", &addr);
        //  printf("op 0x%llX\n", opcode);
        //   printf("w0 0x%llX\n", lo);
        //   printf("w1 0x%llX\n", hi);

        switch(opcode) {
            case G_DL:
                // G_DL's hi contains an addr to another DL.
                generate_collision_mesh((Gfx*) hi, surfaceType, sectionId);
                break;
            case G_DL_OTR_HASH:
                gfx++;
                uint64_t hash = gfx->words.w0 << 32 | gfx->words.w1;
                // printf("name of dl hash: 0x%llX\n", hash);
                // printf("name of dl: %s\n", ResourceGetNameByCrc(hash));
                generate_collision_mesh(ResourceGetDataByCrc(hash), surfaceType, sectionId);
                break;
            case G_DL_OTR_FILEPATH:
                generate_collision_mesh(ResourceGetDataByName((const char*)hi), surfaceType, sectionId);
                break;
            case G_VTX:{
                uintptr_t ptr = hi;
                set_vtx_buffer((uintptr_t) ptr, (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1);
                break;
            }
            case G_VTX_OTR_FILEPATH: {
                const char* filePath = (const char*)hi;
                // Fast64 outputs garbage data. Lets skip that...
                if (is_cull_box(filePath)) {
                    gfx++;
                    continue;
                }
                gfx++;
                size_t count = gfx->words.w0;
                size_t index = (gfx->words.w1 >> 16);
                size_t vtxDataOff = gfx->words.w1 & 0xFFFF;
                Vtx* vtx = ( (Vtx*)ResourceGetDataByName(filePath) ) + vtxDataOff;

                set_vtx_buffer(vtx, count, index);
                break;
            }
            case G_VTX_OTR_HASH:
                gfx++;
                hash = gfx->words.w0 << 32 | gfx->words.w1;
                // printf("name of vtx hash: 0x%lX\n", hash);
                // printf("name of vtx: %s\n", ResourceGetNameByCrc(hash));
                int numVerts = (lo >> 12) & ((1<<8)-1);
                int bufferIndex = ((lo >> 1) & ((1<<7)-1));
                bufferIndex = numVerts - bufferIndex; 
                // printf("numVerts: %d\n", numVerts);
                // printf("bufferIndex: %d\n", bufferIndex);
                set_vtx_buffer((uintptr_t) ResourceGetDataByCrc(hash), numVerts, bufferIndex);
                break;
            case G_TRI1:
                D_8015F58C += 1;
                set_vtx_from_triangle(hi, surfaceType, sectionId);
                break;
            case G_TRI1_OTR: {
                uint32_t v1 = (lo & 0x0000FFFF) * 2;
                uint32_t v2 = (hi >> 16) * 2;
                uint32_t v3 = (hi & 0x0000FFFF) * 2;

                // Repack into a format supported by f3dex
                uint32_t repack = (v1 << 16) | (v2 << 8) | v3;

                D_8015F58C += 1;
                set_vtx_from_triangle(repack, surfaceType, sectionId);
                break;
            }
            case G_TRI2:
                D_8015F58C += 2;
                set_vtx_from_tri2(lo, hi, surfaceType, sectionId);
                break;
            case G_QUAD:
                D_8015F58C += 2;
                set_vtx_from_quadrangle(hi, surfaceType, sectionId);
                break;
            case G_ENDDL:
                return; // end of loop
            case G_MARKER:
            case G_MTX_OTR:
            case G_SETTIMG_OTR_HASH:
                gfx++;
                i++;
                break;
        }


        // if (opcode == (G_DL << 24)) {
        //     // G_DL's hi contains an addr to another DL.
        //     generate_collision_mesh((Gfx*) hi, surfaceType, sectionId);

        // } else if (opcode == (G_VTX << 24)) {
        //     set_vtx_buffer((hi), (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1);

        // } else if (opcode == (G_TRI1 << 24)) {
        //     D_8015F58C += 1;
        //     set_vtx_from_triangle(hi, surfaceType, sectionId);

        // } else if (opcode == (G_TRI2 << 24)) {
        //     D_8015F58C += 2;

        //     set_vtx_from_tri2(lo, hi, surfaceType, sectionId);

        // } else if (opcode == (G_QUAD << 24)) {
        //     D_8015F58C += 2;
        //     set_vtx_from_quadrangle(hi, surfaceType, sectionId);

        // } else if (opcode == (int32_t) (G_ENDDL << 24)) {
        //     break;
        // }

        gfx++;
    }
}

bool is_cull_box(const char* filePath) {
    const char* suffix = "cull";
    size_t fileLen = strlen(filePath);
    size_t suffixLen = strlen(suffix);

    if (fileLen < suffixLen) {
        return false;
    }

    return strcmp(filePath + fileLen - suffixLen, suffix) == 0;
}

/**
 * Search for G_SETTILESIZE and set its args.
 */
void find_and_set_tile_size(uintptr_t addr, s32 uls, s32 ult) {
    Gfx* gfx = (Gfx*) addr;
    if (GameEngine_OTRSigCheck(gfx)) {
        gfx = (Gfx*) ResourceGetDataByName(gfx);
    }
    u32 opcode;

    uls = (uls << 12) & 0xFFF000;
    ult &= 0xFFF;

    while (true) {

        opcode = GFX_GET_OPCODE(gfx->words.w0);

        if (opcode == (u32) G_ENDDL << 24) {
            break;
        } else if (opcode == (u32) (G_SETTILESIZE << 24)) {
            gfx->words.w0 = (G_SETTILESIZE << 24) | uls | ult;

            break;
        }
        gfx++;
    }
}

void set_vertex_colours(uintptr_t addr, u32 vertexCount, UNUSED s32 vert3, s8 alpha, u8 red, u8 green, u8 blue) {
    s32 i;
    Vtx* vtx = (Vtx*) addr;

    for (i = 0; (u32) i < vertexCount; i++) {
        if (red) {
            vtx->v.cn[0] = red;
            vtx->v.cn[1] = green;
            vtx->v.cn[2] = blue;
        }
        vtx->v.cn[3] = alpha;
        vtx++;
    }
}

/**
 * Recursive search for vertices and set their colour values.
 */
void find_vtx_and_set_colours(Gfx* displayList, s8 alpha, u8 red, u8 green, u8 blue) {
    if (GameEngine_OTRSigCheck(displayList)) {
        displayList = (Gfx*) ResourceGetDataByName(displayList);
    }
    Gfx* gfx = displayList;
    uintptr_t lo;
    uintptr_t hi;
    s32 opcode;

    while (true) {
        lo = gfx->words.w0;
        hi = gfx->words.w1;
        opcode = GFX_GET_OPCODE(lo);
        if (opcode == (G_ENDDL << 24)) {
            break;
        } else if (opcode == (G_DL << 24)) {
            find_vtx_and_set_colours((Gfx*) hi, alpha, red, green, blue);
        } else if (opcode == (G_DL_OTR_HASH << 24)) {
            gfx++;
            uint64_t hash = gfx->words.w0 << 32 | gfx->words.w1;
            find_vtx_and_set_colours(ResourceGetDataByCrc(hash), alpha, red, green, blue);
        } else if (opcode == (G_DL_OTR_FILEPATH << 24)) {
            find_vtx_and_set_colours(ResourceGetDataByName((const char*)hi), alpha, red, green, blue);
        } else if (opcode == (G_VTX << 24)) {
            // G_VTX contains an addr hi
            set_vertex_colours(hi, (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1, alpha, red, green, blue);
        } else if (opcode == (G_VTX_OTR_FILEPATH << 24)) {
            const char* filePath = (const char*)hi;
            // Fast64 outputs garbage data. Lets skip that...
            if (is_cull_box(filePath)) {
                gfx++;
                continue;
            }
            gfx++;
            size_t count = gfx->words.w0;
            size_t index = (gfx->words.w1 >> 16);
            size_t vtxDataOff = gfx->words.w1 & 0xFFFF;
            Vtx* vtx = ( (Vtx*)ResourceGetDataByName(filePath) ) + vtxDataOff;

            set_vertex_colours((uintptr_t)vtx, count, index, alpha, red, green, blue);
        } else if (opcode == (G_VTX_OTR_HASH << 24)) {
            gfx++;
            uint64_t hash = gfx->words.w0 << 32 | gfx->words.w1;
            int numVerts = (lo >> 12) & ((1<<8)-1);
            int bufferIndex = ((lo >> 1) & ((1<<7)-1));
            bufferIndex = numVerts - bufferIndex; 
            set_vertex_colours((uintptr_t) ResourceGetDataByCrc(hash), numVerts, bufferIndex, alpha, red, green, blue);
        }
        if (opcode == G_MARKER || opcode == G_MTX_OTR || opcode == G_SETTIMG_OTR_HASH) {
            gfx++;
        }
        gfx++;
    }
}

void subtract_scaled_vector(Vec3f pos1, f32 boundingBoxSize, Vec3f pos2) {
    pos2[0] -= pos1[0] * boundingBoxSize;
    pos2[1] -= pos1[1] * boundingBoxSize;
    pos2[2] -= pos1[2] * boundingBoxSize;
}

u16 player_terrain_collision(Player* player, KartTyre* tyre, f32 tyre2X, f32 tyre2Y, f32 tyre2Z) {
    Collision wtf;
    Collision* collision = &wtf;
    UNUSED s32 pad;
    u16 i;
    u16 meshIndex;
    u16 numTriangles;
    u16 sectionIndex;
    f32 tyreX;
    f32 tyreY;
    f32 tyreZ;
    f32 boundingBoxSize;
    f32 height;

    s32 courseLengthX;
    s32 courseLengthZ;

    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridIndex;

    s32 sectionX;
    s32 sectionZ;
    UNUSED s32 pad2[9];

    collision->surfaceDistance[0] = 1000.0f;
    collision->surfaceDistance[1] = 1000.0f;
    collision->surfaceDistance[2] = 1000.0f;
    boundingBoxSize = player->boundingBoxSize;
    collision->meshIndexYX = 5000;
    collision->meshIndexZY = 5000;
    collision->meshIndexZX = 5000;
    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    tyreX = tyre->pos[0];
    tyreY = tyre->pos[1];
    tyreZ = tyre->pos[2];
    switch (tyre->surfaceFlags) {
        case 0x80:
            if (is_colliding_with_wall1(collision, boundingBoxSize, tyreX, tyreY, tyreZ, tyre->collisionMeshIndex,
                                        tyre2X, tyre2Y, tyre2Z) == 1) {
                height = calculate_surface_height(tyreX, tyreY, tyreZ, tyre->collisionMeshIndex);
                if ((!(height > player->pos[1])) && !((player->pos[1] - height) > (2 * boundingBoxSize))) {
                    tyre->baseHeight = height;
                    subtract_scaled_vector(collision->unk54, collision->surfaceDistance[1], tyre->pos);
                    return 1;
                }
            }
            break;
        case 0x40:
            if (is_colliding_with_drivable_surface(collision, boundingBoxSize, tyreX, tyreY, tyreZ,
                                                   tyre->collisionMeshIndex, tyre2X, tyre2Y, tyre2Z) == 1) {
                height = calculate_surface_height(tyreX, tyreY, tyreZ, tyre->collisionMeshIndex);
                if (!(player->pos[1] < height) && !((2 * boundingBoxSize) < (player->pos[1] - height))) {
                    tyre->baseHeight = height;
                    subtract_scaled_vector(collision->orientationVector, collision->surfaceDistance[2], tyre->pos);
                    return 1;
                }
            }
            break;
        case 0x20:
            if (is_colliding_with_wall2(collision, boundingBoxSize, tyreX, tyreY, tyreZ, tyre->collisionMeshIndex,
                                        tyre2X, tyre2Y, tyre2Z) == 1) {
                height = calculate_surface_height(tyreX, tyreY, tyreZ, tyre->collisionMeshIndex);
                if (!(player->pos[1] < height) && !((2 * boundingBoxSize) < (player->pos[1] - height))) {
                    tyre->baseHeight = height;
                    subtract_scaled_vector(collision->unk48, collision->surfaceDistance[0], tyre->pos);
                    return 1;
                }
            }
            break;
        case 0:
            break;
    }

    // If the surface flags are not set then try setting them.

    courseLengthX = (s32) gCourseMaxX - gCourseMinX;
    courseLengthZ = (s32) gCourseMaxZ - gCourseMinZ;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (tyreX - gCourseMinX) / sectionX;
    sectionIndexZ = (tyreZ - gCourseMinZ) / sectionZ;

    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = sectionIndexX + sectionIndexZ * GRID_SIZE;
    numTriangles = gCollisionGrid[gridIndex].numTriangles;

    if (numTriangles == 0) {
        return 0;
    }

    sectionIndex = gCollisionGrid[gridIndex].triangle;

    for (i = 0; i < numTriangles; i++) {
        meshIndex = gCollisionIndices[sectionIndex];
        if (gCollisionMesh[meshIndex].flags & FACING_Y_AXIS) {
            if (meshIndex != tyre->collisionMeshIndex) {
                if (is_colliding_with_drivable_surface(collision, boundingBoxSize, tyreX, tyreY, tyreZ, meshIndex,
                                                       tyre2X, tyre2Y, tyre2Z) == 1) {
                    height = calculate_surface_height(tyreX, tyreY, tyreZ, meshIndex);

                    if (!(player->pos[1] < height) && !((2 * boundingBoxSize) < (player->pos[1] - height))) {
                        subtract_scaled_vector(collision->orientationVector, collision->surfaceDistance[2], tyre->pos);
                        tyre->baseHeight = height;
                        tyre->surfaceType = (u8) gCollisionMesh[meshIndex].surfaceType;
                        tyre->surfaceFlags = 0x40;
                        tyre->collisionMeshIndex = meshIndex;
                        if (gCollisionMesh[meshIndex].flags & 0x1000) {
                            tyre->unk_14 = 1;
                        } else {
                            tyre->unk_14 = 0;
                        }
                        return 1;
                    }
                }
            }
        } else if (gCollisionMesh[meshIndex].flags & FACING_X_AXIS) {
            if (gCollisionMesh[meshIndex].normalY != 0.0f) {
                if (meshIndex != tyre->collisionMeshIndex) {
                    if (is_colliding_with_wall1(collision, boundingBoxSize, tyreX, tyreY, tyreZ, meshIndex, tyre2X,
                                                tyre2Y, tyre2Z) == 1) {
                        height = calculate_surface_height(tyreX, tyreY, tyreZ, meshIndex);
                        if (!(player->pos[1] < height) && !((2 * boundingBoxSize) < (player->pos[1] - height))) {
                            tyre->baseHeight = height;
                            subtract_scaled_vector(collision->unk54, collision->surfaceDistance[1], tyre->pos);
                            tyre->baseHeight = calculate_surface_height(tyreX, tyreY, tyreZ, meshIndex);
                            tyre->surfaceType = (u8) gCollisionMesh[meshIndex].surfaceType;
                            tyre->surfaceFlags = 0x80;
                            tyre->collisionMeshIndex = meshIndex;
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (gCollisionMesh[meshIndex].normalY != 0.0f) {
                if (meshIndex != tyre->collisionMeshIndex) {
                    if (is_colliding_with_wall2(collision, boundingBoxSize, tyreX, tyreY, tyreZ, meshIndex, tyre2X,
                                                tyre2Y, tyre2Z) == 1) {
                        height = calculate_surface_height(tyreX, tyreY, tyreZ, meshIndex);
                        if (!(player->pos[1] < height) && !((2 * boundingBoxSize) < (player->pos[1] - height))) {
                            tyre->baseHeight = height;
                            subtract_scaled_vector(collision->unk48, collision->surfaceDistance[0], tyre->pos);
                            tyre->surfaceType = (u8) gCollisionMesh[meshIndex].surfaceType;
                            tyre->surfaceFlags = 0x20;
                            tyre->collisionMeshIndex = meshIndex;
                            return 1;
                        }
                    }
                }
            }
        }
        sectionIndex++;
    }
    tyre->baseHeight = tyreY;
    tyre->surfaceType = 0;
    return 0;
}
