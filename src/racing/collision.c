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
#include <assert.h>

// Used to delete the choco mountain guard rail
void nullify_displaylist(uintptr_t addr) {
    Gfx *macro;

    macro = (Gfx *) addr;
    macro->words.w0 = (G_ENDDL << 24);
    macro->words.w1 = 0;
}

void func_802AAAAC(Collision *collision) {
    collision->unk36 = 5000;
    collision->unk38 = 5000;
    collision->unk3A = 5000;
    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    collision->unk3C[0] = 0;
    collision->unk3C[1] = 0;
    collision->unk3C[2] = 0;
    vec3f_set(collision->unk48, 0.0f, 0.0f, 1.0f);
    vec3f_set(collision->unk54, 1.0f, 0.0f, 0.0f);
    vec3f_set(collision->orientationVector, 0.0f, 1.0f, 0.0f);
}

f32 func_802AAB4C(Player *player) {
    f32 playerX;
    f32 playerZ;
    s32 temp_v1;

    playerX = player->pos[0];
    playerZ = player->pos[2];
    switch (gCurrentCourseId) {
        case COURSE_BOWSER_CASTLE:
            if (playerX > 1859.0f) {
                return D_8015F8E4;
            }
            if (playerX < 1549.0f) {
                return D_8015F8E4;
            }
            if (playerZ > -1102.0f) {
                return D_8015F8E4;
            }
            if (playerZ < -1402.0f) {
                return D_8015F8E4;
            }
            return 20.0f;
        case COURSE_KOOPA_BEACH:
            if (playerX > 239.0f) {
                return D_8015F8E4;
            }
            if (playerX < 67.0f) {
                return D_8015F8E4;
            }
            if (playerZ > 2405.0f) {
                return D_8015F8E4;
            }
            if (playerZ < 2233.0f) {
                return D_8015F8E4;
            }
            return 0.8f;
        case COURSE_SHERBET_LAND:
            if ((get_surface_type(player->unk_110.unk3A) & 0xFF) == SNOW) {
                return (f32) (gCourseMinY - 0xA);
            }
            return D_8015F8E4;
        case COURSE_DK_JUNGLE:
            temp_v1 = func_802ABD40(player->unk_110.unk3A) & 0xFF;
            if (temp_v1 == 0xFF) {
                if ((get_surface_type(player->unk_110.unk3A) & 0xFF) == CAVE) {
                    return -475.0f;
                }
                if (playerX > -478.0f) {
                    return -33.9f;
                }
                if (playerX < -838.0f) {
                    return -475.0f;
                }
                if (playerZ > -436.0f) {
                    return -475.0f;
                }
                if (playerZ < -993.0f) {
                    return -33.9f;
                }
                if (playerZ < playerX) {
                    return -475.0f;
                }
                return -33.9f;
            }
            if (temp_v1 >= 0x14) {
                return -475.0f;
            }
            return -33.9f;
        default:
            return D_8015F8E4;
    }
}

s32 func_802AAE4C(Collision *collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    mk64_surface_map_ram *surfaceMap = &gSurfaceMap[index];
    UNUSED f32 pad;
    f32 x3;
    UNUSED f32 pad2;
    f32 z3;
    UNUSED f32 pad3[4];
    f32 x2;
    f32 z2;
    f32 x1;
    f32 z1;
    f32 temp_f0_5;
    f32 temp_f10;
    f32 temp_f0_3;
    f32 temp_f2_2;
    s32 flag = 1;

    if (surfaceMap->gravity < -0.9f)
        return 0;
    if (surfaceMap->vtx31 > posX)
        return 0;
    if (surfaceMap->vtx33 > posZ)
        return 0;
    if (surfaceMap->vtx21 < posX)
        return 0;
    if (surfaceMap->vtx23 < posZ)
        return 0;
    if ((surfaceMap->vtx32 - boundingBoxSize * 3.0f) > posY)
        return 0;

    x1 = surfaceMap->vtxPoly1->v.ob[0];
    z1 = surfaceMap->vtxPoly1->v.ob[2];

    x2 = surfaceMap->vtxPoly2->v.ob[0];
    z2 = surfaceMap->vtxPoly2->v.ob[2];

    x3 = surfaceMap->vtxPoly3->v.ob[0];
    z3 = surfaceMap->vtxPoly3->v.ob[2];

    temp_f10 = (z1 - posZ) * (x2 - posX) - (x1 - posX) * (z2 - posZ);

    if (!temp_f10) {

        temp_f0_3 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        temp_f2_2 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

        if ((temp_f0_3 * temp_f2_2) < 0.0f) {
            flag = 0;
        }
    } else {

        temp_f0_3 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        if (!temp_f0_3) {
            temp_f2_2 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

            if ((temp_f10 * temp_f2_2) < 0.0f) {
                flag = 0;
            }
        } else {
            if ((temp_f10 * temp_f0_3) < 0.0f) {
                flag = 0;
            } else {
                temp_f2_2 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);
                if (temp_f2_2 != 0) {
                    if ((temp_f0_3 * temp_f2_2) < 0.0f) {
                        flag = 0;
                    }
                }
            }
        }
    }
    if (!flag) {
        return 0;
    }
    temp_f0_5 = ((surfaceMap->height * posX) + (surfaceMap->gravity * posY) + (surfaceMap->rotation * posZ)
                 + surfaceMap->height2)
                - boundingBoxSize;
    if (temp_f0_5 > 0.0f) {
        if (collision->unk3C[2] > temp_f0_5) {
            collision->unk34 = 1;
            collision->unk3A = index;
            collision->unk3C[2] = temp_f0_5;
            collision->orientationVector[0] = surfaceMap->height;
            collision->orientationVector[1] = surfaceMap->gravity;
            collision->orientationVector[2] = surfaceMap->rotation;
        }
        return 0;
    }

    if (temp_f0_5 > -16.0f) {
        collision->unk34 = 1;
        collision->unk3A = index;
        collision->unk3C[2] = temp_f0_5;
        collision->orientationVector[0] = surfaceMap->height;
        collision->orientationVector[1] = surfaceMap->gravity;
        collision->orientationVector[2] = surfaceMap->rotation;
        return 1;
    }
    return 0;
}


s32 func_802AB288(Collision *collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    mk64_surface_map_ram *surfaceMap = &gSurfaceMap[index];
    UNUSED f32 pad[6];
    f32 x3;
    f32 y3;
    UNUSED f32 pad2[1];

    UNUSED f32 pad3[5];
    f32 x2;
    f32 y2;
    f32 x1;
    f32 y1;
    f32 temp_f0_4;
    f32 temp_f2_2;
    f32 temp_f0_2;
    f32 temp_f2_3;
    s32 flag = 1;

    if (surfaceMap->vtx31 > posX) {
        return 0;
    }
    if (surfaceMap->vtx21 < posX) {
        return 0;
    }
    if (surfaceMap->vtx22 < posY) {
        return 0;
    }
    if (surfaceMap->vtx32 > posY) {
        return 0;
    }

    if ((surfaceMap->vtx33 - boundingBoxSize * 3.0f) > posZ) {
        return 0;
    }
    if ((surfaceMap->vtx23 + boundingBoxSize * 3.0f) < posZ) {
        return 0;
    }

    x1 = surfaceMap->vtxPoly1->v.ob[0];
    y1 = surfaceMap->vtxPoly1->v.ob[1];

    x2 = surfaceMap->vtxPoly2->v.ob[0];
    y2 = surfaceMap->vtxPoly2->v.ob[1];

    x3 = surfaceMap->vtxPoly3->v.ob[0];
    y3 = surfaceMap->vtxPoly3->v.ob[1];

    temp_f2_2 = (y1 - posY) * (x2 - posX) - (x1 - posX) * (y2 - posY);

    if (!temp_f2_2) {

        temp_f0_2 = (y2 - posY) * (x3 - posX) - (x2 - posX) * (y3 - posY);

        temp_f2_3 = (y3 - posY) * (x1 - posX) - (x3 - posX) * (y1 - posY);

        if ((temp_f0_2 * temp_f2_3) < 0.0f) {
            flag = 0;
        }
    } else {
        temp_f0_2 = (y2 - posY) * (x3 - posX) - (x2 - posX) * (y3 - posY);
        if (!temp_f0_2) {
            temp_f2_3 = (y3 - posY) * (x1 - posX) - (x3 - posX) * (y1 - posY);
            if (temp_f2_2 * temp_f2_3 < 0.0f) {
                flag = 0;
            }
        } else {
            if ((temp_f2_2 * temp_f0_2) < 0.0f) {
                flag = 0;
            } else {
                temp_f2_3 = ((y3 - posY) * (x1 - posX)) - ((x3 - posX) * (y1 - posY));
                if (temp_f2_3 != 0) {
                    if ((temp_f0_2 * temp_f2_3) < 0.0f) {
                        flag = 0;
                    }
                }
            }
        }
    }
    if (!flag) {
        return 0;
    }
    temp_f0_4 = ((surfaceMap->height * posX) + (surfaceMap->gravity * posY) + (surfaceMap->rotation * posZ) + surfaceMap->height2) - boundingBoxSize;
    if (temp_f0_4 > 0.0f) {
        if (temp_f0_4 < collision->unk3C[0]) {
            collision->unk30 = 1;
            collision->unk36 = index;
            collision->unk3C[0] = temp_f0_4;
            collision->unk48[0] = surfaceMap->height;
            collision->unk48[1] = surfaceMap->gravity;
            collision->unk48[2] = surfaceMap->rotation;
        }
        return 0;
    }

    if (temp_f0_4 > -16.0f) {
        collision->unk30 = 1;
        collision->unk36 = index;
        collision->unk3C[0] = temp_f0_4;
        collision->unk48[0] = surfaceMap->height;
        collision->unk48[1] = surfaceMap->gravity;
        collision->unk48[2] = surfaceMap->rotation;
        return 1;
    }
    return 0;
}


