#include <libultraship.h>
#include <macros.h>
#include "profiler.h"
#include <mk64.h>
#include "main.h"

struct ProfilerFrameData gProfilerFrameData[2];

s32 gEnableResourceMeters = 0;

s16 D_800DC664 = 0;
s16 D_800DC668 = 1;
s16 D_800DC66C = 0;

void profiler_log_thread5_time(enum ProfilerGameEvent eventID) {
    gProfilerFrameData[D_800DC668].gameTimes[eventID] = osGetTime();

    if (eventID == THREAD5_END) {
        D_800DC668 ^= 1;
        gProfilerFrameData[D_800DC668].numSoundTimes = 0;
    }
}

void profiler_log_thread4_time(void) {
    struct ProfilerFrameData* profiler = &gProfilerFrameData[D_800DC668];

    if (profiler->numSoundTimes < ARRAY_COUNT(profiler->soundTimes)) {
        profiler->soundTimes[profiler->numSoundTimes++] = osGetTime();
    }
}

void profiler_log_gfx_time(enum ProfilerGfxEvent eventID) {
    if (eventID == TASKS_QUEUED) {
        D_800DC66C ^= 1;
        gProfilerFrameData[D_800DC66C].numVblankTimes = 0;
    }
    gProfilerFrameData[D_800DC66C].gfxTimes[eventID] = osGetTime();
}

void profiler_log_vblank_time(void) {
    struct ProfilerFrameData* profiler = &gProfilerFrameData[D_800DC66C];

    if (profiler->numVblankTimes < ARRAY_COUNT(profiler->vblankTimes)) {
        profiler->vblankTimes[profiler->numVblankTimes++] = osGetTime();
    }
}

// Draw the specified profiler given the information passed.
// This function is the same as the one in SM64
void draw_profiler_bar(OSTime clockBase, OSTime clockStart, OSTime clockEnd, s16 posY, u16 color) {
    s64 durationStart, durationEnd;
    s32 rectX1, rectX2;

    // set the duration to start, and floor to 0 if the result is below 0.
    if ((durationStart = clockStart - clockBase) < 0) {
        durationStart = 0;
    }
    // like the above, but with end.
    if ((durationEnd = clockEnd - clockBase) < 0) {
        durationEnd = 0;
    }

    // calculate the x coordinates of where start and end begins, respectively.
    rectX1 = ((((durationStart * 1000000) / osClockRate * 3) / 1000) + 30);
    rectX2 = ((((durationEnd * 1000000) / osClockRate * 3) / 1000) + 30);

    //! I believe this is supposed to cap rectX1 and rectX2 to 320, but the
    //  code seems to use the wrong variables... it's possible that the variable
    //  names were very similar within a single letter.
    if (rectX1 > 319) {
        clockStart = 319;
    }
    if (rectX2 > 319) {
        clockEnd = 319;
    }

    // perform the render if start is less than end. in most cases, it should be.
    if (rectX1 < rectX2) {
        gDPPipeSync(gDisplayListHead++);
        gDPSetFillColor(gDisplayListHead++, color << 16 | color);
        gDPFillRectangle(gDisplayListHead++, rectX1, posY, rectX2, posY + 2);
    }
}

// Also the same function as SM64
void draw_reference_profiler_bars(void) {
    // Draws the reference "max" bars underneath the real thing.

    // Blue
    gDPPipeSync(gDisplayListHead++);
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(40, 80, 255, 1) << 16 | GPACK_RGBA5551(40, 80, 255, 1));
    gDPFillRectangle(gDisplayListHead++, 30, 220, 79, 222);

    // Yellow
    gDPPipeSync(gDisplayListHead++);
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(255, 255, 40, 1) << 16 | GPACK_RGBA5551(255, 255, 40, 1));
    gDPFillRectangle(gDisplayListHead++, 79, 220, 128, 222);

    // Orange
    gDPPipeSync(gDisplayListHead++);
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(255, 120, 40, 1) << 16 | GPACK_RGBA5551(255, 120, 40, 1));
    gDPFillRectangle(gDisplayListHead++, 128, 220, 177, 222);

    // Red
    gDPPipeSync(gDisplayListHead++);
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(255, 40, 40, 1) << 16 | GPACK_RGBA5551(255, 40, 40, 1));
    gDPFillRectangle(gDisplayListHead++, 177, 220, 226, 222);
}

/**
 * Likely identical to sm64 but in mk64 they likely commented it out.
 */
void draw_profiler_mode_1(void) {
}

