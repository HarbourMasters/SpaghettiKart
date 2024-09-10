#ifndef CRASH_SCREEN_ENHANCEMENT_H
#define CRASH_SCREEN_ENHANCEMENT_H

#include <libultraship.h>
#include <macros.h>

void crash_screen_draw(OSThread* thread);

// Add this to the top of main.c or crash_screen.c
// #define CRASH_SCREEN_ENHANCEMENT

/**
 * Example of how to force crash screen to run.
 * Make sure to include the header crash_screen_enhancement.h in main.h
 */

// void display_and_vsync(void) {
//     profiler_log_thread5_time(BEFORE_DISPLAY_LISTS);
//     osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
//     exec_display_list(&gGfxPool->spTask);
//     profiler_log_thread5_time(AFTER_DISPLAY_LISTS);
//     osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
//     osViSwapBuffer((void *) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[sRenderedFramebuffer]));
//     profiler_log_thread5_time(THREAD5_END);
//     osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
//     crash_screen_set_framebuffer((uintptr_t *) gPhysicalFramebuffers[sRenderedFramebuffer]);

//     crash_screen_draw((u16 *) gPhysicalFramebuffers[sRenderedFramebuffer], &gGameLoopThread);
//     Add this line ^

//     if (++sRenderedFramebuffer == 3) {
//         sRenderedFramebuffer = 0;
//     }
//     if (++sRenderingFramebuffer == 3) {
//         sRenderingFramebuffer = 0;
//     }
//     gGlobalTimer++;
// }

#endif // CRASH_SCREEN_ENHANCEMENT_H
