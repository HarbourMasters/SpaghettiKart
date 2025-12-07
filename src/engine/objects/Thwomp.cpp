#include <libultraship.h>
#include <libultra/gbi.h>
#include "Thwomp.h"
#include <vector>
#include "engine/courses/Course.h"
#include "engine/World.h"

#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "actors.h"
#include "math_util.h"
#include "sounds.h"
#include "update_objects.h"
#include "render_player.h"
#include "external.h"
#include "bomb_kart.h"
#include "collision.h"
#include "code_80086E70.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "defines.h"
#include "code_80005FD0.h"
#include "math_util_2.h"
#include "collision.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
#include "assets/textures/tracks/bowsers_castle/bowsers_castle_data.h"
#include "ceremony_and_credits.h"
#include "objects.h"
#include "update_objects.h"
#include "render_objects.h"
#include "some_data.h"
#include "assets/models/common_data.h"
extern s8 gPlayerCount;
}

f32 D_800E594C[][2] = {
    { -8.0, 8.0 },
    { 8.0, 8.0 },
    { 0.0, 0.0 },
    { 8.0, -8.0 },
    { -8.0, -8.0 },
    // This feels super fake, but it matches
    { -0.0, 0.0 },
};

s16 D_800E597C[] = { 0x0000, 0x0000, 0x4000, 0x8000, 0x8000, 0xc000 };

size_t OThwomp::_count = 0;
size_t OThwomp::_rand = 0;

OThwomp::OThwomp(const SpawnParams& params) : OObject(params) { // s16 x, s16 z, s16 direction, f32 scale, s16 behaviour, s16 primAlpha, u16 boundingBoxSize) {
    FVector loc = params.Location.value_or(FVector{0, 0, 0});
    IRotator rot = params.Rotation.value_or(IRotator{0, 0, 0});
    BoundingBoxSize = params.BoundingBoxSize.value_or(0);
    Behaviour = static_cast<OThwomp::States>(params.Behaviour.value_or(1));
    PrimAlpha = params.PrimAlpha.value_or(0);
    FVector scale = params.Scale.value_or(FVector(0, 0, 0));

    Name = "Thwomp";
    ResourceName = "mk:thwomp";
    Model = "d_course_bowsers_castle_dl_thwomp";
    _idx = _count;
    _faceDirection = rot.yaw;

    find_unused_obj_index(&_objectIndex);

    s32 objectId = _objectIndex;
    init_object(objectId, 0);
    gObjectList[objectId].origin_pos[0] = loc.x * xOrientation;
    gObjectList[objectId].origin_pos[2] = loc.z;
    gObjectList[objectId].unk_0D5 = Behaviour;
    gObjectList[objectId].primAlpha = PrimAlpha;
    gObjectList[objectId].boundingBoxSize = BoundingBoxSize + 5;

    if (scale.y == 0.0f) {
        scale.y = 1.0f;
    }

    gObjectList[objectId].sizeScaling = scale.y;

    _count++;
}

void OThwomp::SetSpawnParams(SpawnParams& params) {
    Object* object = &gObjectList[_objectIndex];
    params.Name = std::string(ResourceName);
    params.Location = FVector(
        object->origin_pos[0],
        object->origin_pos[1],
        object->origin_pos[2]
    );
    IRotator rot; rot.Set(0, object->orientation[1], 0);
    params.Rotation = rot;
    params.Scale = FVector(0, object->sizeScaling, 0);
    params.Behaviour = Behaviour;
    params.PrimAlpha = PrimAlpha;
    params.BoundingBoxSize = BoundingBoxSize;

}

void OThwomp::Tick60fps() { // func_80081210
    Player* player;
    s32 objectIndex;
    s32 var_s2_3;
    s32 var_s4;

    func_800722CC(_objectIndex, 0x00000010);
    OThwomp::SetVisibility(_objectIndex);

    OThwomp::func_8007F8D8();

    // Tick lights only one time. This gets applied to every thwomp.
    // Running this more than once per frame will result in the lights moving at a high rate of speed.
    if (_idx == 0) {
        D_80165834[0] += 0x100;
        D_80165834[1] += 0x200;
        _rand += 1;
    }

    if (gObjectList[_objectIndex].state != 0) {
        switch(Behaviour) {
            case States::STATIONARY:
                OThwomp::StationaryBehaviour(_objectIndex);
                break;
            case States::MOVE_AND_ROTATE:
                OThwomp::MoveAndRotateBehaviour(_objectIndex);
                break;
            case States::MOVE_FAR:
                OThwomp::MoveFarBehaviour(_objectIndex);
                break;
            case States::STATIONARY_FAST:
                OThwomp::StationaryFastBehaviour(_objectIndex);
                break;
            case States::JAILED:
                OThwomp::JailedBehaviour(_objectIndex);
                break;
            case States::SLIDE:
                OThwomp::SlidingBehaviour(_objectIndex);
                break;
        }
    }

    player = gPlayerOne;
    for (var_s4 = 0; var_s4 < NUM_PLAYERS; var_s4++, player++) {
        player->tyres[FRONT_LEFT].unk_14 &= ~3;
        player->unk_046 &= ~0x0006;

        if (!(player->effects & BOO_EFFECT)) {
            OThwomp::func_80080B28(_objectIndex, var_s4);
        }
        if (is_obj_flag_status_active(_objectIndex, 0x00020000) != 0) {
            OThwomp::SetPlayerCrushedEffect(_objectIndex, player);
        }
        if (is_obj_flag_status_active(_objectIndex, 0x00010000) != 0) {
            OThwomp::func_80080A4C(_objectIndex, var_s4);
        }
    }
    OThwomp::func_8007542C(3);

    if (func_80072320(_objectIndex, 0x00000020)) {
        func_800722CC(_objectIndex, 0x00000020);
        OThwomp::AddParticles(_objectIndex);
    }

    if (_idx == 0) {
        for (var_s4 = 0; var_s4 < gObjectParticle2_SIZE; var_s4++) {
            // @port: Tag the transform.
            FrameInterpolation_RecordOpenChild("Thwomp_part", (uintptr_t) var_s4);

            objectIndex = gObjectParticle2[var_s4];
            if (objectIndex == DELETED_OBJECT_ID) {
                continue;
            }
            if (gObjectList[objectIndex].state == 0) {
                continue;
            }
            OThwomp::func_800810F4(objectIndex);
            if (gObjectList[objectIndex].state != 0) {
                continue;
            }
            delete_object_wrapper(&gObjectParticle2[var_s4]);

            // @port Pop the transform id.
            FrameInterpolation_RecordCloseChild();
        }
    }
}

