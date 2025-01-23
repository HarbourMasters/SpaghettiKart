#pragma once

#include <libultraship.h>
#include "CoreMath.h"

class PlayerBombKart {
public:

    enum PlayerBombKartState {
        DISABLED,
        ACTIVE,
    };

    s16 state = PlayerBombKartState::DISABLED;
    FVector pos = {0, 0, 0};
    f32 surfaceHeight = 0;
    s32 _primAlpha = 0;

    void Draw(size_t playerId, s32 cameraId);
    void func_800563DC(s32 cameraId, s32 arg2);
    void func_8005669C(s32 arg2);
    void func_800568A0(s32 cameraId);
};