s32 func_802AB6C4(Collision *collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    mk64_surface_map_ram *surfaceMap = &gSurfaceMap[index];
    bool flag = 1;
    UNUSED f32 pad[7];
    f32 sp20;
    f32 temp_f8;
    UNUSED f32 pad2[5];
    f32 sp6C;
    f32 sp70;
    f32 temp_f4;
    f32 sp24;
    f32 temp_f0_4;
    f32 temp_f2_2;
    f32 temp_f0_2;
    f32 temp_f2_3;
    if (surfaceMap->vtx33 > posZ) {
        return 0;
    }
    if (surfaceMap->vtx23 < posZ) {
        return 0;
    }
    if (surfaceMap->vtx22 < posY) {
        return 0;
    }
    if (surfaceMap->vtx32 > posY) {
        return 0;
    }
    if ((surfaceMap->vtx31 - (boundingBoxSize * 3.0f)) > posX) {
        return 0;
    }
    if ((surfaceMap->vtx21 + (boundingBoxSize * 3.0f)) < posX) {
        return 0;
    }

    sp24 = surfaceMap->vtxPoly1->v.ob[2];
    temp_f4 = surfaceMap->vtxPoly1->v.ob[1];

    sp70 = surfaceMap->vtxPoly2->v.ob[2];
    sp6C = surfaceMap->vtxPoly2->v.ob[1];

    temp_f8 = surfaceMap->vtxPoly3->v.ob[2];
    sp20 = surfaceMap->vtxPoly3->v.ob[1];

    temp_f2_2 = (temp_f4 - posY) * (sp70 - posZ) - (sp24 - posZ) * (sp6C - posY);

    if (!temp_f2_2) {

        temp_f0_2 = ((sp6C - posY) * (temp_f8 - posZ)) - ((sp70 - posZ) * (sp20 - posY));

        temp_f2_3 = ((sp20 - posY) * (sp24 - posZ)) - ((temp_f8 - posZ) * (temp_f4 - posY));

        if ((temp_f0_2 * temp_f2_3) < 0.0f) {
            flag = 0;
        }
    } else {

        temp_f0_2 = ((sp6C - posY) * (temp_f8 - posZ)) - ((sp70 - posZ) * (sp20 - posY));

        if (temp_f0_2 == 0) {
            temp_f2_3 = ((sp20 - posY) * (sp24 - posZ)) - ((temp_f8 - posZ) * (temp_f4 - posY));

            if ((temp_f2_2 * temp_f2_3) < 0.0f) {
                flag = 0;
            }
        } else {

            if ((temp_f2_2 * temp_f0_2) < 0.0f) {
                flag = 0;
            } else {
                temp_f2_3 = ((sp20 - posY) * (sp24 - posZ)) - ((temp_f8 - posZ) * (temp_f4 - posY));
                if (temp_f2_3 != 0) {
                    if ((temp_f0_2 * temp_f2_3) < 0.0f) {
                        flag = 0;
                    }
                }
            }
        }
    }
    if (!flag) {
        return 0;
    }

    temp_f0_4 = ((((surfaceMap->height * posX) + (surfaceMap->gravity * posY)) + (surfaceMap->rotation * posZ))
                 + surfaceMap->height2)
                - boundingBoxSize;

    if (temp_f0_4 > 0.0f) {
        if (temp_f0_4 < collision->unk3C[1]) {
            collision->unk32 = 1;
            collision->unk38 = index;
            collision->unk3C[1] = temp_f0_4;
            collision->unk54[0] = surfaceMap->height;
            collision->unk54[1] = surfaceMap->gravity;
            collision->unk54[2] = surfaceMap->rotation;
        }
        return 0;
    }
    if (temp_f0_4 > (-16.0f)) {
        collision->unk32 = 1;
        collision->unk38 = index;
        collision->unk3C[1] = temp_f0_4;
        collision->unk54[0] = surfaceMap->height;
        collision->unk54[1] = surfaceMap->gravity;
        collision->unk54[2] = surfaceMap->rotation;
        return 1;
    }
    return 0;
}

s32 func_802ABB04(f32 posX, f32 posZ, u16 index) {
    mk64_surface_map_ram *surfaceMap = &gSurfaceMap[index];
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
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f0_2;
    s32 flag = 1;

    x1 = surfaceMap->vtxPoly1->v.ob[0];
    z1 = surfaceMap->vtxPoly1->v.ob[2];

    x2 = surfaceMap->vtxPoly2->v.ob[0];
    z2 = surfaceMap->vtxPoly2->v.ob[2];

    x3 = surfaceMap->vtxPoly3->v.ob[0];
    z3 = surfaceMap->vtxPoly3->v.ob[2];

    temp_f2_2 = (z1 - posZ) * (x2 - posX) - (x1 - posX) * (z2 - posZ);

    if (!temp_f2_2) {

        temp_f0_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        temp_f2_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

        if ((temp_f0_2 * temp_f2_3) < 0.0f) {
            flag = 0;
        }
    } else {
        temp_f0_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);
        if (!temp_f0_2) {
            temp_f2_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);
            if (temp_f2_2 * temp_f2_3 < 0.0f) {
                flag = 0;
            }
        } else {
            if ((temp_f2_2 * temp_f0_2) < 0.0f) {
                flag = 0;
            } else {
                temp_f2_3 = ((z3 - posZ) * (x1 - posX)) - ((x3 - posX) * (z1 - posZ));
                if (temp_f2_3 != 0) {
                    if ((temp_f0_2 * temp_f2_3) < 0.0f) {
                        flag = 0;
                    }
                }
            }
        }
    }
    return flag;
}

s8 get_surface_type(u16 index) {
    mk64_surface_map_ram *tile = &gSurfaceMap[index];
    return tile->surfaceType;
}

s16 func_802ABD40(u16 index) {
    mk64_surface_map_ram *tile = &gSurfaceMap[index];
    return tile->flags & 0xFF;
}

s16 func_802ABD7C(u16 index) {
    mk64_surface_map_ram *tile = &gSurfaceMap[index];
    return tile->flags & 0x1000;
}

s16 func_802ABDB8(u16 index) {
    mk64_surface_map_ram *tile = &gSurfaceMap[index];
    return tile->flags & 0x400;
}

s16 func_802ABDF4(u16 index) {
    mk64_surface_map_ram *tile = &gSurfaceMap[index];
    return tile->flags & 0x800;
}

f32 func_802ABE30(f32 x, f32 y, f32 z, u16 index) {
    mk64_surface_map_ram *tile = &gSurfaceMap[index];
    if (tile->gravity == 0.0f) {
        return y;
    }
    return ((tile->height * x) + (tile->rotation * z) + tile->height2) / -tile->gravity;
}

