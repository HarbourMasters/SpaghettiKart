#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <actor_types.h>
#include "code_800029B0.h"
#include "math_util.h"
#include "memory.h"
#include "waypoints.h"
#include "code_80005FD0.h"
#include "menu_items.h"
#include "collision.h"
#include "actors.h"
#include "actors_extended.h"
#include "audio/external.h"
#include "update_objects.h"
#include "effects.h"
#include "sounds.h"
#include "port/Game.h"
#include <cglm/cglm.h>

void copy_collision(Collision* src, Collision* dest) {
    dest->unk30 = src->unk30;
    dest->unk32 = src->unk32;
    dest->unk34 = src->unk34;
    dest->meshIndexYX = src->meshIndexYX;
    dest->meshIndexZY = src->meshIndexZY;
    dest->meshIndexZX = src->meshIndexZX;
    dest->surfaceDistance[0] = src->surfaceDistance[0];
    dest->surfaceDistance[1] = src->surfaceDistance[1];
    dest->surfaceDistance[2] = src->surfaceDistance[2];

    glm_vec3_copy(src->unk48, dest->unk48);
    glm_vec3_copy(src->unk54, dest->unk54);
    glm_vec3_copy(src->orientationVector, dest->orientationVector);
}

void triple_shell_actor_collide_with_player(struct ShellActor* shell, s32 shellType) {
    TripleShellParent* parent = (TripleShellParent*) GET_ACTOR(shell->parentIndex);

    parent->shellsAvailable--;

    switch ((s16) shell->shellId) {
        case 0:
            parent->shellIndices[0] = -1.0f;
            break;
        case 1:
            parent->shellIndices[1] = -1.0f;
            break;
        case 2:
            parent->shellIndices[2] = -1.0f;
            break;
    }

    shell->flags = 0x8000; // bitflag
    shell->rotAngle = 0;
    shell->velocity[1] = 3.0f;
    shell->someTimer = 60;

    switch (shellType) {
        case ACTOR_GREEN_SHELL:
            shell->state = GREEN_SHELL_HIT_A_RACER;
            break;
        case ACTOR_RED_SHELL:
            shell->state = DESTROYED_SHELL;
            break;
    }
}

// Sets velocities for a banana, used when a racer runs into
// a banana bunch.
void func_802B039C(struct BananaActor* banana) {
    banana->state = DROPPED_BANANA;
    banana->unk_04 = 0x00B4;
    banana->velocity[0] = ((f32) (random_int(0x00C8) - 0x64) * 0.015);
    banana->velocity[1] = ((f32) random_int(0x00C8)) * 0.015;
    banana->velocity[2] = ((f32) (random_int(0x00C8) - 0x64) * 0.015);
}

void func_802B0464(s16 bananaIndex) {
    struct BananaActor* banana;

    if (bananaIndex != -1) {
        banana = (struct BananaActor*) GET_ACTOR(bananaIndex);
        func_802B039C(banana);
        func_802B0464(banana->youngerIndex);
    }
}

void func_802B04E8(UNUSED struct BananaActor* arg0, s16 bananaIndex) {
    struct BananaActor* banana;

    if (bananaIndex != -1) {
        banana = (struct BananaActor*) GET_ACTOR(bananaIndex);
        func_802B039C(banana);
        func_802B04E8(banana, banana->elderIndex);
    }
}

