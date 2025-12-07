#include <libultraship.h>
#include "engine/objects/Object.h"
#include "BombKart.h"
#include <vector>

#include "port/Game.h"
#include "engine/Matrix.h"

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
#include <assets/models/common_data.h>
extern s8 gPlayerCount;
}

size_t OBombKart::_count = 0;

OBombKart::OBombKart(const SpawnParams& params) : OObject(params) {
    Name = "Bomb Kart";
    ResourceName = "mk:bomb_kart";

    _idx = _count;
    uint32_t pathIndex = params.PathIndex.value_or(0);
    uint32_t pathPoint = params.PathPoint.value_or(0);
    FVector constPos;

    // Spawn kart on a surface with the provided position
    if (params.Location.has_value()) {
        constPos = params.Location.value();

        // Set height to the default value of 2000.0f unless Pos[1] is higher.
        // This allows placing these on very high surfaces.
        f32 height = (constPos.y > 2000.0f) ? constPos.y : 2000.0f;
        constPos.y = spawn_actor_on_surface(constPos.x, height, constPos.z);
    } else { // Spawn kart on waypoint
        constPos.x = gTrackPaths[pathIndex][pathPoint].x;
        constPos.y = gTrackPaths[pathIndex][pathPoint].y;
        constPos.z = gTrackPaths[pathIndex][pathPoint].z;
    }

    Behaviour = static_cast<OBombKart::States>(params.Behaviour.value_or(OBombKart::States::COUNTERCLOCKWISE));
    SpeedB = params.SpeedB.value_or(2.7f); // Chase speed

    WaypointIndex = params.PathPoint.value_or(0);
    Unk_3C = params.Speed.value_or(0);

    Pos[0] = constPos.x;
    Pos[1] = constPos.y;
    Pos[2] = constPos.z;
    CenterY = constPos.y;
    WheelPos[0][0] = constPos.x;
    WheelPos[0][1] = constPos.y;
    WheelPos[0][2] = constPos.z;
    WheelPos[1][0] = constPos.x;
    WheelPos[1][1] = constPos.y;
    WheelPos[1][2] = constPos.z;
    WheelPos[2][0] = constPos.x;
    WheelPos[2][1] = constPos.y;
    WheelPos[2][2] = constPos.z;
    WheelPos[3][0] = constPos.x;
    WheelPos[3][1] = constPos.y;
    WheelPos[3][2] = constPos.z;
    check_bounding_collision(&_Collision, 2.0f, constPos.x, constPos.y, constPos.z);

    find_unused_obj_index(&_objectIndex);

    Object* object = &gObjectList[_objectIndex];

    object->origin_pos[0] = Pos[0];
    object->origin_pos[1] = Pos[1];
    object->origin_pos[2] = Pos[2];

    _count++;
}

