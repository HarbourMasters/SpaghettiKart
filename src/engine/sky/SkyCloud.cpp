#include <libultraship.h>
#include <libultra/gbi.h>
#include "SkyCloud.h"
#include <vector>
#include "engine/tracks/Track.h"
#include "engine/World.h"

#include "port/Engine.h"
#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "update_objects.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "assets/models/common_data.h"
#include "math_util_2.h"
#include "render_objects.h"
}

size_t SkyCloud::_count = 0;

// Pack-branch variant selection, calibrated empirically with tracer art on
// both sheet groups. The quad's T space spans 2048 units per sheet, so the
// per-variant pitch is 2048/N, shifted up one original texel (32 units) to
// match the renderer's sampling convention. S spans 0..2016: upstream's
// shared cloudvtx tables span double that (every cloud drew twice side by
// side with replacement packs) and their fixed T pitch only suits 4-slice
// sheets.
static const Vtx sPackCloudVtx4[4][4] = {
    {
        { { { -32, -16, 0 }, 0, { 0, -32 }, { 255, 255, 255, 255 } } },
        { { { 31, -16, 0 }, 0, { 2016, -32 }, { 255, 255, 255, 255 } } },
        { { { 31, 15, 0 }, 0, { 2016, 480 }, { 255, 255, 255, 255 } } },
        { { { -32, 15, 0 }, 0, { 0, 480 }, { 255, 255, 255, 255 } } },
    },
    {
        { { { -32, -16, 0 }, 0, { 0, 480 }, { 255, 255, 255, 255 } } },
        { { { 31, -16, 0 }, 0, { 2016, 480 }, { 255, 255, 255, 255 } } },
        { { { 31, 15, 0 }, 0, { 2016, 992 }, { 255, 255, 255, 255 } } },
        { { { -32, 15, 0 }, 0, { 0, 992 }, { 255, 255, 255, 255 } } },
    },
    {
        { { { -32, -16, 0 }, 0, { 0, 992 }, { 255, 255, 255, 255 } } },
        { { { 31, -16, 0 }, 0, { 2016, 992 }, { 255, 255, 255, 255 } } },
        { { { 31, 15, 0 }, 0, { 2016, 1504 }, { 255, 255, 255, 255 } } },
        { { { -32, 15, 0 }, 0, { 0, 1504 }, { 255, 255, 255, 255 } } },
    },
    {
        { { { -32, -16, 0 }, 0, { 0, 1504 }, { 255, 255, 255, 255 } } },
        { { { 31, -16, 0 }, 0, { 2016, 1504 }, { 255, 255, 255, 255 } } },
        { { { 31, 15, 0 }, 0, { 2016, 2016 }, { 255, 255, 255, 255 } } },
        { { { -32, 15, 0 }, 0, { 0, 2016 }, { 255, 255, 255, 255 } } },
    },
};
static const Vtx sPackCloudVtx3[3][4] = {
    {
        { { { -32, -16, 0 }, 0, { 0, -32 }, { 255, 255, 255, 255 } } },
        { { { 31, -16, 0 }, 0, { 2016, -32 }, { 255, 255, 255, 255 } } },
        { { { 31, 15, 0 }, 0, { 2016, 651 }, { 255, 255, 255, 255 } } },
        { { { -32, 15, 0 }, 0, { 0, 651 }, { 255, 255, 255, 255 } } },
    },
    {
        { { { -32, -16, 0 }, 0, { 0, 651 }, { 255, 255, 255, 255 } } },
        { { { 31, -16, 0 }, 0, { 2016, 651 }, { 255, 255, 255, 255 } } },
        { { { 31, 15, 0 }, 0, { 2016, 1333 }, { 255, 255, 255, 255 } } },
        { { { -32, 15, 0 }, 0, { 0, 1333 }, { 255, 255, 255, 255 } } },
    },
    {
        { { { -32, -16, 0 }, 0, { 0, 1333 }, { 255, 255, 255, 255 } } },
        { { { 31, -16, 0 }, 0, { 2016, 1333 }, { 255, 255, 255, 255 } } },
        { { { 31, 15, 0 }, 0, { 2016, 2016 }, { 255, 255, 255, 255 } } },
        { { { -32, 15, 0 }, 0, { 0, 2016 }, { 255, 255, 255, 255 } } },
    },
};

