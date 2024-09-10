#ifndef FRAMEBUFFER_EFFECTS_H
#define FRAMEBUFFER_EFFECTS_H

#include <libultraship.h>

extern s32 gPauseFrameBuffer;
extern s32 gBlurFrameBuffer;
extern s32 gReusableFrameBuffer;
extern s32 gN64ResFrameBuffer;

void FB_CreateFramebuffers(void);
void FB_CopyToFramebuffer(Gfx* gfx, s32 fb_src, s32 fb_dest, u8 oncePerFrame, u8* hasCopied);
void FB_WriteFramebufferSliceToCPU(Gfx* gfx, void* buffer, u8 byteSwap);
void FB_DrawFromFramebuffer(Gfx* gfx, s32 fb, u8 alpha);
void FB_DrawFromFramebufferScaled(Gfx* gfx, s32 fb, u8 alpha, float scaleX, float scaleY);

#endif // FRAMEBUFFER_EFFECTS_H