void OBombKart::Tick() {
    f32 sp118;
    f32 var_f18;
    TrackPathPoint* temp_v0_2;
    f32 temp_f0_3;
    f32 sp108;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f0;
    f32 temp_f0_4;
    u16 waypoint;
    f32 unk_3C;
    u16 someRot;
    f32 temp_f12;
    f32 temp_f12_3;
    f32 temp_f12_4;
    f32 temp_f14_2;
    f32 spAC;
    f32 temp_f16_2;
    f32 spA0;
    f32 temp_f2;
    f32 temp_f2_4;
    f32 sp94;
    f32 sp88;
    Vec3f newPos;
    OBombKart::States state;
    u16 bounceTimer;
    UNUSED u16 sp4C;
    u16 temp_t6;
    u16 temp_t7;
    u16 circleTimer;
    TrackPathPoint* temp_v0_4;
    Player* player;

    state = Behaviour;

    if (state == States::DISABLED) {
        return;
    }

    if (((Unk_4A != 1) || (IsPodiumCeremony()))) {
        newPos[0] = Pos[0];
        newPos[1] = Pos[1];
        newPos[2] = Pos[2];
        waypoint = WaypointIndex;
        unk_3C = Unk_3C;
        someRot = SomeRot;
        bounceTimer = BounceTimer;
        circleTimer = CircleTimer;
        if ((state != States::DISABLED) && (state != States::EXPLODE)) {
            if (IsPodiumCeremony()) {
                if (D_8016347E == 1) {
                    player = gPlayerFour;
                    temp_f0 = newPos[0] - player->pos[0];
                    temp_f2 = newPos[2] - player->pos[1];
                    temp_f12 = newPos[2] - player->pos[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f12 * temp_f12)) < 25.0f) {
                        circleTimer = 0;
                        state = States::EXPLODE;
                        Behaviour = States::EXPLODE;
                        player->soundEffects |= 0x400000;
                        player->type &= ~0x2000;
                    }
                }
            } else {

                for (size_t i = 0; i < gPlayerCount; i++) {
                    player = &gPlayers[i];
                    if (!(player->effects & 0x80000000)) {
                        temp_f0 = newPos[0] - player->pos[0];
                        temp_f2 = newPos[1] - player->pos[1];
                        temp_f12 = newPos[2] - player->pos[2];
                        if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f12 * temp_f12)) < 25.0f) {
                            state = States::EXPLODE;
                            Behaviour = States::EXPLODE;
                            circleTimer = 0;
                            if (IsFrappeSnowland()) {
                                player->soundEffects |= 0x01000000;
                            } else {
                                player->soundEffects |= 0x400000;
                            }
                        }
                    }
                }
            }
        }
        switch(state) {
            case States::COUNTERCLOCKWISE:
                circleTimer = (circleTimer + 356) % 360;
                temp_t6 = (circleTimer * 0xFFFF) / 360;
                sp118 = coss(temp_t6) * 25.0;
                temp_f0_3 = sins(temp_t6) * 25.0;
                temp_v0_2 = &gTrackPaths[0][waypoint];
                newPos[0] = temp_v0_2->x + sp118;
                newPos[1] = CenterY + 3.5f;
                newPos[2] = temp_v0_2->z + temp_f0_3;
                D_80162FB0[0] = newPos[0];
                D_80162FB0[1] = newPos[1];
                D_80162FB0[2] = newPos[2];
                temp_t7 = (((circleTimer + 1) % 360) * 0xFFFF) / 360;
                sp118 = coss(temp_t7) * 25.0;
                temp_f0_3 = sins(temp_t7) * 25.0;
                D_80162FC0[0] = temp_v0_2->x + sp118;
                D_80162FC0[1] = temp_v0_2->y;
                D_80162FC0[2] = temp_v0_2->z + temp_f0_3;
                someRot = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                break;
            case States::CLOCKWISE:
                circleTimer = (circleTimer + 4) % 360;
                temp_t6 = (circleTimer * 0xFFFF) / 360;
                sp118 = coss(temp_t6) * 25.0;
                temp_f0_3 = sins(temp_t6) * 25.0;
                temp_v0_2 = &gTrackPaths[0][waypoint];
                newPos[0] = temp_v0_2->x + sp118;
                newPos[1] = CenterY + 3.5f;
                newPos[2] = temp_v0_2->z + temp_f0_3;
                D_80162FB0[0] = newPos[0];
                D_80162FB0[1] = newPos[1];
                D_80162FB0[2] = newPos[2];
                temp_t7 = (((circleTimer + 1) % 360) * 0xFFFF) / 360;
                sp118 = coss(temp_t7) * 25.0;
                temp_f0_3 = sins(temp_t7) * 25.0;
                D_80162FC0[0] = temp_v0_2->x + sp118;
                D_80162FC0[1] = temp_v0_2->y;
                D_80162FC0[2] = temp_v0_2->z + temp_f0_3;
                someRot = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                break;
            case States::STATIONARY:
                newPos[1] = CenterY + 3.5f;
                someRot = 0;
                break;

            case States::PODIUM_CEREMONY:
                if ((D_8016347C == 0) || (gNearestPathPointByPlayerId[3] < 5)) {
                    break;
                } else {
                    waypoint = func_8000D2B4(newPos[0], newPos[1], newPos[2], waypoint, 3);
                    if ((waypoint < 0) || (gPathCountByPathIndex[3] < waypoint)) {
                        waypoint = 0;
                    }
                    if (((s32) waypoint) < 0x1A) {
                        temp_v0_2 = &gTrackPaths[3][(waypoint + 1) % gPathCountByPathIndex[3]];
                        D_80162FB0[0] = temp_v0_2->x;
                        D_80162FB0[1] = temp_v0_2->y;
                        D_80162FB0[2] = temp_v0_2->z;
                        temp_v0_4 = &gTrackPaths[3][(waypoint + 2) % gPathCountByPathIndex[3]];
                        D_80162FC0[0] = temp_v0_4->x;
                        D_80162FC0[1] = temp_v0_4->y;
                        D_80162FC0[2] = temp_v0_4->z;
                        someRot = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                    } else {
                        D_80162FB0[0] = newPos[0];
                        D_80162FB0[1] = newPos[1];
                        D_80162FB0[2] = newPos[2];
                        D_80162FC0[0] = -2409.197f;
                        D_80162FC0[1] = 0.0f;
                        D_80162FC0[2] = -355.254f;
                        someRot = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                    }
                    temp_f14 = ((D_80162FB0[0] + D_80162FC0[0]) * 0.5f) - newPos[0];
                    temp_f16 = ((D_80162FB0[2] + D_80162FC0[2]) * 0.5f) - newPos[2];
                    temp_f0_4 = sqrtf((temp_f14 * temp_f14) + (temp_f16 * temp_f16));
                    if (temp_f0_4 > 0.01f) {
                        newPos[0] += (Unk_3C * temp_f14) / temp_f0_4;
                        newPos[2] += (Unk_3C * temp_f16) / temp_f0_4;
                    } else {
                        newPos[0] += temp_f14 / 5.0f;
                        newPos[2] += temp_f16 / 5.0f;
                    }
                    newPos[1] = calculate_surface_height(newPos[0], 2000.0f, newPos[2], _Collision.meshIndexZX) + 3.5f;
                    if (newPos[1] < (-1000.0)) {
                        newPos[1] = Pos[1];
                    }
                    check_bounding_collision(&_Collision, 10.0f, newPos[0], newPos[1], newPos[2]);
                }
                break;
            case States::CHASE:
                // Prevents chasing during race staging
                if (gRaceState != RACE_IN_PROGRESS)  {
                    return;
                }

                if (!_target) {
                    _target = OBombKart::FindTarget();
                } else {
                    OBombKart::Chase(_target, newPos);
                }
                break;
            case States::EXPLODE:
                temp_v0_2 = &gTrackPaths[0][waypoint];
                D_80162FB0[0] = temp_v0_2->x;
                D_80162FB0[1] = temp_v0_2->y;
                D_80162FB0[2] = temp_v0_2->z;
                temp_v0_4 = &gTrackPaths[0][(waypoint + 1) % gPathCountByPathIndex[0]];
                D_80162FC0[0] = temp_v0_4->x;
                D_80162FC0[1] = temp_v0_4->y;
                D_80162FC0[2] = temp_v0_4->z;
                newPos[1] += 3.0f - (circleTimer * 0.3f);
                someRot = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                break;
            default:
                break;
        }

        if (state == States::EXPLODE) {
            sp108 = 2.0f * circleTimer;
            sp118 = coss(0xFFFF - someRot) * circleTimer;
            var_f18 = sins(0xFFFF - someRot) * circleTimer;
            circleTimer++;
            temp_f2_4 = (newPos[1] - 2.3f) + (sp108 / 3.0f);
            spAC = temp_f2_4;
            spA0 = temp_f2_4;
            sp94 = temp_f2_4;
            sp88 = temp_f2_4;
            if (circleTimer > 30) {
                state = States::DISABLED;
                Behaviour = States::DISABLED;
            }
        } else {
            sp118 = coss(0xFFFF - someRot) * 1.5f;
            var_f18 = sins(0xFFFF - someRot) * 1.5f;
            temp_f16_2 = newPos[1] - 2.3f;
            temp_f12_3 = (bounceTimer % 3) * 0.15f;
            temp_f14_2 = temp_f16_2 - temp_f12_3;
            temp_f12_4 = temp_f16_2 + temp_f12_3;
            spAC = temp_f14_2;
            sp94 = temp_f14_2;
            spA0 = temp_f12_4;
            sp88 = temp_f12_4;
            newPos[1] += sins((bounceTimer * 0x13FFEC) / 360);
            bounceTimer = (bounceTimer + 1) % 18;
        }

        WheelPos[0][0] = (sp118 - var_f18) + newPos[0];
        WheelPos[0][1] = spAC;
        WheelPos[0][2] = (var_f18 + sp118) + newPos[2];
        WheelPos[1][0] = (var_f18 + sp118) + newPos[0];
        WheelPos[1][1] = spA0;
        WheelPos[1][2] = (var_f18 - sp118) + newPos[2];
        WheelPos[2][0] = ((-sp118) - var_f18) + newPos[0];
        WheelPos[2][1] = sp94;
        WheelPos[2][2] = ((-var_f18) + sp118) + newPos[2];
        WheelPos[3][0] = ((-sp118) + var_f18) + newPos[0];
        WheelPos[3][1] = sp88;
        WheelPos[3][2] = ((-var_f18) - sp118) + newPos[2];
        Pos[0] = newPos[0];
        Pos[1] = newPos[1];
        Pos[2] = newPos[2];
        WaypointIndex = waypoint;
        Unk_3C = unk_3C;
        SomeRot = someRot;
        // State = state;
        BounceTimer = bounceTimer;
        CircleTimer = circleTimer;
    }
}