void OThwomp::func_800810F4(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OThwomp::func_80081080(objectIndex);
            break;
        case 2:
            object_add_velocity_offset_xz(objectIndex);
            f32_step_up_towards(&gObjectList[objectIndex].offset[1], 14.0f, 0.5f);
            func_8007415C(objectIndex, &gObjectList[objectIndex].sizeScaling, 0.25f, 0.75f, 0.025f, 1, 0);
            if (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0x000000FF, 0, 4, 0, 0) != 0) {
                object_next_state(objectIndex);
            }
            break;
        case 3:
            func_80072428(objectIndex);
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void OThwomp::func_80081080(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->activeTexture = (const char*) D_8018D490;
    object->textureList = (const char**) D_8018D490;
    object->primAlpha = 0x00FF;
    object->direction_angle[1] = 0;
    object->orientation[0] = 0;
    object->orientation[2] = 0;
    object->offset[0] = 0.0f;
    object->offset[1] = 0.0f;
    object->offset[2] = 0.0f;
    object->sizeScaling = 0.25f;
    object_next_state(objectIndex);
}

void OThwomp::AddParticles(s32 arg0) {
    s32 objectIndex;
    s32 i;

    for (i = 0; i < 6; i++) {
        objectIndex = add_unused_obj_index(gObjectParticle2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
        OThwomp::func_80080E8C(objectIndex, arg0, i);
    }
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/YMJDJ
// No idea what the problem is
void OThwomp::func_80080E8C(s32 objectIndex1, s32 objectIndex2, s32 arg2) {
    u16 anAngle;
    f32 thing0;
    f32 thing1;
    f32* temp_v1;

    init_object(objectIndex1, arg2);
    temp_v1 = D_800E594C[arg2];
    gObjectList[objectIndex1].unk_0D5 = 2;
    anAngle = gObjectList[objectIndex2].direction_angle[1];
    thing1 = func_800416D8(temp_v1[1], temp_v1[0], anAngle);
    thing0 = func_80041724(temp_v1[1], temp_v1[0], anAngle);
    gObjectList[objectIndex1].origin_pos[0] = gObjectList[objectIndex2].pos[0] + thing0;
    gObjectList[objectIndex1].origin_pos[1] = gObjectList[objectIndex2].surfaceHeight - 9.0;
    gObjectList[objectIndex1].origin_pos[2] = gObjectList[objectIndex2].pos[2] + thing1;
    anAngle = D_800E597C[arg2] + gObjectList[objectIndex2].direction_angle[1];
    gObjectList[objectIndex1].velocity[0] = sins(anAngle) * 0.6;
    gObjectList[objectIndex1].velocity[2] = coss(anAngle) * 0.6;
}
#else
GLOBAL_ASM("asm/non_matchings/update_objects/func_80080E8C.s")
#endif

void OThwomp::SetPlayerCrushedEffect(s32 objectIndex, Player* player) {
    if (is_within_horizontal_distance_of_player(objectIndex, player, 12.0f) != 0) {
        player->tyres[FRONT_LEFT].unk_14 |= 3;
    }
}

void OThwomp::func_80080A4C(s32 objectIndex, s32 cameraPlayerId) {
    Camera* camera = &camera1[cameraPlayerId];
    Player* player = &gPlayerOne[cameraPlayerId];

    if (gScreenModeSelection != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        if ((func_80072320(objectIndex, 0x00000010) != 0) &&
            (is_within_horizontal_distance_of_player(objectIndex, player, 500.0f) != false)) {
            func_8001CA10(camera);
            func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity,
                          SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}

void OThwomp::func_8007542C(s32 arg0) {
    s32 objectIndex;
    s32 var_s2;
    s32* var_s3;
    Object* object;

    D_8016582C[0] += 0x2000;
    D_8016582C[1] += 0x1000;
    D_8016582C[2] += 0x1800;
    for (var_s2 = 0; var_s2 < 0x80; var_s2++) {
        switch (arg0) { /* irregular */
            case 1:
                var_s3 = gObjectParticle1;
                break;
            case 2:
                var_s3 = gObjectParticle2;
                break;
            case 3:
                var_s3 = gObjectParticle3;
                break;
        }
        objectIndex = var_s3[var_s2];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state != 0) {
                OThwomp::func_80074FD8(objectIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&var_s3[var_s2]);
                }
            }
        }
    }
}

void OThwomp::func_80074FD8(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 0:
            break;
        case 1:
            if (func_80087E08(objectIndex, gObjectList[objectIndex].velocity[1], 0.12f,
                              gObjectList[objectIndex].unk_034, gObjectList[objectIndex].direction_angle[1],
                              0x00000064) != 0) {
                object_next_state(objectIndex);
            }
            object_calculate_new_pos_offset(objectIndex);
            gObjectList[objectIndex].orientation[0] += D_8016582C[0];
            gObjectList[objectIndex].orientation[1] += D_8016582C[1];
            gObjectList[objectIndex].orientation[2] += D_8016582C[2];
            break;
        case 2:
            func_80086F60(objectIndex);
            func_80072428(objectIndex);
            break;
    }
}

void OThwomp::SetVisibility(s32 objectIndex) { // func_8008A4CC
    s32 loopIndex;
    Camera* camera;

    clear_object_flag(objectIndex, 0x00070000);
    for (loopIndex = 0, camera = camera1; loopIndex < gPlayerCountSelection1; loopIndex++, camera++) {
        if (gObjectList[objectIndex].state != 0) {
            if ((D_8018CF68[loopIndex] >= (gObjectList[objectIndex].unk_0DF - 1)) &&
                ((gObjectList[objectIndex].unk_0DF + 1) >= D_8018CF68[loopIndex])) {
                set_object_flag(objectIndex, 0x00010000);
                if (D_8018CF68[loopIndex] == gObjectList[objectIndex].unk_0DF) {
                    set_object_flag(objectIndex, 0x00020000);
                }
                if (is_object_visible_on_camera(objectIndex, camera, 0x2AABU) != 0) {
                    set_object_flag(objectIndex, VISIBLE);
                }
            }
        }
    }
}

