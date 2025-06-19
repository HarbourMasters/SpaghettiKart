#include <actors.h>
#include <waypoints.h>
#include <defines.h>
#include <main.h>
#include <actors_extended.h>
#include <code_800029B0.h>
#include "port/Game.h"

void func_802B3B44(struct ShellActor* shell) {
    u16 currentWaypoint;
    u16 nextWaypoint;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12_3;
    f32 temp_f14_2;
    f32 temp_f16_2;
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f28;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f12_0;
    f32 temp_f12_1;
    f32 temp_f12_2;
    f32 temp_f18_3;
    f32 temp_f16_3;
    f32 temp_f26;
    Vec3f origPos;

    currentWaypoint = shell->pathIndex;
    temp_f2 = D_80164490[currentWaypoint].posX;
    temp_f12 = D_80164490[currentWaypoint].posY;
    temp_f28 = D_80164490[currentWaypoint].posZ;
    nextWaypoint = currentWaypoint + 1;

    if (nextWaypoint >= D_80164430) {
        nextWaypoint -= D_80164430;
    }

    temp_f20 = temp_f2 - shell->pos[0];
    temp_f22 = temp_f12 - shell->pos[1];
    temp_f24 = temp_f28 - shell->pos[2];
    temp_f0 = (temp_f20 * temp_f20) + (temp_f22 * temp_f22) + (temp_f24 * temp_f24);
    if (temp_f0 > 400.0f) {
        temp_f18_3 = D_80164490[nextWaypoint].posX;
        temp_f16_3 = D_80164490[nextWaypoint].posY;
        temp_f26 = D_80164490[nextWaypoint].posZ;

        temp_f12_0 = temp_f18_3 - shell->pos[0];
        temp_f12_1 = temp_f16_3 - shell->pos[1];
        temp_f12_2 = temp_f26 - shell->pos[2];

        temp_f0_3 = (temp_f12_0 * temp_f12_0) + (temp_f12_1 * temp_f12_1) + (temp_f12_2 * temp_f12_2);
        if (temp_f0_3 < temp_f0) {
            shell->pathIndex = nextWaypoint;
        } else {
            temp_f0_2 = sqrtf(temp_f0) * 4.0f;
            temp_f20 /= temp_f0_2;
            temp_f22 /= temp_f0_2;
            temp_f24 /= temp_f0_2;

            temp_f12_3 = shell->velocity[0];
            temp_f14_2 = shell->velocity[1];
            temp_f16_2 = shell->velocity[2];

            temp_f12_3 += temp_f20;
            temp_f14_2 += temp_f22;
            temp_f16_2 += temp_f24;
            temp_f0 = sqrtf((temp_f12_3 * temp_f12_3) + (temp_f14_2 * temp_f14_2) + (temp_f16_2 * temp_f16_2));
            if (temp_f0 > 6.0f) {
                temp_f0 /= 6.0f;
                temp_f12_3 /= temp_f0;
                temp_f14_2 /= temp_f0;
                temp_f16_2 /= temp_f0;
            }
            shell->velocity[0] = temp_f12_3;
            shell->velocity[1] = temp_f14_2;
            shell->velocity[2] = temp_f16_2;

            origPos[0] = shell->pos[0];
            origPos[1] = shell->pos[1];
            origPos[2] = shell->pos[2];

            shell->pos[0] += temp_f12_3;
            shell->pos[1] += temp_f14_2;
            shell->pos[2] += temp_f16_2;
            actor_terrain_collision(&shell->unk30, 4.0f, shell->pos[0], shell->pos[1], shell->pos[2], origPos[0],
                                    origPos[1], origPos[2]);
            func_802B4E30((struct Actor*) shell);
        }
    } else {
        if (temp_f0 > 5.0f) {
            shell->pos[0] = temp_f2;
            shell->pos[1] = shell->boundingBoxSize + temp_f12;
            shell->pos[2] = temp_f28;
            shell->pathIndex = nextWaypoint;
        } else {
            temp_f18_3 = D_80164490[nextWaypoint].posX;
            temp_f16_3 = D_80164490[nextWaypoint].posY;
            temp_f26 = D_80164490[nextWaypoint].posZ;

            shell->pos[0] = (temp_f2 + temp_f18_3) * 0.5f;
            shell->pos[1] = ((temp_f12 + temp_f16_3) * 0.5f) + shell->boundingBoxSize;
            shell->pos[2] = (temp_f28 + temp_f26) * 0.5f;

            shell->velocity[0] = (temp_f18_3 - temp_f2) * 0.5f;
            shell->velocity[1] = (temp_f16_3 - temp_f12) * 0.5f;
            shell->velocity[2] = (temp_f26 - temp_f28) * 0.5f;
        }
    }
}