void OBombKart::Draw(s32 cameraId) {
    if (gModeSelection == BATTLE) {
        for (size_t playerId = 0; playerId < NUM_BOMB_KARTS_BATTLE; playerId++) {
            Object* object = &gObjectList[_objectIndex];
            if (object->state != 0) {
                s32 primAlpha = object->primAlpha;
                Player* player = &gPlayerOne[playerId];
                object->pos[0] = player->pos[0];
                object->pos[1] = player->pos[1] - 2.0;
                object->pos[2] = player->pos[2];
                object->surfaceHeight = player->unk_074;
                func_800563DC(_objectIndex, cameraId, primAlpha);
                func_8005669C(_objectIndex, cameraId, primAlpha);
                func_800568A0(_objectIndex, cameraId);
            }
        }
        return;
    }

    if (IsPodiumCeremony()) {
        if ((_idx == 0) && (WaypointIndex < 16)) {
            return;
        } else {
            cameraId = PLAYER_FOUR;
        }
    }

    if (gGamestate == ENDING) {
        cameraId = 0;
    }
    Camera* camera = &camera1[cameraId];
    if (cameraId == PLAYER_ONE) {
        if (is_obj_flag_status_active(_objectIndex, 0x00200000) != 0) {
            Unk_4A = 0;
        } else if (gGamestate != ENDING) {
            Unk_4A = 1;
        }
        clear_object_flag(_objectIndex, 0x00200000);
    }

    // huh???
    OBombKart::States state = Behaviour;
    if (state != States::DISABLED) {
        gObjectList[_objectIndex].pos[0] = Pos[0];
        gObjectList[_objectIndex].pos[1] = Pos[1];
        gObjectList[_objectIndex].pos[2] = Pos[2];
        s32 temp_s4 = func_8008A364(_objectIndex, cameraId, 0x31C4U, 0x000001F4);
        if (is_obj_flag_status_active(_objectIndex, VISIBLE) != 0) {
            set_object_flag(_objectIndex, 0x00200000);
            D_80183E80[0] = 0;
            D_80183E80[1] = func_800418AC(Pos[0], Pos[2], camera->pos);
            D_80183E80[2] = 0x8000;
            func_800563DC(_objectIndex, cameraId, 0x000000FF);
            OBombKart::SomeRender(camera->pos);
            if (((u32) temp_s4 < 0x4E21U) && (state != OBombKart::States::EXPLODE)) {
                OBombKart::LoadMtx();
            }
        }
    }
}