void OThwomp::func_8007F8D8() {
    Player* player;
    s32 var_s0;
    s32 someIndex;
    s32 var_s4;
    Object* object;

    player = gPlayerOne;
    var_s4 = 1;

    object = &gObjectList[_objectIndex];
    if (object->unk_0D5 == 3) {
        var_s0 = 0;
        if ((object->state >= 2) && (func_80072354(_objectIndex, 8) != 0)) {
            var_s0 = 1;
        }
        var_s4 *= var_s0;
    }

    if (var_s4 != 0) {
        for (var_s0 = 0; var_s0 < 4; var_s0++, player++) {
            if ((player->type & PLAYER_EXISTS) && !(player->type & PLAYER_CPU)) {
                if (OThwomp::func_8007F75C(var_s0) != 0) {
                    break;
                }
            }
        }
    }
}

s32 OThwomp::func_8007F75C(s32 playerId) {
    s32 someIndex;
    static s32 temp_s7 = 0; // Must be static to sync far travelling thwomp instances
    s32 var_s6;
    s32 waypoint;

    waypoint = gNearestPathPointByPlayerId[playerId];
    var_s6 = 0;
    if ((waypoint >= 0xAA) && (waypoint < 0xB5)) {
        if (_idx == 0) {
            temp_s7 = random_int(0x0032U) + 0x32;
        }
        if (gObjectList[_objectIndex].unk_0D5 == 3) {
            var_s6 = 1;
            OThwomp::func_8007F660(_objectIndex, playerId, temp_s7);
        }

    } else if ((waypoint >= 0xD7) && (waypoint < 0xE2)) {
        if (gObjectList[_objectIndex].unk_0D5 == 3) {
            var_s6 = 1;
            OThwomp::func_8007F6C4(_objectIndex, playerId);
        }
    }
    return var_s6;
}

s32 OThwomp::func_8007E50C(s32 objectIndex, Player* player, Camera* camera) {
    s32 sp24;

    sp24 = 0;
    if ((func_80072354(objectIndex, 4) != 0) &&
        (is_within_horizontal_distance_of_player(objectIndex, player, 300.0f) != 0) &&
        (func_8008A0B4(objectIndex, player, camera, 0x4000U) != 0) &&
        (func_8008A060(objectIndex, camera, 0x1555U) != 0)) {
        func_800722A4(objectIndex, 4);
        sp24 = 1;
    }
    return sp24;
}

s32 OThwomp::func_8007E59C(s32 objectIndex) {
    Camera* camera;
    Player* player;
    s32 temp_v0;
    s32 someIndex;

    temp_v0 = 0;
    player = gPlayerOne;
    camera = camera1;
    for (someIndex = 0; someIndex < gPlayerCountSelection1; someIndex++) {
        temp_v0 = OThwomp::func_8007E50C(objectIndex, player++, camera++);
        if (temp_v0 != 0) {
            break;
        }
    }
    return temp_v0;
}

void OThwomp::func_8007F544(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0DD) { /* irregular */
        case 1:
            OThwomp::func_8007EFBC(objectIndex);
            break;
        case 2:
            OThwomp::func_8007F280(objectIndex);
            break;
    }
}

void OThwomp::func_8007EFBC(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0800U, 0x00008000) != 0) {
                gObjectList[objectIndex].unk_01C[0] = (f32) ((f64) xOrientation * 200.0);
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            if (f32_step_towards(gObjectList[objectIndex].offset, gObjectList[objectIndex].unk_01C[0], 4.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 3:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
                func_800726CC(objectIndex, 3);
                func_80086FD4(objectIndex);
            }
            break;
        case 5:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 6:
            if (f32_step_down_towards(&gObjectList[objectIndex].offset[2], -100.0f, 2.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 7:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00004000) != 0) {
                func_80086FD4(objectIndex);
                func_800726CC(objectIndex, 3);
            }
            break;
        case 9:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 10:
            if (f32_step_towards(gObjectList[objectIndex].offset, 0.0f, 4.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 11:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
                func_80086FD4(objectIndex);
                func_800726CC(objectIndex, 3);
            }
            break;
        case 13:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00014000) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 14:
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[2], 0.0f, 2.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 15:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
                func_80086FD4(objectIndex);
                func_800726CC(objectIndex, 3);
            }
            break;
        case 17:
            func_8008701C(objectIndex, 1);
            break;
        case 0:
        default:
            break;
    }
}

void OThwomp::func_8007F280(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
                gObjectList[objectIndex].unk_01C[0] = (f32) ((f64) xOrientation * -200.0);
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            if (f32_step_towards(gObjectList[objectIndex].offset, gObjectList[objectIndex].unk_01C[0], 4.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 3:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00010000) != 0) {
                func_800726CC(objectIndex, 3);
                func_80086FD4(objectIndex);
            }
            break;
        case 5:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00004000) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 6:
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[2], 100.0f, 2.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 7:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
                func_80086FD4(objectIndex);
                func_800726CC(objectIndex, 3);
            }
            break;
        case 9:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 10:
            if (f32_step_towards(gObjectList[objectIndex].offset, 0.0f, 4.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 11:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
                func_80086FD4(objectIndex);
                func_800726CC(objectIndex, 3);
            }
            break;
        case 13:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x0000C000) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 14:
            if (f32_step_down_towards(&gObjectList[objectIndex].offset[2], 0.0f, 2.0f) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 15:
            if (func_80073E18(objectIndex, (u16*) &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00014000) != 0) {
                func_80086FD4(objectIndex);
                func_800726CC(objectIndex, 3);
            }
            break;
        case 17:
            func_8008701C(objectIndex, 1);
            break;
        case 0:
        default:
            break;
    }
}

void OThwomp::func_8007F660(s32 objectIndex, s32 arg1, s32 arg2) {
    Object* object;

    func_800722A4(objectIndex, 8);
    func_80086E70(objectIndex);
    object = &gObjectList[objectIndex];
    object->unk_0DD = 1;
    object->unk_0D1 = arg1;
    object->unk_048 = arg2;
}

void OThwomp::func_8007F6C4(s32 objectIndex, s32 playerId) {
    Player* player;

    player = &gPlayerOne[playerId];
    func_800722A4(objectIndex, 8);
    func_80086E70(objectIndex);
    gObjectList[objectIndex].unk_0DD = 2;
    gObjectList[objectIndex].unk_01C[0] = player->pos[0] - gObjectList[objectIndex].origin_pos[0];
    gObjectList[objectIndex].unk_0D1 = playerId;
}

