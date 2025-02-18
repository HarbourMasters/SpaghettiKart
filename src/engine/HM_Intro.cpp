#include <libultra/gbi.h>
#include <libultra/gu.h>
#include "HM_Intro.h"

#include "port/Game.h"

extern "C" {
#include "main.h"
#include "memory.h"
#include "math_util.h"
#include "courses/harbour/track.h"
#include "courses/harbour/powered.h"
#include "courses/harbour/ship2_model.h"
#include "courses/harbour/ship3_model.h"
#include "collision.h"
#include "menu_items.h"
#include "external.h"
#include "menus.h"
extern Gfx ship1_spag1_mesh[];
}

HarbourMastersIntro::HarbourMastersIntro() {
}

void HarbourMastersIntro::HM_InitIntro() {
    _camera.Pos.x = 0;
    _camera.Pos.y = 0;
    _camera.Pos.z = 0;

    // The camera setup was broken when this was first made. And then it was fixed.
    // That's why this file has weird coordinates as the objects were positioned in-front of the camera view
    // Instead of moving the camera to the right spot in the scene.
    _camera.LookAt.x = 0;
    _camera.LookAt.y = 0;
    _camera.LookAt.z = -2500;

    _cameraSpeed = 0.0f;
    _cameraMaxSpeed = 16.0f;
    _cameraAcceleration = 1.0f;

    _pos = FVector(-1000, -205, -800); // -1000, -210, -800
    _rot = FRotation(-5, 100, 0);
    _scale = 0.7f;
    _trackScale = 2.0f;

    _ship2Pos = FVector(300, -210, -1960);
    _ship2Rot = FRotation(0, 45, 0);

    _shipPos = FVector(20, -210, -1650);
    _shipRot = FRotation(0, 45, 0);

    _posHM64 = FVector(0, -500, -1000);
    _rotHM64 = FRotation(0, -90, -4); // -0x2100

    _hPos = FVector(-2000, 100, -4900);
    _hRot = FRotation(0, 0, 0);

    ground_f3d_material_013_lights = gdSPDefLights1(
	0x7F, 0x30, 0x80,
	0x60, 20, 10, 0x49, 0x49, 0x49
    );
}

const float BOB_AMPLITUDE = 1.1f;
const float BOB_SPEED = 0.05f;

const float TILT_AMPLITUDE = 1.5f;
const float TILT_SPEED = 0.05f;

const float ROLL_AMPLITUDE = 1.0f;
const float ROLL_SPEED = 0.03f;

void HarbourMastersIntro::HM_TickIntro() {
    _water += 1;
    if (_water > 255) {
        _water = 0;
    }

    HarbourMastersIntro::SpagBob(_pos, _rot, 2.5f, 0.18f, 1.5f, 0.05f, 1.7f, 0.06f);
    HarbourMastersIntro::Bob(_shipPos, _shipRot, 1.2f, 0.06f, 1.6f, -0.04f, 1.1f, -0.04f);
    HarbourMastersIntro::Bob(_ship2Pos, _ship2Rot, 1.2f, 0.06f, 1.6f, -0.04f, 1.1f, -0.04f);

    _pos.x += 9;
    _camera.Pos.x -= 1;

    if (_pos.x >= 880) {
        gMenuFadeType = 0;
        gMenuSelection = LOGO_INTRO_MENU;
        gFadeModeSelection = FADE_MODE_LOGO;
    }

    find_and_set_tile_size((uintptr_t) ((void*)mat_water_water1), 0, _water);
    find_and_set_tile_size((uintptr_t) ((void*)mat_water_water2), _water, 0);;
}

// @args amplitudes
void HarbourMastersIntro::Bob(FVector& pos, FRotation& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed) {
    float time = (float)gGlobalTimer;

    pos.y = -210 + bobAmp * sin(time * bobSpeed);

    rot.pitch = tiltAmp * sin(time * tiltSpeed);

    rot.roll = rollAmp * sin(time * rollSpeed);
}