SkyCloud::SkyCloud(ScreenContext* screen, u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent) : SkyActor(screen) {
    _idx = _count;
    mScreen = screen;
    mCloudVariant = cloudVariant;
    mY = posY;
    mRotY = rotY;
    mScale = (f32) scalePercent / 100.0;
    mTextureWidth = 64;
    mTextureHeight = 32;

    ResolveTexture();

    _count += 1;
}


// Resolved on every draw, not cached: the alt-assets toggle evicts and reloads
// textures, so a pointer resolved at course load can dangle and draw stale or
// foreign pixels (e.g. the minimap) once the allocator reuses the memory.
void SkyCloud::ResolveTexture() {
    if (GameEngine_ResourceGetTexTypeByName((const char*)CM_GetProps()->CloudTexture) != 1) { // Stock
        mTexture = ((u8*) LOAD_ASSET_RAW(CM_GetProps()->CloudTexture)) + (mCloudVariant * 1024);
        mVtx = (Vtx*)D_0D005FB0;
    } else { // Texture pack: pass the asset name, resolved fresh at import time
        mTexture = CM_GetProps()->CloudTexture;

        if ((strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust3) == 0) ||
           (strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust4) == 0) ||
           (strcmp((const char*)CM_GetProps()->CloudTexture, gTextureExhaust5) == 0)) {
            mVtx = (Vtx*)sPackCloudVtx4[mCloudVariant];
        } else {
            mVtx = (Vtx*)sPackCloudVtx3[mCloudVariant];
        }
    }
}

void SkyCloud::Tick() { // func_800788F8
    s16 cameraRot;

    s16 mUnk200 = mScreen->camera->fieldOfView + 40.0f;
    mUnk208 = ((mUnk200 / 2) * 0xB6) + 0x71C;
    mUnk210 = (-(mUnk200 / 2) * 0xB6) - 0x71C;
    mUnk1E8 = 1.7578125 / mUnk200;
    mUnk218 = SCREEN_WIDTH / 2;

    // Adjustable culling factor
    const float cullingFactor = OTRGetAspectRatio();

    // Calculate the cloud's rotation relative to the camera
    cameraRot = (u16)mScreen->camera->rot[1] + (u16)mRotY;
    // Adjust bounds based on the culling factor
    s16 adjustedLowerBound = (s16) (mUnk210 * cullingFactor);
    s16 adjustedUpperBound = (s16) (mUnk208 * cullingFactor);

    // Check if the object is within the adjusted bounds
    if ((cameraRot >= adjustedLowerBound) && (adjustedUpperBound >= cameraRot)) {
        // Calculate and update the object's X position
        // 160 (SCREEN_WIDTH / 2) + (D_8018D1E8 * cameraRot);
        // Grab center of screen, scale by fov factor, offset based on camera rotation
        mX = mUnk218 + (mUnk1E8 * cameraRot);

        // Mark the object as visible
        mVisible = true;
    } else {
        // If outside the bounds, mark the object as not visible
        mVisible = false;
    }
}

void SkyCloud::Draw(ScreenContext* screen, s32 arg0) { // render_clouds
   // Object* object = &gObjectList[_objectIndex];
    s32 posY = arg0 - mY;
    func_8004B6C4(255, 255, 255);
    // Skip drawing the object this frame if it warped to the other side of the screen
    if ((fabs(mX - mOldX) > SCREEN_WIDTH / 2) || (fabs(posY - mOldY) > SCREEN_HEIGHT / 2)) {
        mOldX = mX;
        mOldY = posY;
        return;
    }
    if (mVisible) {
        FrameInterpolation_RecordOpenChild("render_clouds", TAG_CLOUDS((_idx << 4) | (mScreen - gScreenContexts)));

        ResolveTexture();
        // Rebind when the resolved pointer moves, not just on variant change:
        // in static scenes (e.g. the Harbour intro) the variant never cycles,
        // so after the alt-assets toggle evicts the texture the gate would keep
        // drawing from the freed buffer's address (foreign pixels once reused).
        static const u8* sLastBoundTex = nullptr;
        if (D_8018D228 != mCloudVariant || sLastBoundTex != mTexture) {
            D_8018D228 = mCloudVariant;
            sLastBoundTex = mTexture;
            func_80044DA0(mTexture, mTextureWidth, mTextureHeight);
        }
        func_80042330_unchanged(mX, posY, 0, mScale);
        gSPVertex(gDisplayListHead++, (uintptr_t)mVtx, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);

        FrameInterpolation_RecordCloseChild();
    }
    mOldX = mX;
    mOldY = posY;
}