void func_802B3E7C(struct ShellActor* shell, Player* player) {
    f32 x_velocity;
    f32 z_velocity;
    f32 xz_dist;
    Vec3f newPosition;

    x_velocity = player->pos[0];
    x_velocity -= shell->pos[0];
    z_velocity = player->pos[2];
    z_velocity -= shell->pos[2];
    xz_dist = sqrtf((x_velocity * x_velocity) + (z_velocity * z_velocity)) / 8;
    if (xz_dist == 0.0f) {
        x_velocity = 0.0f;
        z_velocity = 0.0f;
    } else {
        x_velocity /= xz_dist;
        z_velocity /= xz_dist;
    }

    newPosition[0] = shell->pos[0];
    newPosition[1] = shell->pos[1];
    newPosition[2] = shell->pos[2];
    shell->pos[0] += x_velocity;
    shell->pos[1] -= 2.0f;
    shell->pos[2] += z_velocity;
    shell->velocity[0] = x_velocity;
    shell->velocity[1] = -2.0f;
    shell->velocity[2] = z_velocity;

    if (player->effects & BOO_EFFECT) {
        destroy_destructable_actor((struct Actor*) shell);
    } else {
        actor_terrain_collision(&shell->unk30, 4.0f, shell->pos[0], shell->pos[1], shell->pos[2], newPosition[0],
                                newPosition[1], newPosition[2]);
        func_802B4E30((struct Actor*) shell);
        func_802B4104(shell);
    }
}

/**
 * Only used in Battle mode
 * Likely trying to find the nearest player that is not the shell's owner and is not dead
 **/
s16 func_802B3FD0(Player* owner, struct ShellActor* shell) {
    Player* player;
    s32 playerIndex;
    f32 playerToShellDistance;
    s16 playerId = -1;
    f32 smallestDistance = 25000000.0f;

    for (playerIndex = 0; playerIndex < 4; playerIndex++) {
        player = &gPlayers[playerIndex];
        if ((player->type & PLAYER_EXISTS) == 0) {
            continue;
        }
        if (player == owner) {
            continue;
        }
        if (gPlayerBalloonCount[playerIndex] < 0) {
            continue;
        }
        // calc_sq_xy_diff_plus_2_z_diff is not quite a 3D distance function, it doubles (rather than squares) the Z difference of the
        // positions
        playerToShellDistance = calc_sq_xy_diff_plus_2_z_diff(player->pos, shell->pos);
        if (playerToShellDistance < smallestDistance) {
            smallestDistance = playerToShellDistance;
            playerId = player - gPlayerOne;
        }
    }

    return playerId;
}

