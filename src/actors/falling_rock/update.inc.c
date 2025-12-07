#include "math_util.h"
#include <actors.h>
#include <code_800029B0.h>
#include <main.h>
#include <assets/models/tracks/choco_mountain/choco_mountain_data.h>

void func_8029CF0C(struct ActorSpawnData* spawnData, struct FallingRock* rock) {
    struct ActorSpawnData* temp_v0 = spawnData;
    Vec3s sp24 = { 60, 120, 180 };
    temp_v0 += rock->unk_06;
    rock->respawnTimer = sp24[rock->unk_06]; // * 2
    rock->pos[0] = (f32) temp_v0->pos[0] * gCourseDirection;
    rock->pos[1] = (f32) temp_v0->pos[1] + 10.0f;
    rock->pos[2] = (f32) temp_v0->pos[2];
    vec3f_set(rock->velocity, 0, 0, 0);
    vec3s_set(rock->rot, 0, 0, 0);
}

/**
 * @brief Spawns falling sRockSpawnData.
 * Used in Choco Mountain.
 *
 * @param spawnData
 */
static struct ActorSpawnData* sRockSpawnData;
void spawn_falling_rocks(struct ActorSpawnData* spawnData) {
    struct ActorSpawnData* temp_s0 = spawnData;
    sRockSpawnData = spawnData;
    struct FallingRock* temp_v1;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s16 temp;

    while (temp_s0->pos[0] != -0x8000) {
        startingPos[0] = temp_s0->pos[0] * gCourseDirection;
        startingPos[1] = temp_s0->pos[1] + 10.0f;
        startingPos[2] = temp_s0->pos[2];
        vec3f_set(startingVelocity, 0, 0, 0);
        vec3s_set(startingRot, 0, 0, 0);
        temp = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_FALLING_ROCK);
        temp_v1 = (struct FallingRock*) GET_ACTOR(temp);

        temp_v1->unk_06 = temp_s0->someId;
        func_802AAAAC((Collision*) &temp_v1->unk30);
        temp_s0++;
    }
}

/**
 * @brief Updates the falling rock actor.
 * Actor used in Choco Mountain.
 *
 * @param rock
 */
void update_actor_falling_rocks(struct FallingRock* rock) {
    Vec3f unkVec;
    f32 pad0;
    f32 pad1;

    if (rock->respawnTimer != 0) {
        rock->respawnTimer -= 1;
        return;
    }
    if (rock->pos[1] < CM_GetWaterLevel(rock->pos, NULL)) {
        func_8029CF0C(sRockSpawnData, rock);
    }
    rock->rot[0] += (s16) ((rock->velocity[2] * 5461.0f) / 20.0f);
    rock->rot[2] += (s16) ((rock->velocity[0] * 5461.0f) / 20.0f);
    rock->velocity[1] -= 0.1;
    if (rock->velocity[1] < (-2.0f)) {
        rock->velocity[1] = -2.0f;
    }
    rock->pos[0] += rock->velocity[0];
    rock->pos[1] += rock->velocity[1];
    rock->pos[2] += rock->velocity[2];
    pad1 = rock->velocity[1];
    check_bounding_collision(&rock->unk30, 10.0f, rock->pos[0], rock->pos[1], rock->pos[2]);
    pad0 = rock->unk30.surfaceDistance[2];
    if (pad0 < 0.0f) {
        unkVec[0] = -rock->unk30.orientationVector[0];
        unkVec[1] = -rock->unk30.orientationVector[1];
        unkVec[2] = -rock->unk30.orientationVector[2];
        rock->pos[0] += unkVec[0] * rock->unk30.surfaceDistance[2];
        rock->pos[1] += unkVec[1] * rock->unk30.surfaceDistance[2];
        rock->pos[2] += unkVec[2] * rock->unk30.surfaceDistance[2];
        adjust_pos_orthogonally(unkVec, pad0, rock->velocity, 2.0f);
        rock->velocity[1] = -1.2f * pad1;
        func_800C98B8(rock->pos, rock->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
    }
    pad0 = rock->unk30.surfaceDistance[0];
    if (pad0 < 0.0f) {
        unkVec[1] = -rock->unk30.unk48[1];
        if (unkVec[1] == 0.0f) {
            rock->velocity[1] *= -1.2f;
            return;
        } else {
            unkVec[0] = -rock->unk30.unk48[0];
            unkVec[2] = -rock->unk30.unk48[2];
            rock->pos[0] += unkVec[0] * rock->unk30.surfaceDistance[0];
            rock->pos[1] += unkVec[1] * rock->unk30.surfaceDistance[0];
            rock->pos[2] += unkVec[2] * rock->unk30.surfaceDistance[0];
            adjust_pos_orthogonally(unkVec, pad0, rock->velocity, 2.0f);
            rock->velocity[1] = -1.2f * pad1;
            func_800C98B8(rock->pos, rock->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
    pad0 = rock->unk30.surfaceDistance[1];
    if (pad0 < 0.0f) {
        unkVec[1] = -rock->unk30.unk54[1];
        if (unkVec[1] == 0.0f) {
            rock->velocity[1] *= -1.2f;
        } else {
            unkVec[0] = -rock->unk30.unk54[0];
            unkVec[2] = -rock->unk30.unk54[2];
            rock->pos[0] += unkVec[0] * rock->unk30.surfaceDistance[1];
            rock->pos[1] += unkVec[1] * rock->unk30.surfaceDistance[1];
            rock->pos[2] += unkVec[2] * rock->unk30.surfaceDistance[1];
            pad1 = rock->velocity[1];
            adjust_pos_orthogonally(unkVec, pad0, rock->velocity, 2.0f);
            rock->velocity[1] = -1.2f * pad1;
            func_800C98B8(rock->pos, rock->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}
