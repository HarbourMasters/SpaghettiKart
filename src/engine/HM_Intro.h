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
    void Bob(FVector& pos, FRotation& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed);
    void SpagBob(FVector& pos, FRotation& rot, f32 bobAmp, f32 bobSpeed, f32 tiltAmp, f32 tiltSpeed, f32 rollAmp, f32 rollSpeed);
    void MoveCloserToCamera(float moveSpeed);

    struct HMCamera {
        FVector Pos;
        FVector LookAt;
    };

    HMCamera _camera;

    f32 _cameraSpeed;
    f32 _cameraMaxSpeed;
    f32 _cameraAcceleration;

    FVector _pos;
    f32 _scale;
    f32 _trackScale;
    FRotation _rot;

    FVector _shipPos;
    FRotation _shipRot;

    FVector _ship2Pos;
    FRotation _ship2Rot;

    s32 _water = 0;

    FVector _posHM64;
    FRotation _rotHM64;

    FVector _hPos;
    FRotation _hRot;
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