void OBombKart::DrawBattle(s32 cameraId) {

}

void OBombKart::SomeRender(Vec3f arg1) {
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    gSPDisplayList(gDisplayListHead++, (Gfx*)D_0D0079C8);
    load_texture_block_rgba16_mirror((u8*) D_0D02AA58, 0x00000010, 0x00000010);
    D_80183E80[1] = func_800418AC(WheelPos[0][0], WheelPos[0][2], arg1);
    func_800431B0(WheelPos[0], D_80183E80, 0.15f, (Vtx*)common_vtx_rectangle);
    D_80183E80[1] = func_800418AC(WheelPos[1][0], WheelPos[1][2], arg1);
    func_800431B0(WheelPos[1], D_80183E80, 0.15f, (Vtx*)common_vtx_rectangle);
    D_80183E80[1] = func_800418AC(WheelPos[2][0], WheelPos[2][2], arg1);
    func_800431B0(WheelPos[2], D_80183E80, 0.15f, (Vtx*)common_vtx_rectangle);
    D_80183E80[1] = func_800418AC(WheelPos[3][0], WheelPos[3][2], arg1);
    func_800431B0(WheelPos[3], D_80183E80, 0.15f, (Vtx*)common_vtx_rectangle);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void OBombKart::LoadMtx() {
    Mat4 mat;

    D_80183E50[0] = Pos[0];
    D_80183E50[1] = CenterY + 1.0;
    D_80183E50[2] = Pos[2];
    set_transform_matrix(mat, _Collision.orientationVector, D_80183E50, 0U, 0.5f);
    //convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], mat);

    AddHudMatrix(mat, G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);

    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, (Gfx*)D_0D007B98);
}