void HarbourMastersIntro::SpagBob(FVector& pos, FRotation& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed) {
    float time = (float)gGlobalTimer;

    pos.y = -205 + bobAmp * sin(time * bobSpeed);

    rot.pitch = -5 + tiltAmp * sin(time * tiltSpeed);

    rot.roll = rollAmp * sin(time * rollSpeed);
}

void HarbourMastersIntro::HM_DrawIntro() {
    const f32 conv = 8192.0f / 45.0f; // Convert to hex degrees
    Mtx* mtx = &gGfxPool->mtxObject[0];
    HarbourMastersIntro::Setup();

    Mat4 someMtx;
    gSPMatrix(gDisplayListHead++, &gGfxPool->mtxScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, &gGfxPool->mtxLookAt[0], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    mtxf_identity(someMtx);
    render_set_position(someMtx, 0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0x00);

    Vec3f pos = {_pos.x, _pos.y, _pos.z};
    Vec3s rot = {_rot.pitch * conv, _rot.yaw * conv, _rot.roll * conv};
    Mat4 matrix;
    mtxf_pos_rotation_xyz(matrix, pos, rot);
    mtxf_scale(matrix, _scale);
    render_set_position(matrix, 0);
    gSPDisplayList(gDisplayListHead++, ship1_spag1_mesh);

    Mat4 matrix2;
    Vec3s rot2 = {_ship2Rot.pitch * conv, _ship2Rot.yaw * conv, _ship2Rot.roll * conv};
    Vec3f pos2 = {_ship2Pos.x, _ship2Pos.y, _ship2Pos.z};
    mtxf_pos_rotation_xyz(matrix2, pos2, rot2);
    mtxf_scale(matrix2, _scale);
    render_set_position(matrix2, 0);
    gSPDisplayList(gDisplayListHead++, ship2_SoH_mesh);

    Mat4 matrix3;
    Vec3s rot3 = {_shipRot.pitch * conv, _shipRot.yaw * conv, _shipRot.roll * conv};
    Vec3f pos3 = {_shipPos.x, _shipPos.y, _shipPos.z};
    mtxf_pos_rotation_xyz(matrix3, pos3, rot3);
    mtxf_scale(matrix3, _scale);
    render_set_position(matrix3, 0);
    gSPDisplayList(gDisplayListHead++, ship3_2Ship_mesh);

    Mat4 hMatrix;
    Vec3f hPos = {_hPos.x, _hPos.y, _hPos.z};
    Vec3s hRot = {_hRot.pitch * conv, -0x2100, _hRot.roll * conv};
    mtxf_pos_rotation_xyz(hMatrix, hPos, hRot);
    mtxf_scale(hMatrix, _trackScale);
    render_set_position(hMatrix, 0);

    gSPDisplayList(gDisplayListHead++, ground_map_mesh);
    gSPDisplayList(gDisplayListHead++, powered_Text_mesh);
    gSPDisplayList(gDisplayListHead++, castle_map_002_mesh);
    gSPDisplayList(gDisplayListHead++, road_map_001_mesh);
    gSPDisplayList(gDisplayListHead++, water_water1_mesh);

    HarbourMastersIntro::Sync();
}

void HarbourMastersIntro::Setup() {
    u16 perspNorm;
    move_segment_table_to_dmem();
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    guPerspective(&gGfxPool->mtxScreen, &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    guLookAt(&gGfxPool->mtxLookAt[0], _camera.Pos.x, _camera.Pos.y, _camera.Pos.z, _camera.LookAt.x, _camera.LookAt.y, _camera.LookAt.z, 0.0f, 1.0f, 0.0f);

    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_CULL_BACK | G_LIGHTING);
    gDPSetFillColor(gDisplayListHead++, 0x00010001);
    gDPFillRectangle(gDisplayListHead++, 0, 0, 319, 239);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_CULL_BACK | G_LIGHTING);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPPipeSync(gDisplayListHead++);
}

void HarbourMastersIntro::Sync() {
    gDPPipeSync(gDisplayListHead++);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
}