f32 func_802ABEAC(Collision *collision, Vec3f pos) {
    if (collision->unk34 == 1) {
        return func_802ABE30(pos[0], pos[1], pos[2], collision->unk3A);
    }
    if (collision->unk30 == 1) {
        return func_802ABE30(pos[0], pos[1], pos[2], collision->unk36);
    }
    if (collision->unk32 == 1) {
        return func_802ABE30(pos[0], pos[1], pos[2], collision->unk38);
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
    f32 temp;

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

    temp = 1.0f / magnitude * velocityMagnitude;

    velocity[0] = x * temp;
    velocity[1] = y * temp;
    velocity[2] = z * temp;
}

void shell_collision(Collision *collision, Vec3f velocity) {
    if (collision->unk3C[0] < 0.0f) {
        process_shell_collision(collision->unk48, collision->unk3C[0], velocity, 2.0f);
    }

    if (collision->unk3C[1] < 0.0f) {
        process_shell_collision(collision->unk54, collision->unk3C[1], velocity, 2.0f);
    }
}

/**
 * Adjusts the position of pos2 based on pos1 but in the orthogonal direction to pos2.
 */
void func_802AC114(Vec3f pos1, f32 boundingBoxSize, Vec3f pos2, UNUSED f32 unk) {
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

    orthoX = x2  - (dotProduct * x1);
    orthoY = y2  - (dotProduct * y1);
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

s32 func_802AC22C(KartBoundingBoxCorner *arg0) {
    Collision spD0;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    UNUSED s32 stackPadding5;
    UNUSED s32 stackPadding6;
    UNUSED s32 stackPadding7;
    UNUSED s32 stackPadding8;
    UNUSED s32 stackPadding9;
    UNUSED s32 stackPaddingA;
    UNUSED s32 stackPaddingB;
    s32 temp1;
    s32 temp2;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    s16 temp_f4;
    s16 temp_f6;
    u16 var_s2;
    u16 temp_v1;
    u16 temp_s0;
    s16 temp_v1_2;
    u16 var_s1;

    spD0.unk30 = 0;
    spD0.unk32 = 0;
    spD0.unk34 = 0;
    spD0.unk3C[0] = 1000.0f;
    spD0.unk3C[1] = 1000.0f;
    spD0.unk3C[2] = 1000.0f;
    temp_f22 = arg0->cornerPos[0];
    temp_f24 = arg0->cornerPos[1];
    temp_f26 = arg0->cornerPos[2];
    switch (arg0->surfaceFlags) {                              /* irregular */
    case 0x80:
        if (func_802AB6C4(&spD0, 5.0f, temp_f22, temp_f24, temp_f26, (u16) (s32) arg0->surfaceMapIndex) == 1) {
            arg0->cornerGroundY = func_802ABE30(temp_f22, temp_f24, temp_f26, arg0->surfaceMapIndex);
            return 1;
        }
        break;
    case 0x40:
        if (func_802AAE4C(&spD0, 5.0f, temp_f22, temp_f24, temp_f26, (u16) (s32) arg0->surfaceMapIndex) == 1) {
            arg0->cornerGroundY = func_802ABE30(temp_f22, temp_f24, temp_f26, arg0->surfaceMapIndex);
            return 1;
        }
        break;
    case 0x20:
        if (func_802AB288(&spD0, 5.0f, temp_f22, temp_f24, temp_f26, (u16) (s32) arg0->surfaceMapIndex) == 1) {
            arg0->cornerGroundY = func_802ABE30(temp_f22, temp_f24, temp_f26, arg0->surfaceMapIndex);
            return 1;
        }
        break;
    default:
        break;
    }
    temp1 = gCourseMaxX - gCourseMinX;
    temp2 = gCourseMaxZ - gCourseMinZ;
    temp_f4 = (temp_f22 - gCourseMinX) / (temp1 / 32);
    temp_f6 = (temp_f26 - gCourseMinZ) / (temp2 / 32);
    if (temp_f4 < 0) {
        return 0;
    }
    if (temp_f6 < 0) {
        return 0;
    }
    if (temp_f4 >= 0x20) {
        return 0;
    }
    if (temp_f6 >= 0x20) {
        return 0;
    }

    temp_v1_2 = (temp_f4 + temp_f6 * 32);
    temp_v1 = D_8014F110[temp_v1_2].unk2;
    if (temp_v1 == 0) {
        return 0;
    }
    var_s1 = D_8014F110[temp_v1_2].unk0;
    for (var_s2 = 0; var_s2 < temp_v1; var_s2++) {
        temp_s0 = D_8015F584[var_s1];
        if (gSurfaceMap[temp_s0].flags & 0x4000) {
            if (temp_s0 != arg0->surfaceMapIndex) {
                if (func_802AAE4C(&spD0, 5.0f, temp_f22, temp_f24, temp_f26, temp_s0) == 1) {
                    arg0->cornerGroundY = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0);
                    arg0->surfaceType = gSurfaceMap[temp_s0].surfaceType;
                    arg0->surfaceFlags = 0x40;
                    arg0->surfaceMapIndex = temp_s0;
                    if (gSurfaceMap[temp_s0].flags & 0x1000) {
                        arg0->unk_14 = 1;
                    } else {
                        arg0->unk_14 = 0;
                    }
                    return 1;
                }
            }
        } else if (gSurfaceMap[temp_s0].flags & 0x8000) {
            if ((gSurfaceMap[temp_s0].height != 1.0f) && (temp_s0 != arg0->surfaceMapIndex)) {
                if (func_802AB6C4(&spD0, 5.0f, temp_f22, temp_f24, temp_f26, temp_s0) == 1) {
                    arg0->cornerGroundY = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0);
                    arg0->surfaceType = gSurfaceMap[temp_s0].surfaceType;
                    arg0->surfaceFlags = 0x80;
                    arg0->surfaceMapIndex = temp_s0;
                    return 1;
                }
            }
        } else if ((gSurfaceMap[temp_s0].rotation != 1.0f) && (temp_s0 != arg0->surfaceMapIndex)) {
            if (func_802AB288(&spD0, 5.0f, temp_f22, temp_f24, temp_f26, temp_s0) == 1) {
                arg0->cornerGroundY = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0);
                arg0->surfaceType = gSurfaceMap[temp_s0].surfaceType;
                arg0->surfaceFlags = 0x20;
                arg0->surfaceMapIndex = temp_s0;
                return 1;
            }
        }
        var_s1++;
    }
    arg0->cornerGroundY = temp_f24;
    arg0->surfaceType = 0;
    // Another function that has a return value but doesn't have an explicit return statement in one of its codepaths.
    // The return value at this point will be whatever was last returned by func_802AAE4C/func_802AB6C4/func_802AB288
    // depending on which (if any) if statements were entered on the loop's last cycle
}

s32 is_colliding_with_drivable_surface(Collision *collision, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 index, f32 posX, f32 posY, f32 posZ) {
    mk64_surface_map_ram *tile = &gSurfaceMap[index];
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
    f32 temp_f0_4;
    f32 temp;
    UNUSED s32 pad2[2];
    f32 area;
    f32 area2;
    f32 area3;
    s32 flag = 1;

    if (tile->vtx31 > x1) {
        return 0;
    }
    if (tile->vtx33 > z1) {
        return 0;
    }
    if (tile->vtx21 < x1) {
        return 0;
    }
    if (tile->vtx23 < z1) {
        return 0;
    }
    if ((tile->vtx32 - boundingBoxSize * 3.0f) > y1) {
        return 0;
    }

    x2 = (f32) tile->vtxPoly1->v.ob[0];
    z2 = (f32) tile->vtxPoly1->v.ob[2];

    x3 = (f32) tile->vtxPoly2->v.ob[0];
    z3 = (f32) tile->vtxPoly2->v.ob[2];

    x4 = (f32) tile->vtxPoly3->v.ob[0];
    z4 = (f32) tile->vtxPoly3->v.ob[2];

    area = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);

    if (area == 0) {
        area2 = (z3 - z1) * (x4 - x1) - (x3 - x1) * (z4 - z1);
        area3 = (z4 - z1) * (x2 - x1) - (x4 - x1) * (z2 - z1);
        if (area2 * area3 < 0.0f) {
            flag = 0;
        }
    } else {

        area2 = (z3 - z1) * (x4 - x1) - (x3 - x1) * (z4 - z1);


        if (area2 == 0) {

            area3 = (z4 - z1) * (x2 - x1) - (x4 - x1) * (z2 - z1);

            if (area * area3 < 0.0f) {
                flag = 0;
            }
        } else {
            if ((area * area2) < 0.0f) {
                flag = 0;
            } else {
                area3 = (z4- z1) * (x2 - x1) - (x4 - x1) * (z2 - z1);
                if (area3 != 0) {
                    if (area2 * area3 < 0.0f) {
                        flag = 0;
                    }
                }
            }
        }
    }
    if (flag == 0) {
        return 0;
    }

    temp_f0_4 = (tile->height * x1) + (tile->gravity * y1) + (tile->rotation * z1) + tile->height2;

    if (temp_f0_4 > boundingBoxSize) {
        if (collision->unk3C[2] > temp_f0_4) {
            collision->unk34 = 1;
            collision->unk3A = index;
            collision->unk3C[2] = temp_f0_4 - boundingBoxSize;
            collision->orientationVector[0] = tile->height;
            collision->orientationVector[1] = tile->gravity;
            collision->orientationVector[2] = tile->rotation;
        }
        return 0;
    }

    temp = (tile->height * posX) + (tile->gravity * posY) + (tile->rotation * posZ) + tile->height2;

    if (temp < 0.0f) {
        return 0;
    }
        collision->unk34 = 1;
        collision->unk3A = index;
        collision->unk3C[2] = temp_f0_4 - boundingBoxSize;
        collision->orientationVector[0] = tile->height;
        collision->orientationVector[1] = tile->gravity;
        collision->orientationVector[2] = tile->rotation;
        return 1;
}