void func_802B4104(struct ShellActor* shell) {
    if ((shell->unk30.surfaceDistance[0] < 0.0f) &&
        ((shell->unk30.unk48[1] < 0.25f) || (shell->unk30.unk48[1] > -0.25f))) {
        destroy_destructable_actor((struct Actor*) shell);
        func_800C98B8(shell->pos, shell->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
        shell->flags |= 0x80;
    } else if ((shell->unk30.surfaceDistance[1] < 0.0f) &&
               ((shell->unk30.unk54[1] < 0.25f) || (shell->unk30.unk54[1] < -0.25f))) {
        destroy_destructable_actor((struct Actor*) shell);
        func_800C98B8(shell->pos, shell->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
        shell->flags |= 0x80;
    }
}

/**
 * @brief Update the red and blue shell actors
 *
 * @param shell
 */
void update_actor_red_blue_shell(struct ShellActor* shell) {
    UNUSED f32 pad9;
    Player* player;
    f32 height;
    UNUSED f32 temp_f14;
    f32 temp_f2;
    s16 temp_v0;
    UNUSED s16 pad3;
    Vec3f somePosVel;
    struct Controller* controller;
    TripleShellParent* parent;
    UNUSED f32 pad0;
    UNUSED f32 pad1;
    UNUSED f32 pad2;
    UNUSED f32 pad4;
    UNUSED f32 pad5;
    UNUSED f32 pad6;
    UNUSED f32 pad7;
    UNUSED f32 pad8;
    UNUSED f32 pad10;
    UNUSED f32 pad11;
    UNUSED f32 pad12;
    UNUSED s16 pad13;
    UNUSED s16 pad13_2;
    UNUSED f32 pad14;
    UNUSED f32 pad15;
    UNUSED f32 pad16;
    UNUSED f32 pad17;
    Vec3f origPos;

    pad1 = shell->pos[0];
    pad0 = shell->pos[2];
    pad2 = shell->pos[1];
    pad13 = shell->type;
    if ((pad0 < (f32) gCourseMinZ) || ((f32) gCourseMaxZ < pad0) || (pad1 < (f32) gCourseMinX) ||
        ((f32) gCourseMaxX < pad1) || (pad2 < (f32) gCourseMinY)) {
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
            pad7 = player->pos[1] - somePosVel[1];
            shell->pos[2] = player->pos[2] + somePosVel[2];
            height = calculate_surface_height(shell->pos[0], pad7, shell->pos[2], player->collision.meshIndexZX);
            temp_f2 = pad7 - height;

            if ((temp_f2 < 5.0f) && (temp_f2 > -5.0f)) {
                shell->pos[1] = shell->boundingBoxSize + height;
            } else {
                shell->pos[1] = pad7;
            }

            if ((player->type & PLAYER_HUMAN) != 0) {
                if (gDemoMode) {
                    controller = gControllerOne;
                } else {
                    controller = &gControllers[shell->playerId];
                }
            } else {
                controller = gControllerOne;
            }

            if ((controller->buttonDepressed & Z_TRIG) != 0) {
                controller->buttonDepressed &= ~Z_TRIG;
                shell->state = RELEASED_SHELL;
                if (player->unk_0C0 > 0) {
                    shell->rotAngle = 0x78E3;
                } else {
                    shell->rotAngle = -0x78E4;
                }
            }
            break;
        case RELEASED_SHELL:
            player = &gPlayers[shell->playerId];
            if (shell->rotAngle > 0) {
                shell->rotAngle -= 0x71C;
                if (shell->rotAngle < 0) {
                    shell->state = MOVING_SHELL;
                    func_800C9060(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    func_800C90F4(shell->playerId,
                                  (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    if (pad13 == ACTOR_RED_SHELL) {
                        add_red_shell_in_unexpired_actor_list(CM_FindActorIndex(shell));
                    } else {
                        add_blue_shell_in_unexpired_actor_list(CM_FindActorIndex(shell));
                        func_800C9D80(shell->pos, shell->velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                    }
                }
            } else {
                shell->rotAngle += 0x71C;
                if (shell->rotAngle > 0) {
                    shell->state = MOVING_SHELL;
                    func_800C9060(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    func_800C90F4(shell->playerId,
                                  (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    if (pad13 == ACTOR_RED_SHELL) {
                        add_red_shell_in_unexpired_actor_list(CM_FindActorIndex(shell));
                    } else {
                        add_blue_shell_in_unexpired_actor_list(CM_FindActorIndex(shell));
                        func_800C9D80(shell->pos, shell->velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                    }
                }
            }
            if (shell->state == MOVING_SHELL) {
                shell->someTimer = 0x001E;
                height = 8.0f;
                if (player->unk_094 > 8.0f) {
                    height = player->unk_094 * 1.2f;
                }
                somePosVel[0] = 0.0f;
                somePosVel[1] = 0.0f;
                somePosVel[2] = height;
                func_802B64C4(somePosVel, (s16) (player->rotation[1] + player->unk_0C0));
                shell->velocity[0] = somePosVel[0];
                shell->velocity[1] = somePosVel[1];
                shell->velocity[2] = somePosVel[2];
            } else {
                somePosVel[0] = sins(shell->rotAngle) * 8.0f;
                somePosVel[1] = shell->boundingBoxSize - player->boundingBoxSize;
                somePosVel[2] = coss(shell->rotAngle) * 8.0f;
                mtxf_translate_vec3f_mat3(somePosVel, player->orientationMatrix);
                shell->pos[0] = player->pos[0] + somePosVel[0];
                shell->pos[1] = player->pos[1] + somePosVel[1];
                shell->pos[2] = player->pos[2] + somePosVel[2];
            }
            break;
        case MOVING_SHELL:
            player = &gPlayers[shell->playerId];
            shell->someTimer -= 1;
            if (shell->someTimer == 0) {
                shell->flags &= 0xEFFF;
                if (shell->type == ACTOR_BLUE_SPINY_SHELL) {
                    shell->targetPlayer = gPlayerPositionLUT[0];
                    shell->state = BLUE_SHELL_LOCK_ON;
                    shell->shellId = 1000.0f;
                    temp_v0 = gNearestWaypointByPlayerId[player - gPlayerOne] + 8;
                    if ((s32) D_80164430 < temp_v0) {
                        temp_v0 -= D_80164430;
                    }
                    shell->pathIndex = temp_v0;
                } else if (gModeSelection == BATTLE) {
                    shell->shellId = 1000.0f;
                    shell->targetPlayer = func_802B3FD0(player, shell);
                    if (shell->targetPlayer < 0) {
                        shell->flags = 0x8000;
                        shell->velocity[1] = 3.0f;
                        shell->pathIndex = 0;
                        shell->someTimer = 0x003C;
                        shell->state = DESTROYED_SHELL;
                    } else {
                        shell->state = RED_SHELL_LOCK_ON;
                    }
                } else {
                    if (player->currentRank == 0) {
                        shell->state = TRIPLE_GREEN_SHELL;
                        shell->someTimer = 0x0258;
                        temp_v0 = gNearestWaypointByPlayerId[player - gPlayerOne] + 8;
                        if ((s32) D_80164430 < temp_v0) {
                            temp_v0 -= D_80164430;
                        }
                        shell->pathIndex = temp_v0;
                    } else if (player->currentRank >= 5) {
                        shell->state = GREEN_SHELL_HIT_A_RACER;
                        shell->shellId = 1000.0f;
                        temp_v0 = gNearestWaypointByPlayerId[player - gPlayerOne] + 8;
                        if ((s32) D_80164430 < temp_v0) {
                            temp_v0 -= D_80164430;
                        }
                        shell->pathIndex = temp_v0;
                        shell->targetPlayer = gPlayerPositionLUT[player->currentRank - 1];
                    } else {
                        shell->state = RED_SHELL_LOCK_ON;
                        shell->shellId = 1000.0f;
                        shell->targetPlayer = gPlayerPositionLUT[player->currentRank - 1];
                    }
                }
            }
            shell->velocity[1] -= 0.5;
            if (shell->velocity[1] < -2.0f) {
                shell->velocity[1] = -2.0f;
            }
            origPos[0] = shell->pos[0];
            origPos[1] = shell->pos[1];
            origPos[2] = shell->pos[2];
            shell->pos[0] += shell->velocity[0];
            shell->pos[1] += shell->velocity[1];
            shell->pos[2] += shell->velocity[2];
            actor_terrain_collision(&shell->unk30, 4.0f, shell->pos[0], shell->pos[1], shell->pos[2], origPos[0],
                                    origPos[1], origPos[2]);
            func_802B4E30((struct Actor*) shell);
            func_802B4104(shell);
            break;
        case RED_SHELL_LOCK_ON:
            func_802B3E7C(shell, &gPlayers[shell->targetPlayer]);
            break;
        case TRIPLE_GREEN_SHELL:
            func_802B3B44(shell);
            if (shell->someTimer == 0) {
                if ((shell->flags & 0xF) == 0) {
                    destroy_destructable_actor((struct Actor*) shell);
                } else {
                    shell->someTimer -= 1;
                }
            }
            break;
        case GREEN_SHELL_HIT_A_RACER:
            func_802B3B44(shell);
            player = &gPlayers[shell->targetPlayer];
            height = player->pos[0];
            height -= shell->pos[0];
            temp_f2 = player->pos[2];
            temp_f2 -= shell->pos[2];
            if (((height * height) + (temp_f2 * temp_f2)) < 40000.0f) {
                shell->state = RED_SHELL_LOCK_ON;
            }
            break;
        case TRIPLE_RED_SHELL:
            player = &gPlayers[shell->playerId];
            parent = (TripleShellParent*) GET_ACTOR(shell->parentIndex);
            if (parent->type != ACTOR_TRIPLE_RED_SHELL) {
                destroy_destructable_actor((struct Actor*) shell);
            } else {
                shell->rotAngle += parent->rotVelocity;
                somePosVel[0] = sins(shell->rotAngle) * 8.0f;
                somePosVel[1] = shell->boundingBoxSize - player->boundingBoxSize;
                somePosVel[2] = coss(shell->rotAngle) * 8.0f;
                mtxf_translate_vec3f_mat3(somePosVel, player->orientationMatrix);
                origPos[0] = shell->pos[0];
                origPos[1] = shell->pos[1];
                origPos[2] = shell->pos[2];
                shell->pos[0] = player->pos[0] + somePosVel[0];
                shell->pos[1] = player->pos[1] + somePosVel[1];
                shell->pos[2] = player->pos[2] + somePosVel[2];
                actor_terrain_collision(&shell->unk30, 4.0f, shell->pos[0], shell->pos[1], shell->pos[2], origPos[0],
                                        origPos[1], origPos[2]);
                func_802B4E30((struct Actor*) shell);
            }
            break;
        case DESTROYED_SHELL:
            shell->velocity[1] -= 0.3f;
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
        case BLUE_SHELL_LOCK_ON:
            func_802B3B44(shell);
            shell->targetPlayer = gPlayerPositionLUT[0];
            player = &gPlayers[gPlayerPositionLUT[0]];
            height = player->pos[0];
            height -= shell->pos[0];
            temp_f2 = player->pos[2];
            temp_f2 -= shell->pos[2];
            if (((height * height) + (temp_f2 * temp_f2)) < 40000.0f) {
                shell->state = BLUE_SHELL_TARGET_ELIMINATED;
            }
            break;
        case 9:
            func_802B3E7C(shell, &gPlayers[shell->targetPlayer]);
            break;
        default:
            break;
    }
}
