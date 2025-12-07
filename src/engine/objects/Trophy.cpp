#include "Trophy.h"
#include "assets/models/common_data.h"
#include "assets/models/data_segment2.h"
#include "port/Game.h"

extern "C" {
#include "main.h"
#include "defines.h"
#include "update_objects.h"
#include "code_80057C60.h"
#include "assets/models/ceremony_data.h"
#include "podium_ceremony_actors.h"
#include "engine/particles/StarEmitter.h"
#include "math_util.h"
#include "math_util_2.h"
#include "data/some_data.h"
#include "engine/Matrix.h"
#include "render_objects.h"
#include "code_80086E70.h"
#include "menu_items.h"
}

OTrophy::OTrophy(const SpawnParams& params) : OObject(params) {
    Name = "Trophy";
    ResourceName = "mk:trophy";
    _type = static_cast<TrophyType>(params.Type.value_or(0));
    FVector spawnPos = params.Location.value_or(FVector(0, 0, 0));
    spawnPos.y += 16.0f; // Adjust the height so the trophy sits on the surface when positioned to 0,0,0
    SpawnPos = spawnPos; // Don't save the + 16.0f adjustment
    _bhv = static_cast<Behaviour>(params.Behaviour.value_or(0));

    find_unused_obj_index(&_objectIndex);

    // If you're trying to place multiple trophies in the podium ceremony
    // This will make the camera look at the last trophy spawned.
    // Thus this will need to be changed if that's not desired.
    gTrophyIndex = _objectIndex;

    if (_bhv == OTrophy::Behaviour::PODIUM_CEREMONY) {
        _toggleVisibility = &D_801658CE;
    } else {
        _toggle = true;
        _toggleVisibility = &_toggle;
        _isMod = true;
    }

    // This allows spawning for mods
    if (*_toggleVisibility == true) {
        init_object(_objectIndex, 0);
    }

    switch (_type) {
        case TrophyType::GOLD:
            gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl10;
            break;
        case TrophyType::SILVER:
            gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl12;
            break;
        case TrophyType::BRONZE:
            gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl14;
            break;
        case TrophyType::GOLD_150:
            gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl11;
            break;
        case TrophyType::SILVER_150:
            gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl13;
            break;
        case TrophyType::BRONZE_150:
            gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl15;
            break;
    }

    // Set defaults for modded behaviours
    if (_bhv != OTrophy::Behaviour::PODIUM_CEREMONY) {
        gObjectList[_objectIndex].sizeScaling = 0.025f;
        gObjectList[_objectIndex].unk_084[1] = 0x0200;
        object_next_state(_objectIndex);
        func_80086E70(_objectIndex);
    }

    switch(_bhv) {
        case OTrophy::Behaviour::GO_FISH:
            gObjectList[_objectIndex].sizeScaling = 0.010f;
            break;
    }

    Object *object = &gObjectList[_objectIndex];
    object->origin_pos[0] = spawnPos.x;
    object->origin_pos[1] = spawnPos.y;
    object->origin_pos[2] = spawnPos.z;
    object->pos[0] = spawnPos.x;
    object->pos[1] = spawnPos.y;
    object->pos[2] = spawnPos.z;

    _emitter = reinterpret_cast<StarEmitter*>(gWorldInstance.AddEmitter(new StarEmitter()));
}

void OTrophy::SetSpawnParams(SpawnParams& params) {
    OObject::SetSpawnParams(params);
    Object *object = &gObjectList[_objectIndex];
    params.Type = _type;
    params.Behaviour = _bhv;
}

