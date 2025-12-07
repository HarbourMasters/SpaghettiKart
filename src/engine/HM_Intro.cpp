#include <libultra/gbi.h>
#include <libultra/gu.h>
#include "HM_Intro.h"

#include "engine/Matrix.h"
#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"

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
    _rot = IRotator(-5, 100, 0);
    _scale = {0.7f, 0.7f, 0.7f};

    _ship2Pos = FVector(300, -210, -1960);
    _ship2Rot = IRotator(0, 45, 0);

    _shipPos = FVector(20, -210, -1650);
    _shipRot = IRotator(0, 45, 0);

    _posHM64 = FVector(0, -500, -1000);
    _rotHM64 = IRotator(0, -90, -4); // -0x2100

    _hPos = FVector(-2000, 100, -4900);
    _hRot = IRotator(0, -45.0f, 0);
    _hScale = {2.0f, 2.0f, 2.0f};

    lusPos = FVector(0, -400, -614); // 12, 190, -1000
    lusRot = IRotator(0, 0, 0);
    lusScale = FVector(1, 1, 1);

    ground_f3d_material_013_lights = gdSPDefLights1(
	0x7F, 0x30, 0x80,
	0x60, 20, 10, 0x49, 0x49, 0x49
    );
}

void HarbourMastersIntro::HM_TickIntro() {
    _water += 1;
    if (_water > 255) {
        _water = 0;
    }

    HarbourMastersIntro::SpagBob(_pos, _rot, 2.5f, 0.18f, 1.5f, 0.05f, 1.7f, 0.06f);
    HarbourMastersIntro::Bob(_shipPos, _shipRot, 1.2f, 0.06f, 1.6f, -0.04f, 1.1f, -0.04f);
    HarbourMastersIntro::Bob(_ship2Pos, _ship2Rot, 1.2f, 0.06f, 1.6f, -0.04f, 1.1f, -0.04f);

    _pos.x += 9;
    if (lusPos.y < -60) {
        lusPos.y += 3;
    }
    //_camera.Pos.x -= 1;

    // SWITCH TO NEXT MENU LOGO_INTRO_MENU
    if (_pos.x >= 1100) {
        gMenuFadeType = 0;
        gMenuSelection = LOGO_INTRO_MENU;
        gFadeModeSelection = FADE_MODE_LOGO;
    }

    find_and_set_tile_size((uintptr_t) ((void*)mat_water_water1), 0, _water);
    find_and_set_tile_size((uintptr_t) ((void*)mat_water_water2), _water, 0);;
}

void HarbourMastersIntro::Bob(FVector& pos, IRotator& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed) {
    float time = (float)gGlobalTimer;

    pos.y = -210 + bobAmp * sin(time * bobSpeed);

    rot.pitch = (tiltAmp * sin(time * tiltSpeed)) * (UINT16_MAX / 360);

    rot.roll = (rollAmp * sin(time * rollSpeed)) * (UINT16_MAX / 360);
}

void HarbourMastersIntro::SpagBob(FVector& pos, IRotator& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed) {
    float time = (float)gGlobalTimer;

    pos.y = -205 + bobAmp * sin(time * bobSpeed);

    rot.pitch = (-5 + tiltAmp * sin(time * tiltSpeed))  * (UINT16_MAX / 360);

    rot.roll = (rollAmp * sin(time * rollSpeed))  * (UINT16_MAX / 360);
}

void HarbourMastersIntro::HM_DrawIntro() {
    //FrameInterpolation_ShouldInterpolateFrame(false);
    HarbourMastersIntro::Setup();

    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0x00);

    //FrameInterpolation_RecordOpenChild("spag_ship", 0);
    Mat4 mtx_spaghettiShip;
    ApplyMatrixTransformations(mtx_spaghettiShip, _pos, _rot, _scale);
    render_set_position(mtx_spaghettiShip, 0);
    gSPDisplayList(gDisplayListHead++, ship1_spag1_mesh);
    //FrameInterpolation_RecordCloseChild();

    //FrameInterpolation_RecordOpenChild("ship2", 0);
    Mat4 mtx_ship2;
    ApplyMatrixTransformations(mtx_ship2, _ship2Pos, _ship2Rot, _scale);
    render_set_position(mtx_ship2, 0);
    gSPDisplayList(gDisplayListHead++, ship2_SoH_mesh);
    //FrameInterpolation_RecordCloseChild();

    //FrameInterpolation_RecordOpenChild("ship3", 0);
    Mat4 mtx_ship3;
    ApplyMatrixTransformations(mtx_ship3, _shipPos, _shipRot, _scale);
    render_set_position(mtx_ship3, 0);
    gSPDisplayList(gDisplayListHead++, ship3_2Ship_mesh);
    //FrameInterpolation_RecordCloseChild();

    //FrameInterpolation_RecordOpenChild("hm_geo", 0);
    Mat4 mtx_geo;
    ApplyMatrixTransformations(mtx_geo, _hPos, _hRot, _hScale);
    render_set_position(mtx_geo, 0);

    gSPDisplayList(gDisplayListHead++, ground_map_mesh);
    //gSPDisplayList(gDisplayListHead++, powered_Text_mesh); // Replaced by poweredbylus
    gSPDisplayList(gDisplayListHead++, castle_map_002_mesh);
    gSPDisplayList(gDisplayListHead++, road_map_001_mesh);
    gSPDisplayList(gDisplayListHead++, water_water1_mesh);
    //FrameInterpolation_RecordCloseChild();

    //FrameInterpolation_RecordOpenChild("hm_lus", 0);
    Mat4 lusMtx;
    ApplyMatrixTransformations(lusMtx, lusPos, lusRot, lusScale);
    render_set_position(lusMtx, 0);
    gSPDisplayList(gDisplayListHead++, (Gfx*)"__OTR__hmintro/poweredbylus");
    //FrameInterpolation_RecordCloseChild();

    HarbourMastersIntro::Sync();
    //FrameInterpolation_ShouldInterpolateFrame(true);
}

void HarbourMastersIntro::Setup() {
    u16 perspNorm;

    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

    // Setup camera perspective
    guPerspective(&PerspectiveMatrix, &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, &PerspectiveMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    // Setup camera lookAt
    guLookAt(&LookAtMatrix, _camera.Pos.x, _camera.Pos.y, _camera.Pos.z, _camera.LookAt.x, _camera.LookAt.y, _camera.LookAt.z, 0.0f, 1.0f, 0.0f);
    gSPMatrix(gDisplayListHead++, &LookAtMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

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