void OBombKart::Waypoint(s32 screenId) {
    s32 playerWaypoint;
    s32 bombWaypoint;
    s32 waypointDiff;

    playerWaypoint = gNearestPathPointByPlayerId[screenId];
    playerHUD[screenId].unk_74 = 0;

    OBombKart::States state = Behaviour;
    if ((state == States::EXPLODE) || (state == States::DISABLED)) { return; };
    bombWaypoint = WaypointIndex;
    waypointDiff = bombWaypoint - playerWaypoint;
    if ((waypointDiff < -5) || (waypointDiff > 0x1E)) { return; };
    playerHUD[screenId].unk_74 = 1;
}

Player* OBombKart::FindTarget() {
    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        if (gPlayers[i].type & PLAYER_HUMAN) {
            if (is_within_distance_2d(Pos[0], Pos[2], gPlayers[i].pos[0], gPlayers[i].pos[2], 500)) {
                return &gPlayers[i];
            }
        }
    }
    return NULL;
}

void OBombKart::Chase(Player* player, Vec3f pos) {
    const f32 speed = SpeedB; // Speed the kart uses in a chase

    if (!player) return; // Ensure player is valid

    // Calculate the directional vector toward the player
    Vec3f direction;
    direction[0] = player->pos[0] - pos[0];
    direction[1] = player->pos[1] - pos[1];
    direction[2] = player->pos[2] - pos[2];

    // Calculate the distance in the XZ plane
    f32 xz_dist = sqrtf((direction[0] * direction[0]) + (direction[2] * direction[2]));

    // Reset distance
    if (xz_dist > 700.0f) {
        _target = NULL;
        pos[0] = gObjectList[_objectIndex].origin_pos[0];
        pos[1] = gObjectList[_objectIndex].origin_pos[1];
        pos[2] = gObjectList[_objectIndex].origin_pos[2];
        return;
    }

    // Break off the chase if player has boo item
    if (player->effects & BOO_EFFECT) {
        _target = NULL;
        pos[0] = gObjectList[_objectIndex].origin_pos[0];
        pos[1] = gObjectList[_objectIndex].origin_pos[1];
        pos[2] = gObjectList[_objectIndex].origin_pos[2];
        return;
    }

    if (xz_dist > 0.0f) {
        
        // Normalize the direction vector in the XZ plane
        direction[0] /= xz_dist;
        direction[2] /= xz_dist;

        // Scale the movement by a fixed step size
        pos[0] += direction[0] * speed;
        pos[2] += direction[2] * speed;
    }

    Vec3f newPosition;
    newPosition[0] = pos[0];
    newPosition[1] = pos[1];
    newPosition[2] = pos[2];

    newPosition[1] = calculate_surface_height(pos[0], 2000.0f, pos[2], _Collision.meshIndexZX) + 3.5f;

    pos[0] = newPosition[0];
    pos[1] = newPosition[1];
    pos[2] = newPosition[2];

    check_bounding_collision(&_Collision, 10.0f, pos[0], pos[1], pos[2]);
}

void OBombKart::Translate(FVector pos) {
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;
    if (_objectIndex != -1) {
        Object* object = &gObjectList[_objectIndex];
        object->pos[0] = pos.x;
        object->pos[1] = pos.y;
        object->pos[2] = pos.z;
        object->origin_pos[0] = pos.x;
        object->origin_pos[1] = pos.y;
        object->origin_pos[2] = pos.z;
    } else {
        printf("Editor tried to translate null OObject\n");
    }
}

void OBombKart::DrawEditorProperties() {
    Object* obj = &gObjectList[_objectIndex];

    ImGui::Text("Behaviour");
    ImGui::SameLine();

    int32_t behaviour = static_cast<int32_t>(Behaviour);
    const char* items[] = { "Disabled", "Counterclockwise", "Clockwise", "Stationary", "Chase", "Explode", "Podium" };

    if (ImGui::Combo("##Behaviour", &behaviour, items, IM_ARRAYSIZE(items))) {
        Behaviour = static_cast<OBombKart::States>(behaviour);
    }

    ImGui::Text("Location");
    ImGui::SameLine();
    FVector location = FVector(obj->pos[0], obj->pos[1], obj->pos[2]);
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

    ImGui::Text("Chase Speed");
    ImGui::SameLine();

    float speed = SpeedB;
    if (ImGui::DragFloat("##Speed", &speed, 0.1f)) {
        SpeedB = speed;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetSpeed")) {
        SpeedB = 2.7f;
    }
}
