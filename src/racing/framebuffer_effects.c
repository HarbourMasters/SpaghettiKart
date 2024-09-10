#include <libultraship.h>
#include "framebuffer_effects.h"
#include "mk64.h"
#include <assets/common_data.h>
#include "port/Engine.h"
#include <stdio.h>

int gfx_create_framebuffer(uint32_t width, uint32_t height, uint32_t native_width, uint32_t native_height,
                           uint8_t resize);

s32 gPauseFrameBuffer = -1;
s32 gBlurFrameBuffer = -1;
// A framebuffer that should only be used for drawing in the same frame that it is copied too
// i.e. the VisMono and VisFbuf effects
s32 gReusableFrameBuffer = -1;

// N64 resolution sized buffer (320x240), used by picto box and deku bubble
s32 gN64ResFrameBuffer = -1;

void FB_CreateFramebuffers(void) {
    if (gPauseFrameBuffer == -1) {
        gPauseFrameBuffer = gfx_create_framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, true);
    }

    if (gBlurFrameBuffer == -1) {
        gBlurFrameBuffer = gfx_create_framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, true);
    }

    if (gReusableFrameBuffer == -1) {
        gReusableFrameBuffer = gfx_create_framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, true);
    }

    if (gN64ResFrameBuffer == -1) {
        gN64ResFrameBuffer = gfx_create_framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    }
}

// Fixed point macros
#define FTOFIX(f) ((s32) ((f) * 65536.0))
#define ITOFIX(i) ((s32) ((i) << 16))
#define FIXTOF(x) ((double) ((x) / 65536.0))
#define FIXTOI(x) ((s32) ((x) >> 16))

#define toFixedInt(f) (FTOFIX(f) >> 16)
#define toFrac(f) (FTOFIX(f) & 0xFFFF)

// Setup a fixed-point matrix using floats or doubles. Recommend using doubles for more precision.
#define toFixedPointMatrix(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16)                    \
    {                                                                                                                \
        { ((toFixedInt(x1)) << 16) | toFixedInt(x2), ((toFixedInt(x3)) << 16) | toFixedInt(x4),                      \
          (toFixedInt(x5) << 16) | toFixedInt(x6), (toFixedInt(x7) << 16) | toFixedInt(x8) },                        \
            { ((toFixedInt(x9)) << 16) | toFixedInt(x10), ((toFixedInt(x11)) << 16) | toFixedInt(x12),               \
              (toFixedInt(x13) << 16) | toFixedInt(x14), (toFixedInt(x15) << 16) | toFixedInt(x16) },                \
            { ((toFrac(x1)) << 16) | toFrac(x2), ((toFrac(x3)) << 16) | toFrac(x4), (toFrac(x5) << 16) | toFrac(x6), \
              (toFrac(x7) << 16) | toFrac(x8) },                                                                     \
        {                                                                                                            \
            ((toFrac(x9)) << 16) | toFrac(x10), ((toFrac(x11)) << 16) | toFrac(x12),                                 \
                (toFrac(x13) << 16) | toFrac(x14), (toFrac(x15) << 16) | toFrac(x16)                                 \
        }                                                                                                            \
    }

Mtx gIdentityMtx = {
    toFixedPointMatrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0),
};

/**
 * Copies the current texture data from the source frame buffer to the destination frame buffer
 * Setting oncePerFrame ensures that the copy will only happen once every game frame. This
 * is important for effects that could be affected by increased frame interpolation (like motion blur).
 * A pointer to a boolean is passed to the render for the render to set once the copy has been performed.
 * This function uses opcodes from f3dex2 but may be called when s2dex is loaded, such as during shrink window. Make
 * sure f3dex2 is loaded before this function is called.
 */
void FB_CopyToFramebuffer(Gfx* gfx, s32 fb_src, s32 fb_dest, u8 oncePerFrame, u8* hasCopied) {

    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_OPA_SURF | G_RM_OPA_SURF2);

    gSPClearGeometryMode(gfx++, G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
    gSPSetGeometryMode(gfx++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);

    gDPSetBlendColor(gfx++, 255, 255, 255, 8);
    gDPSetPrimDepth(gfx++, 0xFFFF, 0xFFFF);

    gDPSetEnvColor(gfx++, 255, 255, 255, 255);
    gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0,
                      ENVIRONMENT);

    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    gDPCopyFB(gfx++, fb_dest, fb_src, oncePerFrame, hasCopied);
}

/**
 * Copies a 4:3 slice of the current framebuffer scaled down to 320x240 to a CPU buffer address.
 * The buffer output will be in RGBA16 format.
 * Specify the byteswap flag to force the buffer data to be written as BigEndian, which is
 * required if the buffer is being used as a texture in F3D.
 */