// Handle a banana being ran over while it is still part of a banana bunch
void destroy_banana_in_banana_bunch(struct BananaActor* banana) {
    struct BananaBunchParent* temp_v0_2;

    func_802B0464(banana->youngerIndex);
    func_802B04E8(banana, banana->elderIndex);
    if ((gPlayers[banana->playerId].type & 0x4000) != 0) {
        func_800C9060(banana->playerId, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
    }
    banana->flags = -0x8000;
    banana->unk_04 = 0x003C;
    banana->state = DESTROYED_BANANA;
    banana->velocity[1] = 3.0f;
    temp_v0_2 = (struct BananaBunchParent*) GET_ACTOR(banana->parentIndex);
    temp_v0_2->bananaIndices[0] = -1;
    temp_v0_2->bananaIndices[1] = -1;
    temp_v0_2->bananaIndices[2] = -1;
    temp_v0_2->bananaIndices[3] = -1;
    temp_v0_2->bananaIndices[4] = -1;
}

// Drop a banana from a banana bunch?
void func_802B0648(struct BananaBunchParent* banana_bunch) {
    s16 elderIndex;
    struct BananaActor* banana;

    banana_bunch->bananasAvailable -= 1;
    if (banana_bunch->bananaIndices[4] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[4]);
        banana_bunch->bananaIndices[4] = -1;
    } else if (banana_bunch->bananaIndices[3] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[3]);
        banana_bunch->bananaIndices[3] = -1;
    } else if (banana_bunch->bananaIndices[2] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[2]);
        banana_bunch->bananaIndices[2] = -1;
    } else if (banana_bunch->bananaIndices[1] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[1]);
        banana_bunch->bananaIndices[1] = -1;
    } else if (banana_bunch->bananaIndices[0] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[0]);
        banana_bunch->bananaIndices[0] = -1;
    } else {
        return;
    }

    banana->state = DROPPED_BANANA;
    banana->unk_04 = 0x00B4;
    banana->velocity[0] = 0.0f;
    banana->velocity[1] = 1.5f;
    banana->velocity[2] = 0.0f;
    elderIndex = banana->elderIndex;
    if (elderIndex != -1) {
        ((struct BananaActor*) GET_ACTOR(elderIndex))->youngerIndex = -1;
    }
}

// Drop a banana from a banana bunch and throw it forward?
void func_802B0788(s16 rawStickY, struct BananaBunchParent* banana_bunch, Player* player) {
    struct BananaActor* banana;
    struct BananaActor* elderBanana;
    f32 var_f0;
    f32 var_f12;

    banana_bunch->bananasAvailable -= 1;
    if (banana_bunch->bananaIndices[4] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[4]);
        banana_bunch->bananaIndices[4] = -1;
    } else if (banana_bunch->bananaIndices[3] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[3]);
        banana_bunch->bananaIndices[3] = -1;
    } else if (banana_bunch->bananaIndices[2] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[2]);
        banana_bunch->bananaIndices[2] = -1;
    } else if (banana_bunch->bananaIndices[1] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[1]);
        banana_bunch->bananaIndices[1] = -1;
    } else if (banana_bunch->bananaIndices[0] != -1) {
        banana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[0]);
        banana_bunch->bananaIndices[0] = -1;
    } else {
        return;
    }

    banana->state = DROPPED_BANANA;
    banana->unk_04 = 0x001E;
    if (banana->elderIndex != -1) {
        elderBanana = (struct BananaActor*) GET_ACTOR(banana->elderIndex);
        elderBanana->youngerIndex = -1;
    }
    if (player->unk_094 < 2.0f) {
        var_f0 = ((rawStickY - 30.0f) / 20.0f) + 1.5f;
        var_f12 = 4.0f;
    } else {
        var_f0 = ((rawStickY - 30.0f) / 20.0f) + 1.5f;
        var_f12 = (player->unk_094 * 0.75f) + 4.5f + var_f0;
    }
    Vec3f velocity = {0.0f, var_f0, var_f12};
    func_802B64C4(velocity, player->rotation[1] + player->unk_0C0);
    banana->velocity[0] = velocity[0];
    banana->velocity[1] = velocity[1];
    banana->velocity[2] = velocity[2];
}

s32 func_802B09C0(s16 bananaId) {
    struct BananaActor* banana;
    if (bananaId == -1) {
        return 0;
    }
    banana = (struct BananaActor*) GET_ACTOR(bananaId);
    if (banana->state == FIRST_BANANA_BUNCH_BANANA) {
        return 1;
    }
    if (banana->state == BANANA_BUNCH_BANANA) {
        return 1;
    }
    return 0;
}