void OThwomp::func_80080B28(s32 objectIndex, s32 playerId) {
    f32 temp_f0;
    Player* player;

    player = &gPlayerOne[playerId];
    if (is_obj_flag_status_active(objectIndex, 0x00000200) != 0) {
        if (!(player->soundEffects & 0x100)) {
            temp_f0 = func_80088F54(objectIndex, player);
            if ((temp_f0 <= 9.0) && !(player->effects & 0x04000000) &&
                (has_collided_horizontally_with_player(objectIndex, player) != 0)) {
                if ((player->type & 0x8000) && !(player->type & 0x100)) {
                    if (!(player->effects & 0x200)) {
                        func_80089474(objectIndex, playerId, 1.4f, 1.1f, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x4C));
                    } else if (func_80072354(objectIndex, 0x00000040) != 0) {
                        if (player->type & 0x1000) {
                            func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        } else {
                            func_800C9060((u8) playerId, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        }
                        OThwomp::func_80080DE4(objectIndex);
                        func_80075304(gObjectList[objectIndex].pos, 3, 3, D_8018D3C4);
                        clear_object_flag(objectIndex, 0x00000200);
                        func_800722A4(objectIndex, 0x00000040);
                        func_80086F60(objectIndex);
                        func_800726CC(objectIndex, 0x000000C8);
                    }
                }
            } else if ((temp_f0 <= 17.5) && (func_80072320(objectIndex, 1) != 0) &&
                       (is_within_horizontal_distance_of_player(objectIndex, player,
                                                                (player->speed * 0.5) + BoundingBoxSize) != 0)) {
                if ((player->type & 0x8000) && !(player->type & 0x100)) {
                    if (is_obj_flag_status_active(objectIndex, 0x04000000) != 0) {
                        func_80072180();
                    }
                    func_800722A4(objectIndex, 2);
                    player->unk_040 = (s16) objectIndex;
                    player->unk_046 |= 2;
                    player->soundEffects |= 0x100;
                    reset_player_speed_and_velocity(player);
                }
            }
        } else {
            reset_player_speed_and_velocity(player);
        }
    }
}

Lights1 D_800E4638 = gdSPDefLights1(85, 85, 85, 255, 255, 255, 0, -120, 0);

Lights1 D_800E4650 = gdSPDefLights1(85, 85, 0, 255, 255, 0, 0, 120, 0);

Lights1 D_800E4668 = gdSPDefLights1(85, 85, 85, 255, 255, 255, -66, 82, -55);

Lights1 D_800E4680 = gdSPDefLights1(85, 85, 85, 255, 255, 255, 0, 0, 120);

Lights1 D_800E4698 = gdSPDefLights1(85, 85, 85, 255, 255, 255, 0, 0, 120);

void OThwomp::Draw(s32 cameraId) {
    s32 objectIndex = _objectIndex;
    s32 i;
    UNUSED s32 stackPadding0;
    s16 minusone, plusone;
    Camera* camera;
    Object* object;

    camera = &cameras[cameraId];
    if (cameraId == PLAYER_ONE || cameraId == 4) { // 4 == freecam
        clear_object_flag(objectIndex, 0x00070000);
        func_800722CC(objectIndex, 0x00000110);
    }

    OThwomp::TranslateThwompLights();

    minusone = gObjectList[objectIndex].unk_0DF - 1;
    plusone = gObjectList[objectIndex].unk_0DF + 1;

    if (gGamestate != CREDITS_SEQUENCE) {
        OThwomp::DrawModel(objectIndex);
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D0079C8);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gSPNumLights(gDisplayListHead++, 1);
    gSPLight(gDisplayListHead++, &D_800E4668.l[0], LIGHT_1);
    gSPLight(gDisplayListHead++, &D_800E4668.a, LIGHT_2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);
    load_texture_block_rgba16_mirror((u8*) d_course_bowsers_castle_thwomp_side, 0x00000020, 0x00000020);
    for (i = 0; i < gObjectParticle3_SIZE; i++) {
        objectIndex = gObjectParticle3[i];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if ((object->state > 0) && (Behaviour == States::MOVE_FAR)) {
                rsp_set_matrix_transformation(object->pos, object->orientation, object->sizeScaling);
                gSPVertex(gDisplayListHead++, (uintptr_t) D_0D005C00, 3, 0);
                gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D006930);
            }
        }
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007AE0);
    load_texture_block_ia8_nomirror(D_8018D490, 0x00000020, 0x00000020);
    func_8004B3C8(0);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        objectIndex = gObjectParticle2[i];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if ((object->state >= 2) && (Behaviour == States::MOVE_AND_ROTATE)) {
                func_8004B138(0x000000FF, 0x000000FF, 0x000000FF, (s32) object->primAlpha);
                D_80183E80[1] = func_800418AC(object->pos[0], object->pos[2], camera->pos);
                func_800431B0(object->pos, D_80183E80, object->sizeScaling, (Vtx*) D_0D005AE0);
            }
        }
    }
}

void OThwomp::DrawModel(s32 objectIndex) {
    if ((gObjectList[objectIndex].state >= 2) && (func_80072354(objectIndex, 0x00000040) != 0)) {
        FrameInterpolation_RecordOpenChild("Thwomp_Main", TAG_THWOMP(this));
        func_8004A7AC(objectIndex, 1.75f);
        rsp_set_matrix_transformation(gObjectList[objectIndex].pos, gObjectList[objectIndex].orientation,
                                      gObjectList[objectIndex].sizeScaling);
        OThwomp::ThwompLights(objectIndex);
        gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007828);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTLUT_pal256(gDisplayListHead++, d_course_bowsers_castle_thwomp_tlut);
        rsp_load_texture_mask((u8*) gObjectList[objectIndex].activeTexture, 0x00000010, 0x00000040, 4);
        gSPDisplayList(gDisplayListHead++, gObjectList[objectIndex].model);
        FrameInterpolation_RecordCloseChild();
    }
}

void OThwomp::TranslateThwompLights() {
    func_800419F8();
    D_800E4638.l[0].l.dir[0] = D_80165840[0];
    D_800E4638.l[0].l.dir[1] = D_80165840[1];
    D_800E4638.l[0].l.dir[2] = D_80165840[2];
}

