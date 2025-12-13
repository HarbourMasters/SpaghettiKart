#include <libultraship.h>

#include "Cloud.h"
#include "engine/Actor.h"
#include "engine/World.h"
#include "port/Game.h"

extern "C" {
#include "macros.h"
#include "common_structs.h"
#include "math_util.h"
#include "actor_types.h"
#include "actors.h"
#include "assets/textures/other_textures.h"
extern f32 gKartHopInitialVelocityTable[];
extern f32 gKartGravityTable[];
}

ACloud::ACloud(const SpawnParams& params) : AActor(params) {
    Name = "Cloud (HM64)";
    ResourceName = "hm:cloud";
    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;

    Rot[0] = 0;
    Rot[1] = 0;
    Rot[2] = 0;

    TimerLength = params.Type.value_or(500); // How long the effect lasts
    Hop = params.Speed.value_or(3.0f); // How long the effect lasts
    Gravity = params.SpeedB.value_or(200.0f); // How long the effect lasts

    // Flags = -0x8000 | 0x4000;

    BoundingBoxSize = 2.0f;
}

void ACloud::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.Name = ResourceName;
    params.Type = TimerLength;
    params.Speed = Hop;
    params.SpeedB = Gravity;
}

extern Gfx cloud_mesh[];
void ACloud::BeginPlay() {
    // Prevent collision mesh from being generated extra times.
    if (Editor_IsEnabled()) {
        if (Triangles.size() == 0) {
            Editor::GenerateCollisionMesh(this, (Gfx*)cloud_mesh, 1.0f);
        }
    }
}

void ACloud::Tick() {
    Rot[1] += 0x200;

    if (PickedUp) {
        Timer++; // Increment timer
    }

    if (Timer > TimerLength) { // Time has expired, reset the actor and player
        PickedUp = false;
        if (_player) {
            gKartHopInitialVelocityTable[_player->characterId] = OldHop; // reset back to normal
            gKartGravityTable[_player->characterId] = OldGravity;
            _player = NULL; // Reset
        }
        Timer = 0;
    }
}

void ACloud::Draw(Camera* camera) {
    Mat4 mtx;

    if (PickedUp) {
        return; // Skip drawing if the actor has been picked up
    }

    mtxf_pos_rotation_xyz(mtx, Pos, Rot);
    if (render_set_position(mtx, 0) != 0) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPDisplayList(gDisplayListHead++, (Gfx*) cloud_mesh);
    }
}

void ACloud::Collision(Player* player, AActor* actor) {
    if (!PickedUp) {
        if (query_collision_player_vs_actor_item(player, GetWorld()->ConvertAActorToActor(actor))) {
            // Player has picked up the actor, activate the cloud effect
            _player = player;
            PickedUp = true;

            OldHop = gKartHopInitialVelocityTable[player->characterId];
            OldGravity = gKartGravityTable[player->characterId];
            // Hop height
            gKartHopInitialVelocityTable[player->characterId] = Hop;
            // How strong gravity is
            gKartGravityTable[player->characterId] = Gravity;
        }
    }
}

bool ACloud::IsMod() {
    return true;
}

Vtx cloud_mesh_vtx_cull[8] = {
    { { { 0, -4, -4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } }, { { { 0, -4, 4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } },
    { { { 0, 4, 4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } },   { { { 0, 4, -4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } },
    { { { 0, -4, -4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } }, { { { 0, -4, 4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } },
    { { { 0, 4, 4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } },   { { { 0, 4, -4 }, 0, { 0, 0 }, { 0, 0, 0, 0 } } },
};

Vtx cloud_mesh_vtx_0[4] = {
    { { { 0, 4, 4 }, 0, { -524, -530 }, { 255, 255, 255, 166 } } },
    { { { 0, -4, 4 }, 0, { -530, 1516 }, { 255, 255, 255, 201 } } },
    { { { 0, -4, -4 }, 0, { 1516, 1522 }, { 255, 255, 255, 188 } } },
    { { { 0, 4, -4 }, 0, { 1522, -524 }, { 255, 255, 255, 154 } } },
};

Gfx cloud_mesh_tri_0[] = {
    gsSPVertex(cloud_mesh_vtx_0 + 0, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

Gfx mat_cloud_cutout[] = {
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_SHADING_SMOOTH),
    gsSPClearGeometryMode(G_CULL_FRONT | G_CULL_BACK | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD |
                          G_CLIPPING),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, PRIMITIVE, PRIMITIVE, TEXEL1, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0,
                       COMBINED),
    gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20,
                     G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
    gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_TEX_EDGE2),
    gsSPTexture(65535, 65535, 0, 0, 1),
    gsDPSetPrimColor(0, 0, 255, 220, 203, 255),
    gsDPSetTextureImage(G_IM_FMT_I, G_IM_SIZ_8b_LOAD_BLOCK, 1, gGroundDust),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0,
                G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
    gsDPLoadBlock(7, 0, 0, 511, 512),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 1, G_TX_WRAP | G_TX_NOMIRROR, 5, 1),
    gsDPSetTileSize(0, 98, 100, 124, 124),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, gTexture66C8F4),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 128, 6, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0,
                G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
    gsDPLoadBlock(6, 0, 0, 1023, 256),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 128, 1, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR,
                5, 0),
    gsDPSetTileSize(1, 0, 0, 124, 124),
    gsSPEndDisplayList(),
};

Gfx cloud_mesh[] = {
    // gsSPClearGeometryMode(G_LIGHTING),
    // gsSPVertex(cloud_mesh_vtx_cull + 0, 8, 0),
    //gsSPSetGeometryMode(G_LIGHTING),
    // gsSPCullDisplayList(0, 7),
    gsSPDisplayList(mat_cloud_cutout),
    gsSPDisplayList(cloud_mesh_tri_0),
    gsSPEndDisplayList(),
};

void ACloud::DrawEditorProperties() {
    ImGui::Text("Location");
    ImGui::SameLine();
    FVector location = FVector(Pos[0], Pos[1], Pos[2]);
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

    ImGui::Text("Effect Timer");
    ImGui::SameLine();
    int32_t type = static_cast<int32_t>(TimerLength);
    if (ImGui::InputInt("##Type", &type)) {
        TimerLength = static_cast<uint32_t>(type);
    }

    ImGui::Text("Hop");
    ImGui::SameLine();

    if (ImGui::DragFloat("##Speed", &Hop, 0.01f)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetSpeed")) {
        Hop = 0.0f;
    }

    ImGui::Text("Gravity");
    ImGui::Text("Higher value = stronger gravity");

    if (ImGui::DragFloat("##SpeedB", &Gravity, 1.0f)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetSpeedB")) {
        Gravity = 0.0f;
    }
}