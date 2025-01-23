#include <actors.h>
#include <main.h>
#include <code_800029B0.h>
#include <defines.h>
#include "collision.h"
#include "port/Game.h"

/**
 * @brief Updates the green shell actor.
 *
 * @param shell
 */
void update_actor_green_shell(struct ShellActor* shell) {
    Player* player;
    UNUSED f32 pad9;
    UNUSED f32 padA;
    Vec3f somePos2;
    Vec3f somePosVel;
    f32 var_f2;
    struct Controller* controller;
    TripleShellParent* parent;
    f32 height;
    f32 z;
    UNUSED f32 pad2;
    UNUSED f32 pad3;
    UNUSED f32 pad4;
    UNUSED f32 pad5;
    f32 y;
    UNUSED f32 pad7;
    UNUSED f32 pad8;

    height = shell->pos[0];
    y = shell->pos[1];
    z = shell->pos[2];
    if ((z < gCourseMinZ) || (gCourseMaxZ < z) || (height < gCourseMinX) || (gCourseMaxX < height) ||
        (y < gCourseMinY)) {
        destroy_destructable_actor((struct Actor*) shell);
    }
    shell->rotVelocity += 0x71C;
    switch (shell->state) {
        case HELD_SHELL:
            player = &gPlayers[shell->playerId];
            copy_collision(&player->collision, &shell->unk30);
            somePosVel[0] = 0.0f;
            somePosVel[1] = player->boundingBoxSize;
            somePosVel[2] = -(player->boundingBoxSize + shell->boundingBoxSize + 2.0f);
            mtxf_translate_vec3f_mat3(somePosVel, player->orientationMatrix);
            shell->pos[0] = player->pos[0] + somePosVel[0];
            pad2 = player->pos[1] - somePosVel[1];
            shell->pos[2] = player->pos[2] + somePosVel[2];
            height = calculate_surface_height(shell->pos[0], pad2, shell->pos[2], player->collision.meshIndexZX);
            z = pad2 - height;
            if ((z < 5.0f) && (z > -5.0f)) {
                shell->pos[1] = shell->boundingBoxSize + height;
            } else {
                shell->pos[1] = pad2;
            }
            if ((player->type & PLAYER_HUMAN) != 0) {
                controller = &gControllers[shell->playerId];
                if ((controller->buttonDepressed & Z_TRIG) != 0) {
                    controller->buttonDepressed &= ~Z_TRIG;
                    if (controller->rawStickY < -0x2D) {
                        var_f2 = 8.0f;
                        if (player->unk_094 > 8.0f) {
                            var_f2 = player->unk_094 * 1.2f;
                        }
                        somePosVel[0] = 0.0f;
                        somePosVel[1] = 0.0f;
                        somePosVel[2] = -var_f2;
                        func_802B64C4(somePosVel, player->rotation[1] + player->unk_0C0);
                        shell->velocity[0] = somePosVel[0];
                        shell->velocity[1] = somePosVel[1];
                        shell->velocity[2] = somePosVel[2];
                        shell->state = 2;
                        func_800C9060(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        func_800C90F4(shell->playerId,
                                      (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        add_green_shell_in_unexpired_actor_list(CM_FindActorIndex(shell));
                        return;
                    } else {
                        shell->state = 1;
                        if (player->unk_0C0 > 0) {
                            shell->rotAngle = 0x78E3;
                        } else {
                            shell->rotAngle = -0x78E4;
                        }
                    }
                }
            }
            break;
        case RELEASED_SHELL:
            player = &gPlayers[shell->playerId];
            if (shell->rotAngle > 0) {
                shell->rotAngle -= 0xE38;
                if (shell->rotAngle < 0) {
                    shell->state = 2;
                    shell->someTimer = 0x001E;
                    func_800C9060(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    func_800C90F4(shell->playerId,
                                  (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    add_green_shell_in_unexpired_actor_list(CM_FindActorIndex(shell));
                }
            } else {
                shell->rotAngle += 0xE38;
                if (shell->rotAngle > 0) {
                    shell->state = 2;
                    shell->someTimer = 0x001E;
                    func_800C9060(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    func_800C90F4(shell->playerId,
                                  (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    add_green_shell_in_unexpired_actor_list(CM_FindActorIndex(shell));
                }
            }
            if (shell->state == 2) {
                var_f2 = 8.0f;
                if (player->unk_094 > 8.0f) {
                    var_f2 = player->unk_094 * 1.2f;
                }
                somePosVel[0] = 0.0f;
                somePosVel[1] = 0.0f;
                somePosVel[2] = var_f2;
                func_802B64C4(somePosVel, player->rotation[1] + player->unk_0C0);
                shell->velocity[0] = somePosVel[0];
                shell->velocity[1] = somePosVel[1];
                shell->velocity[2] = somePosVel[2];
            } else {
                somePosVel[0] = sins(shell->rotAngle) * 6.0f;
                somePosVel[1] = shell->boundingBoxSize - player->boundingBoxSize;
                somePosVel[2] = coss(shell->rotAngle) * 6.0f;
                mtxf_translate_vec3f_mat3(somePosVel, player->orientationMatrix);
                shell->pos[0] = player->pos[0] + somePosVel[0];
                shell->pos[1] = player->pos[1] + somePosVel[1];
                shell->pos[2] = player->pos[2] + somePosVel[2];
            }
            break;
        case MOVING_SHELL:
            if (shell->parentIndex > 0) {
                shell->parentIndex -= 1;
                if (shell->parentIndex == 0) {
                    shell->flags &= ~0x1000;
                }
            }
            shell->velocity[1] -= 0.5f;
            if (shell->velocity[1] < -2.0f) {
                shell->velocity[1] = -2.0f;
            }
            somePos2[0] = shell->pos[0];
            somePos2[1] = shell->pos[1];
            somePos2[2] = shell->pos[2];
            shell->pos[0] += shell->velocity[0];
            shell->pos[1] += shell->velocity[1];
            shell->pos[2] += shell->velocity[2];
            actor_terrain_collision(&shell->unk30, 4.0f, shell->pos[0], shell->pos[1], shell->pos[2], somePos2[0],
                                    somePos2[1], somePos2[2]);
            func_802B4E30((struct Actor*) shell);
            if ((shell->unk30.surfaceDistance[0] < 0.0f) || (shell->unk30.surfaceDistance[1] < 0.0f)) {
                shell_collision(&shell->unk30, shell->velocity);
                func_800C98B8(shell->pos, shell->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
                shell->flags |= 0x80;
            }
            break;
        case TRIPLE_GREEN_SHELL:
            player = &gPlayers[shell->playerId];
            parent = (TripleShellParent*) GET_ACTOR(shell->parentIndex);
            if (parent->type != ACTOR_TRIPLE_GREEN_SHELL) {
                destroy_destructable_actor((struct Actor*) shell);
            } else {
                shell->rotAngle += parent->rotVelocity;
                somePosVel[0] = sins(shell->rotAngle) * 8.0f;
                somePosVel[1] = shell->boundingBoxSize - player->boundingBoxSize;
                somePosVel[2] = coss(shell->rotAngle) * 8.0f;
                mtxf_translate_vec3f_mat3(somePosVel, player->orientationMatrix);
                somePos2[0] = shell->pos[0];
                somePos2[1] = shell->pos[1];
                somePos2[2] = shell->pos[2];
                shell->pos[0] = player->pos[0] + somePosVel[0];
                shell->pos[1] = player->pos[1] + somePosVel[1];
                shell->pos[2] = player->pos[2] + somePosVel[2];
                actor_terrain_collision(&shell->unk30, 4.0f, shell->pos[0], shell->pos[1], shell->pos[2], somePos2[0],
                                        somePos2[1], somePos2[2]);
                func_802B4E30((struct Actor*) shell);
            }
            break;
        case GREEN_SHELL_HIT_A_RACER:
            // Somehow, this fake match affects stack management up in case 2
            shell->velocity[1] -= (0, 0.3f);
            if (shell->velocity[1] < -5.0f) {
                shell->velocity[1] = -5.0f;
            }
            shell->rotAngle += 0x5B0;
            shell->someTimer -= 1;
            shell->pos[1] += shell->velocity[1];
            if (shell->someTimer == 0) {
                destroy_actor((struct Actor*) shell);
            }
            break;
        default:
            break;
    }
}