void OThwomp::ThwompLights(s32 objectIndex) {
    // Why these don't just use `gSPSetLights1` calls...
    switch (gObjectList[objectIndex].type) { // hmm very strange 80165C18
        case 0:
            gSPLight(gDisplayListHead++, &D_800E4638.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4638.a, LIGHT_2);
            break;
        case 1:
            gSPLight(gDisplayListHead++, &D_800E4650.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4650.a, LIGHT_2);
            break;
        case 2:
            gSPLight(gDisplayListHead++, &D_800E4668.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4668.a, LIGHT_2);
            break;
        case 3:
            gSPLight(gDisplayListHead++, &D_800E4680.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4680.a, LIGHT_2);
            break;
        case 4:
            gSPLight(gDisplayListHead++, &D_800E4698.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4698.a, LIGHT_2);
            break;
        default:
            break;
    }
}

void OThwomp::func_80080DE4(s32 arg0) {
    Player* player;
    s32 var_v1;

    player = gPlayerOne;
    for (var_v1 = 0; var_v1 < NUM_PLAYERS; var_v1++, player++) {
        if (arg0 == player->unk_040) {
            player->soundEffects &= ~0x100;
            player->unk_040 = -1;
        }
    }
}

/** Behaviours **/

void OThwomp::StationaryBehaviour(s32 objectIndex) { // func_8007ED6C
    UNUSED s32 stackPadding[4];
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OThwomp::func_8007EC30(objectIndex);
            break;
        case 2:
            set_and_run_timer_object(objectIndex, 0x0000003C);
            break;
        case 3:
            func_80072568(objectIndex, 0x00000032);
            break;
        case 4:
            if (func_8007E59C(objectIndex) != 0) {
                func_800725E8(objectIndex, 0x0000012C, 2);
            } else {
                func_800726CC(objectIndex, 2);
            }
            break;
    }
    OThwomp::func_8007E63C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    gObjectList[objectIndex].direction_angle[1] = gObjectList[objectIndex].orientation[1];
    func_80073514(objectIndex);
}