void update_actor_banana_bunch(struct BananaBunchParent* banana_bunch) {
    UNUSED s32 pad[2];
    Player* owner;
    struct Controller* controller;
    s32 someCount;

    owner = &gPlayers[banana_bunch->playerId];
    switch (banana_bunch->state) {
        case 0:
            func_802B2914(banana_bunch, owner, 0);
            banana_bunch->unk_04 = 4;
            banana_bunch->state = 1;
            banana_bunch->bananasAvailable = 1;
            break;
        case 1:
            banana_bunch->unk_04 -= 1;
            if (banana_bunch->unk_04 == 0) {
                func_802B2914(banana_bunch, owner, 1);
                banana_bunch->unk_04 = 4;
                banana_bunch->state = 2;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 2:
            banana_bunch->unk_04 -= 1;
            if (banana_bunch->unk_04 == 0) {
                func_802B2914(banana_bunch, owner, 2);
                banana_bunch->unk_04 = 4;
                banana_bunch->state = 3;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 3:
            banana_bunch->unk_04 -= 1;
            if (banana_bunch->unk_04 == 0) {
                func_802B2914(banana_bunch, owner, 3);
                banana_bunch->unk_04 = 4;
                banana_bunch->state = 4;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 4:
            banana_bunch->unk_04 -= 1;
            if (banana_bunch->unk_04 == 0) {
                func_802B2914(banana_bunch, owner, 4);
                banana_bunch->unk_04 = 4;
                banana_bunch->state = 5;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 5:
            banana_bunch->state = 6;
            // Unnecessary type-casting done here purely to help with understanding.
            // We're setting the ->flags of BananaActors, not plain Actors.
            ((struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[0]))->flags |= 0x5000;
            ((struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[1]))->flags |= 0x5000;
            ((struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[2]))->flags |= 0x5000;
            ((struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[3]))->flags |= 0x5000;
            ((struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[4]))->flags |= 0x5000;
            break;
        case 6:
            someCount = 0;
            if (func_802B09C0(banana_bunch->bananaIndices[0]) == 1) {
                someCount = 1;
            }
            if (func_802B09C0(banana_bunch->bananaIndices[1]) == 1) {
                someCount += 1;
            }
            if (func_802B09C0(banana_bunch->bananaIndices[2]) == 1) {
                someCount += 1;
            }
            if (func_802B09C0(banana_bunch->bananaIndices[3]) == 1) {
                someCount += 1;
            }
            if (func_802B09C0(banana_bunch->bananaIndices[4]) == 1) {
                someCount += 1;
            }
            if (someCount == 0) {
                destroy_actor((struct Actor*) banana_bunch);
                owner->soundEffects &= ~HOLD_BANANA_SOUND_EFFECT;
            } else if ((owner->type & 0x4000) != 0) {
                controller = &gControllers[banana_bunch->playerId];
                if ((controller->buttonPressed & Z_TRIG) != 0) {
                    controller->buttonPressed &= ~Z_TRIG;
                    func_800C9060(owner - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    if ((controller->rawStickY >= 0x1F) &&
                        ((controller->rawStickX < 0x28) && (controller->rawStickX >= -0x27))) {
                        func_802B0788(controller->rawStickY, banana_bunch, owner);
                    } else {
                        func_802B0648(banana_bunch);
                    }
                }
            }
            break;
        default:
            break;
    }
}

bool is_shell_exist(s16 arg0) {
    struct ShellActor* actor;
    if (arg0 < 0) {
        return false;
    }
    actor = (struct ShellActor*) GET_ACTOR(arg0);
    if (actor->type == ACTOR_GREEN_SHELL) {
        if (actor->state == TRIPLE_GREEN_SHELL) {
            return true;
        }
        return false;
    }
    if (actor->state == TRIPLE_RED_SHELL) {
        return true;
    }
    return false;
}

void update_actor_triple_shell(TripleShellParent* parent, s16 shellType) {
    UNUSED s32 pad[2];
    s16 playerId;
    UNUSED s32 pad2;
    struct ShellActor* shell;
    Vec3f someVelocity;
    UNUSED s32 pad3;
    s16 shellCount;
    u16 someRotAngle;
    Player* player;

    playerId = parent->playerId;
    player = &gPlayers[playerId];
    parent->rotAngle += parent->rotVelocity;
    someRotAngle = parent->rotAngle;
    switch (parent->state) {
        case SPAWN_FIRST_SHELL:
            if (init_triple_shell(parent, &gPlayers[playerId], shellType, 0U) != -1) {
                func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                parent->shellsAvailable += 1;
            }
            parent->state = SPAWN_SECOND_SHELL;
            break;
        case SPAWN_SECOND_SHELL:
            if (parent->rotVelocity > 0) {
                if (someRotAngle >= 0xD556) {
                    if (init_triple_shell(parent, &gPlayers[playerId], shellType, 1U) != -1) {
                        func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->shellsAvailable += 1;
                    }
                    parent->state = SPAWN_THIRD_SHELL;
                }
            } else {
                if (someRotAngle < 0x2AAA) {
                    if (init_triple_shell(parent, &gPlayers[playerId], shellType, 1U) != -1) {
                        func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->shellsAvailable += 1;
                    }
                    parent->state = SPAWN_THIRD_SHELL;
                }
            }
            break;
        case SPAWN_THIRD_SHELL:
            if (parent->rotVelocity > 0) {
                if ((someRotAngle >= 0x2AAB) && (someRotAngle < 0x31C7)) {
                    if (init_triple_shell(parent, &gPlayers[playerId], shellType, 2U) != -1) {
                        func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->shellsAvailable += 1;
                    }
                    parent->state = 3;
                }
            } else if ((someRotAngle < 0xD555) && (someRotAngle >= 0xCE39)) {
                if (init_triple_shell(parent, &gPlayers[playerId], shellType, 2U) != -1) {
                    func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    parent->shellsAvailable += 1;
                }
                parent->state = 3;
            }
            break;
        case 3:
            parent->state = 4;
            shell = (struct ShellActor*) GET_ACTOR((s16) parent->shellIndices[0]);
            shell->flags |= 0x4000;
            shell = (struct ShellActor*) GET_ACTOR((s16) parent->shellIndices[1]);
            shell->flags |= 0x4000;
            shell = (struct ShellActor*) GET_ACTOR((s16) parent->shellIndices[2]);
            shell->flags |= 0x4000;
            break;
        case 4:
            shellCount = 0;
            if (is_shell_exist(parent->shellIndices[0]) == 1) {
                shellCount = 1;
            } else {
                parent->shellIndices[0] = -1.0f;
            }
            if (is_shell_exist(parent->shellIndices[1]) == 1) {
                shellCount++;
            } else {
                parent->shellIndices[1] = -1.0f;
            }
            if (is_shell_exist(parent->shellIndices[2]) == 1) {
                shellCount++;
            } else {
                parent->shellIndices[2] = -1.0f;
            }
            if (shellCount == 0) {
                destroy_actor((struct Actor*) parent);
                break;
            }
            if ((gControllers[parent->playerId].buttonPressed & Z_TRIG) != 0) {
                parent->unk_08 += 1.0f;
                gControllers[parent->playerId].buttonPressed &= ~Z_TRIG;
            }
            if (parent->unk_08 > 0.0f) {
                if (parent->shellIndices[0] > 0.0f) {
                    shell = (struct ShellActor*) GET_ACTOR((s16) parent->shellIndices[0]);
                    if ((shell->rotAngle < 0x38E) || (shell->rotAngle >= -0x38D)) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        func_802B64C4(someVelocity, player->rotation[1] + player->unk_0C0);
                        shell->velocity[0] = someVelocity[0];
                        shell->velocity[1] = someVelocity[1];
                        shell->velocity[2] = someVelocity[2];
                        shell->state = MOVING_SHELL;
                        shell->someTimer = 0x001E;
                        func_800C9060(parent->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        func_800C90F4(parent->playerId,
                                      (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        if (parent->type == ACTOR_TRIPLE_RED_SHELL) {
                            add_red_shell_in_unexpired_actor_list(parent->shellIndices[0]);
                        } else {
                            add_green_shell_in_unexpired_actor_list(parent->shellIndices[0]);
                        }
                        parent->shellIndices[0] = -1.0f;
                        parent->shellsAvailable -= 1;
                        parent->unk_08 -= 1.0f;
                        break;
                    }
                }
                if (parent->shellIndices[1] > 0.0f) {
                    shell = (struct ShellActor*) GET_ACTOR((s16) parent->shellIndices[1]);
                    if ((shell->rotAngle < 0xAA1) || (shell->rotAngle >= 0x38F)) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        func_802B64C4(someVelocity, player->rotation[1] + player->unk_0C0);
                        shell->velocity[0] = someVelocity[0];
                        shell->velocity[1] = someVelocity[1];
                        shell->velocity[2] = someVelocity[2];
                        shell->state = MOVING_SHELL;
                        shell->someTimer = 0x001E;
                        func_800C90F4(parent->playerId,
                                      (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        func_800C9060(parent->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        if (parent->type == ACTOR_TRIPLE_RED_SHELL) {
                            add_red_shell_in_unexpired_actor_list(parent->shellIndices[1]);
                        } else {
                            add_green_shell_in_unexpired_actor_list(parent->shellIndices[1]);
                        }
                        parent->shellIndices[1] = -1.0f;
                        parent->shellsAvailable -= 1;
                        parent->unk_08 -= 1.0f;
                        break;
                    }
                }
                if (parent->shellIndices[2] > 0.0f) {
                    shell = (struct ShellActor*) GET_ACTOR((s16) parent->shellIndices[2]);
                    if ((shell->rotAngle < -0x38E) || (shell->rotAngle >= -0x71B)) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        func_802B64C4(someVelocity, player->rotation[1] + player->unk_0C0);
                        shell->velocity[0] = someVelocity[0];
                        shell->velocity[1] = someVelocity[1];
                        shell->velocity[2] = someVelocity[2];
                        shell->state = MOVING_SHELL;
                        shell->someTimer = 0x001E;
                        func_800C9060(parent->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        func_800C90F4(parent->playerId,
                                      (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        if (parent->type == ACTOR_TRIPLE_RED_SHELL) {
                            add_red_shell_in_unexpired_actor_list(parent->shellIndices[2]);
                        } else {
                            add_green_shell_in_unexpired_actor_list(parent->shellIndices[2]);
                        }
                        parent->shellIndices[2] = -1.0f;
                        parent->shellsAvailable -= 1;
                        parent->unk_08 -= 1.0f;
                        break;
                    }
                }
            }
            break;
        default:
            break;
    }
}

s32 use_banana_bunch_item(Player* player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos = { 0.0f, 0.0f, 0.0f };
    s16 actorIndex;
    struct BananaBunchParent* bananaBunch;

    // this have a animation of spawning so see update_actor_banana_bunch
    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_BANANA_BUNCH);
    if (actorIndex < 0) {
        return actorIndex;
    }
    bananaBunch = (struct BananaBunchParent*) GET_ACTOR(actorIndex);
    bananaBunch->state = 0;
    bananaBunch->playerId = player - gPlayerOne;
    player->soundEffects |= HOLD_BANANA_SOUND_EFFECT;
    return actorIndex;
}

s32 use_triple_shell_item(Player* player, s16 tripleShellType) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos = { 0.0f, 0.0f, 0.0f };
    s16 actorIndex;
    TripleShellParent* parent;

    // this have a animation of spawning so see update_actor_triple_shell
    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, tripleShellType);
    if (actorIndex < 0) {
        return actorIndex;
    }
    parent = (TripleShellParent*) GET_ACTOR(actorIndex);
    parent->state = 0;
    parent->rotVelocity = 0x05B0;
    parent->rotAngle = -0x8000;
    parent->playerId = player - gPlayerOne;
    parent->shellsAvailable = 0;
    parent->unk_08 = 0.0f;
    return actorIndex;
}

s32 init_triple_shell(TripleShellParent* parent, Player* player, s16 shellType, u16 shellId) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 actorIndex;
    struct ShellActor* shell;

    startingPos[0] = 0.0f;
    startingPos[1] = -player->boundingBoxSize;
    startingPos[2] = player->boundingBoxSize - 4.0f;
    mtxf_translate_vec3f_mat3(startingPos, player->orientationMatrix);
    startingPos[0] += player->pos[0];
    startingPos[1] += player->pos[1];
    startingPos[2] += player->pos[2];

    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, shellType);
    if (actorIndex < 0) {
        parent->shellIndices[shellId] = -1.0f;
        return -1;
    }

    shell = (struct ShellActor*) GET_ACTOR(actorIndex);
    startingPos[0] = player->pos[0];
    startingPos[1] = player->pos[1];
    startingPos[2] = player->pos[2];
    actor_terrain_collision(&shell->unk30, shell->boundingBoxSize + 1.0f, shell->pos[0], shell->pos[1], shell->pos[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    func_802B4E30((struct Actor*) shell);
    shell->flags = 0x9000;
    switch (shellType) {
        case ACTOR_GREEN_SHELL:
            shell->state = TRIPLE_GREEN_SHELL;
            break;
        case ACTOR_RED_SHELL:
            shell->state = TRIPLE_RED_SHELL;
            break;
    }
    shell->rotVelocity = 0;
    shell->rotAngle = -0x8000;
    shell->playerId = player - gPlayerOne;
    shell->parentIndex = CM_FindActorIndex(parent);
    shell->shellId = shellId;
    parent->shellIndices[shellId] = (f32) CM_FindActorIndex(shell);
    return 1;
}

s32 use_green_shell_item(Player* player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 actorIndex;
    struct ShellActor* shell;

    // place behind player
    startingPos[0] = 0.0f;
    startingPos[1] = -player->boundingBoxSize;
    startingPos[2] = player->boundingBoxSize - 4.0f;

    // rotate to match player orientation
    mtxf_translate_vec3f_mat3(startingPos, player->orientationMatrix);

    // move to player position
    startingPos[0] += player->pos[0];
    startingPos[1] += player->pos[1];
    startingPos[2] += player->pos[2];

    // spawn the shell
    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_GREEN_SHELL);
    if (actorIndex < 0) {
        return actorIndex;
    }

    shell = (struct ShellActor*) GET_ACTOR(actorIndex);
    startingPos[0] = player->pos[0];
    startingPos[1] = player->pos[1];
    startingPos[2] = player->pos[2];
    actor_terrain_collision(&shell->unk30, shell->boundingBoxSize + 1.0f, shell->pos[0], shell->pos[1], shell->pos[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    func_802B4E30((struct Actor*) shell);
    shell->state = HELD_SHELL;
    shell->rotVelocity = 0;
    shell->rotAngle = -0x8000;
    shell->playerId = player - gPlayerOne;
    return actorIndex;
}

s32 use_red_shell_item(Player* player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 actorIndex;
    struct ShellActor* shell;

    // place behind player
    startingPos[0] = 0.0f;
    startingPos[1] = -player->boundingBoxSize;
    startingPos[2] = player->boundingBoxSize - 4.0f;

    // rotate to match player orientation
    mtxf_translate_vec3f_mat3(startingPos, player->orientationMatrix);

    // move to player position
    startingPos[0] += player->pos[0];
    startingPos[1] += player->pos[1];
    startingPos[2] += player->pos[2];

    // spawn the shell
    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_RED_SHELL);
    if (actorIndex < 0) {
        return actorIndex;
    }

    shell = (struct ShellActor*) GET_ACTOR(actorIndex);
    startingPos[0] = player->pos[0];
    startingPos[1] = player->pos[1];
    startingPos[2] = player->pos[2];
    actor_terrain_collision(&shell->unk30, shell->boundingBoxSize + 1.0f, shell->pos[0], shell->pos[1], shell->pos[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    func_802B4E30((struct Actor*) shell);
    shell->state = HELD_SHELL;
    shell->rotVelocity = 0;
    shell->rotAngle = player->rotation[1] - 0x8000;
    shell->playerId = player - gPlayerOne;
    return actorIndex;
}

// Interestingly blue shells start their life as a red shell,
// and then just change the type from red to blue shell
void use_blue_shell_item(Player* player) {
    GET_ACTOR(use_red_shell_item(player))->type = ACTOR_BLUE_SPINY_SHELL;
}

#include "actors/banana/update.inc.c"

// This function could reasonably be called "spawn_bananas_for_banana_bunch" or similar
void func_802B2914(struct BananaBunchParent* banana_bunch, Player* player, s16 bananaId) {
    s16 actorIndex;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;
    UNUSED s32 pad;
    UNUSED s32 pad2;
    struct BananaActor* newBanana;
    struct BananaActor* tempBanana;

    startingPos[0] = 0.0f;
    startingPos[1] = -player->boundingBoxSize;
    startingPos[2] = -(player->boundingBoxSize + 4.0f);
    mtxf_translate_vec3f_mat3(startingPos, player->orientationMatrix);
    startingPos[0] += player->pos[0];
    startingPos[1] += player->pos[1];
    startingPos[2] += player->pos[2];
    startingVelocity[0] = player->velocity[0];
    startingVelocity[1] = player->velocity[1];
    startingVelocity[2] = player->velocity[2];
    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;
    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_BANANA);
    if (actorIndex >= 0) {
        newBanana = (struct BananaActor*) GET_ACTOR(actorIndex);
        startingPos[0] = player->pos[0];
        startingPos[1] = player->pos[1];
        startingPos[2] = player->pos[2];
        actor_terrain_collision(&newBanana->unk30, newBanana->boundingBoxSize + 1.0f, newBanana->pos[0],
                                newBanana->pos[1], newBanana->pos[2], startingPos[0], startingPos[1], startingPos[2]);
        func_802B4E30((struct Actor*) newBanana);
        newBanana->flags = 0x9000;
        newBanana->playerId = player - gPlayerOne;
        newBanana->parentIndex = (struct Actor*) CM_FindActorIndex(banana_bunch);
        newBanana->youngerIndex = -1;
        newBanana->unk_04 = 0x0014;
        newBanana->bananaId = bananaId;
        switch (bananaId) {
            case 0:
                newBanana->state = 2;
                banana_bunch->bananaIndices[0] = actorIndex;
                newBanana->elderIndex = -1;
                break;
            case 1:
                newBanana->state = 3;
                banana_bunch->bananaIndices[1] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[0];
                tempBanana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[0]);
                tempBanana->youngerIndex = actorIndex;
                break;
            case 2:
                newBanana->state = 3;
                banana_bunch->bananaIndices[2] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[1];
                tempBanana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[1]);
                tempBanana->youngerIndex = actorIndex;
                break;
            case 3:
                newBanana->state = 3;
                banana_bunch->bananaIndices[3] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[2];
                tempBanana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[2]);
                tempBanana->youngerIndex = actorIndex;
                break;
            case 4:
                newBanana->state = 3;
                banana_bunch->bananaIndices[4] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[3];
                tempBanana = (struct BananaActor*) GET_ACTOR(banana_bunch->bananaIndices[3]);
                tempBanana->youngerIndex = actorIndex;
                break;
        }
        if ((player->type & PLAYER_HUMAN) != 0) {
            func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
        }
    }
}

s32 use_fake_itembox_item(Player* player) {
    struct FakeItemBox* itemBox;
    UNUSED s32 pad[5];
    s16 actorIndex;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;

    // place behind player
    startingPos[0] = 0.0f;
    startingPos[1] = -player->boundingBoxSize;
    startingPos[2] = -(player->boundingBoxSize + 4.0f);

    // rotate to match player orientation
    mtxf_translate_vec3f_mat3(startingPos, player->orientationMatrix);

    // move to player position
    startingPos[0] += player->pos[0];
    startingPos[1] += player->pos[1];
    startingPos[2] += player->pos[2];

    // set the itembox's velocity to the player's velocity
    startingVelocity[0] = player->velocity[0];
    startingVelocity[1] = player->velocity[1];
    startingVelocity[2] = player->velocity[2];

    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;

    // spawn the itembox
    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_FAKE_ITEM_BOX);
    if (actorIndex < 0) {
        return actorIndex;
    }
    itemBox = (struct FakeItemBox*) GET_ACTOR(actorIndex);
    itemBox->playerId = (player - gPlayerOne);
    itemBox->state = HELD_FAKE_ITEM_BOX;
    player->soundEffects |= HOLD_BANANA_SOUND_EFFECT;
    return actorIndex;
}

s32 use_banana_item(Player* player) {
    UNUSED s32 pad[6];
    u16 playerId;
    s16 actorIndex;
    struct BananaActor* banana;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;

    playerId = player - gPlayerOne;
    if (playerId >= 8) {
        return -1;
    }
    // Extremely weird fake match to fix a tiny stack difference
    // place the banana in back of the player
    startingPos[0, 0] = 0.0f;
    startingPos[1] = -player->boundingBoxSize;
    startingPos[2] = -(player->boundingBoxSize + 4.0f);

    // apply the player's orientation to the banana
    mtxf_translate_vec3f_mat3(startingPos, player->orientationMatrix);

    // add the player's position to the banana's position
    startingPos[0] += player->pos[0];
    startingPos[1] += player->pos[1];
    startingPos[2] += player->pos[2];

    // set the banana's velocity to the player's velocity
    startingVelocity[0] = player->velocity[0];
    startingVelocity[1] = player->velocity[1];
    startingVelocity[2] = player->velocity[2];
    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;

    actorIndex = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_BANANA); // spawn banana
    if (actorIndex < 0) {
        return actorIndex;
    }
    banana = (struct BananaActor*) GET_ACTOR(actorIndex);
    banana->playerId = playerId;
    banana->state = HELD_BANANA;
    banana->unk_04 = 0x0014;
    player->soundEffects |= HOLD_BANANA_SOUND_EFFECT;
    return actorIndex;
}

/**
 * Strikes players with thunder
 *
 * @param Activating player (not to be struck)
 */
void use_thunder_item(Player* player) {
    s32 index;
    Player* otherPlayer;

    func_8009E5BC();
    if ((player->type & PLAYER_HUMAN) != 0) {
        // Play sound.
        func_800CAB4C(player - gPlayerOne);
    }

    for (index = 0; index < NUM_PLAYERS; index++) {
        otherPlayer = &gPlayers[index];
        if (player != otherPlayer) {
            otherPlayer->soundEffects |= HIT_ROTATING_SOUND_EFFECT;
        }
    }
}

// Handles item use?
void player_use_item(Player* player) {
    s32 playerId = player - gPlayerOne;

    switch (player->currentItemCopy) {
        case ITEM_GREEN_SHELL:
            use_green_shell_item(player);
            break;
        case ITEM_RED_SHELL:
            use_red_shell_item(player);
            break;
        case ITEM_BLUE_SPINY_SHELL:
            use_blue_shell_item(player);
            break;
        case ITEM_BANANA:
            use_banana_item(player);
            break;
        case ITEM_BANANA_BUNCH:
            use_banana_bunch_item(player);
            break;
        case ITEM_MUSHROOM:
            player->soundEffects |= BOOST_SOUND_EFFECT;
            break;
        case ITEM_DOUBLE_MUSHROOM:
            player->soundEffects |= BOOST_SOUND_EFFECT;
            break;
        case ITEM_TRIPLE_MUSHROOM:
            player->soundEffects |= BOOST_SOUND_EFFECT;
            break;
        case ITEM_SUPER_MUSHROOM:
            player->soundEffects |= BOOST_SOUND_EFFECT;
            break;
        case ITEM_BOO:
            player->soundEffects |= BOO_SOUND_EFFECT;
            break;
        case ITEM_STAR:
            player->soundEffects |= STAR_SOUND_EFFECT;
            break;
        case ITEM_THUNDERBOLT:
            use_thunder_item(player);
            break;
        case ITEM_FAKE_ITEM_BOX:
            use_fake_itembox_item(player);
            break;
        case ITEM_TRIPLE_GREEN_SHELL:
            use_triple_shell_item(player, ACTOR_TRIPLE_GREEN_SHELL);
            break;
        case ITEM_TRIPLE_RED_SHELL:
            use_triple_shell_item(player, ACTOR_TRIPLE_RED_SHELL);
            break;
    }
    consume_item(playerId);
}

// Check if a player is using an item?
void check_player_use_item(void) {
    Player* player;
    struct Controller* target;
    struct Controller* controller;
    struct Controller* loopController;

    for (player = &gPlayers[0], loopController = &gControllers[0], target = &gControllers[4]; loopController != target;
         player++, loopController++) {
        controller = loopController;
        if (prevent_item_use(player) == false) {
            if ((player->type & PLAYER_INVISIBLE_OR_BOMB) != 0) {
                if ((player - gPlayerTwo) == 0) {
                    controller = gControllerSix;
                } else if ((player - gPlayerThree) == 0) {
                    controller = gControllerSeven;
                } else {
                    if ((player - gPlayerOne) == 0) {
                        controller = gControllerEight;
                    }
                }
            }

            if (((player->type & PLAYER_HUMAN) != 0) && (player->currentItemCopy != ITEM_NONE) &&
                ((player->type & PLAYER_START_SEQUENCE) == 0)) {
                if ((controller->buttonPressed & Z_TRIG) != 0) {
                    controller->buttonPressed &= ~Z_TRIG;
                    player_use_item(player);
                }
            }
        }
    }
}

#include "actors/green_shell/update.inc.c"

#include "actors/blue_and_red_shells/update.inc.c"

void func_802B4E30(struct Actor* arg0) {
    if ((arg0->unk30.surfaceDistance[2] < 0.0f) && (arg0->unk30.unk34 == 1)) {
        arg0->pos[0] -= (arg0->unk30.orientationVector[0] * arg0->unk30.surfaceDistance[2]);
        arg0->pos[1] -= (arg0->unk30.orientationVector[1] * arg0->unk30.surfaceDistance[2]);
        arg0->pos[2] -= (arg0->unk30.orientationVector[2] * arg0->unk30.surfaceDistance[2]);
    }
    if ((arg0->unk30.surfaceDistance[0] < 0.0f) && (arg0->unk30.unk30 == 1)) {
        arg0->pos[0] -= (arg0->unk30.unk48[0] * arg0->unk30.surfaceDistance[0]);
        arg0->pos[1] -= (arg0->unk30.unk48[1] * arg0->unk30.surfaceDistance[0]);
        arg0->pos[2] -= (arg0->unk30.unk48[2] * arg0->unk30.surfaceDistance[0]);
    }
    if ((arg0->unk30.surfaceDistance[1] < 0.0f) && (arg0->unk30.unk32 == 1)) {
        arg0->pos[0] -= (arg0->unk30.unk54[0] * arg0->unk30.surfaceDistance[1]);
        arg0->pos[1] -= (arg0->unk30.unk54[1] * arg0->unk30.surfaceDistance[1]);
        arg0->pos[2] -= (arg0->unk30.unk54[2] * arg0->unk30.surfaceDistance[1]);
    }
}