/**
 * Wall collision
*/
s32 is_colliding_with_wall2(Collision *arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 arg5, f32 arg6, f32 arg7, f32 arg8) {
    mk64_surface_map_ram *tile = &gSurfaceMap[arg5];
    UNUSED s32 pad[6];
    f32 x4;
    f32 y4;
    f32 x3;
    f32 y3;
    f32 x2;
    f32 y2;
    UNUSED s32 pad2[3];
    f32 temp_f0_4;
    f32 temp_f4_2;
    UNUSED s32 pad3[2];
    f32 area;
    f32 area2;
    f32 area3;
    s32 flag = 1;
    if (tile->vtx31 > x1) {
        return 0;
    }
    if (tile->vtx21 < x1) {
        return 0;
    }
    if (tile->vtx22 < y1) {
        return 0;
    }
    if (tile->vtx32 > y1) {
        return 0;
    }
    if ((tile->vtx33 - boundingBoxSize * 3.0f) > z1) {
        return 0;
    }
    if ((tile->vtx23 + boundingBoxSize * 3.0f) < z1) {
        return 0;
    }

    x2 = (f32) tile->vtxPoly1->v.ob[0];
    y2 = (f32) tile->vtxPoly1->v.ob[1];

    x3 = (f32) tile->vtxPoly2->v.ob[0];
    y3 = (f32) tile->vtxPoly2->v.ob[1];

    x4 = (f32) tile->vtxPoly3->v.ob[0];
    y4 = (f32) tile->vtxPoly3->v.ob[1];

    area = (y2 - y1) * (x3 - x1) - (x2 - x1) * (y3 - y1);

    if (area == 0) {
        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);
        area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);


        if (area2 * area3 < 0.0f) {
            flag = 0;
        }
    } else {

        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);


            if ((area * area3) < 0.0f) {
                flag = 0;
            }
        } else {
            if ((area * area2) < 0.0f) {
                flag = 0;
            } else {
                area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

                if (area3 != 0) {
                    if ((area2 * area3) < 0.0f) {
                        flag = 0;
                    }
                }
            }
        }
    }
    if (flag == 0) {
        return 0;
    }

    temp_f0_4 = ((tile->height * x1) + (tile->gravity * y1) + (tile->rotation * z1)) + tile->height2;
    if (tile->flags & 0x200) {
        temp_f4_2 = ((tile->height * arg6) + (tile->gravity * arg7) + (tile->rotation * arg8)) + tile->height2;
        //sp48 = temp_f4_2;
        if ((temp_f0_4 > 0.0f) && (temp_f4_2 > 0.0f)) {
            if (temp_f0_4 < boundingBoxSize) {
                arg->unk30 = 1;
                arg->unk36 = arg5;
                arg->unk3C[0] = temp_f0_4 - boundingBoxSize;
                arg->unk48[0] = tile->height;
                arg->unk48[1] = tile->gravity;
                arg->unk48[2] = tile->rotation;
                return 1;
            }
            return 0;
        }

        if ((temp_f0_4 < 0.0f) && (temp_f4_2 < 0.0f)) {
            temp_f0_4 *= -1.0f;
            if (temp_f0_4 < boundingBoxSize) {
                arg->unk30 = 1;
                arg->unk36 = arg5;
                arg->unk3C[0] = temp_f0_4 - boundingBoxSize;
                arg->unk48[0] = -tile->height;
                arg->unk48[1] = -tile->gravity;
                arg->unk48[2] = -tile->rotation;
                return 1;
            }
            return 0;
        }
        if ((temp_f0_4 > 0.0f) && (temp_f4_2 < 0.0f)) {
            arg->unk30 = 1;
            arg->unk36 = arg5;
            arg->unk3C[0] = -(temp_f0_4 + boundingBoxSize);
            arg->unk48[0] = -tile->height;
            arg->unk48[1] = -tile->gravity;
            arg->unk48[2] = -tile->rotation;
            return 1;
        }
        if ((temp_f0_4 < 0.0f) && (temp_f4_2 > 0.0f)) {
            arg->unk30 = 1;
            arg->unk36 = arg5;
            arg->unk3C[0] = temp_f0_4 + boundingBoxSize;
            arg->unk48[0] = tile->height;
            arg->unk48[1] = tile->gravity;
            arg->unk48[2] = tile->rotation;
            return 1;
        }
        if (temp_f0_4 == 0.0f) {
            if (temp_f4_2 >= 0.0f) {
                arg->unk30 = 1;
                arg->unk36 = arg5;
                arg->unk3C[0] = temp_f4_2 + boundingBoxSize;
                arg->unk48[0] = tile->height;
                arg->unk48[1] = tile->gravity;
                arg->unk48[2] = tile->rotation;
                return 1;
            }
            arg->unk30 = 1;
            arg->unk36 = arg5;
            arg->unk3C[0] = -(temp_f4_2 + boundingBoxSize);
            arg->unk48[0] = tile->height;
            arg->unk48[1] = tile->gravity;
            arg->unk48[2] = tile->rotation;
            return 1;
        }
        return 0;
    }
    if (temp_f0_4 > boundingBoxSize) {
        if (temp_f0_4 < arg->unk3C[0]) {
            arg->unk30 = 1;
            arg->unk36 = arg5;
            arg->unk3C[0] = temp_f0_4 - boundingBoxSize;
            arg->unk48[0] = tile->height;
            arg->unk48[1] = tile->gravity;
            arg->unk48[2] = tile->rotation;
        }
        return 0;
    }

    temp_f4_2 = (tile->height * arg6) + (tile->gravity * arg7) + (tile->rotation * arg8) + tile->height2;
    if (temp_f4_2 < 0.0f) {
        return 0;
    }
    arg->unk30 = 1;
    arg->unk36 = arg5;
    arg->unk3C[0] = temp_f0_4 - boundingBoxSize;
    arg->unk48[0] = tile->height;
    arg->unk48[1] = tile->gravity;
    arg->unk48[2] = tile->rotation;
    return 1;
}

/**
 * This is actually more like colliding with face X/Y/Z
*/
s32 is_colliding_with_wall1(Collision *arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 arg5, f32 arg6, f32 arg7, f32 arg8) {
    mk64_surface_map_ram *tile = &gSurfaceMap[arg5];
    s32 flag = 1;
    UNUSED s32 pad[7];
    f32 y4;
    f32 z4;
    f32 y3;
    f32 z3;
    f32 y2;
    f32 z2;
    UNUSED s32 pad3[2];
    f32 temp_f0_4;
    f32 temp_f4_2;
    UNUSED s32 pad4[2];
    f32 area;
    f32 area2;
    f32 area3;

    if (tile->vtx33 > z1) {
        return 0;
    }
    if (tile->vtx23 < z1) {
        return 0;
    }
    if (tile->vtx22 < y1) {
        return 0;
    }
    if (tile->vtx32 > y1) {
        return 0;
    }
    if ((tile->vtx31 - boundingBoxSize * 3.0f) > x1) {
        return 0;
    }
    if ((tile->vtx21 + boundingBoxSize * 3.0f) < x1) {
        return 0;
    }

    z2 = (f32) tile->vtxPoly1->v.ob[2];
    y2 = (f32) tile->vtxPoly1->v.ob[1];

    z3 = (f32) tile->vtxPoly2->v.ob[2];
    y3 = (f32) tile->vtxPoly2->v.ob[1];

    z4 = (f32) tile->vtxPoly3->v.ob[2];
    y4 = (f32) tile->vtxPoly3->v.ob[1];

    area = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);

    if (area == 0) {
        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);
        area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);


        if (area2 * area3 < 0.0f) {
            flag = 0;
        }
    } else {

        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);


            if ((area * area3) < 0.0f) {
                flag = 0;
            }
        } else {
            if ((area * area2) < 0.0f) {
                flag = 0;
            } else {
                area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

                if (area3 != 0) {
                    if ((area2 * area3) < 0.0f) {
                        flag = 0;
                    }
                }
            }
        }
    }
    if (flag == 0) {
        return 0;
    }

    temp_f0_4 = ((tile->height * x1) + (tile->gravity * y1) + (tile->rotation * z1)) + tile->height2;
    if (tile->flags & 0x200) {
        temp_f4_2 = ((tile->height * arg6) + (tile->gravity * arg7) + (tile->rotation * arg8)) + tile->height2;
        //sp48 = temp_f4_2;
        if ((temp_f0_4 > 0.0f) && (temp_f4_2 > 0.0f)) {
            if (temp_f0_4 < boundingBoxSize) {
                arg->unk32 = 1;
                arg->unk38 = arg5;
                arg->unk3C[1] = temp_f0_4 - boundingBoxSize;
                arg->unk54[0] = tile->height;
                arg->unk54[1] = tile->gravity;
                arg->unk54[2] = tile->rotation;
                return 1;
            }
            return 0;
        }

        if ((temp_f0_4 < 0.0f) && (temp_f4_2 < 0.0f)) {
            temp_f0_4 *= -1.0f;
            if (temp_f0_4 < boundingBoxSize) {
                arg->unk32 = 1;
                arg->unk38 = arg5;
                arg->unk3C[1] = temp_f0_4 - boundingBoxSize;
                arg->unk54[0] = -tile->height;
                arg->unk54[1] = -tile->gravity;
                arg->unk54[2] = -tile->rotation;
                return 1;
            }
            return 0;
        }
        if ((temp_f0_4 > 0.0f) && (temp_f4_2 < 0.0f)) {
            arg->unk32 = 1;
            arg->unk38 = arg5;
            arg->unk3C[1] = -(temp_f0_4 + boundingBoxSize);
            arg->unk54[0] = -tile->height;
            arg->unk54[1] = -tile->gravity;
            arg->unk54[2] = -tile->rotation;
            return 1;
        }
        if ((temp_f0_4 < 0.0f) && (temp_f4_2 > 0.0f)) {
            arg->unk32 = 1;
            arg->unk38 = arg5;
            arg->unk3C[1] = temp_f0_4 + boundingBoxSize;
            arg->unk54[0] = tile->height;
            arg->unk54[1] = tile->gravity;
            arg->unk54[2] = tile->rotation;
            return 1;
        }
        if (temp_f0_4 == 0.0f) {
            if (temp_f4_2 >= 0.0f) {
                arg->unk32 = 1;
                arg->unk38 = arg5;
                arg->unk3C[1] = temp_f4_2 + boundingBoxSize;
                arg->unk54[0] = tile->height;
                arg->unk54[1] = tile->gravity;
                arg->unk54[2] = tile->rotation;
                return 1;
            }
            arg->unk32 = 1;
            arg->unk38 = arg5;
            arg->unk3C[1] = -(temp_f4_2 + boundingBoxSize);
            arg->unk54[0] = tile->height;
            arg->unk54[1] = tile->gravity;
            arg->unk54[2] = tile->rotation;
            return 1;
        }
        return 0;
    }
    if (temp_f0_4 > boundingBoxSize) {
        if (arg->unk3C[1] > temp_f0_4) {
            arg->unk32 = 1;
            arg->unk38 = arg5;
            arg->unk3C[1] = temp_f0_4 - boundingBoxSize;
            arg->unk54[0] = tile->height;
            arg->unk54[1] = tile->gravity;
            arg->unk54[2] = tile->rotation;
        }
        return 0;
    }

    temp_f4_2 = (tile->height * arg6) + (tile->gravity * arg7) + (tile->rotation * arg8) + tile->height2;
    if (temp_f4_2 < 0.0f) {
        return 0;
    }
    arg->unk32 = 1;
    arg->unk38 = arg5;
    arg->unk3C[1] = temp_f0_4 - boundingBoxSize;
    arg->unk54[0] = tile->height;
    arg->unk54[1] = tile->gravity;
    arg->unk54[2] = tile->rotation;
    return 1;
}

