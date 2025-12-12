#include <libultraship.h>
#include <libultra/gbi.h>
#include <course_offsets.h>
#include <macros.h>
#include <mk64.h>
#include <align_asset_macro.h>
#include <sounds.h>
#include <common_structs.h>
#include <actor_types.h>
#include <defines.h>
#include <macros.h>
#include <stdio.h>

#include "code_800029B0.h"
#include "main.h"
#include "math_util.h"
#include "memory.h"
#include "actors_extended.h"
#include "actors.h"
#include "waypoints.h"
#include "macros.h"
#include "code_80005FD0.h"
#include "update_objects.h"
#include "effects.h"
#include "collision.h"
#include "audio/external.h"
#include <assets/models/common_data.h>
#include "courses/all_course_data.h"
#include "main.h"
#include <assets/textures/other_textures.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_data.h>
#include <assets/models/tracks/luigi_raceway/luigi_raceway_data.h>
#include <assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_data.h>
#include <assets/other/tracks/dks_jungle_parkway/dks_jungle_parkway_data.h>
#include <assets/models/tracks/wario_stadium/wario_stadium_data.h>
#include <assets/models/tracks/frappe_snowland/frappe_snowland_data.h>
#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"
#include "engine/CoreMath.h"

#include <assets/other/tracks/moo_moo_farm/moo_moo_farm_data.h>

// Appears to be textures
// or tluts
char* texture_green_shell[] = {
    texture_green_shell_0, texture_green_shell_1, texture_green_shell_2, texture_green_shell_3,
    texture_green_shell_4, texture_green_shell_5, texture_green_shell_6, texture_green_shell_7,
};
char* gTextureBlueshell[] = {
    texture_blue_shell_0, texture_blue_shell_1, texture_blue_shell_2, texture_blue_shell_3,
    texture_blue_shell_4, texture_blue_shell_5, texture_blue_shell_6, texture_blue_shell_7,
};
char* texture_red_shell[] = {
    texture_red_shell_0, texture_red_shell_1, texture_red_shell_2, texture_red_shell_3,
    texture_red_shell_4, texture_red_shell_5, texture_red_shell_6, texture_red_shell_7,
};

struct Actor* gActorHotAirBalloonItemBox;
s8 gTLUTRedShell[512]; // tlut 256
u16 D_802BA260;

/**
 * Once the amount of spawned player red and green shell count has reached 21 or higher
 * the game will cleanup any dead red or green shells by deleting their actors.
 */
void cleanup_red_and_green_shells(struct ShellActor* shell) {
    s32 actorIndex;
    struct ShellActor* compare;

    // try finding the dead green shell
    for (actorIndex = gNumPermanentActors; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) CM_GetActor(actorIndex);
        if ((shell != compare) && !(compare->flags & ACTOR_IS_NOT_EXPIRED) && (compare->type == ACTOR_GREEN_SHELL)) {
            if (compare->state == MOVING_SHELL) {
                delete_actor_in_unexpired_actor_list(actorIndex);
            }
            gNumSpawnedShells--;
            destroy_actor((struct Actor*) compare);
            return;
        }
    }

    // try finding the dead red shell
    for (actorIndex = gNumPermanentActors; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) CM_GetActor(actorIndex);
        if ((shell != compare) && !(compare->flags & ACTOR_IS_NOT_EXPIRED) && (compare->type == ACTOR_RED_SHELL)) {
            switch (compare->state) {
                case MOVING_SHELL:
                case RED_SHELL_LOCK_ON:
                case TRIPLE_GREEN_SHELL:
                case GREEN_SHELL_HIT_A_RACER:
                case BLUE_SHELL_LOCK_ON:
                case BLUE_SHELL_TARGET_ELIMINATED:
                    delete_actor_in_unexpired_actor_list(actorIndex);
                case DESTROYED_SHELL:
                    gNumSpawnedShells -= 1;
                    destroy_actor((struct Actor*) compare);
                    return;
                default:
                    break;
            }
        }
    }

    // try finding the green shell
    for (actorIndex = gNumPermanentActors; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) CM_GetActor(actorIndex);
        if ((shell != compare) && (compare->type == ACTOR_GREEN_SHELL)) {
            switch (compare->state) {
                case MOVING_SHELL:
                    delete_actor_in_unexpired_actor_list(actorIndex);
                case DESTROYED_SHELL:
                    gNumSpawnedShells -= 1;
                    destroy_actor((struct Actor*) compare);
                    return;
            }
        }
    }

    // try finding the red or blue shell
    for (actorIndex = gNumPermanentActors; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) CM_GetActor(actorIndex);
        if ((shell != compare) && (compare->type == ACTOR_RED_SHELL)) {
            switch (compare->state) {
                case MOVING_SHELL:
                case RED_SHELL_LOCK_ON:
                case TRIPLE_GREEN_SHELL:
                case GREEN_SHELL_HIT_A_RACER:
                case BLUE_SHELL_LOCK_ON:
                case BLUE_SHELL_TARGET_ELIMINATED:
                    delete_actor_in_unexpired_actor_list(actorIndex);
                case DESTROYED_SHELL:
                    gNumSpawnedShells -= 1;
                    destroy_actor((struct Actor*) compare);
                    return;
            }
        }
    }
}

// Sets introductory values for a new actor (ex. Banana).
void actor_init(struct Actor* actor, Vec3f startingPos, Vec3s startingRot, Vec3f startingVelocity, s16 actorType) {
    vec3f_copy_return(actor->pos, startingPos);
    vec3s_copy(actor->rot, startingRot);
    vec3f_copy_return(actor->velocity, startingVelocity);
    actor->type = actorType;
    actor->flags = -0x8000;
    actor->unk_04 = 0;
    actor->state = 0;
    actor->unk_08 = 0.0f;
    actor->boundingBoxSize = 0.0f;
    actor->model = NULL;
    func_802AAAAC(&actor->unk30);
    switch (actorType) {
        case ACTOR_BOX_TRUCK:
            if ((s32) D_802BA260 >= 3) {
                D_802BA260 = 0;
            }
            actor->state = (s16) D_802BA260;
            D_802BA260 += 1;
            break;
        case ACTOR_YOSHI_EGG:
            actor->flags |= 0x4000;
            actor->unk_08 = 70.0f;
            actor->boundingBoxSize = 20.0f;
            actor->velocity[0] = actor->pos[0];
            actor->velocity[1] = actor->pos[1];
            actor->velocity[2] = actor->pos[2] + 70.0f;
            actor->model = d_course_yoshi_valley_dl_egg_lod0;
            break;
        case ACTOR_KIWANO_FRUIT:
            actor->state = 0;
            actor->rot[0] = 0;
            actor->rot[1] = 0;
            actor->rot[2] = 0;
            actor->boundingBoxSize = 2.0f;
            actor->model = d_course_dks_jungle_parkway_dl_kiwano_fruit;
            break;
        case ACTOR_FALLING_ROCK:
            actor->flags |= 0x4000;
            actor->boundingBoxSize = 10.0f;
            actor->model = d_course_choco_mountain_dl_falling_rock;
            break;
        case ACTOR_TRAIN_ENGINE:
            actor->unk_08 = 10.0f;
            break;
        case ACTOR_BANANA:
            actor->flags = actor->flags | 0x4000 | 0x1000;
            actor->boundingBoxSize = 2.0f;
            actor->model = common_model_flat_banana;
            break;
        case ACTOR_GREEN_SHELL:
            gNumSpawnedShells += 1;
            actor->unk_04 = 0;
            actor->boundingBoxSize = 4.0f;
            actor->flags = actor->flags | 0x4000 | 0x2000 | 0x1000;
            if ((s32) gNumSpawnedShells >= 0x15) {
                cleanup_red_and_green_shells((struct ShellActor*) actor);
            }
            break;
        case ACTOR_RED_SHELL:
            gNumSpawnedShells += 1;
            actor->unk_04 = 0;
            actor->boundingBoxSize = 4.0f;
            actor->flags = actor->flags | 0x4000 | 0x2000 | 0x1000;
            if ((s32) gNumSpawnedShells >= 0x15) {
                cleanup_red_and_green_shells((struct ShellActor*) actor);
            }
            break;
        case ACTOR_TREE_MARIO_RACEWAY:
            gNumSpawnedShells += 1;
            actor->flags |= 0x4000;
            actor->state = 0x0043;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 20.0f;
            actor->model = d_course_mario_raceway_dl_tree;
            break;
        case ACTOR_MARIO_SIGN:
            actor->flags |= 0x4000;
            actor->model = d_course_mario_raceway_dl_sign;
            break;
        case ACTOR_TREE_YOSHI_VALLEY:
            actor->flags |= 0x4000;
            actor->state = 0x0043;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 23.0f;
            actor->model = d_course_yoshi_valley_dl_tree;
            break;
        case ACTOR_TREE_ROYAL_RACEWAY:
            actor->flags |= 0x4000;
            actor->state = 0x0043;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 17.0f;
            actor->model = d_course_royal_raceway_dl_tree;
            break;
        case ACTOR_TREE_MOO_MOO_FARM:
            actor->state = 0x0043;
            actor->flags = -0x8000;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 17.0f;
            actor->model = d_course_moo_moo_farm_dl_tree;
            break;
        case ACTOR_TREE_LUIGI_RACEWAY:
            actor->flags |= 0x4000;
            actor->state = 0x0043;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 17.0f;
            actor->model = d_course_luigi_raceway_dl_FC70;
            break;
        case ACTOR_TREE_PEACH_CASTLE:
            actor->state = 0x0043;
            actor->flags = -0x8000;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 17.0f;
            actor->model = d_course_royal_raceway_dl_castle_tree;
            break;
        case ACTOR_BUSH_BOWSERS_CASTLE:
            actor->flags |= 0x4000;
            actor->state = 0x0043;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 17.0f;
            actor->model = d_course_bowsers_castle_dl_bush;
            break;
        case ACTOR_TREE_FRAPPE_SNOWLAND:
            actor->flags |= 0x4000;
            actor->state = 0x0043;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 17.0f;
            actor->model = d_course_frappe_snowland_dl_tree;
            break;
        case ACTOR_CACTUS1_KALAMARI_DESERT:
            actor->flags |= 0x4000;
            actor->state = 0x0019;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 7.0f;
            actor->model = d_course_kalimari_desert_dl_cactus1;
            break;
        case ACTOR_CACTUS2_KALAMARI_DESERT:
            actor->flags |= 0x4000;
            actor->state = 0x0019;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 7.0f;
            actor->model = d_course_kalimari_desert_dl_cactus2;
            break;
        case ACTOR_CACTUS3_KALAMARI_DESERT:
            actor->flags |= 0x4000;
            actor->state = 0x0019;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 7.0f;
            actor->model = d_course_kalimari_desert_dl_cactus3;
            break;
        case ACTOR_PALM_TREE:
            actor->flags |= 0x4000;
            actor->state = 0x003C;
            actor->boundingBoxSize = 3.0f;
            actor->unk_08 = 13.0f;
            break;
        case ACTOR_FAKE_ITEM_BOX:
            actor->flags = actor->flags | 0x4000 | 0x1000;
            actor->unk_08 = 0.35f;
            actor->boundingBoxSize = 1.925f;
            check_bounding_collision(&actor->unk30, 1.925f, actor->pos[0], actor->pos[1], actor->pos[2]);
            break;
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            actor->flags |= 0x4000;
            actor->unk_04 = 0;
            actor->state = 5;
            actor->boundingBoxSize = 5.5f;
            actor->model = D_0D003090;
            break;
        case ACTOR_ITEM_BOX:
            actor->flags |= 0x4000;
            actor->unk_04 = 0;
            actor->state = 0;
            actor->boundingBoxSize = 5.5f;
            actor->model = D_0D003090;
            break;
        case ACTOR_PIRANHA_PLANT:
            actor->flags |= 0x4000;
            actor->state = 0x001E;
            actor->boundingBoxSize = 5.0f;
            actor->model = d_course_mario_raceway_dl_piranha_plant;
            break;
        default:
            break;
    }
}

