#include <libultraship.h>
#include <macros.h>
#include <mk64.h>
#include <debug.h>
#include <stubs.h>
#include <common_structs.h>
#include "../camera.h"
#include "main.h"
#include "memory.h"
#include <assets/models/common_data.h>
#include "render_player.h"
#include "render_objects.h"
#include "podium_ceremony_actors.h"
#include "camera_junk.h"
#include "player_controller.h"
#include "code_80057C60.h"
#include "code_80005FD0.h"
#include "code_80281C40.h"
#include "math_util.h"
#include <string.h>
#include "port/interpolation/FrameInterpolation.h"

#include "src/port/Game.h"
#include "engine/Matrix.h"

s32 fireworkConeColour[] = {
    0x00FF4080, // pink
    0x008040FF, // purple
    0x0040FF40, // green
    0x00FFFF40  // yellow
};

/**
 * Mainly colours the explosion effect.
 * Appears to colour the cones as well.
 */
s32 fireworkColour[] = {
    0x007F2040, // red
    0x0040207F, // purple
    0x00207F20, // green
    0x007F7F20  // gold/yellow
};

u16 sRandomSeed16 = 0;

ActorInitParams initBalloon = {
    Balloon, { 0xF3AF, 34, 0xFE2D }, 0, 0, 0, 0,
};

ActorInitParams initBurst = {
    FireworkBurst, { 0xF3AF, 34, 0xFE2D }, 0, 0, 0, 0,
};

ActorInitParams initCone = {
    FireworkCone, { 0xF3AF, 34, 0xFE2D }, 0, 0, 0, 0,
};

// This appears to be a dummy actor placed in index 0 of the actor list.
ActorInitParams initDummy = {
    Initial, { 0xF2CC, 250, 0xFE11 }, 0, 0, 0, 0,
};

// This array appears to always be zero.
// This may not be an array, it appears to contain uninitialized random values to seed the random generator.
// The first index used; D_802874BE (7), requires to be an array.
// Based on that, the whole thing is now an array.
s16 D_802874B0[17];

Gfx* D_802874D4; // ptr to gDisplayListHead

struct_D_802874D8 D_802874D8; // 31 bytes

// s32 sActorTimer;

// u8 D_802874F4;
// u8 D_802874F5;
// u8 D_802874F6;
CeremonyActor* sPodiumActorList;
s32 D_802874FC;

void func_80280650(void) {
}

void set_initial_position(CeremonyActor* actor) {
    ActorInitParams* params = actor->initParams;

    actor->pos[0] = params->unk2[0];
    actor->pos[1] = params->unk2[1];
    actor->pos[2] = params->unk2[2];

    // Place value in the high bits of s16.
    // Example: 85, 0b01010101 -> 0b0101010100000000
    actor->unkA = params->unk8 << 8;
    actor->unkC = params->unk9 << 8;
    actor->unkE = params->unkA << 8;
    actor->type = params->type;
}

CeremonyActor* find_available_entry(void) {
    UNUSED s32 pad[2];
    CeremonyActor* actor = sPodiumActorList;
    s32 i;

    for (i = 0; i < 200; i++) {

        // Find an inactive actor.
        if ((actor->isActive & 1) == 0) {
            bzero(actor, sizeof(CeremonyActor));
            actor->isActive = 1;
            actor->unk24 = 1.0f;
            return actor;
        }

        actor++;
    }
    return NULL;
}

CeremonyActor* new_actor(ActorInitParams* arg0) {
    CeremonyActor* actor = find_available_entry();

//! @bug No null check.
//! @todo More indepth error checking/return value
#ifdef AVOID_UB
    if (actor == NULL) {
        return (CeremonyActor*) &sPodiumActorList[0]; // Return first actor to prevent crash
    }
#endif

    actor->initParams = arg0;
    set_initial_position(actor);
    return actor;
}

void clear_D_802874D8_actors() {
    memset(&D_802874D8, 0, sizeof(struct_D_802874D8));
}

u16 random_u16_credits(void) {
    u16 temp1, temp2;

    if (sRandomSeed16 == 22026) {
        sRandomSeed16 = 0;
    }

    temp1 = (sRandomSeed16 & 0x00FF) << 8;
    temp1 = temp1 ^ sRandomSeed16;

    sRandomSeed16 = ((temp1 & 0x00FF) << 8) + ((temp1 & 0xFF00) >> 8);

    temp1 = ((temp1 & 0x00FF) << 1) ^ sRandomSeed16;
    temp2 = (temp1 >> 1) ^ 0xFF80;

    if ((temp1 & 1) == 0) {
        if (temp2 == 43605) {
            sRandomSeed16 = 0;
        } else {
            sRandomSeed16 = temp2 ^ 0x1FF4;
        }
    } else {
        sRandomSeed16 = temp2 ^ 0x8180;
    }

    return sRandomSeed16;
}

f32 random_float_between_0_and_1(void) {
    return random_u16_credits() / 65536.0f;
}

f32 random_who_knows(f32 arg0) {
    return (random_float_between_0_and_1() * arg0) - (arg0 * 0.5f);
}