void FB_WriteFramebufferSliceToCPU(Gfx* gfx, void* buffer, u8 byteSwap) {
    // Gfx* gfx = *gfxp;
    printf("write!\n");
    FB_CopyToFramebuffer(gfx, 0, gReusableFrameBuffer, false, NULL);

    // Set the N64 resolution framebuffer as the draw target (320x240)
    gsSPSetFB(gfx++, gN64ResFrameBuffer);
    // Reset scissor for new framebuffer
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    int16_t s0 = 0, t0 = 0;
    int16_t s1 = OTRGetGameRenderWidth();
    int16_t t1 = OTRGetGameRenderHeight();
    printf("half!\n");
    float aspectRatio = OTRGetAspectRatio();
    float fourByThree = 4.0f / 3.0f;

    // Adjust the texture coordinates so that only a 4:3 region from the center is drawn
    // to the N64 resolution buffer. Currently ratios smaller than 4:3 will just stretch to fill.
    if (aspectRatio > fourByThree) {
        int16_t adjustedWidth = OTRGetGameRenderWidth() / (aspectRatio / fourByThree);
        s0 = (OTRGetGameRenderWidth() - adjustedWidth) / 2;
        s1 -= s0;
    }
    printf("Aspect!\n");
    gDPSetTextureImageFB(gfx++, 0, 0, 0, gReusableFrameBuffer);
    gDPImageRectangle(gfx++, 0 << 2, 0 << 2, s0, t0, SCREEN_WIDTH << 2, SCREEN_HEIGHT << 2, s1, t1, G_TX_RENDERTILE,
                      OTRGetGameRenderWidth(), OTRGetGameRenderHeight());

    // Read the final N64 framebuffer back as rgba16 into the CPU-side buffer
    gDPReadFB(gfx++, gN64ResFrameBuffer, buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, byteSwap);

    gsSPResetFB(gfx++);
    // Reset scissor for original framebuffer
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    printf("Complete!\n");
}

/**
 * Draws the texture data from the specified frame buffer as a full screen image
 */
void FB_DrawFromFramebuffer(Gfx* gfx, s32 fb, u8 alpha) {

    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gDPSetEnvColor(gfx++, 255, 255, 255, alpha);

    gDPSetOtherMode(gfx++,
                    G_AD_NOISE | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP |
                        G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_CLD_SURF | G_RM_CLD_SURF2);

    gSPClearGeometryMode(gfx++, G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
    gSPSetGeometryMode(gfx++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);

    gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0,
                      ENVIRONMENT);

    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    gDPSetTextureImageFB(gfx++, 0, 0, 0, fb);
    gDPImageRectangle(gfx++, OTRGetRectDimensionFromLeftEdge(0) << 2, 0 << 2, 0, 0,
                      OTRGetRectDimensionFromRightEdge(SCREEN_WIDTH) << 2, SCREEN_HEIGHT << 2, OTRGetGameRenderWidth(),
                      OTRGetGameRenderHeight(), G_TX_RENDERTILE, OTRGetGameRenderWidth(), OTRGetGameRenderHeight());
}

/**
 * Similar to FB_DrawFromFramebuffer, but scales the image relative to the center of the screen.
 * This function uses opcodes from f3dex2 but may be called when s2dex is loaded, such as during shrink window. Make
 * sure f3dex2 is loaded before this function is called.
 */
void FB_DrawFromFramebufferScaled(Gfx* gfx, s32 fb, u8 alpha, float scaleX, float scaleY) {

    gDPSetEnvColor(gfx++, 255, 255, 255, alpha);

    gDPSetOtherMode(gfx++,
                    G_AD_NOISE | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP |
                        G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_CLD_SURF | G_RM_CLD_SURF2);

    gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0,
                      ENVIRONMENT);

    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    gDPSetTextureImageFB(gfx++, 0, 0, 0, fb);

    float x0 = SCREEN_WIDTH * 0.5f * scaleX;
    float y0 = SCREEN_HEIGHT * 0.5f * scaleY;

    gDPImageRectangle(gfx++, OTRGetRectDimensionFromLeftEdge(x0) << 2, (int) (y0) << 2, 0, 0,
                      OTRGetRectDimensionFromRightEdge((float) (SCREEN_WIDTH - x0)) << 2,
                      (int) ((float) (SCREEN_HEIGHT - y0)) << 2, OTRGetGameRenderWidth(), OTRGetGameRenderHeight(),
                      G_TX_RENDERTILE, OTRGetGameRenderWidth(), OTRGetGameRenderHeight());
}