void actor_not_rendered(Camera* arg0, struct Actor* arg1) {
    switch (arg0 - camera1) {
        case PLAYER_ONE:
            arg1->flags &= ~(1 << PLAYER_ONE);
            break;
        case PLAYER_TWO:
            arg1->flags &= ~(1 << PLAYER_TWO);
            break;
        case PLAYER_THREE:
            arg1->flags &= ~(1 << PLAYER_THREE);
            break;
        case PLAYER_FOUR:
            arg1->flags &= ~(1 << PLAYER_FOUR);
            break;
    }
}

void actor_rendered(Camera* arg0, struct Actor* arg1) {
    switch (arg0 - camera1) {
        case PLAYER_ONE:
            arg1->flags |= 1 << PLAYER_ONE;
            break;
        case PLAYER_TWO:
            arg1->flags |= 1 << PLAYER_TWO;
            break;
        case PLAYER_THREE:
            arg1->flags |= 1 << PLAYER_THREE;
            break;
        case PLAYER_FOUR:
            arg1->flags |= 1 << PLAYER_FOUR;
            break;
    }
}

void func_80297340(Camera* arg0) {
    Mat4 mtx;
    s16 temp = D_8015F8D0[2];
    s32 maxObjectsReached;

    if (gGamestate == CREDITS_SEQUENCE) {
        return;
    }

    mtxf_translate(mtx, D_8015F8D0);

    maxObjectsReached = render_set_position(mtx, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (temp < arg0->pos[2]) {
        if (bFog) {

            gDPSetFogColor(gDisplayListHead++, gFogColour.r, gFogColour.g, gFogColour.b, gFogColour.a);
            gSPDisplayList(gDisplayListHead++, D_0D001C20);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D001B90);
        }
    } else if (bFog) {

        gDPSetFogColor(gDisplayListHead++, gFogColour.r, gFogColour.g, gFogColour.b, gFogColour.a);
        gSPDisplayList(gDisplayListHead++, D_0D001C88);
    } else {
        gSPDisplayList(gDisplayListHead++, D_0D001BD8);
    }
}

UNUSED void func_80297524(uintptr_t addr, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(addr), G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void func_802976D8(Vec3s arg0) {
    arg0[0] = 0x4000;
    arg0[1] = 0;
    arg0[2] = 0;
}

void func_802976EC(Collision* arg0, Vec3s arg1) {
    f32 x, y, z;

    if (arg0->unk34 == 0) {
        func_802976D8(arg1);
        return;
    }
    // sp1C = arg0->unk30;

    x = arg0->orientationVector[0];
    y = arg0->orientationVector[1];
    z = arg0->orientationVector[2];

    arg1[0] = atan2s(z, y) + 0x4000;
    arg1[1] = 0;
    arg1[2] = atan2s(x, y);
}

void func_80297760(struct Actor* arg0, Vec3f arg1) {
    arg1[0] = arg0->pos[0];
    arg1[1] = arg0->pos[1];
    arg1[2] = arg0->pos[2];
    arg1[1] = calculate_surface_height(arg1[0], arg1[1], arg1[2], arg0->unk30.meshIndexZX);
}

void func_802977B0(Player* player) {
    player->tyres[FRONT_RIGHT].unk_14 |= 2;
    player->tyres[FRONT_LEFT].unk_14 |= 2;
    player->tyres[BACK_RIGHT].unk_14 |= 2;
    player->tyres[BACK_LEFT].unk_14 |= 2;
}

void func_802977E4(Player* player) {
    player->tyres[FRONT_RIGHT].unk_14 &= ~2 & 0xFFFF;
    player->tyres[FRONT_LEFT].unk_14 &= ~2 & 0xFFFF;
    player->tyres[BACK_RIGHT].unk_14 &= ~2 & 0xFFFF;
    player->tyres[BACK_LEFT].unk_14 &= ~2 & 0xFFFF;
}

// Generate the red shell tlut by invert green the green one
void init_red_shell_texture(void) {
    s16* tlut = (s16*) LOAD_ASSET(common_tlut_green_shell);
    s16* red_shell_texture = (s16*) &gTLUTRedShell[0];
    s16* green_shell_texture = (s16*) tlut;
    s16 color_pixel, red_color, green_color, blue_color, alpha_color;
    s32 i;

    for (i = 0; i < 256; i++) {
        color_pixel = BSWAP16(*green_shell_texture);
        red_color = BSWAP16(color_pixel & 0xF800);
        green_color = BSWAP16(color_pixel & 0x7C0);
        blue_color = BSWAP16(color_pixel & 0x3E);
        alpha_color = BSWAP16(color_pixel & 0x1);

        *red_shell_texture = (red_color >> 5) | (green_color << 5) | blue_color | alpha_color; // Invert green to red
        green_shell_texture++;
        red_shell_texture++;
    }
}

UNUSED void func_80297944(void) {};

void func_8029794C(Vec3f pos, Vec3s rot, f32 scale) {
    Mat4 sp20;
    pos[1] += 2.0f;

    mtxf_pos_rotation_xyz(sp20, pos, rot);
    mtxf_scale(sp20, scale);
    if (render_set_position(sp20, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, D_0D007B20);
        pos[1] -= 2.0f;
    }
}

void func_802979F8(struct Actor* arg0, UNUSED f32 arg1) {
    Vec3f pos;
    Vec3s rot;

    if (arg0->unk30.unk34 != 0) {

        func_802976EC(&arg0->unk30, rot); // arg0 + 0x30
        func_80297760(arg0, pos);
        func_8029794C(pos, rot, 0.45f);
    }
}

#include "actors/cow/render.inc.c"

#include "actors/yoshi_egg/update.inc.c"

void update_actor_static_plant(struct Actor* arg0) {
    if (((arg0->flags & 0x800) == 0) && ((arg0->flags & 0x400) != 0)) {
        arg0->pos[1] = arg0->pos[1] + 4.0f;
        if (arg0->pos[1] > 800.0f) {
            arg0->flags |= 0x800;
        }
    }
}

#include "actors/kiwano_fruit/update.inc.c"

#include "actors/paddle_boat/update.inc.c"

#include "actors/train/update.inc.c"

#include "actors/piranha_plant/update.inc.c"

#include "actors/piranha_plant/render.inc.c"

void render_cows(Camera* camera, Mat4 arg1) {
    u16 temp_s1;
    f32 temp_f0;
    struct ActorSpawnData* var_t1;
    struct ActorSpawnData* var_s1;
    struct ActorSpawnData* var_s5;
    Vec3f sp88;
    u32 soundThing = SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4D);

    var_t1 = (struct ActorSpawnData*) LOAD_ASSET(d_course_moo_moo_farm_cow_spawn);
    D_8015F704 = 6.4e7f;
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    var_s5 = NULL;
    var_s1 = var_t1;

    while (var_s1->pos[0] != END_OF_SPAWN_DATA) {
        sp88[0] = var_s1->pos[0] * gTrackDirection;
        sp88[1] = var_s1->pos[1];
        sp88[2] = var_s1->pos[2];
        temp_f0 = is_within_render_distance(camera->pos, sp88, camera->rot[1], 0.0f, gCameraFOV[camera - camera1],
                                            4000000.0f);
        if (temp_f0 > 0.0f) {
            if (temp_f0 < D_8015F704) {
                D_8015F704 = temp_f0;
                var_s5 = var_s1;
            }
            arg1[3][0] = sp88[0];
            arg1[3][1] = sp88[1];
            arg1[3][2] = sp88[2];

            // @port: Tag the transform.
            FrameInterpolation_RecordOpenChild("render_actor_cow", ((var_s1->pos[0] & 0xFFFF) << 32) |
                                                                       ((var_s1->pos[1] & 0xFFFF) << 16) |
                                                                       (var_s1->pos[2] & 0xFFFF));

            if ((gMatrixObjectCount < MTX_OBJECT_POOL_SIZE) && (render_set_position(arg1, 0) != 0)) {
                switch (var_s1->someId) {
                    case 0:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow1);
                        break;
                    case 1:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow2);
                        break;
                    case 2:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow3);
                        break;
                    case 3:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow4);
                        break;
                    case 4:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow5);
                        break;
                }
            } else {
                return;
            }

            // @port Pop the transform id.
            FrameInterpolation_RecordCloseChild();
        }
        var_s1++;
    }

    if ((camera == camera1) && (var_s5 != NULL)) {
        if (D_8015F700 == 0) {
            temp_s1 = var_s5 - var_t1;
            if ((temp_s1 != D_8015F702) && (D_8015F704 < 160000.0f)) {
                func_800C99E0(D_8015F708, soundThing);
                D_8015F708[0] = var_s5->pos[0] * gTrackDirection;
                D_8015F708[1] = var_s5->pos[1];
                D_8015F708[2] = var_s5->pos[2];
                D_8015F702 = temp_s1;
                func_800C98B8(D_8015F708, D_802B91C8, soundThing);
                D_8015F700 = 0x00F0;
            }
        } else {
            D_8015F700 -= 1;
        }
    }
}

