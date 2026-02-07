#include "PlayerBombKart.h"
#include "Matrix.h"
#include "port/interpolation/FrameInterpolation.h"

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

size_t PlayerBombKart::_count = 0;

PlayerBombKart::PlayerBombKart() {
    _idx = _count;
    _count++;
}

void PlayerBombKart::Draw(size_t playerId, s32 cameraId) { // render_player_bomb_kart
    Player* player = &gPlayerOne[playerId];
    if (state != PlayerBombKartState::DISABLED) {
        pos.x = player->pos[0];
        pos.y = player->pos[1] - 2.0;
        pos.z = player->pos[2];
        surfaceHeight = player->unk_074;
        PlayerBombKart::func_800563DC(cameraId, _primAlpha);
        PlayerBombKart::func_8005669C(cameraId, _primAlpha);
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
    FrameInterpolation_RecordOpenChild("player_bomb_kart2", (_idx << 4) | cameraId);
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
    FrameInterpolation_RecordCloseChild();
    func_8004B414(0, 0, 0, arg2);
    D_80183E40[1] = D_80183E40[1] + 4.0;
    D_80183E80[2] = 0;
    PlayerBombKart::func_800562E4(cameraId, temp_s0 % 3, temp_s0 % 4, arg2, 0);
    temp_v0 = temp_s0 + 1;
    D_80183E80[2] = 0x6000;
    PlayerBombKart::func_800562E4(cameraId, temp_v0 % 3, temp_v0 % 4, arg2, 1);
    temp_v0 = temp_s0 + 2;
    D_80183E80[2] = 0xA000;
    PlayerBombKart::func_800562E4(cameraId, temp_v0 % 3, temp_v0 % 4, arg2, 2);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

u32 PlayerBombKart::vec[3][3] = {
    { 255, 255, 255 },
    { 255, 255, 0 },
    { 255, 0, 0 },
};

void PlayerBombKart::func_800562E4(s32 cameraId, s32 arg0, s32 arg1, s32 arg2, s32 id) {
    D_80165860 = PlayerBombKart::vec[arg0][0];
    D_8016586C = PlayerBombKart::vec[arg0][1];
    D_80165878 = PlayerBombKart::vec[arg0][2];
    func_8004B138(D_80165860, D_8016586C, D_80165878, arg2);
    FrameInterpolation_RecordOpenChild("player_bomb_kart_spark", (id << 12) | (_idx << 5) | cameraId);
    rsp_set_matrix_transformation(D_80183E40, D_80183E80, 0.2f);
    func_80044BF8((uint8_t*)common_texture_particle_spark[arg1], 32, 32);
    gSPVertex(gDisplayListHead++, (uintptr_t)D_0D005AE0, 4, 0);
    gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);
    FrameInterpolation_RecordCloseChild();
}

void PlayerBombKart::func_8005669C(s32 cameraId, s32 arg2) {
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D0079E8);
    func_8004B310(arg2);
    load_texture_block_rgba16_mirror((u8*) D_0D02AA58, 0x00000010, 0x00000010);
    D_80183E40[1] = pos.y - 2.0;
    D_80183E40[0] = pos.x + 2.0;
    D_80183E40[2] = pos.z + 2.0;
    FrameInterpolation_RecordOpenChild("player_bomb_kart_rect", (0 << 12) | (_idx << 5) | cameraId);
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    FrameInterpolation_RecordCloseChild();
    D_80183E40[0] = pos.x + 2.0;
    D_80183E40[2] = pos.z - 2.0;
    FrameInterpolation_RecordOpenChild("player_bomb_kart_rect2", (1 << 12) | (_idx << 5) | cameraId);
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    FrameInterpolation_RecordCloseChild();
    D_80183E40[0] = pos.x - 2.0;
    D_80183E40[2] = pos.z - 2.0;
    FrameInterpolation_RecordOpenChild("player_bomb_kart_rect3", (2 << 12) | (_idx << 5) | cameraId);
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    FrameInterpolation_RecordCloseChild();
    D_80183E40[0] = pos.x - 2.0;
    D_80183E40[2] = pos.z + 2.0;
    FrameInterpolation_RecordOpenChild("player_bomb_kart_rect4", (3 << 12) | (_idx << 5) | cameraId);
    func_800431B0(D_80183E40, D_80183E80, 0.15f, (Vtx*) common_vtx_rectangle);
    FrameInterpolation_RecordCloseChild();
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void PlayerBombKart::func_800568A0(s32 cameraId) {
    Mat4 mtx;
    Player* player;

    player = &gPlayerOne[cameraId];
    D_80183E50[0] = pos.x;
    D_80183E50[1] = surfaceHeight + 0.8;
    D_80183E50[2] = pos.z;
    FrameInterpolation_RecordOpenChild("player_bomb_kart", (_idx << 4) | cameraId);
    set_transform_matrix(mtx, player->collision.orientationVector, D_80183E50, 0U, 0.5f);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], mtx);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(mtx, G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007B98);
    FrameInterpolation_RecordCloseChild();
}