// Stationary
void OThwomp::func_8007EC30(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    if (gIsMirrorMode != 0) {
        set_obj_direction_angle(objectIndex, 0U, -_faceDirection, 0U);
        set_obj_orientation(objectIndex, 0U, -_faceDirection, 0U);
    } else {
        set_obj_direction_angle(objectIndex, 0U, _faceDirection, 0U);
        set_obj_orientation(objectIndex, 0U, _faceDirection, 0U);
    }
    init_texture_object(objectIndex, (uint8_t*) d_course_bowsers_castle_thwomp_tlut,
                        (const char**) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object->model = (Gfx*) d_course_bowsers_castle_dl_thwomp;
    object->unk_01C[1] = 30.0f;
    set_object_flag(objectIndex, 0x05000220);
    object->type = 0;
    object->unk_0DF = 6;
    func_800724DC(objectIndex);
    object_next_state(objectIndex);
}

void OThwomp::MoveAndRotateBehaviour(s32 objectIndex) { // func_8007F5A8

    switch (gObjectList[objectIndex].state) { /* irregular */
        case 0:
            break;
        case 1:
            OThwomp::func_8007EE5C(objectIndex);
            break;
        case 3:
            func_80072568(objectIndex, 0x00000032);
            break;
        case 4:
            func_80086FD4(objectIndex);
            object_next_state(objectIndex);
            break;
    }
    OThwomp::func_8007E63C(objectIndex);
    func_8007F544(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

// MoveAndRotateBehaviour
void OThwomp::func_8007EE5C(s32 objectIndex) {
    Object* object;

    init_texture_object(objectIndex, (u8*) d_course_bowsers_castle_thwomp_tlut,
                        (const char**) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->model = (Gfx*) d_course_bowsers_castle_dl_thwomp;
    set_object_flag(objectIndex, 0x04000220);
    object->type = 0;
    object->unk_0DF = 6;
    func_80086E70(objectIndex);
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 20.0f, 0.0f);
    object->unk_01C[1] = 20.0f;
    if (gIsMirrorMode != 0) {
        set_obj_direction_angle(objectIndex, 0U, -_faceDirection, 0U);
        set_obj_orientation(objectIndex, 0U, -_faceDirection, 0U);
    } else {
        set_obj_direction_angle(objectIndex, 0U, _faceDirection, 0U);
        set_obj_orientation(objectIndex, 0U, _faceDirection, 0U);
    }
    object->unk_0AE = 1;
    if (object->primAlpha == 0) {
        object->unk_0DD = 1;
    } else {
        object->unk_0DD = 2;
    }
    object_next_state(objectIndex);
}

void OThwomp::MoveFarBehaviour(s32 objectIndex) { // func_8007FFC0
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OThwomp::func_8007FA08(objectIndex);
            break;
        case 3:
            func_80072568(objectIndex, 0x00000032);
            break;
        case 4:
            object_next_state(objectIndex);
            func_80086FD4(objectIndex);
            break;
    }
    OThwomp::func_8007E63C(objectIndex);
    OThwomp::func_8007FF5C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

// MoveFarBehaviour
void OThwomp::func_8007FA08(s32 objectIndex) {
    Object* object;

    init_texture_object(objectIndex, (u8*) d_course_bowsers_castle_thwomp_tlut,
                        (const char**) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->model = (Gfx*) d_course_bowsers_castle_dl_thwomp;
    set_object_flag(objectIndex, 0x04000220);
    object->type = 0;
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    if (gIsMirrorMode != 0) {
        set_obj_orientation(objectIndex, 0U, _faceDirection, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, -_faceDirection, 0U);
    }
    object->velocity[0] = 0.0f;
    object->direction_angle[1] = object->orientation[1];
    object->unk_0DD = 1;
    object->unk_0DF = 8;
    object->offset[1] = 15.0f;
    object->unk_01C[1] = 15.0f;
    object_next_state(objectIndex);
}

void OThwomp::func_8007FF5C(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0DD) {
        case 1:
            OThwomp::func_8007FB48(objectIndex);
            break;
        case 2:
            OThwomp::func_8007FEA4(objectIndex);
            break;
    }
}

void OThwomp::func_8007FB48(s32 objectIndex) {
    s32 var_v0;
    UNUSED s32 stackPadding;
    Player* player;

    player = &gPlayerOne[gObjectList[objectIndex].unk_0D1];
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            gObjectList[objectIndex].unk_0B0 = 0x00A0;
            gObjectList[objectIndex].offset[0] = 0.0f;
            gObjectList[objectIndex].offset[2] = 0.0f;
            gObjectList[objectIndex].velocity[2] = 0.0f;
            func_80086FD4(objectIndex);
            break;
        case 2:
            gObjectList[objectIndex].velocity[0] = player->speed * xOrientation * 1.25;
            if (gObjectList[objectIndex].unk_048 >= gObjectList[objectIndex].unk_0B0) {
                if (gObjectList[objectIndex].unk_0B0 == gObjectList[objectIndex].unk_048) {
                    if (_rand & 1) { // D_8018D400 & 1
                        gObjectList[objectIndex].velocity[2] = 1.5f;
                    } else {
                        gObjectList[objectIndex].velocity[2] = -1.5f;
                    }
                }
                if (gObjectList[objectIndex].velocity[2] >= 0.0) {
                    if (gObjectList[objectIndex].offset[2] >= 40.0) {
                        gObjectList[objectIndex].velocity[2] = -1.5f;
                    }
                } else if ((f64) gObjectList[objectIndex].offset[2] <= -40.0) {
                    gObjectList[objectIndex].velocity[2] = 1.5f;
                }
            }
            object_add_velocity_offset_xz(objectIndex);
            if (gObjectList[objectIndex].unk_0B0 < 0x65) {
                gObjectList[objectIndex].orientation[1] = func_800417B4(
                    gObjectList[objectIndex].orientation[1], (gObjectList[objectIndex].direction_angle[1] + 0x8000));
                if (gObjectList[objectIndex].unk_0B0 == 0x0064) {
                    gObjectList[objectIndex].textureListIndex = 1;
                }
            }
            var_v0 = 0;
            if (gIsMirrorMode != 0) {
                if (gObjectList[objectIndex].offset[0] <= -1000.0) {
                    var_v0 = 1;
                }
            } else if (gObjectList[objectIndex].offset[0] >= 1000.0) {
                var_v0 = 1;
            }
            gObjectList[objectIndex].unk_0B0--;
            if ((gObjectList[objectIndex].unk_0B0 == 0) || (var_v0 != 0)) {
                gObjectList[objectIndex].unk_034 = 0.0f;
                func_800726CC(objectIndex, 3);
                func_80086FD4(objectIndex);
            }
            break;
        case 4:
            f32_step_towards(&gObjectList[objectIndex].offset[2], 0.0f, 2.0f);
            f32_step_towards(gObjectList[objectIndex].offset, 0.0f, 5.0f);
            if ((gObjectList[objectIndex].offset[0] + gObjectList[objectIndex].offset[2]) == 0.0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 5:
            gObjectList[objectIndex].orientation[1] =
                func_800417B4(gObjectList[objectIndex].orientation[1], gObjectList[objectIndex].direction_angle[1]);
            if (gObjectList[objectIndex].orientation[1] == gObjectList[objectIndex].direction_angle[1]) {
                func_800722CC(objectIndex, 8);
                func_80086FD4(objectIndex);
                gObjectList[objectIndex].textureListIndex = 0;
            }
            break;
        case 0:
        case 3:
        default:
            break;
    }
}

void OThwomp::func_8007FEA4(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->unk_0AE) {
        case 1:
            if (f32_step_towards(&object->offset[0], object->unk_01C[0], 5.0f) != 0) {
                func_800726CC(objectIndex, 3);
                func_80086FD4(objectIndex);
                break;
            }
        case 0:
        case 2:
            break;
        case 3:
            if (f32_step_towards(&object->offset[0], 0.0f, 5.0f) != 0) {
                func_80086FD4(objectIndex);
                func_800722CC(objectIndex, 8);
            }
            break;
    }
}

void OThwomp::StationaryFastBehaviour(s32 objectIndex) { // func_800801FC
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 0:
            break;
        case 1:
            OThwomp::func_80080078(objectIndex);
            break;
        case 2:
            set_and_run_timer_object(objectIndex, object->timer);
            break;
        case 3:
            func_80072568(objectIndex, 0x00000032);
            break;
        case 4:
            object->timer = 0x0000003C;
            func_800726CC(objectIndex, 2);
            break;
    }
    OThwomp::func_8007E63C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

// StationaryFastBehaviour
void OThwomp::func_80080078(s32 objectIndex) { // func_80080078
    Object* object;

    init_texture_object(objectIndex, (u8*) d_course_bowsers_castle_thwomp_tlut,
                        (const char**) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->model = (Gfx*) d_course_bowsers_castle_dl_thwomp;
    set_object_flag(objectIndex, 0x04000220);
    object->type = 2;
    object->unk_0DF = 8;
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object->unk_01C[1] = 30.0f;
    if (gIsMirrorMode != 0) {
        set_obj_orientation(objectIndex, 0U, -_faceDirection, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, _faceDirection, 0U);
    }
    switch (object->primAlpha) { /* irregular */
        case 0:
            object->timer = 2;
            break;
        case 1:
            object->timer = 0x0000003C;
            break;
        case 2:
            object->timer = 0x00000078;
            break;
        case 3:
            object->timer = 0x000000B4;
            break;
    }
    func_800724DC(objectIndex);
    object_next_state(objectIndex);
}

void OThwomp::JailedBehaviour(s32 objectIndex) { // func_80080408
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OThwomp::func_800802C0(objectIndex);
            break;
        case 2:
            func_8008A6DC(objectIndex, 100.0f);
            if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
                object_next_state(objectIndex);
            }
            break;
        case 3:
            if (func_800730BC(objectIndex, 3, 5, 1, 6, 6) != 0) {
                gObjectList[objectIndex].textureListIndex = 0;
            }
            break;
        case 4:
            if (set_and_run_timer_object(objectIndex, 0x0000012C) != 0) {
                func_800726CC(objectIndex, 2);
            }
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
    func_80073514(objectIndex);
}

// JailedBehaviour
void OThwomp::func_800802C0(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 0;
    init_texture_object(objectIndex, (u8*) d_course_bowsers_castle_thwomp_tlut,
                        (const char**) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object->model = (Gfx*) d_course_bowsers_castle_dl_thwomp;
    object->textureListIndex = 0;
    // object->sizeScaling = 1.5f;
    set_object_flag(objectIndex, 0x05000220);
    object->type = 1;
    object->unk_0DF = 6;
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->surfaceHeight = 0.0f;
    object->origin_pos[1] = 0.0f;
    object->offset[1] = 10.0f;
    object->unk_01C[1] = 10.0f;
    if (gIsMirrorMode != 0) {
        set_obj_orientation(objectIndex, 0U, -_faceDirection, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, _faceDirection, 0U);
    }
    object->offset[0] = 0.0f;
    object->offset[2] = 0.0f;
    func_800724DC(objectIndex);
    object_next_state(objectIndex);
}

void OThwomp::SlidingBehaviour(s32 objectIndex) { // func_800808CC
    switch (gObjectList[objectIndex].state) {     /* irregular */
        case 0:
            break;
        case 1:
            OThwomp::func_80080524(objectIndex);
            break;
        case 2:
            func_800730BC(objectIndex, 3, 5, 1, 6, -1);
            break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        OThwomp::func_8007E63C(objectIndex);
        OThwomp::func_8008085C(objectIndex);
        func_80073514(objectIndex);
        if (gGamestate != 9) {
            if (((_rand & 0x3F) == 0) && (gObjectList[objectIndex].state == 2)) { // D_8018D40C == 0 &&
                func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
            }
        } else if ((gCutsceneShotTimer < 0xBF) && (((s16) gCutsceneShotTimer % 88) == 0x0000001E)) {
            func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity,
                          SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
        }
    }
}

void OThwomp::func_8008085C(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0DD) {
        case 1:
            OThwomp::func_800806BC(objectIndex);
            break;
        case 2:
            OThwomp::func_8008078C(objectIndex);
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void OThwomp::func_800806BC(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 0:
            break;
        case 1:
            if (f32_step_towards(&gObjectList[objectIndex].offset[2], 250.0f, gObjectList[objectIndex].velocity[2]) !=
                0) {
                gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            if (f32_step_towards(&gObjectList[objectIndex].offset[2], 0.0f, gObjectList[objectIndex].velocity[2]) !=
                0) {
                gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
                func_8008701C(objectIndex, 1);
            }
            break;
    }
}

void OThwomp::func_8008078C(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 0:
            break;
        case 1:
            if (f32_step_towards(&gObjectList[objectIndex].offset[2], -250.0f, gObjectList[objectIndex].velocity[2]) !=
                0) {
                gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            if (f32_step_towards(&gObjectList[objectIndex].offset[2], 0.0f, gObjectList[objectIndex].velocity[2]) !=
                0) {
                gObjectList[objectIndex].velocity[2] = -gObjectList[objectIndex].velocity[2];
                func_8008701C(objectIndex, 1);
            }
            break;
    }
}

// SlidingBehaviour
void OThwomp::func_80080524(s32 objectIndex) {
    Object* object;

    init_texture_object(objectIndex, (u8*) d_course_bowsers_castle_thwomp_tlut,
                        (const char**) d_course_bowsers_castle_thwomp_faces, 0x10U, (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->model = (Gfx*) d_course_bowsers_castle_dl_thwomp;
    object->textureListIndex = 0;
    set_object_flag(objectIndex, 0x04000220);
    object->type = 0;
    object->unk_0DF = 0x0A;
    func_80086E70(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    object->surfaceHeight = 70.0f;
    object->origin_pos[1] = 70.0f;
    object->unk_01C[1] = 0.0f;
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    if ((gIsMirrorMode != 0) || (gGamestate == CREDITS_SEQUENCE)) {
        set_obj_orientation(objectIndex, 0U, -_faceDirection, 0U);
    } else {
        set_obj_orientation(objectIndex, 0U, _faceDirection, 0U);
    }
    switch (object->primAlpha) { /* irregular */
        case 0:
            object->unk_0DD = 2;
            object->velocity[2] = -1.0f;
            break;
        case 1:
            object->unk_0DD = 2;
            object->velocity[2] = -1.5f;
            break;
    }
    func_800722A4(objectIndex, 0x00000080);
    object_next_state(objectIndex);
}

void OThwomp::func_8007E63C(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 0x32:
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], gObjectList[objectIndex].unk_01C[1] + 15.0,
                                    1.5f) != 0) {
                set_object_flag(objectIndex, 0x00000200);
                func_800722A4(objectIndex, 1);
                func_800722CC(objectIndex, 2);
                object_next_state(objectIndex);
            }
            break;
        case 0x33:
            if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 0.0f, 2.0f) != 0) {
                if (gObjectList[objectIndex].offset[1] >= 16.0f) {
                    gObjectList[objectIndex].textureListIndex = 0;
                } else if (gObjectList[objectIndex].offset[1] >= 8.0f) {
                    gObjectList[objectIndex].textureListIndex = 1;
                } else {
                    gObjectList[objectIndex].textureListIndex = 2;
                }
                func_800722CC(objectIndex, 1);
                if (is_obj_flag_status_active(objectIndex, 0x00010000) != 0) {
                    func_800722A4(objectIndex, 0x00000010);
                    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                        func_800722A4(objectIndex, 0x00000020);
                    }
                }
                if (func_80072320(objectIndex, 2) != 0) {
                    func_800726CC(objectIndex, 0x00000064);
                } else {
                    object_next_state(objectIndex);
                }
            }
            break;
        case 0x34:
            func_80072AAC(objectIndex, 3, 6);
            break;
        case 0x35:
            func_80072AAC(objectIndex, 2, 0x00000032);
            break;
        case 0x36:
            if (gObjectList[objectIndex].offset[1] >= 20.0f) {
                gObjectList[objectIndex].textureListIndex = 0;
            } else if (gObjectList[objectIndex].offset[1] >= 18.0f) {
                gObjectList[objectIndex].textureListIndex = 1;
            }
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], gObjectList[objectIndex].unk_01C[1], 0.5f) !=
                0) {
                clear_object_flag(objectIndex, 0x00000200);
                func_8007266C(objectIndex);
            }
            break;
        case 0x64:
            func_80072E54(objectIndex, 3, 5, 1, 8, 0);
            break;
        case 0x65:
            set_and_run_timer_object(objectIndex, 0x0000001E);
            break;
        case 0x66:
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], 20.0f, 1.5f) != 0) {
                object_next_state(objectIndex);
            }
            break;
        case 0x67:
            if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 0.0f, 1.5f) != 0) {
                if (is_obj_flag_status_active(objectIndex, 0x00020000) != 0) {
                    func_800722A4(objectIndex, 0x00000010);
                    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                        func_800722A4(objectIndex, 0x00000020);
                    }
                }
                object_next_state(objectIndex);
            }
            break;
        case 0x68:
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], 12.0f, 1.5f) != 0) {
                object_next_state(objectIndex);
            }
            break;
        case 0x69:
            if (f32_step_down_towards(&gObjectList[objectIndex].offset[1], 0.0f, 1.5f) != 0) {
                if (is_obj_flag_status_active(objectIndex, 0x00020000) != 0) {
                    func_800722A4(objectIndex, 0x00000010);
                    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                        func_800722A4(objectIndex, 0x00000020);
                    }
                }
                func_800C98B8(gObjectList[objectIndex].pos, gObjectList[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
                object_next_state(objectIndex);
            }
            break;
        case 0x6A:
            if (func_8007326C(objectIndex, 5, 3, 1, 6, 3) != 0) {
                OThwomp::func_80080DE4(objectIndex);
            }
            break;
        case 0x6B:
            if (gObjectList[objectIndex].offset[1] >= 22.0f) {
                gObjectList[objectIndex].textureListIndex = 0;
            } else if (gObjectList[objectIndex].offset[1] >= 20.0f) {
                gObjectList[objectIndex].textureListIndex = 1;
            } else if (gObjectList[objectIndex].offset[1] >= 18.0f) {
                gObjectList[objectIndex].textureListIndex = 2;
            } else if (gObjectList[objectIndex].offset[1] >= 16.0f) {
                gObjectList[objectIndex].textureListIndex = 3;
            } else if (gObjectList[objectIndex].offset[1] >= 14.0f) {
                gObjectList[objectIndex].textureListIndex = 4;
            } else {
                func_800730BC(objectIndex, 3, 5, 1, 6, -1);
            }
            if (f32_step_up_towards(&gObjectList[objectIndex].offset[1], gObjectList[objectIndex].unk_01C[1], 0.5f) !=
                0) {
                set_object_timer_state(objectIndex, 0);
                object_next_state(objectIndex);
            }
            break;
        case 0x6C:
            if (set_and_run_timer_object(objectIndex, 0x00000064) != 0) {
                func_800722CC(objectIndex, 2);
                clear_object_flag(objectIndex, 0x00000200);
                func_8007266C(objectIndex);
            }
            break;
        case 0xC8:
            if (set_and_run_timer_object(objectIndex, 0x0000012C) != 0) {
                func_80072320(objectIndex, 0x00000080);
                func_80072428(objectIndex);
                func_800726CC(objectIndex, 1);
            }
            break;
        case 0x12C:
            if (func_80073E18(objectIndex, &gObjectList[objectIndex].orientation[1], 0x0400U, 0x00008000) != 0) {
                func_800722CC(objectIndex, 4);
                func_8007266C(objectIndex);
            }
            break;
    }
}