void func_80280884(void) {
}

// extern s16 D_802874CA;

void balloon_update(CeremonyActor* actor) {
    render_balloon(actor->pos, 1.0f, actor->unk2E, actor->unk2C);
    actor->pos[1] += 0.8f;
    actor->unk2E = sins(actor->unk30) * actor->unk34;
    actor->unk30 += actor->unk32;
    actor->timer++;

    // Delete actor
    if (actor->timer > 800) {
        actor->isActive = 0;
    }
    if (D_802874B0[13] == 1) {
        actor->isActive = 0;
    }
}

/**
 * Spawns an aerial shell that ascends into the sky.
 * (note that the explode effect is a different actor)
 */
void firework_cone_update_and_spawn_burst(Firework* cone) {
    if (cone->unk44 < 30) {
        cone->pos[1] += 2.5f;
        cone->pos[0] += random_who_knows(0.2f);
        cone->pos[2] += random_who_knows(0.2f);
    } else if (cone->unk2C == 4) {
        Firework* burst = (Firework*) new_actor(&initCone);
        burst->pos[0] = cone->pos[0];
        burst->pos[1] = cone->pos[1];
        burst->pos[2] = cone->pos[2];
        burst->unk30 = fireworkConeColour[cone->unk48];
        burst->unk3C = 0xFF;
        burst->unk40 = -0x11;
        burst->unk44 = 0x64;
        burst->unk34 = 1.8700001f;
        burst->unk38 = 1.8700001f;
    }
}

Mat4 D_80287500;

void func_80280A28(Vec3f arg0, Vec3s arg1, f32 arg2) {
    Mat4 mtx;

    mtxf_translate_rotate(mtx, arg0, arg1);
    mtx[0][0] = D_80287500[0][0] * arg2;
    mtx[0][1] = D_80287500[1][0] * arg2;
    mtx[0][2] = D_80287500[2][0] * arg2;
    mtx[1][0] = D_80287500[0][1] * arg2;
    mtx[1][1] = D_80287500[1][1] * arg2;
    mtx[1][2] = D_80287500[2][1] * arg2;
    mtx[2][0] = D_80287500[0][2] * arg2;
    mtx[2][1] = D_80287500[1][2] * arg2;
    mtx[2][2] = D_80287500[2][2] * arg2;
    // convert_to_fixed_point_matrix(&gGfxPool->mtxEffect[gMatrixEffectCount], mtx);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    AddEffectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void render_fireworks(Vec3f arg0, f32 arg1, s32 rgb, s16 alpha) {
    Vec3f sp4C;
    Vec3s sp44;
    s16 red;
    s16 green;
    s16 blue;

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = camera1->rot[1];
    sp44[2] = 0;
    func_80280A28(sp4C, sp44, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    // ???????????????????????????????????
    red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????
    func_8004B35C(red, green, blue, alpha);
    gSPDisplayList(gDisplayListHead++, D_0D008E48);
    gMatrixEffectCount += 1;
}

void firework_update(Firework* actor) {
    s32 i;
    Vec3f pos;
    
    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("render_fireworks", (uintptr_t) actor);
    if (actor->unk44 < 30) {
        for (i = 0; i < 10; i++) {
            pos[0] = actor->pos[0];
            pos[1] = actor->pos[1] - i * 2;
            pos[2] = actor->pos[2];
            render_fireworks(pos, ((10 - i) / 10.0f) * 2, fireworkColour[actor->unk48],
                             (((((30 - actor->unk44) * 100)) / 30.0f)));
        }

    } else {
        if (actor->unk2C < 5) {
            actor->unk3C += actor->unk40 * 2;
            actor->unk34 += actor->unk38 * 2;
        } else {
            actor->unk3C += actor->unk40 / (1.0f + (((actor->unk2C * 7) - 0x23) / 10.0f));
            actor->unk34 += actor->unk38 / (1.0f + (((actor->unk2C * 7) - 0x23) / 10.0f));
            if (actor->unk3C < 0) {
                actor->unk3C = 0;
            }
        }
        actor->unk2C += 1;
        if (((actor->unk3C > 0) && ((actor->unk34 > 0.0f))) || (actor->unk2C < 30)) {
            render_fireworks(actor->pos, actor->unk34, actor->unk30, (s16) actor->unk3C);
        } else {
            actor->isActive = 0;
        }
    }
    actor->unk44 += 1;
    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();  
}

void unused_80280FA0(UNUSED CeremonyActor* actor) {
}

void unused_80280FA8(UNUSED CeremonyActor* actor) {
}

void balloons_and_fireworks_init(void) {
    D_802874D8.actorTimer = 0;
    sPodiumActorList = (CeremonyActor*) get_next_available_memory_addr(sizeof(CeremonyActor) * 200);
    bzero(sPodiumActorList, (sizeof(CeremonyActor) * 200));
    new_actor(&initDummy);
}

void func_80280FFC(void) {
    D_802874D8.unk1C = 1;
}

void func_8028100C(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2) {
}

/**
 * Spawn balloons!
 * The floats passed to random_who_knows appears to effect the spread of the balloons
 */
void spawn_balloons(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;

    for (i = 0; i < 100; i++) {
        CeremonyActor* balloon;
        balloon = new_actor(&initBalloon);
        balloon->pos[0] = random_who_knows(200.0f) + arg0;
        balloon->pos[1] = random_who_knows(380.0f) + arg1;
        balloon->pos[2] = random_who_knows(600.0f) + arg2;
        balloon->unk2C = (s16) (s32) (random_float_between_0_and_1() * 7.0f);
        balloon->unk30 = random_u16_credits();
        balloon->unk32 = (s16) (s32) (random_who_knows(400.0f) + 900.0f);
        balloon->unk34 = (s16) (s32) (random_who_knows(2000.0f) + 3000.0f);
    }
}

// extern s16 D_802874BE[3];
// extern s16 D_802874C6;
// extern s16 D_802874C8;
extern s32 fireworkColour[];

/**
 * The explosive effect when the firework's charge detonates
 * resulting in a small variety of amusing colours.
 * D_802874BE appears to be zero.
 */
void spawn_firework_cone(s32 arg0, s32 arg1, s32 arg2) {
    f32 num;
    static u32 D_80287540;

    if (((f32) random_float_between_0_and_1() * (D_802874B0[7] + 0xD)) < 1.0f) {
        Firework* cone;
        cone = (Firework*) new_actor(&initBurst);
        cone->pos[0] = random_who_knows(0.0f) + arg0;
        cone->pos[1] = random_who_knows((f32) (D_802874B0[11] + 100)) + (f32) arg1;
        cone->pos[2] = random_who_knows((f32) (D_802874B0[12] + 700)) + (f32) arg2;

        num = 1.1f;

        // Wrap the counter from zero to three
        cone->unk48 = D_80287540 % 4U;
        cone->unk30 = fireworkColour[D_80287540 % 4U];
        cone->unk34 = num;
        cone->unk38 = num;
        D_80287540 += 1;
        cone->unk3C = 0xFF;
        cone->unk40 = -0x11;
    }
}

// extern s16 D_802874C4;
// extern s32 sActorTimer;
extern Mat4 D_80287500;

/**
 * Allegedly controls fireworks movement.
 */
void spawn_timer(void) {
    Camera* camera = &cameras[0];
    f32 lookAtY;

    guLookAtF(D_80287500, camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0], camera->lookAt[1],
              camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_802874D8.unk1D < 3) {
        if (D_802874D8.actorTimer < 300) {
            lookAtY = camera->lookAt[1];
            spawn_firework_cone(-0xE0E, (s32) (((lookAtY - camera->pos[1]) * 1.5f) + lookAtY), -0x258);
        }
        if (D_802874D8.actorTimer == 120) {
            spawn_balloons(-0xC6C, (s32) ((f32) D_802874B0[10] + 210.0f), -0x1EF);
        }
    } else if (D_802874D8.actorTimer == 2) {
        spawn_balloons(-0xC6C, (s32) ((f32) D_802874B0[10] + 210.0f), -0x1EF);
    }

    D_802874D8.actorTimer += 1;
}

