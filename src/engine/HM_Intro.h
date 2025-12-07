#ifndef _HM_INTRO_H
#define _HM_INTRO_H

#include <libultraship.h>
#include <libultra/gbi.h>
#include "CoreMath.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "common_structs.h"
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class HarbourMastersIntro {
public:
    HarbourMastersIntro();

    void HM_InitIntro();
    void HM_TickIntro();
    void HM_DrawIntro();
private:
    void Setup();
    void Sync();
    void Bob(FVector& pos, IRotator& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed);
    void SpagBob(FVector& pos, IRotator& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed);
    void MoveCloserToCamera(float moveSpeed);

    struct HMCamera {
        FVector Pos;
        FVector LookAt;
    };

    Mtx PerspectiveMatrix;
    Mtx LookAtMatrix;
    HMCamera _camera;

    f32 _cameraSpeed;
    f32 _cameraMaxSpeed;
    f32 _cameraAcceleration;

    FVector _pos;
    IRotator _rot;
    FVector _scale;

    FVector _shipPos;
    IRotator _shipRot;

    FVector _ship2Pos;
    IRotator _ship2Rot;

    s32 _water = 0;

    FVector _posHM64;
    IRotator _rotHM64;

    FVector _hPos;
    IRotator _hRot;
    FVector _hScale;

    FVector lusPos;
    IRotator lusRot;
    FVector lusScale;
    int32_t DespawnValue = 0;
};

extern "C" {
#endif

void HM_InitIntro(void);
void HM_TickIntro(void);
void HM_DrawIntro(void);

#ifdef __cplusplus
}
#endif

#endif // _HM_INTRO_H