void OThwomp::DrawEditorProperties() {
    ImGui::Text("Behaviour");
    ImGui::SameLine();

    int32_t behaviour = static_cast<int32_t>(Behaviour);
    const char* items[] = { "Disabled", "Stationary", "Move and Rotate", "Move Far", "Stationary Fast", "Slide", "Jailed" };

    if (ImGui::Combo("##Behaviour", &behaviour, items, IM_ARRAYSIZE(items))) {
        Behaviour = static_cast<OThwomp::States>(behaviour);
        gObjectList[_objectIndex].unk_0D5 = static_cast<uint8_t>(behaviour);
        gObjectList[_objectIndex].state = behaviour;
    }

    ImGui::Text("Location");
    ImGui::SameLine();
    FVector location = GetLocation();
    if (ImGui::DragFloat3("##Location", (float*)&location)) {
        Translate(location);
        gEditor.eObjectPicker.eGizmo.Pos = location;
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPos")) {
        FVector location = FVector(0, 0, 0);
        Translate(location);
        gEditor.eObjectPicker.eGizmo.Pos = location;
    }

    ImGui::Text("Rotation");
    ImGui::SameLine();

    IRotator objRot = GetRotation();

    // Convert to temporary int values (to prevent writing 32bit values to 16bit variables)
    int rot[3] = {
        objRot.pitch,
        objRot.yaw,
        objRot.roll
    };

    if (ImGui::DragInt3("##Rotation", rot, 5.0f)) {
        for (size_t i = 0; i < 3; i++) {
            // Wrap around 0–65535
            rot[i] = (rot[i] % 65536 + 65536) % 65536;
        }
        IRotator newRot;
        newRot.Set(
            static_cast<uint16_t>(rot[0]),
            static_cast<uint16_t>(rot[1]),
            static_cast<uint16_t>(rot[2])
        );
        Rotate(newRot);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetRot")) {
        IRotator rot = IRotator(0, 0, 0);
        Rotate(rot);
    }

    FVector scale = GetScale();
    ImGui::Text("Scale   ");
    ImGui::SameLine();

    if (ImGui::DragFloat3("##Scale", (float*)&scale, 0.1f)) {
        SetScale(scale);
        gObjectList[_objectIndex].sizeScaling = scale.y;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetScale")) {
        FVector scale = FVector(1.0f, 1.0f, 1.0f);
        SetScale(scale);
        gObjectList[_objectIndex].sizeScaling = 1.0f;
    }

    int32_t primAlpha = PrimAlpha;
    ImGui::Text("Prim Alpha");
    ImGui::SameLine();

    if (ImGui::InputInt("##PrimAlpha", (int*)&primAlpha)) {
        PrimAlpha = static_cast<int16_t>(primAlpha);
        gObjectList[_objectIndex].primAlpha = static_cast<int16_t>(primAlpha);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPrimAlpha")) {
        PrimAlpha = 0;
        gObjectList[_objectIndex].primAlpha = 0;
    }

    int32_t boundingBoxSize = static_cast<int32_t>(BoundingBoxSize);
    ImGui::Text("Bounding Box Size");
    ImGui::SameLine();

    if (ImGui::InputInt("##BoundingBoxSize", (int*)&boundingBoxSize)) {
        if (boundingBoxSize < 0) boundingBoxSize = 0;
        BoundingBoxSize = static_cast<OThwomp::States>(boundingBoxSize);
        gObjectList[_objectIndex].boundingBoxSize = static_cast<uint16_t>(boundingBoxSize);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetBoundingBoxSize")) {
        BoundingBoxSize = 0;
        gObjectList[_objectIndex].boundingBoxSize = 0;
    }
}