/*
  Draw Profiler Mode 0. This mode renders bars over each other to make it
  easier to see which processes take the longest.

  This function differs from the one in SM64 by two lines

  Information:

  (red): Sound Updates
  (yellow): Level Script Execution
  (orange): Rendering
  (orange): RDP Duration
  (yellow): RSP Duration
  (red): VBlank Duration
*/
void draw_profiler_mode_0(void) {
    s32 i;
    struct ProfilerFrameData* profiler;

    u64 clockStart;
    // Does this naming apply to MK64?
    u64 levelScriptDuration;
    u64 renderDuration;
    u64 taskStart;
    u64 rspDuration;
    u64 rdpDuration;
    u64 vblank;
    u64 soundDuration;

    // get the last frame profiler. gCurrentFrameIndex1 has the current frame being processed, so
    // xor it to get the last frame profiler.
    profiler = &gProfilerFrameData[D_800DC668 ^ 1];

    // was thread 5 ran before thread 4? set the lower one to be the clockStart.
    clockStart = profiler->gameTimes[0] <= profiler->soundTimes[0] ? profiler->gameTimes[0] : profiler->soundTimes[0];

    // set variables for duration of tasks.
    levelScriptDuration = profiler->gameTimes[1] - clockStart;
    renderDuration = profiler->gameTimes[2] - profiler->gameTimes[1];
    taskStart = 0;
    rspDuration = profiler->gfxTimes[1] - profiler->gfxTimes[0];
    rdpDuration = profiler->gfxTimes[2] - profiler->gfxTimes[0];
    vblank = 0;

    // like above functions, toss the odd bit.
    profiler->numSoundTimes &= 0xFFFE;

    // sound duration seems to be rendered with empty space and not actually drawn.
    for (i = 0; i < profiler->numSoundTimes; i += 2) {
        // calculate sound duration of max - min
        soundDuration = profiler->soundTimes[i + 1] - profiler->soundTimes[i];
        taskStart += soundDuration;
        // was the sound time minimum less than level script execution?
        if (profiler->soundTimes[i] < profiler->gameTimes[1]) {
            // overlay the levelScriptDuration onto the profiler by subtracting the soundDuration.
            levelScriptDuration -= soundDuration;
        } else if (profiler->soundTimes[i] < profiler->gameTimes[2]) {
            // overlay the renderDuration onto the profiler by subtracting the soundDuration.
            renderDuration -= soundDuration;
        }
    }

    // same as above. toss the odd bit.
    profiler->numSoundTimes &= 0xFFFE;

    //! wrong index used to retrieve vblankTimes, thus empty pairs can
    //  potentially be passed to draw_profiler_bar, because it could be sending
    //  pairs that are beyond the numVblankTimes enforced non-odd limit, due to
    //  using the wrong num value.
    for (i = 0; i < profiler->numSoundTimes; i += 2) {
        vblank += (profiler->vblankTimes[i + 1] - profiler->vblankTimes[i]);
    }

    // Draw top profilers.

    // draw sound duration as the first bar. (red)
    clockStart = 0;
    draw_profiler_bar(0, clockStart, clockStart + taskStart, 212, GPACK_RGBA5551(255, 40, 40, 1));

    // draw level script execution duration. (yellow)
    clockStart += taskStart;
    draw_profiler_bar(0, clockStart, clockStart + levelScriptDuration, 212, GPACK_RGBA5551(255, 255, 40, 1));

    // draw render duration. (orange)
    clockStart += levelScriptDuration;
    draw_profiler_bar(0, clockStart, clockStart + renderDuration, 212, GPACK_RGBA5551(255, 120, 40, 1));

    // Two added lines in Mario Kart 64
    D_800DC568 = (s32) (clockStart + renderDuration);
    D_800DC56C[0] = rdpDuration;

    // Draw bottom profilers.

    // rdp duration (orange)
    draw_profiler_bar(0, 0, rdpDuration, 216, GPACK_RGBA5551(255, 120, 40, 1));
    // rsp duration (yellow)
    draw_profiler_bar(0, 0, rspDuration, 216, GPACK_RGBA5551(255, 255, 40, 1));
    // vblank duration (red)
    draw_profiler_bar(0, 0, vblank, 216, GPACK_RGBA5551(255, 40, 40, 1));

    draw_reference_profiler_bars();
}

// Similar to draw_screen_borders from SM64, with a hint of draw_profiler
void resource_display(void) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(0, 0, 0, 0) << 16 | GPACK_RGBA5551(0, 0, 0, 0));

    if ((gControllerOne->buttonPressed & 0x20) != 0) {
        D_800DC664 ^= 1;
    }
    draw_profiler_mode_0();
}
