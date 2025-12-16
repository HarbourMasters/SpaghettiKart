#include "PlayerBombKart.h"
#include "Matrix.h"

extern "C" {
#include "common_structs.h"
#include "assets/models/common_data.h"
#include "assets/textures/common_data.h"
#include "camera.h"
#include "render_objects.h"
#include "math_util_2.h"
#include "code_80057C60.h"
#include "menus.h"
}

void PlayerBombKart::Draw(size_t playerId, s32 cameraId) { // render_player_bomb_kart
    Player* player = &gPlayerOne[playerId];
    if (state != PlayerBombKartState::DISABLED) {
        pos.x = player->pos[0];
        pos.y = player->pos[1] - 2.0;
        pos.z = player->pos[2];
        surfaceHeight = player->unk_074;
        PlayerBombKart::func_800563DC(cameraId, _primAlpha);
        PlayerBombKart::func_8005669C(_primAlpha);
        PlayerBombKart::func_800568A0(cameraId);
    }
}

void PlayerBombKart::func_800563DC(s32 cameraId, s32 arg2) {
    s32 temp_s0;
    s32 temp_v0;
    s32 residue;
    Camera* camera = &camera1[cameraId];

    residue = D_801655CC % 4U;
    D_80183E40[0] = pos.x;
    D_80183E40[1] = pos.y + 1.0;
    D_80183E40[2] = pos.z;
    D_80183E80[0] = 0;
    D_80183E80[1] = func_800418AC(pos.x, pos.z, camera->pos);
    D_80183E80[2] = 0x8000;
    rsp_set_matrix_transformation(D_80183E40, D_80183E80, 0.2f);
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007E98);
    func_8004B310(arg2);

    int heigh = 32;
    int width = 32;

    gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_bomb);
    rsp_load_texture((u8*) common_texture_bomb[residue], width, heigh);
    gSPVertex(gDisplayListHead++, (uintptr_t) D_0D005AE0, 4, 0);
    gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);

    temp_s0 = D_8018D400;
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007B00);
    func_8004B414(0, 0, 0, arg2);
    D_80183E40[1] = D_80183E40[1] + 4.0;
    D_80183E80[2] = 0;
    func_800562E4(temp_s0 % 3, temp_s0 % 4, arg2);
    temp_v0 = temp_s0 + 1;
    D_80183E80[2] = 0x6000;
    func_800562E4(temp_v0 % 3, temp_v0 % 4, arg2);
    temp_v0 = temp_s0 + 2;
    D_80183E80[2] = 0xA000;
    func_800562E4(temp_v0 % 3, temp_v0 % 4, arg2);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void PlayerBombKart::func_8005669C(s32 arg2) {
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D0079E8);
    func_8004B310(arg2);
    load_texture_block_rgba16_mirror((u8*) D_0D02AA58, 0x00000010, 0x00000010);
    D_80183E40[1] = pos.y - 2.0;
    D_80183E40[0] = pos.x + 2.0;
    D_80183E40[2] = pos.z + 2.0;
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    D_80183E40[0] = pos.x + 2.0;
    D_80183E40[2] = pos.z - 2.0;
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    D_80183E40[0] = pos.x - 2.0;
    D_80183E40[2] = pos.z - 2.0;
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    D_80183E40[0] = pos.x - 2.0;
    D_80183E40[2] = pos.z + 2.0;
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void PlayerBombKart::func_800568A0(s32 cameraId) {
    Mat4 mtx;
    Player* player;

    player = &gPlayerOne[cameraId];
    D_80183E50[0] = pos.x;
    D_80183E50[1] = surfaceHeight + 0.8;
    D_80183E50[2] = pos.z;
    set_transform_matrix(mtx, player->collision.orientationVector, D_80183E50, 0U, 0.5f);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], mtx);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(mtx, G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007B98);
}