void OTrophy::Tick() { // func_80086D80
    s32 objectIndex = _objectIndex;
    s32 var_s0;

    // Fallback for podium ceremony where the trophy is not spawned until it is needed
    if ((*_toggleVisibility == true) && (D_801658DC == 0) && (_isMod == false)) {
        init_object(objectIndex, 0);
        D_801658DC = 1;
    }

    switch(_bhv) {
        case OTrophy::Behaviour::PODIUM_CEREMONY:
            if (gObjectList[objectIndex].state != 0 && (*_toggleVisibility == true)) {
                OTrophy::func_80086C14(objectIndex);
                OTrophy::func_80086940(objectIndex);
                if (D_801658F4 != 0) {
                    if (D_8016559C == 0) {
                        OTrophy::func_80086C6C(objectIndex);
                    }
                } else {
                    for (var_s0 = 0; var_s0 < 2; var_s0++) {
                        OTrophy::func_80086C6C(objectIndex);
                    }
                }
            }
            break;

        case OTrophy::Behaviour::STATIONARY:
            if (gObjectList[objectIndex].state != 0) {
                    gObjectList[objectIndex].sizeScaling = 0.025f;

                    set_obj_origin_pos(objectIndex, SpawnPos.x,
                                    SpawnPos.y + 16.0, SpawnPos.z);
                    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
                    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
                    gObjectList[objectIndex].unk_084[1] = 0x0200;
                    object_next_state(objectIndex);
                    func_80086E70(objectIndex);
                    OTrophy::func_80086940(objectIndex);
            }
            break;
        case OTrophy::Behaviour::ROTATE:
            if (gObjectList[objectIndex].state != 0) {

                gObjectList[objectIndex].direction_angle[0] += 0x400;
                gObjectList[objectIndex].direction_angle[1] -= 0x200;
            }
            break;
        case OTrophy::Behaviour::ROTATE2:
            if (gObjectList[objectIndex].state != 0) {

                gObjectList[objectIndex].direction_angle[0] += 0x400;
                gObjectList[objectIndex].direction_angle[1] = 0xE800;
                gObjectList[objectIndex].direction_angle[2] = 0xDA00;
            }
            break;
        case OTrophy::Behaviour::GO_FISH:
            if (gObjectList[objectIndex].state != 0) {

                // Get the player's yaw
                Player *player = &gPlayers[0];
                float yaw = (player->rotation[1] + 0x4000) * (M_PI / 32768.0f);   // Convert degrees to radians

                // Calculate forward direction based on yaw (same as before)
                float lookAtX = player->pos[0] + cos(yaw);
                float lookAtZ = player->pos[2] + sin(yaw);

                float forwardX = lookAtX - player->pos[0];
                float forwardY = 0; // Optional: Ignore height changes
                float forwardZ = lookAtZ - player->pos[2];

                // Normalize the forward vector
                float length = sqrtf(forwardX * forwardX + forwardZ * forwardZ);
                if (length != 0) {
                    forwardX /= length;
                    forwardZ /= length;
                }

                float distance = 30.0f;

                // Calculate the object's position in front of the player (same as before)
                gObjectList[objectIndex].pos[0] = player->pos[0] + forwardX * distance;
                gObjectList[objectIndex].pos[1] = player->pos[1] + 8.0f;  // Optional height offset
                gObjectList[objectIndex].pos[2] = player->pos[2] + forwardZ * distance;

                // Apply more sensitive random movement based on player velocity to simulate floating behavior
                float velocityFactor = 0.1f; // Increased factor for more sensitivity
                gObjectList[objectIndex].pos[0] += player->velocity[0] * velocityFactor;
                gObjectList[objectIndex].pos[1] += player->velocity[1] * velocityFactor; // Optional: Add vertical movement
                gObjectList[objectIndex].pos[2] += player->velocity[2] * velocityFactor;

                // Increase oscillation for more dynamic movement (sine wave effect)
                float oscillationSpeed = 4.0f; // Increased speed for quicker oscillations
                float oscillationAmplitude = 0.4f; // Increased amplitude for more noticeable movement
                gObjectList[objectIndex].pos[1] += oscillationAmplitude * sinf(oscillationSpeed * gGlobalTimer); // Vertical oscillation based on time

                // Now use smooth interpolation (lerp) to gradually move the trophy towards its target position
                float lerpFactor = 0.25f;  // Increased to make the trophy follow the player more quickly

                gObjectList[objectIndex].pos[0] = _oldPos[0] + lerpFactor * (gObjectList[objectIndex].pos[0] - _oldPos[0]);
                gObjectList[objectIndex].pos[1] = _oldPos[1] + lerpFactor * (gObjectList[objectIndex].pos[1] - _oldPos[1]);
                gObjectList[objectIndex].pos[2] = _oldPos[2] + lerpFactor * (gObjectList[objectIndex].pos[2] - _oldPos[2]);

                // Save the current position for the next frame
                _oldPos[0] = gObjectList[objectIndex].pos[0];
                _oldPos[1] = gObjectList[objectIndex].pos[1];
                _oldPos[2] = gObjectList[objectIndex].pos[2];

            
                gObjectList[objectIndex].direction_angle[0] += 0x400;
                gObjectList[objectIndex].direction_angle[1] -= 0x200;

                // Emit particles when player is moving quickly
                if (((player->speed / 18.0f) * 216.0f) >= 60.0f) {
                    OTrophy::func_80086C6C(objectIndex);
                }
            }
            break;
    }
}