u16 func_802AD950(Collision *collision, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2) {
    s32 temp_v0_4;
    s32 temp_v1;
    s16 temp_f10;
    s16 temp_f16;
    u16 temp_s6;
    u16 surfaceIndex;
    s16 temp_v1_2;

    u16 phi_s2;



    u16 flags = 0;
    s32 temp1;
    s32 temp2;

    u16 i;

    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    collision->unk3C[0] = 1000.0f;
    collision->unk3C[1] = 1000.0f;
    collision->unk3C[2] = 1000.0f;

    if ((s32) collision->unk3A < (s32) D_8015F588) {
        if (is_colliding_with_drivable_surface(collision, boundingBoxSize, x1, y1, z1, collision->unk3A, x2, y2, z2) == 1) {
            flags |= 0x4000;
        }
    }

    if ((s32) collision->unk36 < (s32) D_8015F588) {
        if (is_colliding_with_wall2(collision, boundingBoxSize, x1, y1, z1, collision->unk36, x2, y2, z2) == 1) {
            flags |= 0x2000;
        }
    }

    if ((s32) collision->unk38 < (s32) D_8015F588) {
        if (is_colliding_with_wall1(collision, boundingBoxSize, x1, y1, z1, collision->unk38, x2, y2, z2) == 1) {
            flags |= 0x8000;
        }
    }

    if (flags == (0x4000 | 0x2000 | 0x8000)) {
        return flags;
    }


    temp_v0_4 = (s32) gCourseMaxX - gCourseMinX;
    temp_v1 = (s32) gCourseMaxZ - gCourseMinZ;

    temp1 = temp_v0_4 / 32;
    temp2 = temp_v1 / 32;

    temp_f10 = (x1 - gCourseMinX) / temp1;
    temp_f16 = (z1 - gCourseMinZ) / temp2;

    if (temp_f10 < 0) {
        return 0;
    }
    if (temp_f16 < 0) {
        return 0;
    }
    if (temp_f10 >= 32) {
        return 0;
    }
    if (temp_f16 >= 32) {
        return 0;
    }

    temp_v1_2 = (temp_f10 + temp_f16 * 32);
    temp_s6 = D_8014F110[temp_v1_2].unk2;

    if (temp_s6 == 0) {
        return flags;
    }

    phi_s2 = D_8014F110[temp_v1_2].unk0;

    for (i = 0; i < temp_s6; i++) {
        if (flags == (0x4000 | 0x2000 | 0x8000)) {
            return flags;
        }

        surfaceIndex = D_8015F584[phi_s2];

        if ((gSurfaceMap[surfaceIndex].flags & 0x4000)) {
            if ((flags & 0x4000) == 0) {
                if (surfaceIndex != collision->unk3A) {
                    if (is_colliding_with_drivable_surface(collision, boundingBoxSize, x1, y1, z1, surfaceIndex, x2, y2, z2) == 1) {
                        flags |= 0x4000;
                    }
                }
            }
        } else if ((gSurfaceMap[surfaceIndex].flags & 0x8000) != 0) {
            if ((flags & 0x8000) == 0) {
                if (surfaceIndex != collision->unk38) {
                    if (is_colliding_with_wall1(collision, boundingBoxSize, x1, y1, z1, surfaceIndex, x2, y2, z2) == 1) {
                        flags |= 0x8000;
                    }
                }
            }
        } else if ((flags & 0x2000) == 0) {
            if (surfaceIndex != collision->unk36) {
                if (is_colliding_with_wall2(collision, boundingBoxSize, x1, y1, z1, surfaceIndex, x2, y2, z2) == 1) {
                    flags |= 0x2000;
                }
            }
        }
        phi_s2++;

    }
    return flags;
}

u16 func_802ADDC8(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ) {
    u16 temp_v1;
    s32 temp_f4;
    s32 temp_f6;
    u16 temp_v0_4;
    s32 temp1;
    s32 temp2;
    s16 temp_f10;
    s16 temp_f16;
    s16 temp;
    u16 i;

    u16 var_s2;
    u16 var_s4;

    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    collision->unk3C[0] = 1000.0f;
    collision->unk3C[1] = 1000.0f;
    collision->unk3C[2] = 1000.0f;
    var_s4 = 0;
    if (collision->unk3A < D_8015F588) {
        if (func_802AAE4C(collision, boundingBoxSize, posX, posY, posZ, collision->unk3A) == 1) {
            var_s4 |= 0x4000;
        }
    }
    if (collision->unk36 < D_8015F588) {
        if (func_802AB288(collision, boundingBoxSize, posX, posY, posZ, collision->unk36) == 1) {
            var_s4 |= 0x2000;
        }
    }
    if (collision->unk38 < D_8015F588) {
        if (func_802AB6C4(collision, boundingBoxSize, posX, posY, posZ, collision->unk38 ) == 1) {
            var_s4 |= 0x8000;
        }
    }
    if (var_s4 == (0x4000 | 0x2000 | 0x8000)) {
        return var_s4;
    }

    temp_f4 = (s32) gCourseMaxX - gCourseMinX;
    temp_f6 = (s32) gCourseMaxZ - gCourseMinZ;

    temp1 = temp_f4 / 32;
    temp2 = temp_f6 / 32;

    temp_f10 = (posX - gCourseMinX) / temp1;
    temp_f16 = (posZ - gCourseMinZ) / temp2;


    if (temp_f10 < 0) {
        return 0;
    }
    if (temp_f16 < 0) {
        return 0;
    }
    if (temp_f10 >= 32) {
        return 0;
    }
    if (temp_f16 >= 32) {
        return 0;
    }

    temp = temp_f10 + temp_f16 * 32;
    temp_v1 = D_8014F110[temp].unk2;
    if (temp_v1 == 0) {
        return var_s4;
    }
    var_s2 = D_8014F110[temp].unk0;

    for (i = 0; i < temp_v1; i++) {
        if (var_s4 == (0x8000 | 0x4000 | 0x2000)) {
            return var_s4;
        }
        temp_v0_4 = D_8015F584[var_s2];
        if (gSurfaceMap[temp_v0_4].flags & 0x4000) {
            if (!(var_s4 & 0x4000)) {
                if (temp_v0_4 != collision->unk3A) {
                    if (func_802AAE4C(collision, boundingBoxSize, posX, posY, posZ, temp_v0_4) == 1) {
                        var_s4 |= 0x4000;
                    }
                }
            }
        } else if (gSurfaceMap[temp_v0_4].flags & 0x8000) {
            if (!(var_s4 & 0x8000)) {
                if (temp_v0_4 != collision->unk38) {
                    if (func_802AB6C4(collision, boundingBoxSize, posX, posY, posZ, temp_v0_4) == 1) {
                        var_s4 |= 0x8000;
                    }
                }
            }
        } else {
            if (!(var_s4 & 0x2000)) {
                if (temp_v0_4 != collision->unk36) {
                    if (func_802AB288(collision, boundingBoxSize, posX, posY, posZ, temp_v0_4) == 1) {
                        var_s4 |= 0x2000;
                    }
                }
            }
        }
        var_s2++;
    }
    return var_s4;
}

