#pragma once

#include <libultraship.h>
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"

extern "C" {
#include "common_structs.h"
#include "code_800029B0.h"
}

/**
 * SkyActor base class
 *
 *
 */
class SkyActor {
public:
    SkyActor(ScreenContext* screen) {
        mScreen = screen;
    };
    SkyActor(ScreenContext* screen, u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent) {};
    virtual ~SkyActor() {};

    virtual void Draw(ScreenContext* ctx, s32 arg0) {};
    virtual void Tick() {};
    ScreenContext* mScreen;
protected:
    f32 mScale;
    u16 mCloudVariant;
    u8* mTexture;
    s32 mTextureWidth;
    s32 mTextureHeight;
    bool mVisible;
    Vtx* mVtx;
    int32_t mX;
    int32_t mY;
    int32_t mRotY;
    int32_t mOldX;
    int32_t mOldY;

    s16 mUnk208;
    s16 mUnk210;
    f32 mUnk1E8;
    s16 mUnk218;
};
