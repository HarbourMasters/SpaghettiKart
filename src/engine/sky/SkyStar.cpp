#include <libultraship.h>
#include <libultra/gbi.h>
#include "SkyStar.h"
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
#include "assets/textures/common_data.h"
#include "math_util_2.h"
#include "render_objects.h"
}

size_t SkyStar::_count = 0;

SkyStar::SkyStar(ScreenContext* screen, u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent) : SkyActor(screen) {
    _idx = _count;

    // ItemWindowObjects* temp_v0;
    //find_unused_obj_index(&_objectIndex);
    //init_object(_objectIndex, 1);
//    temp_v0 = (ItemWindowObjects*) &gObjectList[_objectIndex];
    mCloudVariant = cloudVariant;
//    temp_v0->unk_0D5 = cloudVariant;
  //  temp_v0->currentItem = ITEM_BANANA;
   // temp_v0->direction_angle[1] = rotY;
    mY = posY;
//    temp_v0->unk_09E = posY;
    mScale = (f32) scalePercent / 100.0;
//    temp_v0->sizeScaling = (f32) scalePercent / 100.0;
    mTexture = (u8*)D_0D0293D8;
//    temp_v0->activeTexture = (u8*)D_0D0293D8;
    //func_80073404(_objectIndex, 0x10U, 0x10U, (Vtx*)common_vtx_rectangle);
    mVtx = (Vtx*)common_vtx_rectangle;
    mTextureWidth = 16;
    mTextureHeight = 16;
    mRotY = rotY;
    
    mPrimAlpha = 0;
    mUnk_0CF = 0;
    mUnk_0AC = 0;
    mUnk_0D0 = 0;


    _count += 1;
}

void SkyStar::Tick() {
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
        // Calculate and update the object's position
        // 160 (SCREEN_WIDTH / 2) + (mUnk1E8 * cameraRot);
        // Grab center of screen, scale by fov factor, offset based on camera rotation
        mX = mUnk218 + (mUnk1E8 * cameraRot);

        mVisible = true;
    } else {
        mVisible = false;
    }
    
    // Vary the star based on star index
    switch (_idx % 5U) {
        case 0:
            star_func_80073B78(1, 0x00000028, 0x000000B4, 0x000000FF, 0, -1);
            break;
        case 1:
            star_func_80073B78(1, 0x00000080, 0x000000FF, 0x000000FF, 0, -1);
            break;
        case 2:
            star_func_80073B78(1, 0x00000050, 0x000000C8, 0x000000FF, 0, -1);
            break;
        case 3:
            star_func_80073B78(1, 0, 0x0000009B, 0x000000FF, 0, -1);
            break;
        case 4:
            star_func_80073B78(1, 0x0000005A, 0x00000080, 0x000000FF, 0, -1);
            break;
    }
}

void SkyStar::Draw(ScreenContext* screen, s32 arg0) { // render_stars
    s32 posY = arg0 - mY;
    func_8004B414(255, 255, 255, 255);
    if (mVisible) {
        FrameInterpolation_RecordOpenChild("render_stars", TAG_CLOUDS((_idx << 4) | (screen - gScreenOneCtx)));

        func_80044DA0((u8*)mTexture, mTextureWidth, mTextureHeight);

        func_8004B138(0xFF, 0xFF, 0xFF, mPrimAlpha);
        func_80042330_unchanged(mX, posY, 0, mScale);
        gSPVertex(gDisplayListHead++, (uintptr_t)mVtx, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);
        FrameInterpolation_RecordCloseChild();
    }
}

bool SkyStar::star_func_80073B78(s32 arg0, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 phi_t0;

    phi_t0 = false;
    if (mUnk_0CF == 0) { // s8
        mUnk_0AC = arg6; // s16
        if (arg0 != 0) {
            mPrimAlpha = arg3;
        }
        mUnk_0D0 = arg7; // s8
        //func_80073800(objectIndex, 1);
        mUnk_0CF = 1;
    } else {
        mUnk_0AC--;
        if (mUnk_0AC < 0) {
            mUnk_0AC = arg6;
            if (mUnk_0CF == 1) {
                mPrimAlpha += arg5;
                if (mPrimAlpha >= arg4) {
                    mPrimAlpha = arg4;
                    mUnk_0CF++;
                }
            } else {
                mPrimAlpha -= arg5;
                if (arg3 >= mPrimAlpha) {
                    mPrimAlpha = arg3;
                    if (mUnk_0D0 > 0) {
                        mUnk_0D0--;
                    }
                    if (mUnk_0D0 == 0) {
                        //func_80073800(objectIndex, 0);
                        mUnk_0CF = 0;
                        //func_8007381C(objectIndex); // does nothing? += unk_0DC
                        phi_t0 = true;
                    } else {
                        mUnk_0CF = 1;
                    }
                }
            }
        }
    }

    return phi_t0;
}