void evaluate_collision_player_palm_trees(Player* player) {
    Vec3f pos;
    struct UnkActorSpawnData* data = (struct UnkActorSpawnData*) LOAD_ASSET(d_course_dks_jungle_parkway_tree_spawn);

    while (data->pos[0] != END_OF_SPAWN_DATA) {
        pos[0] = data->pos[0] * gTrackDirection;
        pos[1] = data->pos[1];
        pos[2] = data->pos[2];
        if (query_and_resolve_collision_player_actor(player, pos, 5.0f, 40.0f, 0.8f) == COLLISION) {
            if ((player->effects & STAR_EFFECT) != 0) {
                func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
                func_800C90F4((u8) (player - gPlayerOne),
                              (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                data->someId |= 0x400;
            }
            if ((player->type & PLAYER_INVISIBLE_OR_BOMB) == 0) {
                func_800C9060((u8) (player - gPlayerOne), SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
            }
            break;
        }
        data++;
    }
}

void evaluate_collision_players_palm_trees(void) {
    s32 index;

    for (index = 0; index < 4; index++) {
        if (((gPlayers[index].type & 0xC000) != 0) &&
            ((s8) (u8) get_surface_type(gPlayers[index].collision.meshIndexZX) == GRASS)) {
            evaluate_collision_player_palm_trees(&gPlayers[index]);
        }
    }
}

void func_80298D10(void) {
    struct UnkActorSpawnData* temp_v1 = (struct UnkActorSpawnData*) LOAD_ASSET(d_course_dks_jungle_parkway_tree_spawn);

    while (temp_v1->pos[0] != END_OF_SPAWN_DATA) {
        temp_v1->pos[1] = temp_v1->unk8;
        temp_v1->someId &= 0xF;
        temp_v1++;
    }
}

void render_palm_trees(Camera* camera, Mat4 arg1) {
    struct UnkActorSpawnData* var_s1 = (struct UnkActorSpawnData*) LOAD_ASSET(d_course_dks_jungle_parkway_tree_spawn);
    UNUSED s32 pad;
    Vec3f spD4;
    f32 var_f22;
    Mat4 sp90;
    Vec3s sp88 = { 0, 0, 0 };
    s32 test;

    if (gGamestate == CREDITS_SEQUENCE) {
        var_f22 = 9000000.0f;
    } else {
        var_f22 = 1000000.0f;
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);

    while (var_s1->pos[0] != END_OF_SPAWN_DATA) {
        test = var_s1->someId;
        if (test & 0x0800) {
            var_s1++;
            continue;
        }

        if ((test & 0x0400) && ((gIsGamePaused == 0) || (camera == camera1))) {
            var_s1->pos[1] += 0xA;
            if (var_s1->pos[1] >= 0x321) {
                var_s1->someId |= 0x0800;
            }
        }
        spD4[0] = var_s1->pos[0] * gTrackDirection;
        spD4[1] = var_s1->pos[1];
        spD4[2] = var_s1->pos[2];

        if (is_within_render_distance(camera->pos, spD4, camera->rot[1], 0.0f, gCameraFOV[camera - camera1], var_f22) <
                0.0f &&
            CVarGetInteger("gNoCulling", 0) == 0) {
            var_s1++;
            continue;
        }

        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("render_actor_cow", ((var_s1->pos[0] & 0xFFFF) << 32) |
                                                                   ((var_s1->pos[1] & 0xFFFF) << 16) |
                                                                   (var_s1->pos[2] & 0xFFFF));

        test &= 0xF;
        test = (s16) test;
        if (test == 6) {
            mtxf_pos_rotation_xyz(sp90, spD4, sp88);
            if (!(gMatrixObjectCount < MTX_OBJECT_POOL_SIZE)) {
                break;
            }
            render_set_position(sp90, 0);
            goto dummylabel;
        } else {
            arg1[3][0] = spD4[0];
            arg1[3][1] = spD4[1];
            arg1[3][2] = spD4[2];
            if (gMatrixObjectCount < MTX_OBJECT_POOL_SIZE) {
                render_set_position(arg1, 0);
            dummylabel:
                gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
                switch (test) {
                    case 0:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_tree1);
                        break;
                    case 4:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_tree2);
                        break;
                    case 5:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_tree3);
                        break;
                    case 6:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_palm_tree);
                        break;
                }
            } else {
                break;
            }
            var_s1++;
        }
        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
    }
}

#include "actors/trees/render.inc.c"

#include "actors/kiwano_fruit/render.inc.c"

void render_actor_shell(Camera* camera, Mat4 matrix, struct ShellActor* shell) {
    u16 index;
    char* phi_t3;
    bool reverseShell = false;

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("Shell", TAG_ITEM_ADDR(shell));

    f32 temp_f0 =
        is_within_render_distance(camera->pos, shell->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 490000.0f);
    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = CLAMP(temp_f0, 0.0f, 40000.0f);
    }
    s32 maxObjectsReached;
    if (temp_f0 < 0.0f) {
        actor_not_rendered(camera, (struct Actor*) shell);
        return;
    }

    actor_rendered(camera, (struct Actor*) shell);
    if (temp_f0 < 40000.0f) {
        func_802979F8((struct Actor*) shell, 3.4f);
    }

    index = (u16) shell->rotVelocity / 4369; // Give a number between 0-15
    if (index >= 8) {
        reverseShell = true;
        index = 15 - index;
    }

    switch (shell->type) {
        case ACTOR_GREEN_SHELL:
            phi_t3 = texture_green_shell[index];
            break;
        case ACTOR_RED_SHELL:
            phi_t3 = texture_red_shell[index];
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            phi_t3 = gTextureBlueshell[index];
            break;
    }

    matrix[3][0] = shell->pos[0];
    matrix[3][1] = (shell->pos[1] - shell->boundingBoxSize) + 1.0f;
    matrix[3][2] = shell->pos[2];

    maxObjectsReached = render_set_position(matrix, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    gDPLoadTextureBlock(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(phi_t3), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    if (reverseShell) { // Reverse shell ?
        gSPDisplayList(gDisplayListHead++, D_0D005338);
    } else {
        gSPDisplayList(gDisplayListHead++, D_0D005368);
    }

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}

UNUSED s16 D_802B8808[] = { 0x0014, 0x0028, 0x0000, 0x0000 };

UNUSED s16 D_802B8810[] = { 0x0fc0, 0x0000, 0xffff, 0xffff, 0x0014, 0x0000, 0x0000, 0x0000, 0x0fc0, 0x0fc0,
                            0xffff, 0xffff, 0xffec, 0x0000, 0x0000, 0x0000, 0x0000, 0x0fc0, 0xffff, 0xffff,
                            0xffec, 0x0028, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff };

#include "actors/green_shell/render.inc.c"

#include "actors/blue_and_red_shells/render.inc.c"

#include "actors/banana/render.inc.c"

#include "actors/wario_sign/update.inc.c"

#include "actors/railroad_crossing/update.inc.c"

#include "actors/mario_sign/update.inc.c"

UNUSED void func_8029ABD4(f32* pos, s16 state) {
    gNumActors = 0;
    GET_ACTOR(spawn_actor_at_pos(pos, ACTOR_UNKNOWN_0x14))->state = state;
}

void func_8029AC18(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    if (is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1],
                                  4000000.0f) < 0 &&
        CVarGetInteger("gNoCulling", 0) == 0) {
        return;
    }

    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1] - arg2->boundingBoxSize;
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, D_0D001750);

        switch (arg2->state) {
            case 0:
                gSPDisplayList(gDisplayListHead++, &D_0D001780);
                break;
            case 1:
                gSPDisplayList(gDisplayListHead++, &D_0D001798);
                break;
            case 2:
                gSPDisplayList(gDisplayListHead++, &D_0D0017B0);
                break;
            case 3:
                gSPDisplayList(gDisplayListHead++, &D_0D0017C8);
                break;
            case 4:
                gSPDisplayList(gDisplayListHead++, &D_0D0017E0);
                break;
            case 5:
                gSPDisplayList(gDisplayListHead++, &D_0D0017F8);
                break;
            case 6:
                gSPDisplayList(gDisplayListHead++, &D_0D001810);
                break;
            case 7:
                gSPDisplayList(gDisplayListHead++, &D_0D001828);
                break;
        }
    }
}

UNUSED void func_8029AE14() {
}

#include "actors/paddle_boat/render.inc.c"

#include "actors/box_truck/render.inc.c"

#include "actors/school_bus/render.inc.c"

#include "actors/car/render.inc.c"

#include "actors/tanker_truck/render.inc.c"

#include "actors/train/render.inc.c"

#include "actors/falling_rock/render.inc.c"

void spawn_piranha_plants(struct ActorSpawnData* spawnData) {
    struct ActorSpawnData* temp_s0 = spawnData;
    struct PiranhaPlant* temp_v1;
    UNUSED s32 pad;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s32 temp;

    if (gGamestate == CREDITS_SEQUENCE) {
        return;
    }

    vec3f_set(startingVelocity, 0, 0, 0);
    vec3s_set(startingRot, 0, 0, 0);

    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * gTrackDirection;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        temp = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_PIRANHA_PLANT);
        temp_v1 = (struct PiranhaPlant*) CM_GetActor(temp);
        temp_v1->visibilityStates[0] = 0;
        temp_v1->visibilityStates[1] = 0;
        temp_v1->visibilityStates[2] = 0;
        temp_v1->visibilityStates[3] = 0;
        temp_v1->timers[0] = 0;
        temp_v1->timers[1] = 0;
        temp_v1->timers[2] = 0;
        temp_v1->timers[3] = 0;
        temp_s0++;
    }
}

void spawn_palm_trees(struct ActorSpawnData* spawnData) {
    struct ActorSpawnData* temp_s0 = spawnData;
    struct PalmTree* temp_v1;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s32 temp;

    vec3f_set(startingVelocity, 0, 0, 0);
    vec3s_set(startingRot, 0, 0, 0);

    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * gTrackDirection;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        temp = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_PALM_TREE);
        temp_v1 = (struct PalmTree*) CM_GetActor(temp);

        temp_v1->variant = temp_s0->someId;
        switch(temp_v1->variant) {
            case 0:
                temp_v1->model = d_course_koopa_troopa_beach_dl_tree_trunk1;
                break;
            case 1:
                temp_v1->model = d_course_koopa_troopa_beach_dl_tree_trunk2;
                break;
            case 2:
                temp_v1->model = d_course_koopa_troopa_beach_dl_tree_trunk3;
                break;
        }
        CM_ActorGenerateCollision(temp_v1);
        check_bounding_collision((Collision*) &temp_v1->unk30, 5.0f, temp_v1->pos[0], temp_v1->pos[1], temp_v1->pos[2]);
        func_802976EC((Collision*) &temp_v1->unk30, temp_v1->rot);
        temp_s0++;
    }
}

#include "actors/falling_rock/update.inc.c"

// Trees, cacti, shrubs, etc.
//! @todo actorType needs to be passed into this function for flexibility
void spawn_foliage(struct ActorSpawnData* actor) {
    UNUSED s32 pad[4];
    Vec3f position;
    Vec3f velocity;
    Vec3s rotation;
    UNUSED s16 pad2;
    s16 actorType = 0;
    struct Actor* temp_s0;
    struct ActorSpawnData* var_s3 = actor;
    vec3f_set(velocity, 0.0f, 0.0f, 0.0f);
    rotation[0] = 0x4000;
    rotation[1] = 0;
    rotation[2] = 0;

    if (gGamestate == CREDITS_SEQUENCE) {
        return;
    }

    while (var_s3->pos[0] != END_OF_SPAWN_DATA) {
        position[0] = var_s3->pos[0] * gTrackDirection;
        position[2] = var_s3->pos[2];
        position[1] = var_s3->pos[1];

        if (IsMarioRaceway()) {
            actorType = 2;
        } else if (IsBowsersCastle()) {
            actorType = 0x0021;
        } else if (IsYoshiValley()) {
            actorType = 3;
        } else if (IsFrappeSnowland()) {
            actorType = 0x001D;
        } else if (IsRoyalRaceway()) {
            switch (var_s3->signedSomeId) {
                case 6:
                    actorType = 0x001C;
                    break;
                case 7:
                    actorType = 4;
                    break;
            }
        } else if (IsLuigiRaceway()) {
            actorType = ACTOR_TREE_LUIGI_RACEWAY;
        } else if (IsMooMooFarm()) {
            actorType = 0x0013;
        } else if (IsKalimariDesert()) {
            switch (var_s3->signedSomeId) {
                case 5:
                    actorType = 0x001E;
                    break;
                case 6:
                    actorType = 0x001F;
                    break;
                case 7:
                    actorType = 0x0020;
                    break;
            }
        }

        temp_s0 = CM_GetActor(add_actor_to_empty_slot(position, rotation, velocity, actorType));
        if (gGamestate == CREDITS_SEQUENCE) {
            func_802976D8(temp_s0->rot);
        } else {
            check_bounding_collision(&temp_s0->unk30, 5.0f, temp_s0->pos[0], temp_s0->pos[1], temp_s0->pos[2]);
            if (temp_s0->unk30.surfaceDistance[2] < 0.0f) {
                temp_s0->pos[1] = calculate_surface_height(temp_s0->pos[0], temp_s0->pos[1], temp_s0->pos[2],
                                                           temp_s0->unk30.meshIndexZX);
            }
            func_802976EC(&temp_s0->unk30, temp_s0->rot);
        }
        var_s3++;
    }
}