void OTrophy::Draw(s32 cameraId) {
    s32 listIndex = _objectIndex;
    Mat4 someMatrix1;
    Mat4 someMatrix2;
    Object* object;
    if (*_toggleVisibility == true) {
        object = &gObjectList[listIndex];
        if (object->state >= 2) {
            gSPMatrix(gDisplayListHead++, cameras[cameraId].perspectiveMatrix,
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, cameras[cameraId].lookAtMatrix,

                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            mtxf_set_matrix_transformation(someMatrix1, object->pos, object->direction_angle, object->sizeScaling);
            //convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], someMatrix1);
            //gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
            //          G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            AddHudMatrix(someMatrix1, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            gSPDisplayList(gDisplayListHead++, (Gfx*)D_0D0077A0);
            gSPDisplayList(gDisplayListHead++, object->model);

            gSPMatrix(gDisplayListHead++, cameras[cameraId].lookAtMatrix,
                    G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

            mtxf_identity(someMatrix2);
            render_set_position(someMatrix2, 0);
        }

        switch (_bhv) {
            case GO_FISH:
                size_t numTrophies = 0;
                func_80057A50(40, 22, (char*) "Trophies Collected: ", (s16) numTrophies);
                break;
        }
    }
}

void OTrophy::func_80086700(s32 objectIndex) {
    gObjectList[objectIndex].sizeScaling = 0.005f;
    set_obj_origin_pos(objectIndex, gObjectList[indexObjectList2[0]].pos[0],
                       gObjectList[indexObjectList2[0]].pos[1] + 16.0, gObjectList[indexObjectList2[0]].pos[2]);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    gObjectList[objectIndex].unk_084[1] = 0x0200;
    object_next_state(objectIndex);
    func_80086E70(objectIndex);
}

void OTrophy::func_80086940(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->unk_0AE) {
        case 0:
            break;
        case 1:
            func_80086FD4(objectIndex);
            break;
        case 2:
            f32_step_towards(&object->sizeScaling, 0.025f, 0.001f);
            func_80087C48(objectIndex, 6.0f, 0.1f, 0x000000C8);
            if ((f64) object->velocity[1] <= 0.0) {
                func_8008701C(objectIndex, 3);
            }
            break;
        case 3:
            func_800871AC(objectIndex, 0x00000064);
            break;
        case 4:
            D_801658D6 = 1;
            object->velocity[1] = -0.4f;
            func_80086FD4(objectIndex);
            // object->origin_pos[1] = 90.0f;
            object->offset[1] = 60.0f;
            // switch (D_802874D8.unk1D) {
            //     case 1:
            //         object->origin_pos[0] -= 3.0;
            //         object->origin_pos[2] += 15.0;
            //         break;
            //     case 2:
            //         object->origin_pos[0] -= 2.0;
            //         object->origin_pos[2] -= 15.0;
            //         break;
            // }
            break;
        case 5:
            if ((f64) object->offset[1] <= 8.0) {
                f32_step_towards(&object->velocity[1], -0.1f, -0.01f);
            }
            object_add_velocity_offset_y(objectIndex);
            if ((f64) object->offset[1] <= 0.0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 6:
            if (func_800871AC(objectIndex, 0x00000041) != 0) {
                D_801658F4 = 1;
            }
            break;
        case 7:
            if (func_800871AC(objectIndex, 0x00000064) != 0) {
                func_8009265C();
                func_80086F60(objectIndex);
            }
            break;
    }
    if (D_801658D6 != 0) {
        object->direction_angle[0] += 0x400;
        object->direction_angle[1] = 0xE800;
        object->direction_angle[2] = 0xDA00;
    } else {
        object->direction_angle[0] += 0x400;
        object->direction_angle[1] -= 0x200;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void OTrophy::func_80086C14(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 1:
            OTrophy::func_80086700(objectIndex);
            break;
        case 0:
        case 2:
            break;
    }
}

void OTrophy::func_80086C6C(s32 objectIndex) {
    Vec3f sp24;

    sp24[0] = (gObjectList[objectIndex].pos[0] - 5.0f) + random_int(0x000AU);
    sp24[2] = (gObjectList[objectIndex].pos[2] - 5.0f) + random_int(0x000AU);
    if (D_801658F4 != 0) {
        sp24[1] = gObjectList[objectIndex].pos[1] + 14.0;
    } else {
        sp24[1] = gObjectList[objectIndex].pos[1] - 2.0;
    }

    if (_emitter != nullptr) {
        _emitter->Emit(sp24, D_801658F4);
    }
}

void OTrophy::DrawEditorProperties() {
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

    ImGui::Text("Cup");
    ImGui::SameLine();

    int32_t type = static_cast<int32_t>(_type);
    const char* items[] = { "Bronze", "Silver", "Gold", "Bronze 150", "Silver 150", "Gold 150" };

    if (ImGui::Combo("##Type", &type, items, IM_ARRAYSIZE(items))) {
        _type = static_cast<TrophyType>(type);

        switch (_type) {
            case TrophyType::GOLD:
                gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl10;
                break;
            case TrophyType::SILVER:
                gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl12;
                break;
            case TrophyType::BRONZE:
                gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl14;
                break;
            case TrophyType::GOLD_150:
                gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl11;
                break;
            case TrophyType::SILVER_150:
                gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl13;
                break;
            case TrophyType::BRONZE_150:
                gObjectList[_objectIndex].model = (Gfx*)gold_trophy_dl15;
                break;
        }
    }

    ImGui::Text("Behaviour");
    ImGui::SameLine();

    int32_t behaviour = static_cast<int32_t>(_bhv);
    const char* items2[] = { "Podium Ceremony", "Stationary", "Opposing Dual-axis Rotation", "Single-axis Rotation", "Go Fish" };

    if (ImGui::Combo("##Behaviour", &behaviour, items2, IM_ARRAYSIZE(items2))) {
        _bhv = static_cast<Behaviour>(behaviour);
    }
}
