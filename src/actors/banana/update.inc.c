#include <actors.h>
#include <code_800029B0.h>
#include <defines.h>
#include <main.h>

/**
 * @brief Update the banana actor
 *
 * @param banana
 */
void update_actor_banana(struct BananaActor* banana) {
    UNUSED f32 pad;
    Player* player;
    struct BananaActor* elderBanana;
    struct Controller* controller;
    Vec3f someOtherVelocity;
    Vec3f someVelocity;
    f32 temp_f0;
    UNUSED f32 var_f8;
    UNUSED f32 pad2;
    UNUSED f32 pad3;
    UNUSED f32 pad4[2];
    f32 temp_f12;
    f32 temp_f2;
    f32 temp_f14;
    f32 temp_f16;
    f32 unkX;
    f32 unkY;
    f32 unkZ;

    player = &gPlayers[banana->playerId];
    switch (banana->state) {
        case HELD_BANANA:
            temp_f2 = player->pos[0] - banana->pos[0];
            temp_f14 = player->pos[1] - banana->pos[1];
            temp_f16 = player->pos[2] - banana->pos[2];
            temp_f12 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16)) / 10.0f;
            if (temp_f12 == 0.0f) {
                banana->pos[0] = player->pos[0] + 0.2f;
                banana->pos[1] = player->pos[1] + 0.2f;
                banana->pos[2] = player->pos[2] + 0.2f;
            } else {
                temp_f2 /= temp_f12;
                temp_f14 /= temp_f12;
                temp_f16 /= temp_f12;
                banana->pos[0] = player->pos[0] - temp_f2;
                banana->pos[1] = player->pos[1] - temp_f14 - 2.0f;
                banana->pos[2] = player->pos[2] - temp_f16;
            }
            check_bounding_collision(&banana->unk30, banana->boundingBoxSize + 1.0f, banana->pos[0], banana->pos[1],
                                     banana->pos[2]);
            func_802B4E30((struct Actor*) banana);
            if ((player->type & PLAYER_HUMAN) != 0) {
                if (gDemoMode) {
                    controller = gControllerOne;
                } else {
                    controller = &gControllers[banana->playerId];
                }
                if ((controller->buttonDepressed & Z_TRIG) != 0) {
                    controller->buttonDepressed &= ~Z_TRIG;
                    banana->state = 1;
                    banana->unk_04 = 0x00B4;
                    player->soundEffects &= ~HOLD_BANANA_SOUND_EFFECT;
                    func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    pad3 = controller->rawStickY;
                    if ((pad3 > 30.0f) && (controller->rawStickX < 10) && (controller->rawStickX >= -9)) {
                        pad3 = pad3 - ((f32) 30);
                        pad3 = (pad3 / 20.0f) + 0.5f;
                        if (player->unk_094 < 2.0f) {
                            temp_f0 = 4.0f;
                        } else {
                            temp_f0 = (player->unk_094 * 0.75f) + 3.5f + pad3;
                        }
                        vec3f_set(someVelocity, 0, pad3, temp_f0);
                        func_802B64C4(someVelocity, player->rotation[1] + player->unk_0C0);
                        banana->velocity[0] = someVelocity[0];
                        banana->velocity[1] = someVelocity[1];
                        banana->velocity[2] = someVelocity[2];
                    } else {
                        banana->velocity[0] = 0;
                        banana->velocity[1] = 1.5f;
                        banana->velocity[2] = 0;
                    }
                }
            }
            break;
        case DROPPED_BANANA:
            if (banana->unk_04 != 0) {
                banana->unk_04 -= 1;
                if (banana->unk_04 == 0) {
                    banana->flags &= ~0x1000;
                }
            }
            banana->pos[0] += banana->velocity[0];
            banana->pos[2] += banana->velocity[2];
            if (banana->velocity[1] > -1.0f) {
                banana->velocity[1] -= 0.15f;
            }
            banana->pos[1] += banana->velocity[1];
            if ((banana->pos[2] < (f32) gCourseMinZ) || ((f32) gCourseMaxZ < banana->pos[2]) ||
                (banana->pos[0] < (f32) gCourseMinX) || ((f32) gCourseMaxX < banana->pos[0]) ||
                (banana->pos[1] < (f32) gCourseMinY)) {
                destroy_destructable_actor((struct Actor*) banana);
            } else {
                check_bounding_collision(&banana->unk30, banana->boundingBoxSize + 1.0f, banana->pos[0], banana->pos[1],
                                         banana->pos[2]);
                banana->unk30.unk34 = 1;
                if ((banana->unk30.unk34 != 0) && (banana->unk30.surfaceDistance[2] < 0.0f)) {
                    someOtherVelocity[0] = -banana->unk30.orientationVector[0];
                    someOtherVelocity[1] = -banana->unk30.orientationVector[1];
                    someOtherVelocity[2] = -banana->unk30.orientationVector[2];
                    banana->pos[0] += someOtherVelocity[0] * banana->unk30.surfaceDistance[2];
                    banana->pos[1] += someOtherVelocity[1] * banana->unk30.surfaceDistance[2];
                    banana->pos[2] += someOtherVelocity[2] * banana->unk30.surfaceDistance[2];
                    banana->flags &= ~0x1000;
                    banana->state = 4;
                }
            }
            break;
        case FIRST_BANANA_BUNCH_BANANA:
            someVelocity[0] = 0.0f;
            someVelocity[1] = 0.0f;
            someVelocity[2] = -5.0f;
            func_802B64C4(someVelocity, player->rotation[1] + player->unk_0C0);
            unkX = player->pos[0] + someVelocity[0];
            unkY = player->pos[1] + someVelocity[1];
            unkZ = player->pos[2] + someVelocity[2];
            temp_f2 = unkX - banana->pos[0];
            temp_f14 = unkY - banana->pos[1];
            temp_f16 = unkZ - banana->pos[2];
            temp_f0 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16));
            if (temp_f0 == 0.0f) {
                banana->pos[0] = player->pos[0] + 0.2f;
                banana->pos[1] = player->pos[1] + 0.2f;
                banana->pos[2] = player->pos[2] + 0.2f;
            } else {
                temp_f2 /= temp_f0;
                temp_f14 /= temp_f0;
                temp_f16 /= temp_f0;
                banana->pos[0] = someVelocity[0] + (unkX - temp_f2);
                banana->pos[1] = unkY - temp_f14 - 2.0f;
                banana->pos[2] = unkZ - temp_f16;
            }
            check_bounding_collision(&banana->unk30, banana->boundingBoxSize + 1.0f, banana->pos[0], banana->pos[1],
                                     banana->pos[2]);
            func_802B4E30((struct Actor*) banana);
            break;
        case BANANA_BUNCH_BANANA:
            elderBanana = (struct BananaActor*) GET_ACTOR(banana->elderIndex);
            temp_f2 = elderBanana->pos[0] - banana->pos[0];
            temp_f14 = elderBanana->pos[1] - banana->pos[1];
            temp_f16 = elderBanana->pos[2] - banana->pos[2];
            temp_f12 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16)) / 5.0f;
            if (temp_f12 == 0.0f) {
                banana->pos[0] = elderBanana->pos[0] + 0.2f;
                banana->pos[1] = elderBanana->pos[1] + 0.2f;
                banana->pos[2] = elderBanana->pos[2] + 0.2f;
            } else {
                temp_f2 /= temp_f12;
                temp_f14 /= temp_f12;
                temp_f16 /= temp_f12;
                banana->pos[0] = elderBanana->pos[0] - temp_f2;
                banana->pos[1] = elderBanana->pos[1] - temp_f14 - 2.0f;
                banana->pos[2] = elderBanana->pos[2] - temp_f16;
            }
            var_f8 = banana->pos[2];
            check_bounding_collision(&banana->unk30, banana->boundingBoxSize + 1.0f, banana->pos[0], banana->pos[1],
                                     banana->pos[2]);
            func_802B4E30((struct Actor*) banana);
            break;
        case DESTROYED_BANANA:
            banana->velocity[1] -= 0.3f;
            if (banana->velocity[1] < -5.0f) {
                banana->velocity[1] = -5.0f;
            }
            banana->pos[1] += banana->velocity[1];
            banana->rot[0] += 0x16C;
            banana->rot[1] -= 0x5B0;
            banana->rot[2] += 0x38E;
            banana->unk_04 -= 1;
            if (banana->unk_04 == 0) {
                destroy_actor((struct Actor*) banana);
            }
            break;
        case BANANA_ON_GROUND:
            banana->flags |= 0xC000;
            banana->flags &= ~0x1000;
            break;
        default:
            break;
    }
}
