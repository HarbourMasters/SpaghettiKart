#include <libultraship.h>
#include <libultra/gbi.h>
#include "SkySnow.h"
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
#include "math_util.h"
}

size_t SkySnow::_count = 0;

SkySnow::SkySnow(ScreenContext* screen) : SkyActor(screen) {
    _idx = _count;

    mVtx = (Vtx*)LOAD_ASSET(common_vtx_rectangle);
    mState = 0;
    mState2 = 0;

    _count += 1;
}

void SkySnow::Tick() {

    s16 mUnk200 = mScreen->camera->fieldOfView + 40.0f;
    mUnk208 = ((mUnk200 / 2) * 0xB6) + 0x71C;
    mUnk210 = (-(mUnk200 / 2) * 0xB6) - 0x71C;
    mUnk1E8 = 1.7578125 / mUnk200;
    mUnk218 = SCREEN_WIDTH / 2;

    if (D_8016559C == 0) {
        D_8018D17C += 1;
        if (D_8018D17C >= D_8018D1F0) {
            D_8018D17C = 0;
        }
        if (mState == 0) {
            //init_object(_objectIndex, 1);
            mState = 1;
        }
    }

    switch (mState) {
        case 1:
            SkySnow::func_80077E20();
            break;
        case 2:
            SkySnow::func_80077F64(mScreen->camera);
            if (mState2 == 0) { // unk_0AE
                mState2 += 1;
                //object_next_state(_objectIndex);
            }
            break;
        case 0:
            break;
        case 3:
            //func_80072428(_objectIndex);
            mState = 0;
            //mStatus = 0;
            mVisible = false;
            mState2 = 0;
            break;
    }
}

void SkySnow::Draw(ScreenContext* screen, s32 arg0) { // render_clouds
    s32 posY = arg0 - mY;
    func_8004B6C4(255, 255, 255);
    // Skip drawing the object this frame if it warped to the other side of the screen
    if ((fabs(mX - mOldX) > SCREEN_WIDTH / 2) || (fabs(posY - mOldY) > SCREEN_HEIGHT / 2)) {
        mOldX = mX;
        mOldY = posY;
        return;
    }
    
    if (mVisible) {
        FrameInterpolation_RecordOpenChild("render_snow", TAG_CLOUDS((_idx << 4) | (screen - gScreenContexts)));

        if (D_8018D228 != mCloudVariant) {
            D_8018D228 = mCloudVariant;
            func_80044DA0((u8*)mTexture, mTextureWidth, mTextureHeight);
        }
        func_80042330_unchanged(mX, posY, 0, mScale);
        gSPVertex(gDisplayListHead++, (uintptr_t)mVtx, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*)common_rectangle_display);

        FrameInterpolation_RecordCloseChild();
    }
    mOldX = mX;
    mOldY = posY;
}

void SkySnow::func_80077E20() {
    u8* tex = (u8*) LOAD_ASSET(D_0D0293D8);
    //Vtx* vtx = (Vtx*) LOAD_ASSET(common_vtx_rectangle);

    mTexture = tex;
    //! @bug frappe snowland There's something up with the handling of common_vtx_rectangle and the loading of 0x10
    //! right here
    // root function: func_80078C70
    //mVtx = vtx;
    mTextureWidth = 16;
    mTextureHeight = 16;
    mScale = 0.15f;
    mVisible = true;

    mState2 = 1; // UNK_0AE func_80086EF0
    mState += 1;
    //object_next_state(objectIndex);
}

void SkySnow::func_80077F64(Camera* camera) {

    f64 rand;

    switch (mState2) { /* irregular */
        case 1: {
            mDirectionAngle[1] = (camera->rot[1] + random_int(0x4000U)) - 0x2000;
            //object_origin_pos_randomize_around_y(objectIndex, 0x00B4, 0x0014U);
            s16 offset_y = random_int(20) - (20 / 2);
            mOrigin.y = 180 + offset_y;
//            gObjectList[objectIndex].origin_pos[1] = y + offset_y;

            rand = random_int(0x0064U);
            mVelocity.y = (f32) (-0.75 - (f64) (f32) (rand * 0.01));
           // gObjectList[objectIndex].velocity[1] = (f32) (-0.75 - (f64) (f32) (rand * 0.01));
            mOffset.x = 0.0f;
            mOffset.y = 0.0f;
            //gObjectList[objectIndex].offset[0] = 0.0f;
            //gObjectList[objectIndex].offset[1] = 0.0f;
            //func_80086FD4(objectIndex);
            mState2 += 1;
            break;
        }
        case 2: {
            //func_80077EB8(objectIndex, gObjectList[objectIndex].direction_angle[1], camera);
            s16 temp_v0 = camera->rot[1] - mDirectionAngle[1];
            if ((temp_v0 >= mUnk210) || (mUnk208 >= temp_v0)) {
                mOffset.x = mUnk218 + (mUnk1E8 * (f32) temp_v0);
                mVisible = true;
                //set_object_flag(objectIndex, 0x00000010);
            } else {
                mVisible = false;
            }
            //clear_object_flag(objectIndex, 0x00000010);
            
            
//            object_add_velocity_offset_y(objectIndex);

            mOffset.y += mVelocity.y;

            //object_calculate_new_pos_offset(objectIndex);

            mX = mOrigin.x + mOffset.x;
            mY = mOrigin.y + mOffset.y;
            //mZ = mOrigin.z + mOffset.z;

            //func_8008BFC0(objectIndex);
            if (mY <= 0.0f) {
                //func_80086FD4(objectIndex);
                mState2 += 1;
                break;
            }
            break;
        }
        case 0:
            break;
        case 3:
            //func_80086F60(objectIndex);
            mState2 = 0;
            break;
    }
}