extern u8 D_8014F1110;

f32 func_802AE1C0(f32 posX, f32 posY, f32 posZ) {
    f32 temp_f0;
    s16 temp_f4;
    s16 temp_f6;
    s16 temp_f66;

    u16 index;
    u16 iter;
    u16 phi_s1;
    f32 phi_f20 = -3000.0f;
    u16 i;

    s32 a;
    s32 b;
    s32 c;
    s32 d;

    a = (gCourseMaxX - gCourseMinX);
    b = (gCourseMaxZ - gCourseMinZ);
    c = a / 32;
    d = b / 32;

    temp_f4 = (s16) ((posX - gCourseMinX) / c);
    temp_f6 = (s16) ((posZ - gCourseMinZ) / d);
    temp_f66 = temp_f4 + (temp_f6 * 32);
    iter = D_8014F110[temp_f66].unk2;

    if (temp_f4 < 0) {
        return 3000.0f;
    }
    if (temp_f6 < 0) {
        return 3000.0f;
    }
    if (temp_f4 >= 32) {
        return 3000.0f;
    }
    if (temp_f6 >= 32) {
        return 3000.0f;
    }
    if (iter == 0) {
        return 3000.0f;
    }

    phi_s1 = D_8014F110[temp_f66].unk0;

    for (i = 0; i < iter; i++) {

        index = D_8015F584[phi_s1];

        if ((gSurfaceMap[index].flags & 0x4000) && (func_802ABB04(posX, posZ, index) == 1)) {
            temp_f0 = func_802ABE30(posX, posY, posZ, index);
            if ((temp_f0 <= posY) && (phi_f20 < temp_f0)) {
                phi_f20 = temp_f0;
            }
        }
        phi_s1++;
    }
    return phi_f20;
}

#define MAX3(a,b,c, out) if (a >= b) {\
        if (a >= c) {\
            out = a;\
        } else\
            out = c;\
\
    } else if (b >= c) {\
        out = b;\
    } else\
        out = c; \

#define MIN3(a,b,c, out) if (a <= b) {\
        if (a <= c) {\
            out = a;\
        } else\
            out = c;\
\
    } else if (b <= c) {\
        out = b;\
    } else\
        out = c;\