void spawn_all_item_boxes(struct ActorSpawnData* spawnData) {
    s16 temp_s1;
    f32 temp_f0;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    struct ActorSpawnData* temp_s0 = spawnData;
    // struct ItemBox *itemBox;

    if ((gModeSelection == TIME_TRIALS) || (gPlaceItemBoxes == 0) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }

    vec3f_set(startingVelocity, 0, 0, 0);
    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * gTrackDirection;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        startingRot[0] = random_u16();
        startingRot[1] = random_u16();
        startingRot[2] = random_u16();
        temp_s1 = add_actor_to_empty_slot(startingPos, startingRot, startingVelocity, ACTOR_ITEM_BOX);
        temp_f0 = spawn_actor_on_surface(startingPos[0], startingPos[1] + 10.0f, startingPos[2]);

        // Should be struct ItemBox but not enough space in the stack.
        // It's either the ItemBox or the SEGMENT/OFFSET variables.
        // itemBox = (struct ItemBox *) GET_ACTOR(temp_s1);

        CM_GetActor(temp_s1)->unk_08 = temp_f0;
        // itemBox->resetDistance = temp_f0;

        CM_GetActor(temp_s1)->velocity[0] = startingPos[1];
        // itemBox->origY = startingPos[1];

        CM_GetActor(temp_s1)->pos[1] = temp_f0 - 20.0f;
        // itemBox->pos[1] = temp_f0 - 20.0f;

        temp_s0++;
    }
}

// Not from decomp
void spawn_item_box(Vec3f pos) {
    Vec3f startingVelocity;
    Vec3s startingRot;
    // struct ItemBox *itemBox;

    if ((gModeSelection == TIME_TRIALS) || (gPlaceItemBoxes == 0) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }

    pos[0] *= gTrackDirection;

    startingRot[0] = random_u16();
    startingRot[1] = random_u16();
    startingRot[2] = random_u16();
    s32 id = add_actor_to_empty_slot(pos, startingRot, startingVelocity, ACTOR_ITEM_BOX);
    f32 height = spawn_actor_on_surface(pos[0], pos[1] + 10.0f, pos[2]);

    struct ItemBox* box = (struct ItemBox*) CM_GetActor(id);

    box->resetDistance = height;
    box->origY = pos[1];
    box->pos[1] = height - 20.0f;
}

// Not from decomp
void spawn_fake_item_box(Vec3f pos) {
    Vec3f startingVelocity;
    Vec3s startingRot;
    // struct ItemBox *itemBox;

    if ((gModeSelection == TIME_TRIALS) || (gPlaceItemBoxes == 0) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }

    pos[0] *= gTrackDirection;

    startingRot[0] = random_u16();
    startingRot[1] = random_u16();
    startingRot[2] = random_u16();
    s32 id = add_actor_to_empty_slot(pos, startingRot, startingVelocity, ACTOR_FAKE_ITEM_BOX);
    f32 height = spawn_actor_on_surface(pos[0], pos[1], pos[2]);
    
    struct FakeItemBox* box = (struct FakeItemBox*) CM_GetActor(id);
    box->state = 1;
    box->targetY = pos[1];
}

void init_kiwano_fruit(void) {
    Vec3f sp64;
    Vec3f sp58;
    Vec3s sp50;
    Player* phi_s1;
    struct Actor* actor;
    s16 phi_s0;
    s32 i;

    // phi_s0 = 0;
    for (i = 0; i < 4; i++) {
        phi_s1 = &gPlayers[i];
        // temp_v0 = *phi_s1;
        if ((phi_s1->type & 0x4000) == 0) {
            continue;
        }
        if ((phi_s1->type & 0x100) != 0) {
            continue;
        }

        phi_s0 = add_actor_to_empty_slot(sp64, sp50, sp58, ACTOR_KIWANO_FRUIT);
        actor = CM_GetActor(phi_s0);
        actor->unk_04 = i;
    }
}

/**
 * Destroys actors via zeroing some of the member data
 * Key word some. When spawning a new actor,
 * members such as pos and rot should be set to prevent using expired data
 **/
void destroy_all_actors(void) {
    s32 i;
    gNumActors = 0;
    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        struct Actor* actor = CM_GetActor(i);
        actor->flags = 0;
        actor->type = 0;
        actor->unk_04 = 0;
        actor->state = 0;
        actor->unk_08 = 0.0f;
        actor->boundingBoxSize = 0.0f;
    }
}

/**
 * @brief Loads actor textures, track specific actor textures.
 * Calls to spawn_track_vehicles and place_track_actors
 *
 */
void init_actors_and_load_textures(void) {
    init_red_shell_texture();
    destroy_all_actors();
    CM_CleanWorld();

    gNumPermanentActors = 0;
    CM_BeginPlay();
    gNumPermanentActors = gNumActors;
}

