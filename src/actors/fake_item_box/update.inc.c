#include <actors.h>
#include <main.h>

/**
 * @brief Updates the fake item box actor.
 *
 * @param fake_item_box
 */
void update_actor_fake_item_box(struct FakeItemBox* fake_item_box) {
    u32 temp_v1 = fake_item_box->playerId;
    Player* temp_v0_4 = &gPlayers[temp_v1];
    struct Controller* temp_v1_3;

    UNUSED s32 pad[7];
    f32 temp_f2_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    UNUSED s32 pad2[3];

    switch (fake_item_box->state) {
        case 0:
            fake_item_box->boundingBoxSize = fake_item_box->sizeScaling * 5.5f;
            fake_item_box->rot[0] -= 0xB6;
            fake_item_box->rot[1] += 0x16C;
            fake_item_box->rot[2] -= 0xB6;

            temp_f14 = temp_v0_4->pos[0] - fake_item_box->pos[0];
            temp_f16 = temp_v0_4->pos[1] - fake_item_box->pos[1];
            temp_f18 = temp_v0_4->pos[2] - fake_item_box->pos[2];

            temp_f2_2 = sqrtf((temp_f14 * temp_f14) + (temp_f16 * temp_f16) + (temp_f18 * temp_f18)) / 10.0f;
            temp_f14 /= temp_f2_2;
            temp_f16 /= temp_f2_2;
            temp_f18 /= temp_f2_2;
            fake_item_box->pos[0] = temp_v0_4->pos[0] - temp_f14;
            fake_item_box->pos[1] = (temp_v0_4->pos[1] - temp_f16) - 1.0f;
            fake_item_box->pos[2] = temp_v0_4->pos[2] - temp_f18;
            check_bounding_collision(&fake_item_box->unk30, fake_item_box->boundingBoxSize, fake_item_box->pos[0],
                                     fake_item_box->pos[1], fake_item_box->pos[2]);
            func_802B4E30((struct Actor*) fake_item_box);
            temp_v1_3 = &gControllers[temp_v1];
            if ((temp_v0_4->type & 0x4000) != 0) {

                if ((temp_v1_3->buttonDepressed & Z_TRIG) != 0) {
                    temp_v1_3->buttonDepressed &= 0xDFFF;
                    func_802A1064(fake_item_box);
                    temp_v0_4->soundEffects &= 0xFFFBFFFF;
                    func_800C9060((u8) (temp_v0_4 - gPlayerOne), SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                }
            }
            break;
        case 1:
            if (fake_item_box->sizeScaling < 1.0f) {
                fake_item_box->sizeScaling += 0.05f;
            } else if (fake_item_box->sizeScaling >= 1.0f) {
                fake_item_box->sizeScaling = 1.0f;
            }

            fake_item_box->boundingBoxSize = fake_item_box->sizeScaling * 5.5f;
            if (fake_item_box->targetY <= fake_item_box->pos[1]) {
                fake_item_box->pos[1] = fake_item_box->targetY;
            } else {
                fake_item_box->pos[1] += 0.2f;
            }
            if ((fake_item_box->flags & 0x1000) != 0) {
                if ((fake_item_box->someTimer <= 0) || (fake_item_box->someTimer >= 0x12D)) {
                    fake_item_box->flags &= 0xEFFF;
                    fake_item_box->someTimer = 0;
                } else {
                    fake_item_box->someTimer--;
                }
            }
            fake_item_box->rot[0] -= 0xB6;
            fake_item_box->rot[1] += 0x16C;
            fake_item_box->rot[2] -= 0xB6;
            break;

        case 2:
            if ((fake_item_box->someTimer >= 0x14) || (fake_item_box->someTimer < 0)) {
                destroy_actor((struct Actor*) fake_item_box);
            } else {
                fake_item_box->someTimer++;
                fake_item_box->rot[0] += 0x444;
                fake_item_box->rot[1] -= 0x2D8;
                fake_item_box->rot[2] += 0x16C;
            }
            break;
        default:
            destroy_actor((struct Actor*) fake_item_box);
            break;
    }
}
