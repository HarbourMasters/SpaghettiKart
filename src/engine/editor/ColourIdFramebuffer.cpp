
#include <libultraship.h>
#include "framebuffer_effects.h"
#include "mk64.h"
#include <assets/common_data.h>
#include "port/Engine.h"
#include "math_util.h"
#include <stdio.h>

int gfx_create_framebuffer(uint32_t width, uint32_t height, uint32_t native_width, uint32_t native_height,
    uint8_t resize);


s32 gColourIdFramebuffer = -1;

void CreateColourIdFramebuffer(void) {
    if (gColourIdFramebuffer == -1) {
        gColourIdFramebuffer = gfx_create_framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, true);
    }
}

void DrawAsColour() {


}