void play_sound_before_despawn(struct Actor* actor) {
    s16 flags = actor->flags;

    if ((flags & 0x200) != 0) {
        func_800C99E0(actor->pos, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
        return;
    }
    if ((flags & 0x100) != 0) {
        func_800C99E0(actor->pos, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
        return;
    }
    if ((flags & 0x80) != 0) {
        func_800C99E0(actor->pos, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
    }
}

/**
 * This func likely plays an audio track based on flag
 * Next, it destroys the actor via zeroing its flags and type.
 * Note that the data from its other members still exist.
 * Actors are expected to set members such as pos and rot data if used. Not doing so could result in the use of expired
 * data.
 *
 * This method does not require modification to gActorList directly.
 * No popping members of gActorList. The list is always the size of ACTOR_LIST_SIZE.
 * Actors are members of gActorList by definition.
 *
 * @param Actor to destroy
 */
void destroy_actor(struct Actor* actor) {
    play_sound_before_despawn(actor);
    actor->flags = 0;
    actor->type = 0;
    gNumActors--;
}

s16 try_remove_destructable_item(Vec3f pos, Vec3s rot, Vec3f velocity, s16 actorType) {
    s32 actorIndex;
    struct ShellActor* compare;

    // try removing a red shell, green shell, banana, or a fake item box if the actor is expired
    for (actorIndex = gNumPermanentActors; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) CM_GetActor(actorIndex);
        if (!(compare->flags & ACTOR_IS_NOT_EXPIRED)) {
            switch (compare->type) {
                case ACTOR_RED_SHELL:
                    switch (compare->state) {
                        case MOVING_SHELL:
                        case RED_SHELL_LOCK_ON:
                        case TRIPLE_GREEN_SHELL:
                        case GREEN_SHELL_HIT_A_RACER:
                        case BLUE_SHELL_LOCK_ON:
                        case BLUE_SHELL_TARGET_ELIMINATED:
                            delete_actor_in_unexpired_actor_list(actorIndex);
                        case DESTROYED_SHELL:
                            play_sound_before_despawn((struct Actor*) compare);
                            actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                        default:
                            break;
                    }
                    break;
                case ACTOR_GREEN_SHELL:
                    switch (compare->state) {
                        case MOVING_SHELL:
                            delete_actor_in_unexpired_actor_list(actorIndex);
                        case DESTROYED_SHELL:
                            play_sound_before_despawn((struct Actor*) compare);
                            actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                    }
                    break;
                case ACTOR_BANANA:
                    switch (compare->state) {
                        case DROPPED_BANANA:
                        case BANANA_ON_GROUND:
                        case DESTROYED_BANANA:
                            play_sound_before_despawn((struct Actor*) compare);
                            actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                    }
                    break;
                case ACTOR_FAKE_ITEM_BOX:
                    switch (compare->state) {
                        case FAKE_ITEM_BOX_ON_GROUND:
                        case DESTROYED_FAKE_ITEM_BOX:
                            play_sound_before_despawn((struct Actor*) compare);
                            actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // will remove the oldest destructable actor in the list
    for (actorIndex = gNumPermanentActors; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) CM_GetActor(actorIndex);
        switch (compare->type) {
            case ACTOR_RED_SHELL:
                switch (compare->state) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        delete_actor_in_unexpired_actor_list(actorIndex);
                    case DESTROYED_SHELL:
                        play_sound_before_despawn((struct Actor*) compare);
                        actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                    default:
                        break;
                }
                break;
            case ACTOR_GREEN_SHELL:
                switch (compare->state) {
                    case MOVING_SHELL:
                        delete_actor_in_unexpired_actor_list(actorIndex);
                    case DESTROYED_SHELL:
                        play_sound_before_despawn((struct Actor*) compare);
                        actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                }
                break;
            case ACTOR_BANANA:
                switch (compare->state) {
                    case DROPPED_BANANA:
                    case BANANA_ON_GROUND:
                    case DESTROYED_BANANA:
                        play_sound_before_despawn((struct Actor*) compare);
                        actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                }
                break;
            case ACTOR_FAKE_ITEM_BOX:
                switch (compare->state) {
                    case FAKE_ITEM_BOX_ON_GROUND:
                    case DESTROYED_FAKE_ITEM_BOX:
                        play_sound_before_despawn((struct Actor*) compare);
                        actor_init((struct Actor*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                }
                break;
            default:
                break;
        }
    }

    return -1;
}

// returns actor index if any available actor type is -1
s16 add_actor_to_empty_slot(Vec3f pos, Vec3s rot, Vec3f velocity, s16 actorType) {
    size_t index;

    // if (gNumActors >= CM_GetActorSize()) {
    //     return try_remove_destructable_item(pos, rot, velocity, actorType);
    // }

    // Cleanup unused actors
    for (index = 0; index < CM_GetActorSize(); index++) {
        // if (CM_GetActor(index)->flags == 0) {
        //! @todo Commented out because deletes too soon.
        // CM_DeleteActor(index);
        // gNumActors--;
        //}
    }
    gNumActors++;
    struct Actor* actor = CM_AddBaseActor();
    actor_init(actor, pos, rot, velocity, actorType);
    CM_ActorBeginPlay(actor);
    return (s16) CM_GetActorSize() - 1; // Return current index;
}

UNUSED s16 spawn_actor_at_pos(Vec3f pos, s16 actorType) {
    Vec3f vel;
    Vec3s rot;

    vec3f_set(vel, 0.0f, 0.0f, 0.0f);
    vec3s_set(rot, 0, 0, 0);
    return add_actor_to_empty_slot(pos, rot, vel, actorType);
}

// not ActorSpawnData but very similar in structure and use
struct test {
    Vec3s thing;
};

UNUSED void prototype_actor_spawn_data(Player* player, uintptr_t arg1) {
    Vec3f sp64;
    struct test* var_s0;

    var_s0 = (struct test*) arg1;
    while (var_s0->thing[0] != END_OF_SPAWN_DATA) {
        sp64[0] = var_s0->thing[0] * gTrackDirection;
        sp64[1] = var_s0->thing[1];
        sp64[2] = var_s0->thing[2];
        if (arg1 & arg1) {}
        query_and_resolve_collision_player_actor(player, sp64, 5.0f, 40.0f, 0.8f);
        var_s0++;
    }
}

bool query_and_resolve_collision_player_actor(Player* player, Vec3f pos, f32 minDist, f32 dist, f32 arg4) {
    f32 yDist;
    f32 sqrtDist;
    f32 zDist;
    f32 xVelocity;
    f32 zVelocity;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 xDist;
    f32 sp28;
    f32 temp_f2_2;

    minDist = player->boundingBoxSize + minDist;
    dist = player->boundingBoxSize + dist;
    xDist = pos[0] - player->pos[0];
    if (minDist < xDist) {
        return NO_COLLISION;
    }
    if (xDist < -minDist) {
        return NO_COLLISION;
    }
    yDist = pos[1] - player->pos[1];
    if (dist < yDist) {
        return NO_COLLISION;
    }
    if (yDist < -dist) {
        return NO_COLLISION;
    }
    zDist = pos[2] - player->pos[2];
    if (minDist < zDist) {
        return NO_COLLISION;
    }
    if (zDist < -minDist) {
        return NO_COLLISION;
    }
    dist = (xDist * xDist) + (yDist * yDist) + (zDist * zDist);
    if (dist < 0.1f) {
        return NO_COLLISION;
    }
    if ((minDist * minDist) < dist) {
        return NO_COLLISION;
    }
    sqrtDist = sqrtf(dist);
    sp28 = sqrtDist - minDist;
    xVelocity = player->velocity[0];
    zVelocity = player->velocity[2];
    if (player->effects & STAR_EFFECT) {
        return COLLISION;
    }
    if (sqrtDist < 0.1f) {
        temp_f0_4 = sqrtf((xVelocity * xVelocity) + (zVelocity * zVelocity));
        if (temp_f0_4 < 0.5f) {
            temp_f0_4 = 0.5f;
        }
        player->velocity[0] = 0;
        player->velocity[2] = 0;
        player->pos[0] += (xVelocity / temp_f0_4) * minDist;
        player->pos[2] += (zVelocity / temp_f0_4) * minDist;
    } else {
        player->effects |= 0x8000;
        xDist /= sqrtDist;
        zDist /= sqrtDist;
        temp_f0_5 = sqrtf((xVelocity * xVelocity) + (zVelocity * zVelocity));
        if (temp_f0_5 < 0.25f) {
            temp_f0_6 = 1.2f;
            player->pos[0] = pos[0] - (xDist * minDist * temp_f0_6);
            player->pos[2] = pos[2] - (zDist * minDist * temp_f0_6);
            player->velocity[0] = 0.0f;
            player->velocity[2] = 0.0f;
            return COLLISION;
        }
        temp_f2_2 = ((xDist * xVelocity) + (zDist * zVelocity)) / temp_f0_5;
        temp_f2_2 = temp_f0_5 * temp_f2_2 * arg4 * 1.3f;
        player->velocity[0] -= xDist * temp_f2_2;
        player->velocity[2] -= zDist * temp_f2_2;
        player->pos[0] += xDist * sp28 * 0.5f;
        player->pos[2] += zDist * sp28 * 0.5f;
    }
    return COLLISION;
}

bool collision_mario_sign(Player* player, struct Actor* marioRacewaySign) {
    if (query_and_resolve_collision_player_actor(player, marioRacewaySign->pos, 7.0f, 200.0f, 0.8f) == COLLISION) {
        if ((player->type & PLAYER_HUMAN) != 0) {
            if ((player->effects & STAR_EFFECT) != 0) {
                marioRacewaySign->flags |= 0x400;
                func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
                func_800C90F4(player - gPlayerOne,
                              (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            } else if ((player->type & PLAYER_INVISIBLE_OR_BOMB) == 0) {
                func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x1A));
            }
        }
        return true;
    }
    return false;
}

bool collision_piranha_plant(Player* player, struct PiranhaPlant* plant) {
    if (query_and_resolve_collision_player_actor(player, plant->pos, plant->boundingBoxSize, plant->boundingBoxSize,
                                                 2.5f) == COLLISION) {
        if ((player->type & PLAYER_HUMAN) != 0) {
            if ((player->effects & STAR_EFFECT) != 0) {
                plant->flags |= 0x400;
                func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                func_800C90F4(player - gPlayerOne,
                              (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            } else if ((player->type & PLAYER_INVISIBLE_OR_BOMB) == 0) {
                func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x52));
            }
        }
        return true;
    }
    return false;
}

bool collision_yoshi_egg(Player* player, struct YoshiValleyEgg* egg) {
    UNUSED f32 pad[5];
    f32 z_dist;
    f32 xz_dist;
    f32 x_dist;
    f32 y_dist;
    f32 totalBox;
    f32 maxDist = 60.0f;
    f32 minDist = 0.0f;

    x_dist = egg->pos[0] - player->pos[0];
    if ((x_dist < minDist) && (x_dist < -maxDist)) {
        return false;
    }
    if (x_dist > maxDist) {
        return false;
    }

    z_dist = egg->pos[2] - player->pos[2];
    if ((z_dist < minDist) && (z_dist < -maxDist)) {
        return false;
    }
    if (z_dist > maxDist) {
        return false;
    }

    xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
    if (xz_dist > maxDist) {
        return false;
    }
    func_802977B0(player);

    y_dist = player->pos[1] - egg->pos[1];
    if (y_dist < minDist) {
        return false;
    }

    totalBox = player->boundingBoxSize + egg->boundingBoxSize;
    if (totalBox < xz_dist) {
        return false;
    }

    if ((player->type & PLAYER_HUMAN) != 0) {
        if ((player->effects & STAR_EFFECT) != 0) {
            egg->flags |= 0x400;
            egg->pathCenter[1] = 8.0f;
            func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            func_800C90F4(player - gPlayerOne, (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
        } else {
            apply_hit_sound_effect(player, player - gPlayerOne);
            if ((gModeSelection == TIME_TRIALS) && ((player->type & PLAYER_CPU) == 0)) {
                gPostTimeTrialReplayCannotSave = 1;
            }
        }
    } else {
        apply_hit_sound_effect(player, player - gPlayerOne);
    }

    return true;
}

bool collision_tree(Player* player, struct Actor* actor) {
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 sp48;
    f32 sp44;
    f32 var_f16;
    f32 xz_dist;
    UNUSED f32 pad[2];
    f32 temp_f12;
    f32 temp_f0_4;
    Vec3f actorPos;
    f32 temp_f2;

    var_f16 = actor->unk_08;
    x_dist = actor->pos[0] - player->pos[0];
    if ((x_dist < 0.0f) && (x_dist < -var_f16)) {
        return false;
    }
    if (var_f16 < x_dist) {
        return false;
    }
    z_dist = actor->pos[2] - player->pos[2];
    if ((z_dist < 0.0f) && (z_dist < -var_f16)) {
        return false;
    }
    if (var_f16 < z_dist) {
        return false;
    }
    y_dist = player->pos[1] - actor->pos[1];
    if (y_dist < 0.0f) {
        return false;
    }
    if ((f32) actor->state < y_dist) {
        return false;
    }
    xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
    if (var_f16 < xz_dist) {
        return false;
    }
    func_802977B0(player);
    var_f16 = player->boundingBoxSize + actor->boundingBoxSize;
    if (var_f16 < xz_dist) {
        return false;
    }
    sp48 = player->velocity[0];
    sp44 = player->velocity[2];
    if (player->type & PLAYER_HUMAN) {
        if (player->effects & STAR_EFFECT) {
            actor->flags |= 0x400;
            func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            func_800C90F4(player - gPlayerOne, (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            return true;
        }
        if (!(player->type & PLAYER_INVISIBLE_OR_BOMB)) {
            func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
        }
    }
    if (!(player->effects & STAR_EFFECT)) {
        player->effects |= 0x8000;
    }
    actorPos[0] = actor->pos[0];
    actorPos[1] = actor->pos[1];
    actorPos[2] = actor->pos[2];
    if (((IsMarioRaceway()) || (IsYoshiValley()) ||
         (IsRoyalRaceway()) || (IsLuigiRaceway())) &&
        (player->speed > 1.0f)) {
        spawn_leaf(actorPos, 0);
    }
    if (xz_dist < 0.1f) {
        sqrtf((sp48 * sp48) + (sp44 * sp44));
        if (xz_dist) {}
        player->velocity[0] = 0;
        player->velocity[2] = 0;
        player->pos[0] = actorPos[0] - (x_dist * var_f16 * 1.2f);
        player->pos[2] = actorPos[2] - (z_dist * var_f16 * 1.2f);
    } else {
        temp_f0_4 = sqrtf((sp48 * sp48) + (sp44 * sp44));
        x_dist /= xz_dist;
        z_dist /= xz_dist;
        if (temp_f0_4 < 0.25f) {
            player->pos[0] = actorPos[0] - (x_dist * var_f16 * 1.2f);
            player->pos[2] = actorPos[2] - (z_dist * var_f16 * 1.2f);
            player->velocity[0] = 0;
            player->velocity[2] = 0;
            return true;
        }
        temp_f12 = ((x_dist * sp48) + (z_dist * sp44)) / temp_f0_4;
        temp_f12 = temp_f0_4 * temp_f12 * 1.5f;
        player->velocity[0] -= x_dist * temp_f12;
        player->velocity[2] -= z_dist * temp_f12;
        temp_f2 = xz_dist - var_f16;
        player->pos[0] += x_dist * temp_f2 * 0.5f;
        player->pos[2] += z_dist * temp_f2 * 0.5f;
    }
    return true;
}

bool query_collision_player_vs_actor_item(Player* player, struct Actor* arg1) {
    f32 temp_f0;
    f32 dist;
    f32 yDist;
    f32 zDist;
    f32 xDist;

    temp_f0 = player->boundingBoxSize + arg1->boundingBoxSize;
    xDist = arg1->pos[0] - player->pos[0];
    if (temp_f0 < xDist) {
        return NO_COLLISION;
    }
    if (xDist < -temp_f0) {
        return NO_COLLISION;
    }
    yDist = arg1->pos[1] - player->pos[1];
    if (temp_f0 < yDist) {
        return NO_COLLISION;
    }
    if (yDist < -temp_f0) {
        return NO_COLLISION;
    }
    zDist = arg1->pos[2] - player->pos[2];
    if (temp_f0 < zDist) {
        return NO_COLLISION;
    }
    if (zDist < -temp_f0) {
        return NO_COLLISION;
    }
    dist = (xDist * xDist) + (yDist * yDist) + (zDist * zDist);
    if (dist < 0.1f) {
        return NO_COLLISION;
    }
    if ((temp_f0 * temp_f0) < dist) {
        return NO_COLLISION;
    }
    return COLLISION;
}

bool query_collision_actor_vs_actor(struct Actor* arg0, struct Actor* arg1) {
    f32 temp_f0;
    f32 dist;
    f32 dist_y;
    f32 dist_z;
    f32 dist_x;

    temp_f0 = arg0->boundingBoxSize + arg1->boundingBoxSize;
    dist_x = arg0->pos[0] - arg1->pos[0];
    if (temp_f0 < dist_x) {
        return NO_COLLISION;
    }
    if (dist_x < -temp_f0) {
        return NO_COLLISION;
    }
    dist_y = arg0->pos[1] - arg1->pos[1];
    if (temp_f0 < dist_y) {
        return NO_COLLISION;
    }
    if (dist_y < -temp_f0) {
        return NO_COLLISION;
    }
    dist_z = arg0->pos[2] - arg1->pos[2];
    if (temp_f0 < dist_z) {
        return NO_COLLISION;
    }
    if (dist_z < -temp_f0) {
        return NO_COLLISION;
    }
    dist = (dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z);
    if (dist < 0.1f) {
        return NO_COLLISION;
    }
    if ((temp_f0 * temp_f0) < dist) {
        return NO_COLLISION;
    }
    return COLLISION;
}

void destroy_destructable_actor(struct Actor* actor) {
    struct ShellActor* shell;
    struct BananaActor* banana;
    struct FakeItemBox* fakeItemBox;
    Player* player;

    switch (actor->type) {
        case ACTOR_BANANA:
            banana = (struct BananaActor*) actor;
            switch (banana->state) {
                case FIRST_BANANA_BUNCH_BANANA:
                case BANANA_BUNCH_BANANA:
                    destroy_banana_in_banana_bunch(banana);
                    break;
                case HELD_BANANA:
                    player = &gPlayers[banana->playerId];
                    player->soundEffects &= ~0x00040000;
                    /* fallthrough */
                case BANANA_ON_GROUND:
                    banana->flags = -0x8000;
                    banana->unk_04 = 0x003C;
                    banana->state = DESTROYED_BANANA;
                    banana->velocity[1] = 3.0f;
                    break;
                case DROPPED_BANANA:
                case DESTROYED_BANANA:
                default:
                    break;
            }
            break;
        case ACTOR_GREEN_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->state != GREEN_SHELL_HIT_A_RACER) {
                switch (shell->state) {
                    case MOVING_SHELL:
                        delete_actor_in_unexpired_actor_list(CM_FindActorIndex(actor));
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->flags = -0x8000;
                        shell->rotAngle = 0;
                        shell->someTimer = 0x003C;
                        shell->state = GREEN_SHELL_HIT_A_RACER;
                        shell->velocity[1] = 3.0f;
                        break;
                    case TRIPLE_GREEN_SHELL:
                        triple_shell_actor_collide_with_player(shell, ACTOR_GREEN_SHELL);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->state != DESTROYED_SHELL) {
                switch (shell->state) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        func_800C9EF4(shell->pos, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                        delete_actor_in_unexpired_actor_list(CM_FindActorIndex(actor));
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->flags = -0x8000;
                        shell->rotAngle = 0;
                        shell->someTimer = 0x003C;
                        shell->state = DESTROYED_SHELL;
                        shell->velocity[1] = 3.0f;
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_RED_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->state != DESTROYED_SHELL) {
                switch (shell->state) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        delete_actor_in_unexpired_actor_list(CM_FindActorIndex(actor));
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->flags = -0x8000;
                        shell->rotAngle = 0;
                        shell->someTimer = 0x003C;
                        shell->state = DESTROYED_SHELL;
                        shell->velocity[1] = 3.0f;
                        break;
                    case TRIPLE_RED_SHELL:
                        triple_shell_actor_collide_with_player(shell, ACTOR_RED_SHELL);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            fakeItemBox = (struct FakeItemBox*) actor;
            player = &gPlayers[(s16) fakeItemBox->playerId];
            if (fakeItemBox->state == HELD_FAKE_ITEM_BOX) {
                player->soundEffects &= ~0x00040000;
            }
            fakeItemBox->state = DESTROYED_FAKE_ITEM_BOX;
            fakeItemBox->flags = -0x8000;
            fakeItemBox->someTimer = 0;
            break;
    }
}

void play_sound_on_destructible_actor_collision(struct Actor* arg0, struct Actor* arg1) {
    switch (arg0->type) {
        case ACTOR_GREEN_SHELL:
            if ((arg0->state == HELD_SHELL) || (arg0->state == TRIPLE_GREEN_SHELL)) {
                arg0->flags |= 0x200;
                func_800C98B8(arg0->pos, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_RED_SHELL:
            if ((arg0->state == HELD_SHELL) || (arg0->state == TRIPLE_RED_SHELL)) {
                arg0->flags |= 0x200;
                func_800C98B8(arg0->pos, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (arg0->state == HELD_SHELL) {
                arg0->flags |= 0x200;
                func_800C98B8(arg0->pos, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            if (arg0->state == HELD_FAKE_ITEM_BOX) {
                arg0->flags |= 0x200;
                func_800C98B8(arg0->pos, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
    }

    switch (arg1->type) {
        case ACTOR_GREEN_SHELL:
            if ((arg1->state == HELD_SHELL) || (arg1->state == TRIPLE_GREEN_SHELL)) {
                arg1->flags |= 0x200;
                func_800C98B8(arg1->pos, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_RED_SHELL:
            if ((arg1->state == HELD_SHELL) || (arg1->state == TRIPLE_RED_SHELL)) {
                arg1->flags |= 0x200;
                func_800C98B8(arg1->pos, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (arg1->state == HELD_SHELL) {
                arg1->flags |= 0x200;
                func_800C98B8(arg1->pos, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            if (arg1->state == HELD_FAKE_ITEM_BOX) {
                arg1->flags |= 0x200;
                func_800C98B8(arg1->pos, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
    }

    arg0->flags |= 0x100;
    func_800C98B8(arg0->pos, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
}

void evaluate_actor_collision_between_two_destructible_actors(struct Actor* actor1, struct Actor* actor2) {
    if (query_collision_actor_vs_actor(actor1, actor2) == COLLISION) {
        if ((actor1->type == ACTOR_BLUE_SPINY_SHELL) && (actor2->type == ACTOR_BLUE_SPINY_SHELL)) {
            destroy_destructable_actor(actor1);
            destroy_destructable_actor(actor2);
            actor1->flags |= 0x100;
            func_800C98B8(actor1->pos, actor1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            return;
        }
        if (actor1->type == ACTOR_BLUE_SPINY_SHELL) {
            if (actor1->state == HELD_SHELL) {
                destroy_destructable_actor(actor1);
            }
        } else {
            destroy_destructable_actor(actor1); // automatically destroy if it's something different of a blueshell
        }
        if (actor2->type == ACTOR_BLUE_SPINY_SHELL) {
            if (actor2->state == HELD_SHELL) {
                destroy_destructable_actor(actor2);
            }
        } else {
            destroy_destructable_actor(actor2); // automatically destroy if it's something different of a blueshell
        }
        play_sound_on_destructible_actor_collision(actor1, actor2);
    }
}

void evaluate_collision_between_player_actor(Player* player, struct Actor* actor) {
    UNUSED s32 pad;
    s16 temp_lo;
    UNUSED s32 pad2[2];
    s16 temp_v1;
    Player* owner;
    f32 temp_f0;
    f32 temp_f2;

    CM_ActorCollision(player, actor);

    temp_lo = player - gPlayerOne;
    switch (actor->type) {
        case ACTOR_YOSHI_EGG:
            if (!(player->effects & BOO_EFFECT) && !(player->type & PLAYER_INVISIBLE_OR_BOMB)) {
                collision_yoshi_egg(player, (struct YoshiValleyEgg*) actor);
            }
            break;
        case ACTOR_BANANA:
            if (player->effects & (BOO_EFFECT | 0x8C0)) {
                break;
            }
            if (player->soundEffects & 1) {
                break;
            }
            temp_v1 = actor->rot[0];
            if (((temp_lo == temp_v1) && (actor->flags & 0x1000)) ||
                (query_collision_player_vs_actor_item(player, actor) != COLLISION)) {
                break;
            }
            player->soundEffects |= 1;
            owner = &gPlayers[temp_v1];
            if (owner->type & 0x4000) {
                if (actor->flags & 0xF) {
                    if (temp_lo != temp_v1) {
                        func_800C90F4(temp_v1, (owner->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                } else {
                    temp_f0 = actor->pos[0] - owner->pos[0];
                    temp_f2 = actor->pos[2] - owner->pos[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 360000.0f) && (temp_lo != temp_v1)) {
                        func_800C90F4(temp_v1, (owner->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                }
            }
            destroy_destructable_actor(actor);
            break;
        case ACTOR_GREEN_SHELL:
            if (player->effects & 0x80000400) {
                break;
            }
            if (player->soundEffects & 4) {
                break;
            }
            temp_v1 = actor->rot[2];
            if (((temp_lo == temp_v1) && (actor->flags & 0x1000)) ||
                (query_collision_player_vs_actor_item(player, actor) != COLLISION)) {
                break;
            }
            player->soundEffects |= 4;
            func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            owner = &gPlayers[temp_v1];
            if ((owner->type & 0x4000) && (temp_lo != temp_v1)) {
                func_800C90F4(temp_v1, (owner->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            destroy_destructable_actor(actor);
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (player->soundEffects & 2) {
                break;
            }
            temp_v1 = actor->rot[2];
            if (((temp_lo == temp_v1) && (actor->flags & 0x1000)) ||
                (query_collision_player_vs_actor_item(player, actor) != COLLISION)) {
                break;
            }
            if (!(player->effects & BOO_EFFECT)) {
                player->soundEffects |= 2;
                func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            }
            owner = &gPlayers[temp_v1];
            if ((owner->type & 0x4000) && (temp_lo != temp_v1)) {
                func_800C90F4(temp_v1, (owner->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            if (temp_lo == actor->unk_04) {
                destroy_destructable_actor(actor);
            }
            break;
        case ACTOR_RED_SHELL:
            temp_v1 = actor->rot[2];
            if (player->effects & 0x01000000) {
                break;
            }
            if (player->soundEffects & 2) {
                break;
            }
            temp_v1 = actor->rot[2];
            if (((temp_lo == temp_v1) && (actor->flags & 0x1000)) ||
                (query_collision_player_vs_actor_item(player, actor) != COLLISION)) {
                break;
            }
            if (!(player->effects & BOO_EFFECT)) {
                player->soundEffects |= 2;
                func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            }
            owner = &gPlayers[temp_v1];
            if ((owner->type & 0x4000) && (temp_lo != temp_v1)) {
                func_800C90F4(temp_v1, (owner->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            destroy_destructable_actor(actor);
            break;
        case ACTOR_PIRANHA_PLANT:
            if (!(player->effects & BOO_EFFECT)) {
                collision_piranha_plant(player, (struct PiranhaPlant*) actor);
            }
            break;
        case ACTOR_MARIO_SIGN:
            if (!(player->effects & BOO_EFFECT)) {
                collision_mario_sign(player, actor);
            }
            break;
        case ACTOR_TREE_MARIO_RACEWAY:
        case ACTOR_TREE_YOSHI_VALLEY:
        case ACTOR_TREE_ROYAL_RACEWAY:
        case ACTOR_TREE_MOO_MOO_FARM:
        case ACTOR_PALM_TREE:
        case 26:
        case ACTOR_TREE_PEACH_CASTLE:
        case ACTOR_TREE_FRAPPE_SNOWLAND:
        case ACTOR_CACTUS1_KALAMARI_DESERT:
        case ACTOR_CACTUS2_KALAMARI_DESERT:
        case ACTOR_CACTUS3_KALAMARI_DESERT:
        case ACTOR_BUSH_BOWSERS_CASTLE:
            if (!(player->effects & BOO_EFFECT)) {
                collision_tree(player, actor);
            }
            break;
        case ACTOR_FALLING_ROCK:
            if (!(player->effects & BOO_EFFECT) && !(player->type & PLAYER_INVISIBLE_OR_BOMB)) {
                if (query_collision_player_vs_actor_item(player, actor) == COLLISION) {
                    func_800C98B8(actor->pos, actor->velocity, SOUND_ACTION_EXPLOSION);
                    if ((gModeSelection == TIME_TRIALS) && !(player->type & PLAYER_CPU)) {
                        gPostTimeTrialReplayCannotSave = 1;
                    }
                    if (player->effects & STAR_EFFECT) {
                        actor->velocity[1] = 10.0f;
                    } else {
                        apply_hit_sound_effect(player, player - gPlayerOne);
                    }
                }
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            temp_v1 = actor->velocity[0];
            if (player->effects & BOO_EFFECT) {
                break;
            }
            temp_v1 = actor->velocity[0];
            if (((temp_lo == temp_v1) && (actor->flags & 0x1000)) ||
                (query_collision_player_vs_actor_item(player, actor) != COLLISION)) {
                break;
            }
            player->soundEffects |= REVERSE_SOUND_EFFECT;
            owner = &gPlayers[temp_v1];
            if (owner->type & 0x4000) {
                if (actor->flags & 0xF) {
                    if (temp_lo != temp_v1) {
                        func_800C90F4(temp_v1, (owner->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                } else {
                    temp_f0 = actor->pos[0] - owner->pos[0];
                    temp_f2 = actor->pos[2] - owner->pos[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 360000.0f) && (temp_lo != temp_v1)) {
                        func_800C90F4(temp_v1, (owner->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                }
                if (actor->state == 0) {
                    owner->soundEffects &= ~0x00040000;
                }
            }
            actor->state = 2;
            actor->flags = -0x8000;
            actor->unk_04 = 0;
            break;
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            if (query_collision_player_vs_actor_item(player, actor) == COLLISION) {
                actor->state = 3;
                actor->flags = -0x8000;
                actor->unk_04 = 0;
                if (player->type & PLAYER_HUMAN) {
                    func_8007ABFC(player - gPlayerOne, ITEM_BLUE_SPINY_SHELL);
                }
            } else if (actor->state == 0) {
                actor->state = 1;
                actor->flags = -0x8000;
            }
            break;
        case ACTOR_ITEM_BOX:
            if (query_collision_player_vs_actor_item(player, actor) == COLLISION) {
                actor->state = 3;
                actor->flags = -0x8000;
                actor->unk_04 = 0;
                if (player->type & PLAYER_HUMAN) {
                    func_8007ABFC(player - gPlayerOne, ITEM_NONE);
                }
            } else if (actor->state == 0) {
                actor->state = 1;
                actor->flags = -0x8000;
            }
            break;
        default:
            break;
    }
}

void evaluate_collision_for_players_and_actors(void) {
    struct Actor* temp_a1;
    s32 i, j;
    Player* phi_s1;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi_s1 = &gPlayers[i];

        if (((phi_s1->type & 0x8000) != 0) && ((phi_s1->effects & 0x4000000) == 0)) {
            func_802977E4(phi_s1);
            for (j = 0; j < ACTOR_LIST_SIZE; j++) {
                temp_a1 = CM_GetActor(j);

                if ((phi_s1->effects & 0x4000000) == 0) {
                    // temp_v0 = temp_a1->unk2;
                    if (((temp_a1->flags & 0x8000) != 0) && ((temp_a1->flags & 0x4000) != 0)) {
                        evaluate_collision_between_player_actor(phi_s1, temp_a1);
                    }
                }
            }
        }
    }
}

// It's look like to check collision between item and other different item
void evaluate_collision_for_destructible_actors(void) {
    struct Actor* actor1;
    struct Actor* actor2;
    s32 i, j;
    UNUSED s32 pad;

    for (i = gNumPermanentActors; i < (ACTOR_LIST_SIZE); i++) {
        actor1 = CM_GetActor(i);

        if ((actor1->flags & 0x8000) == 0) {
            continue;
        }
        if ((actor1->flags & 0x4000) == 0) {
            continue;
        }

        switch (actor1->type) {
            case ACTOR_BANANA:
            case ACTOR_GREEN_SHELL:
            case ACTOR_RED_SHELL:
            case ACTOR_BLUE_SPINY_SHELL:
            case ACTOR_FAKE_ITEM_BOX:

                for (j = i + 1; j < ACTOR_LIST_SIZE; j++) {
                    actor2 = CM_GetActor(j);

                    if ((actor1->flags & 0x8000) == 0) {
                        continue;
                    }
                    if ((actor1->flags & 0x4000) == 0) {
                        continue;
                    }

                    if ((actor2->flags & 0x8000) == 0) {
                        continue;
                    }
                    if ((actor2->flags & 0x4000) == 0) {
                        continue;
                    }

                    switch (actor2->type) {
                        case ACTOR_BANANA:
                            if (actor1->type == ACTOR_BANANA) {
                                continue;
                            }
                            evaluate_actor_collision_between_two_destructible_actors(actor1, actor2);
                            break;
                        case ACTOR_GREEN_SHELL:
                            if (actor1->type == ACTOR_GREEN_SHELL) {
                                if (actor1->rot[2] == actor2->rot[2]) {
                                    continue;
                                }
                            }
                            evaluate_actor_collision_between_two_destructible_actors(actor1, actor2);
                            break;
                        case ACTOR_RED_SHELL:
                            if (actor1->type == ACTOR_RED_SHELL) {
                                if (actor1->rot[2] == actor2->rot[2]) {
                                    continue;
                                }
                            }
                            evaluate_actor_collision_between_two_destructible_actors(actor1, actor2);
                            break;
                        case ACTOR_BLUE_SPINY_SHELL:
                        case ACTOR_FAKE_ITEM_BOX:
                            evaluate_actor_collision_between_two_destructible_actors(actor1, actor2);
                            break;
                    }
                }

                break;
        }
    }
}

void func_802A1064(struct FakeItemBox* fake_item_box) {
    if ((u32) (CM_FindActorIndex(fake_item_box)) <= (u32) ACTOR_LIST_SIZE) {
        if (((fake_item_box->flags & 0x8000) != 0) && (fake_item_box->type == ACTOR_FAKE_ITEM_BOX)) {
            fake_item_box->state = 1;
            fake_item_box->targetY = func_802ABEAC(&fake_item_box->unk30, fake_item_box->pos) + 8.66f;
            fake_item_box->someTimer = 100;
        }
    }
}

#include "actors/fake_item_box/update.inc.c"

void init_actor_hot_air_balloon_item_box(f32 x, f32 y, f32 z) {
    Vec3f pos;
    Vec3f velocity;
    Vec3s rot;
    s16 id;

    if (gModeSelection == TIME_TRIALS) {
        return;
    }

    vec3s_set(rot, 0, 0, 0);
    vec3f_set(velocity, 0, 0, 0);
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    id = add_actor_to_empty_slot(pos, rot, velocity, ACTOR_HOT_AIR_BALLOON_ITEM_BOX);
    gActorHotAirBalloonItemBox = CM_GetActor(id);
}

#include "actors/item_box/update.inc.c"

#include "actors/fake_item_box/render.inc.c"

#include "actors/item_box/render.inc.c"

#include "actors/wario_sign/render.inc.c"

#include "actors/yoshi_egg/render.inc.c"

#include "actors/mario_sign/render.inc.c"

#include "actors/railroad_crossing/render.inc.c"

#include "actors/palm_tree/render.inc.c"

void render_item_boxes(ScreenContext* arg0) {
    Camera* camera = arg0->camera;
    struct Actor* actor;
    s32 i;
    D_8015F8DC = 0;

    for (i = 0; i < CM_GetActorSize(); i++) {
        actor = CM_GetActor(i);

        if (actor->flags == 0) {
            continue;
        }

        switch (actor->type) {
            case ACTOR_FAKE_ITEM_BOX:
                render_actor_fake_item_box(camera, (struct FakeItemBox*) actor);
                break;
            case ACTOR_ITEM_BOX:
                render_actor_item_box(camera, (struct ItemBox*) actor);
                break;
            case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
                render_actor_item_box(camera, (struct ItemBox*) actor);
                break;
        }
    }
}

void render_course_actors(ScreenContext* arg0) {
    Camera* camera = arg0->camera;
    u16 pathCounter = arg0->pathCounter;
    UNUSED s32 pad[12];
    s32 i;

    struct Actor* actor;
    UNUSED Vec3f sp4C = { 0.0f, 5.0f, 10.0f };

    f32 sp48 = sins(camera->rot[1] - 0x8000);
    f32 temp_f0 = coss(camera->rot[1] - 0x8000);

    sBillBoardMtx[0][0] = temp_f0;
    sBillBoardMtx[0][2] = -sp48;
    sBillBoardMtx[2][2] = temp_f0;
    sBillBoardMtx[1][0] = 0.0f;
    sBillBoardMtx[0][1] = 0.0f;
    sBillBoardMtx[2][1] = 0.0f;
    sBillBoardMtx[1][2] = 0.0f;
    sBillBoardMtx[0][3] = 0.0f;
    sBillBoardMtx[1][3] = 0.0f;
    sBillBoardMtx[2][3] = 0.0f; // 2c
    sBillBoardMtx[2][0] = sp48;
    sBillBoardMtx[1][1] = 1.0f;
    sBillBoardMtx[3][3] = 1.0f; // unk3c

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetLights1(gDisplayListHead++, D_800DC610[1]);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    if (gModeSelection != BATTLE) {
        // func_80297340(camera);
    }
    D_8015F8E0 = 0;

    for (i = 0; i < CM_GetActorSize(); i++) {
        actor = CM_GetActor(i);

        if (actor->flags == 0) {
            continue;
        }

        FrameInterpolation_RecordOpenChild(actor, i);

        switch (actor->type) {
            default: // Skip custom actor
                break;
            case ACTOR_TREE_MARIO_RACEWAY:

                render_actor_tree_mario_raceway(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_TREE_YOSHI_VALLEY:
                render_actor_tree_yoshi_valley(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_TREE_ROYAL_RACEWAY:
                render_actor_tree_royal_raceway(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_TREE_MOO_MOO_FARM:
                render_actor_tree_moo_moo_farm(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_TREE_LUIGI_RACEWAY:
                render_actor_tree_luigi_raceway(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_TREE_PEACH_CASTLE:
                render_actor_tree_peach_castle(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_BUSH_BOWSERS_CASTLE:
                render_actor_bush_bowser_castle(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_TREE_FRAPPE_SNOWLAND:
                render_actor_tree_frappe_snowland(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_CACTUS1_KALAMARI_DESERT:
                render_actor_tree_cactus1_kalimari_desert(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_CACTUS2_KALAMARI_DESERT:
                render_actor_tree_cactus2_kalimari_desert(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_CACTUS3_KALAMARI_DESERT:
                render_actor_tree_cactus3_kalimari_desert(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_FALLING_ROCK: // now in C++
                //render_actor_falling_rock(camera, (struct FallingRock*) actor);
                break;
            case ACTOR_KIWANO_FRUIT:
                render_actor_kiwano_fruit(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_BANANA:
                render_actor_banana(camera, sBillBoardMtx, (struct BananaActor*) actor);
                break;
            case ACTOR_GREEN_SHELL:
                render_actor_green_shell(camera, sBillBoardMtx, (struct ShellActor*) actor);
                break;
            case ACTOR_RED_SHELL:
                render_actor_red_shell(camera, sBillBoardMtx, (struct ShellActor*) actor);
                break;
            case ACTOR_BLUE_SPINY_SHELL:
                render_actor_blue_shell(camera, sBillBoardMtx, (struct ShellActor*) actor);
                break;
            case ACTOR_PIRANHA_PLANT:
                render_actor_piranha_plant(camera, sBillBoardMtx, (struct PiranhaPlant*) actor);
                break;
            case ACTOR_TRAIN_ENGINE:
                render_actor_train_engine(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_TENDER:
                render_actor_train_tender(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_PASSENGER_CAR:
                render_actor_train_passenger_car(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_COW:
                render_actor_cow(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_UNKNOWN_0x14:
                func_8029AC18(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_MARIO_SIGN:
                render_actor_mario_sign(camera, sBillBoardMtx, actor);
                break;
            case ACTOR_WARIO_SIGN:
                render_actor_wario_sign(camera, actor);
                break;
            case ACTOR_PALM_TREE:
                render_actor_palm_tree(camera, sBillBoardMtx, (struct PalmTree*) actor);
                break;
            case ACTOR_PADDLE_BOAT:
                render_actor_paddle_boat(camera, (struct PaddleWheelBoat*) actor, sBillBoardMtx, pathCounter);
                break;
            case ACTOR_BOX_TRUCK:
                render_actor_box_truck(camera, actor);
                break;
            case ACTOR_SCHOOL_BUS:
                render_actor_school_bus(camera, actor);
                break;
            case ACTOR_TANKER_TRUCK:
                render_actor_tanker_truck(camera, actor);
                break;
            case ACTOR_CAR:
                render_actor_car(camera, actor);
                break;
            case ACTOR_RAILROAD_CROSSING:
                render_actor_railroad_crossing(camera, (struct RailroadCrossing*) actor);
                break;
            case ACTOR_YOSHI_EGG:
                render_actor_yoshi_egg(camera, sBillBoardMtx, (struct YoshiValleyEgg*) actor, pathCounter);
                break;
        }
        FrameInterpolation_RecordCloseChild();
    }
    if (IsMooMooFarm()) {
        render_cows(camera, sBillBoardMtx);
    } else if (IsDkJungle()) {
        render_palm_trees(camera, sBillBoardMtx);
    }
}

void update_course_actors(void) {
    struct Actor* actor;
    s32 i;
    for (i = 0; i < CM_GetActorSize(); i++) {

        actor = CM_GetActor(i);
        if (actor->flags == 0) {
            continue;
        }

        switch (actor->type) {
            case ACTOR_FALLING_ROCK: // now in C++
                //update_actor_falling_rocks((struct FallingRock*) actor);
                break;
            case ACTOR_GREEN_SHELL:
                update_actor_green_shell((struct ShellActor*) actor);
                break;
            case ACTOR_RED_SHELL:
                update_actor_red_blue_shell((struct ShellActor*) actor);
                break;
            case ACTOR_BLUE_SPINY_SHELL:
                update_actor_red_blue_shell((struct ShellActor*) actor);
                break;
            case ACTOR_KIWANO_FRUIT:
                update_actor_kiwano_fruit((struct KiwanoFruit*) actor);
                break;
            case ACTOR_BANANA:
                update_actor_banana((struct BananaActor*) actor);
                break;
            case ACTOR_PADDLE_BOAT:
                update_actor_paddle_boat((struct PaddleWheelBoat*) actor);
                break;
            case ACTOR_TRAIN_ENGINE:
                update_actor_train_engine((struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_TENDER:
                update_actor_train_tender((struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_PASSENGER_CAR:
                update_actor_train_passenger_car((struct TrainCar*) actor);
                break;
            case ACTOR_ITEM_BOX:
                update_actor_item_box((struct ItemBox*) actor);
                break;
            case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
                update_actor_item_box_hot_air_balloon((struct ItemBox*) actor);
                break;
            case ACTOR_FAKE_ITEM_BOX:
                update_actor_fake_item_box((struct FakeItemBox*) actor);
                break;
            case ACTOR_PIRANHA_PLANT:
                update_actor_piranha_plant((struct PiranhaPlant*) actor);
                break;
            case ACTOR_BANANA_BUNCH:
                update_actor_banana_bunch((struct BananaBunchParent*) actor);
                break;
            case ACTOR_TRIPLE_GREEN_SHELL:
                update_actor_triple_shell((TripleShellParent*) actor, ACTOR_GREEN_SHELL);
                break;
            case ACTOR_TRIPLE_RED_SHELL:
                update_actor_triple_shell((TripleShellParent*) actor, ACTOR_RED_SHELL);
                break;
            case ACTOR_MARIO_SIGN:
                update_actor_mario_sign(actor);
                break;
            case ACTOR_WARIO_SIGN:
                update_actor_wario_sign(actor);
                break;
            case ACTOR_RAILROAD_CROSSING:
                update_actor_railroad_crossing((struct RailroadCrossing*) actor);
                break;
            case ACTOR_TREE_MARIO_RACEWAY:
            case ACTOR_TREE_YOSHI_VALLEY:
            case ACTOR_TREE_ROYAL_RACEWAY:
            case ACTOR_TREE_MOO_MOO_FARM:
            case ACTOR_PALM_TREE:
            case ACTOR_TREE_LUIGI_RACEWAY: // A plant?
            case ACTOR_UNKNOWN_0x1B:
            case ACTOR_TREE_PEACH_CASTLE:
            case ACTOR_TREE_FRAPPE_SNOWLAND:
            case ACTOR_CACTUS1_KALAMARI_DESERT:
            case ACTOR_CACTUS2_KALAMARI_DESERT:
            case ACTOR_CACTUS3_KALAMARI_DESERT:
            case ACTOR_BUSH_BOWSERS_CASTLE:
                update_actor_static_plant(actor);
                break;
            case ACTOR_YOSHI_EGG:
                update_actor_yoshi_egg((struct YoshiValleyEgg*) actor);
                break;
        }
    }
    evaluate_collision_for_destructible_actors();
    check_player_use_item();
}

const char* get_actor_display_name(s32 id) {
    switch (id) {
        case ACTOR_FALLING_ROCK:
            return "Falling Rock";
        case ACTOR_GREEN_SHELL:
            return "Green Shell";
        case ACTOR_RED_SHELL:
            return "Red Shell";
        case ACTOR_BLUE_SPINY_SHELL:
            return "Blue Spiny Shell";
        case ACTOR_KIWANO_FRUIT:
            return "Kiwano Fruit";
        case ACTOR_BANANA:
            return "Banana";
        case ACTOR_PADDLE_BOAT:
            return "Paddle Boat";
        case ACTOR_TRAIN_ENGINE:
            return "Train Engine";
        case ACTOR_TRAIN_TENDER:
            return "Train Tender";
        case ACTOR_TRAIN_PASSENGER_CAR:
            return "Train Passenger Car";
        case ACTOR_CAR:
            return "Car Component";
        case ACTOR_SCHOOL_BUS:
            return "Bus Component";
        case ACTOR_TANKER_TRUCK:
            return "Tanker Truck Component";
        case ACTOR_BOX_TRUCK:
            return "Truck Component";
        case ACTOR_ITEM_BOX:
            return "Item Box";
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            return "Hot Air Balloon Item Box";
        case ACTOR_FAKE_ITEM_BOX:
            return "Fake Item Box";
        case ACTOR_PIRANHA_PLANT:
            return "Piranha Plant";
        case ACTOR_BANANA_BUNCH:
            return "Banana Bunch";
        case ACTOR_TRIPLE_GREEN_SHELL:
            return "Triple Green Shell";
        case ACTOR_TRIPLE_RED_SHELL:
            return "Triple Red Shell";
        case ACTOR_MARIO_SIGN:
            return "Mario Sign";
        case ACTOR_WARIO_SIGN:
            return "Wario Sign";
        case ACTOR_RAILROAD_CROSSING:
            return "Railroad Crossing";
        case ACTOR_TREE_MARIO_RACEWAY:
            return "Tree (Mario Raceway)";
        case ACTOR_TREE_YOSHI_VALLEY:
            return "Tree (Yoshi Valley)";
        case ACTOR_TREE_ROYAL_RACEWAY:
            return "Tree (Royal Raceway)";
        case ACTOR_TREE_MOO_MOO_FARM:
            return "Tree (Moo Moo Farm)";
        case ACTOR_PALM_TREE:
            return "Palm Tree";
        case ACTOR_TREE_LUIGI_RACEWAY:
            return "Tree (Luigi Raceway)";
        case ACTOR_UNKNOWN_0x1B:
            return "Unknown Plant (0x1B)";
        case ACTOR_TREE_PEACH_CASTLE:
            return "Tree (Peach's Castle)";
        case ACTOR_TREE_FRAPPE_SNOWLAND:
            return "Tree (Frappe Snowland)";
        case ACTOR_CACTUS1_KALAMARI_DESERT:
            return "Cactus 1 (Kalamari Desert)";
        case ACTOR_CACTUS2_KALAMARI_DESERT:
            return "Cactus 2 (Kalamari Desert)";
        case ACTOR_CACTUS3_KALAMARI_DESERT:
            return "Cactus 3 (Kalamari Desert)";
        case ACTOR_BUSH_BOWSERS_CASTLE:
            return "Bush (Bowser's Castle)";
        case ACTOR_YOSHI_EGG:
            return "Yoshi Egg";
        default:
            return "Obj";
    }
}

// Returns a namespace:path
const char* get_actor_resource_location_name(s32 id) {
    switch (id) {
        case ACTOR_FALLING_ROCK:
            return "mk:falling_rock";
        case ACTOR_GREEN_SHELL:
            return "mk:green_shell";
        case ACTOR_RED_SHELL:
            return "mk:red_shell";
        case ACTOR_BLUE_SPINY_SHELL:
            return "mk:blue_spiny_shell";
        case ACTOR_KIWANO_FRUIT:
            return "mk:kiwano_fruit";
        case ACTOR_BANANA:
            return "mk:banana";
        case ACTOR_PADDLE_BOAT:
            return "mk:paddle_boat";
        case ACTOR_TRAIN_ENGINE:
            return "mk:train_engine";
        case ACTOR_TRAIN_TENDER:
            return "mk:train_tender";
        case ACTOR_TRAIN_PASSENGER_CAR:
            return "mk:train_passenger_car";
        case ACTOR_ITEM_BOX:
            return "mk:item_box";
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            return "mk:hot_air_balloon_item_box";
        case ACTOR_FAKE_ITEM_BOX:
            return "mk:fake_item_box";
        case ACTOR_PIRANHA_PLANT:
            return "mk:piranha_plant";
        case ACTOR_BANANA_BUNCH:
            return "mk:banana_bunch";
        case ACTOR_TRIPLE_GREEN_SHELL:
            return "mk:triple_green_shell";
        case ACTOR_TRIPLE_RED_SHELL:
            return "mk:triple_red_shell";
        case ACTOR_MARIO_SIGN:
            return "mk:mario_sign";
        case ACTOR_WARIO_SIGN:
            return "mk:wario_sign";
        case ACTOR_RAILROAD_CROSSING:
            return "mk:railroad_crossing";
        case ACTOR_TREE_MARIO_RACEWAY:
            return "mk:tree_mario_raceway";
        case ACTOR_TREE_YOSHI_VALLEY:
            return "mk:tree_yoshi_valley";
        case ACTOR_TREE_ROYAL_RACEWAY:
            return "mk:tree_royal_raceway";
        case ACTOR_TREE_MOO_MOO_FARM:
            return "mk:tree_moo_moo_farm";
        case ACTOR_PALM_TREE:
            return "mk:palm_tree";
        case ACTOR_TREE_LUIGI_RACEWAY:
            return "mk:tree_luigi_raceway";
        case ACTOR_UNKNOWN_0x1B:
            return "mk:unknown_0x1b";
        case ACTOR_TREE_PEACH_CASTLE:
            return "mk:tree_peach_castle";
        case ACTOR_TREE_FRAPPE_SNOWLAND:
            return "mk:tree_frappe_snowland";
        case ACTOR_CACTUS1_KALAMARI_DESERT:
            return "mk:cactus1_kalamari_desert";
        case ACTOR_CACTUS2_KALAMARI_DESERT:
            return "mk:cactus2_kalamari_desert";
        case ACTOR_CACTUS3_KALAMARI_DESERT:
            return "mk:cactus3_kalamari_desert";
        case ACTOR_BUSH_BOWSERS_CASTLE:
            return "mk:bush_bowsers_castle";
        case ACTOR_YOSHI_EGG:
            return "mk:yoshi_egg";
        default:
            return "mk:actor";
    }
}