void* sUpdate[][3] = {
    // Dummy actor
    { unused_80280FA8, unused_80280FA0, 0 },
    // Balloon
    { unused_80280FA8, balloon_update, 0 },
    // Firework Cone
    { unused_80280FA8, firework_update, 0 },
    // Firework Burst
    { firework_cone_update_and_spawn_burst, 0, firework_update },
};

// extern s16 D_802874D0;

void update_actors_loop(void) {
    void (*func)(void*);
    s32 i;
    s32 j;
    spawn_timer();
    D_802874B0[16] = 0;

    // Why do we loop over the actor list three times?
    // Actors don't with no loop. fireworks don't spawn if the iterator is 2.
    for (i = 0; i < 3; i++) {
        CeremonyActor* actor = sPodiumActorList;
        for (j = 0; j < 200; j++) {
            if (actor->isActive & 1) {

                func = sUpdate[actor->type][i];

                if (func != 0) {
                    func(actor);
                }
            }
            actor++;
        }
    }
}

void func_8028150C(void) {
    D_802874D4 = gDisplayListHead;
}

void func_80281520(void) {
}

void func_80281528(void) {
}

void func_80281530(void) {
}

void func_80281538(void) {
}

void func_80281540(void) {
}

void podium_ceremony_loop(void) {
    ClearMatrixPools();
    Editor_ClearMatrix();
    CM_TickEditor();
    gMatrixObjectCount = 0;
    D_802874FC = 0;
    update_camera_podium_ceremony();
    func_80028F70();
    func_80022744();
    func_80059AC8();
    func_80059AC8();
    func_8005A070();
    if (D_802874D8.unk1C != 0) {
        func_8001C14C();
        update_vehicles();
    }
    func_80281D00();
    func_80281540();
#if DVDL
    display_dvdl();
#endif
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
}
