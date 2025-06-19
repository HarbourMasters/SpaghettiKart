#include <actors.h>
#include <main.h>
#include <defines.h>
#include <waypoints.h>

/**
 * @brief Updates the kiwano fruit actor.
 * Actor used in DK's Jungle Parkway.
 *
 * @param fruit
 */
void update_actor_kiwano_fruit(struct KiwanoFruit* fruit) {
    Player* player;
    f32 temp_f2;
    f32 temp_f16;
    f32 temp_f14;
    f32 temp_f12;
    s32 nearestWaypoint;

    player = &gPlayers[fruit->targetPlayer];
    if (((player->type & PLAYER_CPU) != 0) || (player->collision.unk34 == 0)) {
        fruit->state = 0;
        return;
    }
    switch (fruit->state) { /* irregular */
        case 0:
            if ((get_surface_type(player->collision.meshIndexZX) & 0xFF) != GRASS) {
                return;
            }
            fruit->state = 1;
            fruit->velocity[0] = 80.0f;
        case 1:
            nearestWaypoint = gNearestPathPointByPlayerId[(u16) (player - gPlayerOne)];
            temp_f2 = player->pos[0] - gCurrentTrackPath[nearestWaypoint].posX;
            temp_f16 = player->pos[1] - gCurrentTrackPath[nearestWaypoint].posY;
            temp_f14 = player->pos[2] - gCurrentTrackPath[nearestWaypoint].posZ;
            temp_f12 = fruit->velocity[0] / sqrtf((temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f14 * temp_f14));
            temp_f2 *= temp_f12;
            temp_f16 *= temp_f12;
            temp_f14 *= temp_f12;
            fruit->pos[0] = player->pos[0] + temp_f2;
            fruit->pos[1] = player->pos[1] + temp_f16;
            fruit->pos[2] = player->pos[2] + temp_f14;
            fruit->velocity[0] -= 2.0f;
            if (fruit->velocity[0] <= 0.0f) {
                fruit->state = 2;
                fruit->bonkTimer = 30.0f;
                fruit->velocity[0] = 0.0f;
                fruit->velocity[1] = 2.3f;
                fruit->velocity[2] = 0.0f;
                if ((player->effects & STAR_EFFECT) != 0) {
                    func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x52));
                } else {
                    player->effects |= 0x8000;
                    player->pos[0] -= temp_f2 * 4.0f;
                    player->pos[2] -= temp_f14 * 4.0f;
                    player->velocity[0] -= temp_f2 * 0.7f;
                    player->velocity[2] -= temp_f14 * 0.7f;
                    func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
                    if (gModeSelection != GRAND_PRIX) {
                        D_80162DF8 = 1;
                    }
                }
            }
            break;
        case 2:
            fruit->velocity[1] -= 0.3f;
            fruit->pos[0] += fruit->velocity[0];
            fruit->pos[1] += fruit->velocity[1];
            fruit->pos[2] += fruit->velocity[2];
            fruit->bonkTimer -= 1.0f;
            if (fruit->bonkTimer < 0.0f) {
                fruit->state = 0;
            }
            break;
        default:
            break;
    }
    if (fruit->state != 0) {
        fruit->animTimer += 1;
        if (fruit->animTimer == 8) {
            fruit->animTimer = 0;
            fruit->animState += 1;
            if (fruit->animState == 3) {
                fruit->animState = 0;
            }
        }
    }
}