void func_802AE434(Vtx *vtx1, Vtx *vtx2, Vtx *vtx3, s8 surfaceType, u16 sectionId) {
    mk64_surface_map_ram *tile = &gSurfaceMap[D_8015F588];
    s16 x2;
    s16 z2;
    u16 poly1Flag;
    s16 x3;
    s16 x1;
    s16 y1;
    s16 z1;
    u16 poly2Flag;
    s16 y2;
    u16 poly3Flag;
    u16 flags;
    s16 y3;
    s16 z3;

    // printf("mesh index: %d ", D_8015F588);
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

    tile->vtxPoly1 = vtx1;
    tile->vtxPoly2 = vtx2;
    tile->vtxPoly3 = vtx3;
    if ((tile->vtxPoly1->v.flag == 4) &&
        (tile->vtxPoly2->v.flag == 4) &&
        (tile->vtxPoly3->v.flag == 4)) {

        return;
    }

    x1 = tile->vtxPoly1->v.ob[0];
    y1 = tile->vtxPoly1->v.ob[1];
    z1 = tile->vtxPoly1->v.ob[2];
    x2 = tile->vtxPoly2->v.ob[0];
    y2 = tile->vtxPoly2->v.ob[1];
    z2 = tile->vtxPoly2->v.ob[2];
    x3 = tile->vtxPoly3->v.ob[0];
    y3 = tile->vtxPoly3->v.ob[1];
    z3 = tile->vtxPoly3->v.ob[2];
    if ((x1 == x2) && (z1 == z2)) {
        tile->vtxPoly1 = vtx1;
        tile->vtxPoly3 = vtx2;
        tile->vtxPoly2 = vtx3;
        x1 = tile->vtxPoly1->v.ob[0];
        y1 = tile->vtxPoly1->v.ob[1];
        z1 = tile->vtxPoly1->v.ob[2];
        x2 = tile->vtxPoly3->v.ob[0];
        y2 = tile->vtxPoly3->v.ob[1];
        z2 = tile->vtxPoly3->v.ob[2];
        x3 = tile->vtxPoly2->v.ob[0];
        y3 = tile->vtxPoly2->v.ob[1];
        z3 = tile->vtxPoly2->v.ob[2];
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
    magnitude = sqrtf((crossProductX * crossProductX) + (crossProductY * crossProductY) + (crossProductZ * crossProductZ));

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

    tile->vtx21 = maxX;
    tile->vtx23 = maxZ;
    tile->vtx31 = minX;
    tile->vtx33 = minZ;
    tile->vtx32 = minY;
    tile->vtx22 = maxY;

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

    // printf("X 0x%X\n ", normalX);
    // printf("Y 0x%X\n ", normalY);
    // printf("Z 0x%X\n ", normalZ);
    // printf("dist 0x%X\n\n", distance);

    tile->height = normalX;
    tile->gravity = normalY;
    tile->rotation = normalZ;
    tile->height2 = distance;

    tile->surfaceType = (u16) surfaceType;

    crossProductX = crossProductX * crossProductX;
    crossProductY = crossProductY * crossProductY;
    crossProductZ = crossProductZ * crossProductZ;

    D_8015F6FA = 0;
    D_8015F6FC = 0;

    poly1Flag = tile->vtxPoly1->v.flag;
    poly2Flag = tile->vtxPoly2->v.flag;
    poly3Flag = tile->vtxPoly3->v.flag;

    flags = sectionId;

    if ((poly1Flag == 1) && (poly2Flag == 1) && (poly3Flag == 1)) {
        flags |= 0x400;
    } else if ((poly1Flag == 2) && (poly2Flag == 2) && (poly3Flag == 2)) {
        flags |= 0x800;
    } else if ((poly1Flag == 3) && (poly2Flag == 3) && (poly3Flag == 3)) {
        flags |= 0x1000;
    } else if (D_8015F5A4 != 0) {
        flags |= 0x200;
    }

    tile->flags = flags;

    // Y is the significant axis
    if ((crossProductX <= crossProductY) && (crossProductY >= crossProductZ)) {
        tile->flags |= 0x4000;
    // X is the significant axis
    } else if ((crossProductX > crossProductY) && (crossProductX >= crossProductZ)) {
        tile->flags |= 0x8000;
    // Z is the significant axis
    } else {
        tile->flags |= 0x2000;
    }
    D_8015F588++;
}

/**
 * Triangle contains three indices that are used to get the actual vertices.
 */
void set_vtx_from_triangle(u32 triangle, s8 surfaceType, u16 sectionId) {
    u32 vert1 = ( ( triangle & 0x00FF0000 ) >> 16 ) / 2;
    u32 vert2 = ( ( triangle & 0x0000FF00 ) >>  8 ) / 2;
    u32 vert3 = (   triangle & 0x000000FF )         / 2;

    Vtx *vtx1 = vtxBuffer[vert1];
    Vtx *vtx2 = vtxBuffer[vert2];
    Vtx *vtx3 = vtxBuffer[vert3];

    func_802AE434(vtx1, vtx2, vtx3, surfaceType, sectionId);
}

void set_vtx_from_tri2(u32 triangle1, u32 triangle2, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[2];
    u32 vert1 = ( ( triangle1 & 0x00FF0000 ) >> 16 ) / 2;
    u32 vert2 = ( ( triangle1 & 0x0000FF00 ) >>  8 ) / 2;
    u32 vert3 = (   triangle1 & 0x000000FF )         / 2;

    // This is actually triangle 2; vert 1,2,3.
    u32 vert4 = ( ( triangle2 & 0x00FF0000 ) >> 16 ) / 2;
    u32 vert5 = ( ( triangle2 & 0x0000FF00 ) >>  8 ) / 2;
    u32 vert6 = (   triangle2 & 0x000000FF )         / 2;

    // printf("tr1: 0x%X \n", triangle1);
    // printf("tr2: 0x%X \n", triangle2);

    // printf("\nv1 0x%X ", vert1);
    // printf("v2 0x%X ", vert2);
    // printf("v3 0x%X ", vert3);
    // printf("v4 0x%X ", vert4);
    // printf("v5 0x%X ", vert5);
    // printf("v6 0x%X\n", vert6);


    Vtx *vtx1 = vtxBuffer[vert1];
    Vtx *vtx2 = vtxBuffer[vert2];
    Vtx *vtx3 = vtxBuffer[vert3];

    Vtx *vtx4 = vtxBuffer[vert4];
    Vtx *vtx5 = vtxBuffer[vert5];
    Vtx *vtx6 = vtxBuffer[vert6];

    // Triangle 1
    func_802AE434(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    func_802AE434(vtx4, vtx5, vtx6, surfaceType, sectionId);
}

void set_vtx_from_quadrangle(u32 line, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[6];
    Vtx *vtx1;
    Vtx *vtx2;
    Vtx *vtx3;
    Vtx *vtx4;

    u32 vert1 = ( ( line & 0x00FF0000 ) >> 16 ) / 2;
    u32 vert2 = ( ( line & 0x0000FF00 ) >>  8 ) / 2;
    u32 vert3 = (   line & 0x000000FF )         / 2;
    u32 vert4 = ( ( line & 0xFF000000 ) >> 24 ) / 2;

    vtx1 = vtxBuffer[vert1];
    vtx2 = vtxBuffer[vert2];
    vtx3 = vtxBuffer[vert3];
    vtx4 = vtxBuffer[vert4];

    // Triangle 1
    func_802AE434(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    func_802AE434(vtx1, vtx3, vtx4, surfaceType, sectionId);
}

/**
 * Generates a list of pointers to course vtx.
 */
void set_vtx_buffer(uintptr_t addr, u32 numVertices, u32 bufferIndex) {
    u32 i;
    Vtx *vtx = (Vtx *) addr;
    for (i = 0; i < numVertices; i++) {
        //printf("VTX: 0x%llX",vtx);
        if (vtx == 0) {
           assert(false);
        }
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
    mk64_surface_map_ram *surfaceMap = &gSurfaceMap[index];
    s16 x1;
    s16 z1;
    s16 x2;
    s16 z2;
    s16 x3;
    s16 z3;

    x1 = surfaceMap->vtxPoly1->v.ob[0];
    z1 = surfaceMap->vtxPoly1->v.ob[2];
    x2 = surfaceMap->vtxPoly2->v.ob[0];
    z2 = surfaceMap->vtxPoly2->v.ob[2];
    x3 = surfaceMap->vtxPoly3->v.ob[0];
    z3 = surfaceMap->vtxPoly3->v.ob[2];
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
 * Appears to initialize some variables and struct members after
 * course displaylists have been parsed.
 */
void func_802AF314(void) {
    mk64_surface_map_ram *tile;
    s32 i, j, k;
    UNUSED s32 pad[5];
    s16 temp_a1;
    s16 temp_a3;
    s16 temp_s1;
    s16 temp_s2;
    s32 temp_fp;
    s32 temp_s6;
    s32 temp1;
    s32 temp2;
    s32 index;
    temp1 = (s32) gCourseMaxX - gCourseMinX;
    temp2 = (s32) gCourseMaxZ - gCourseMinZ;

    temp_s6 = temp1 / 32;
    temp_fp = temp2 / 32;

    for (i = 0; i < 1024; i++) {
        D_8014F110[i].unk2 = 0;
    }

    D_8015F58A = 0;
    /**
     * @bug possibly bug. Allocate memory but not increment the pointer.
     * This is bad, dumb code, and more importantly it's bad dumb code that doesn't make any sense here.
     * It is incremented after this function completes using a different variable.
     * Not good.
    */
    D_8015F584 = (u16 *) gNextFreeMemoryAddress;

    for (j = 0; j < 32; j++) {
        for (k = 0; k < 32; k++) {
            index = k + j * 32;

            temp_s1 = (gCourseMinX + (temp_s6 * k)) - 20;
            temp_s2 = (gCourseMinZ + (temp_fp * j)) - 20;

            temp_a1 = temp_s1 + temp_s6 + 40;
            temp_a3 = temp_s2 + temp_fp + 40;

            for (i = 0; i < D_8015F588; i++) {
                tile = gSurfaceMap + i;
                if (tile->vtx23 < temp_s2) { continue; }
                if (tile->vtx33 > temp_a3) { continue; }
                if (tile->vtx21 < temp_s1) { continue; }
                if (tile->vtx31 > temp_a1) { continue; }

                if (is_triangle_intersecting_bounding_box(temp_s1, temp_a1, temp_s2, temp_a3, (u16) i) == 1) {
                    if (D_8014F110[index].unk2 == 0) {
                        D_8014F110[index].unk0 = D_8015F58A;
                    }
                    D_8014F110[index].unk2++;
                    D_8015F584[D_8015F58A] = (s16) i;
                    D_8015F58A++;
                }
            }

        }
    }
}

/**
 * Recursive search for vtx and set surfaceTypes to -1 and sectionId's to 0xFF
 */
void set_vertex_data_with_defaults(Gfx *gfx) {
    find_and_set_vertex_data(gfx, SURFACE_DEFAULT, 0xFF);
}

/**
 * Recursive search for vtx and set sectionId's to 0xFF
 */
void set_vertex_data_with_default_section_id(Gfx *gfx, s8 surfaceType) {
    find_and_set_vertex_data(gfx, surfaceType, 0xFF);
}

extern u32 D_8015F58C;
u32 numTimes = 0;

/**
 * Recursive search and set for vertex data
 */
void find_and_set_vertex_data(Gfx *addr, s8 surfaceType, u16 sectionId) {
    s32 opcode;
    uintptr_t lo;
    uintptr_t hi;
    s32 i;
    numTimes++;
    //printf("Initial\n");
    //printf("ptr 0x%llX\n", &addr);
    //printf("w0 0x%llX\n", addr->words.w0);
    //printf("w1 0x%llX\n", addr->words.w1);
    //printf("----loop----\n");
    Gfx *gfx = (Gfx *) addr;
    D_8015F6FA = 0;
    D_8015F6FC = 0;

    //u8 *orig = segmented_gfx_to_virtual(0x07000000);

    // printf("\n\nORIG:\n");
    // for (size_t i = 0; i < 100; i++) {
    //     printf(" 0x%X ", orig[i]);
    // }

    for (i = 0; i < 0x1FFF; i++) {
        lo = gfx->words.w0;
        hi = gfx->words.w1;
        opcode = GFX_GET_OPCODE(lo);
        
        //  printf("ptr 0x%llX\n", &addr);
        //  printf("op 0x%llX\n", opcode);
        //   printf("w0 0x%llX\n", lo);
        //   printf("w1 0x%llX\n", hi);

        if (opcode == (G_DL << 24)) {
            // G_DL's hi contains an addr to another DL.
            //Gfx *dl = segmented_gfx_to_virtual(hi);
            // printf("DL: 0x%llX ", hi);
            //     printf("  w0 0x%llX", lo);
            //     printf("  w1 0x%llX\n", hi);

            find_and_set_vertex_data((Gfx *) (hi), surfaceType, sectionId);

        } else if (opcode == (G_VTX << 24)) {
            set_vtx_buffer((hi), (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1);

        } else if (opcode == (G_TRI1 << 24)) {
            D_8015F58C += 1;
            set_vtx_from_triangle(hi, surfaceType, sectionId);

        } else if (opcode == (G_TRI2 << 24)) {
            D_8015F58C += 2;

            set_vtx_from_tri2(lo, hi, surfaceType, sectionId);

        } else if (opcode == (G_QUAD << 24)) {
            D_8015F58C += 2;
            set_vtx_from_quadrangle(hi, surfaceType, sectionId);

        } else if (opcode == (int32_t)(G_ENDDL << 24)) {
            break;
        }

        gfx++;
    }
}

/**
 * Search for G_SETTILESIZE and set its args.
 */
void find_and_set_tile_size(uintptr_t addr, s32 uls, s32 ult) {
    Gfx *gfx = (Gfx *) addr;
    u32 opcode;

    uls = (uls << 12) & 0xFFF000;
    ult &= 0xFFF;

    while(true) {

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
    Vtx *vtx = (Vtx *) addr;

    for (i = 0; (u32)i < vertexCount; i++) {
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
void find_vtx_and_set_colours(uintptr_t displayList, s8 alpha, u8 red, u8 green, u8 blue) {
    Gfx *gfx = (Gfx *) displayList;
    uintptr_t lo;
    uintptr_t hi;
    s32 opcode;

    while(true) {
        lo = gfx->words.w0;
        hi = gfx->words.w1;
        opcode = GFX_GET_OPCODE(lo);
        if (opcode == (G_ENDDL << 24)) {
            break;
        } else if (opcode == (G_DL << 24)) {
            find_vtx_and_set_colours(hi, alpha, red, green, blue);
        } else if (opcode == (G_VTX << 24)) {
            // G_VTX contains an addr hi
            set_vertex_colours(hi, (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1, alpha, red, green, blue);
        }
        gfx++;
    }
}

void subtract_scaled_vector(Vec3f pos1, f32 boundingBoxSize, Vec3f pos2) {
    pos2[0] -= pos1[0] * boundingBoxSize;
    pos2[1] -= pos1[1] * boundingBoxSize;
    pos2[2] -= pos1[2] * boundingBoxSize;
}

u16 process_collision(Player *player, KartBoundingBoxCorner *corner, f32 cornerPosX, f32 cornerPosY, f32 cornerPosZ) {
    Collision wtf;
    Collision *collision = &wtf;
    UNUSED s32 pad;
    u16 i;
    u16 surfaceMapIndex;
    u16 iter;
    u16 phi_s2;
    f32 cornerPos1;
    f32 cornerPos2;
    f32 cornerPos3;
    f32 boundingBoxSize;
    f32 temp_f0;

    s32 temp_v0_2;
    s32 temp_v1;

    s16 temp_f10;
    s16 temp_f16;
    s16 temp_v1_2;

    s32 temp1;
    s32 temp2;
    UNUSED s32 pad2[9];

    collision->unk3C[0] = 1000.0f;
    collision->unk3C[1] = 1000.0f;
    collision->unk3C[2] = 1000.0f;
    boundingBoxSize = player->boundingBoxSize;
    collision->unk36 = 5000;
    collision->unk38 = 5000;
    collision->unk3A = 5000;
    collision->unk30 = 0;
    collision->unk32 = 0;
    collision->unk34 = 0;
    cornerPos1 = corner->cornerPos[0];
    cornerPos2 = corner->cornerPos[1];
    cornerPos3 = corner->cornerPos[2];
    switch (corner->surfaceFlags) {
        case 0x80:
            if (is_colliding_with_wall1(collision, boundingBoxSize, cornerPos1, cornerPos2, cornerPos3, corner->surfaceMapIndex, cornerPosX, cornerPosY, cornerPosZ) == 1) {
                temp_f0 = func_802ABE30(cornerPos1, cornerPos2, cornerPos3, corner->surfaceMapIndex);
                if ((!(temp_f0 > player->pos[1])) && !((player->pos[1] - temp_f0) > (2 * boundingBoxSize))) {
                    corner->cornerGroundY = temp_f0;
                    subtract_scaled_vector(collision->unk54, collision->unk3C[1], corner->cornerPos);
                    return 1;
                }
            }
            break;
        case 0x40:
            if (is_colliding_with_drivable_surface(collision, boundingBoxSize, cornerPos1, cornerPos2, cornerPos3, corner->surfaceMapIndex, cornerPosX, cornerPosY, cornerPosZ) == 1) {
                temp_f0 = func_802ABE30(cornerPos1, cornerPos2, cornerPos3, corner->surfaceMapIndex);
                if (!(player->pos[1] < temp_f0) && !((2 * boundingBoxSize) < (player->pos[1] - temp_f0))) {
                    corner->cornerGroundY = temp_f0;
                    subtract_scaled_vector(collision->orientationVector, collision->unk3C[2], corner->cornerPos);
                    return 1;
                }
            }
            break;
        case 0x20:
            if (is_colliding_with_wall2(collision, boundingBoxSize, cornerPos1, cornerPos2, cornerPos3, corner->surfaceMapIndex, cornerPosX, cornerPosY, cornerPosZ) == 1) {
                temp_f0 = func_802ABE30(cornerPos1, cornerPos2, cornerPos3, corner->surfaceMapIndex);
                if (!(player->pos[1] < temp_f0) && !((2 * boundingBoxSize) < (player->pos[1] - temp_f0))) {
                    corner->cornerGroundY = temp_f0;
                    subtract_scaled_vector(collision->unk48, collision->unk3C[0], corner->cornerPos);
                    return 1;
                }
            }
            break;
        case 0:
            break;
    }

    // If the surface flags are not set then try setting them.

    temp_v0_2 = (s32) gCourseMaxX - gCourseMinX;
    temp_v1 = (s32) gCourseMaxZ - gCourseMinZ;

    temp1 = temp_v0_2 / 32;
    temp2 = temp_v1 / 32;

    temp_f10 = (cornerPos1 - gCourseMinX) / temp1;
    temp_f16 = (cornerPos3 - gCourseMinZ) / temp2;


    if (temp_f10 < 0) { return 0; }
    if (temp_f16 < 0) { return 0; }
    if (temp_f10 >= 32) { return 0; }
    if (temp_f16 >= 32) { return 0; }


    temp_v1_2 = temp_f10 + temp_f16 * 32;
    iter = D_8014F110[temp_v1_2].unk2;

    if (iter == 0) { return 0; }

    phi_s2 = D_8014F110[temp_v1_2].unk0;

    for (i = 0; i < iter; i++) {
        surfaceMapIndex = D_8015F584[phi_s2];
        if (gSurfaceMap[surfaceMapIndex].flags & 0x4000) {
            if (surfaceMapIndex != corner->surfaceMapIndex) {
                if (is_colliding_with_drivable_surface(collision, boundingBoxSize, cornerPos1, cornerPos2, cornerPos3, surfaceMapIndex, cornerPosX, cornerPosY, cornerPosZ) == 1) {
                    temp_f0 = func_802ABE30(cornerPos1, cornerPos2, cornerPos3, surfaceMapIndex);

                    if (!(player->pos[1] < temp_f0) && !((2 * boundingBoxSize) < (player->pos[1] - temp_f0))) {
                        subtract_scaled_vector(collision->orientationVector, collision->unk3C[2], corner->cornerPos);
                        corner->cornerGroundY = temp_f0;
                        corner->surfaceType = (u8) gSurfaceMap[surfaceMapIndex].surfaceType;
                        corner->surfaceFlags = 0x40;
                        corner->surfaceMapIndex = surfaceMapIndex;
                        if (gSurfaceMap[surfaceMapIndex].flags & 0x1000) {
                            corner->unk_14 = 1;
                        } else {
                            corner->unk_14 = 0;
                        }
                        return 1;
                    }
                }
            }
        } else if (gSurfaceMap[surfaceMapIndex].flags & 0x8000) {
            if (gSurfaceMap[surfaceMapIndex].gravity != 0.0f) {
                if (surfaceMapIndex != corner->surfaceMapIndex) {
                    if (is_colliding_with_wall1(collision, boundingBoxSize, cornerPos1, cornerPos2, cornerPos3, surfaceMapIndex, cornerPosX, cornerPosY, cornerPosZ) == 1) {
                        temp_f0 = func_802ABE30(cornerPos1, cornerPos2, cornerPos3, surfaceMapIndex);
                        if (!(player->pos[1] < temp_f0) && !((2 * boundingBoxSize) < (player->pos[1] - temp_f0))) {
                            corner->cornerGroundY = temp_f0;
                            subtract_scaled_vector(collision->unk54, collision->unk3C[1], corner->cornerPos);
                            corner->cornerGroundY = func_802ABE30(cornerPos1, cornerPos2, cornerPos3, surfaceMapIndex);
                            corner->surfaceType = (u8) gSurfaceMap[surfaceMapIndex].surfaceType;
                            corner->surfaceFlags = 0x80;
                            corner->surfaceMapIndex = surfaceMapIndex;
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (gSurfaceMap[surfaceMapIndex].gravity != 0.0f) {
                if (surfaceMapIndex != corner->surfaceMapIndex) {
                    if (is_colliding_with_wall2(collision, boundingBoxSize, cornerPos1, cornerPos2, cornerPos3, surfaceMapIndex, cornerPosX, cornerPosY, cornerPosZ) == 1) {
                        temp_f0 = func_802ABE30(cornerPos1, cornerPos2, cornerPos3, surfaceMapIndex);
                        if (!(player->pos[1] < temp_f0) && !((2 * boundingBoxSize) < (player->pos[1] - temp_f0))) {
                            corner->cornerGroundY = temp_f0;
                            subtract_scaled_vector(collision->unk48, collision->unk3C[0], corner->cornerPos);
                            corner->surfaceType = (u8) gSurfaceMap[surfaceMapIndex].surfaceType;
                            corner->surfaceFlags = 0x20;
                            corner->surfaceMapIndex = surfaceMapIndex;
                            return 1;
                        }
                    }
                }
            }
        }
        phi_s2++;
    }
    corner->cornerGroundY = cornerPos2;
    corner->surfaceType = 0;
    return 0;
}
