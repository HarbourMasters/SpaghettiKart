#include <libultraship.h>
#include <libultra/vi.h>
#include <libultra/os.h>
#include "buffers/gfx_output_buffer.h"
#include <libultraship/bridge/gfxdebuggerbridge.h>
#include <macros.h>
#include <decode.h>
#include <mk64.h>
#include <stubs.h>
#include "racing/framebuffer_effects.h"

#include "profiler.h"
#include "main.h"
#include "racing/memory.h"
#include "menus.h"
#include <segments.h>
#include <common_structs.h>
#include <defines.h>
#include "buffers.h"
#include "camera.h"
#include "profiler.h"
#include "race_logic.h"
#include "skybox_and_splitscreen.h"
#include "render_objects.h"
#include "effects.h"
#include "code_80281780.h"
#include "audio/external.h"
#include "code_800029B0.h"
#include "code_80280000.h"
#include "podium_ceremony_actors.h"
#include "menu_items.h"
#include "code_80057C60.h"
#include "profiler.h"
#include "player_controller.h"
#include "render_player.h"
#include "render_courses.h"
#include "actors.h"
#include "replays.h"
#include <debug.h>
#include "crash_screen.h"
#include "enhancements/freecam/freecam.h"
#include "engine/editor/Editor.h"
#include "port/interpolation/FrameInterpolation.h"
#include "engine/wasm.h"
#include "port/Game.h"
#include "engine/Matrix.h"

// Declarations (not in this file)
void func_80091B78(void);

void audio_init();

void create_debug_thread(void);
void start_debug_thread(void);

struct SPTask* create_next_audio_frame_task(void);

struct VblankHandler* gVblankHandler1 = NULL;
struct VblankHandler* gVblankHandler2 = NULL;

struct SPTask* gActiveSPTask = NULL;
struct SPTask* sCurrentAudioSPTask = NULL;
struct SPTask* sCurrentDisplaySPTask = NULL;
struct SPTask* sNextAudioSPTask = NULL;
struct SPTask* sNextDisplaySPTask = NULL;

struct Controller gControllers[NUM_PLAYERS];
struct Controller* gControllerOne = &gControllers[0];
struct Controller* gControllerTwo = &gControllers[1];
struct Controller* gControllerThree = &gControllers[2];
struct Controller* gControllerFour = &gControllers[3];
struct Controller* gControllerFive = &gControllers[4]; // All physical controllers combined.`
struct Controller* gControllerSix = &gControllers[5];
struct Controller* gControllerSeven = &gControllers[6];
struct Controller* gControllerEight = &gControllers[7];

Player gPlayers[NUM_PLAYERS];
Player* gPlayerOne = &gPlayers[0];
Player* gPlayerTwo = &gPlayers[1];
Player* gPlayerThree = &gPlayers[2];
Player* gPlayerFour = &gPlayers[3];
Player* gPlayerFive = &gPlayers[4];
Player* gPlayerSix = &gPlayers[5];
Player* gPlayerSeven = &gPlayers[6];
Player* gPlayerEight = &gPlayers[7];

UNUSED s32 D_800FD850[3];
struct GfxPool gGfxPools[2];
struct GfxPool* gGfxPool;

UNUSED s32 gfxPool_padding; // is this necessary?
struct VblankHandler gGameVblankHandler;
struct VblankHandler sSoundVblankHandler;
OSMesgQueue gDmaMesgQueue, gGameVblankQueue, gGfxVblankQueue, unused_gMsgQueue, gIntrMesgQueue, gSPTaskMesgQueue;
OSMesgQueue sSoundMesgQueue;
OSMesg sSoundMesgBuf[1];
OSMesg gDmaMesgBuf[1], gGameMesgBuf;
OSMesg gGfxMesgBuf[1];
UNUSED OSMesg D_8014F010, D_8014F014;
OSMesg gIntrMesgBuf[16], gSPTaskMesgBuf[16];
OSMesg gMainReceivedMesg;
OSIoMesg gDmaIoMesg;
OSMesgQueue gSIEventMesgQueue;
OSMesg gSIEventMesgBuf[3];

OSContStatus gControllerStatuses[4];
OSContPad gControllerPads[4];
u8 gControllerBits;
// Contains a 32x32 grid of indices into gCollisionIndices containing indices into gCollisionMesh
CollisionGrid gCollisionGrid[1024];
u16 gNumActors;
u16 gMatrixObjectCount;
s32 gTickLogic = 2;
s32 gTickVisuals = 1;
s32 gTickGame;
f32 D_80150118;

u16 wasSoftReset;
u16 D_8015011E;

s32 D_80150120;
s32 gGotoMode;
UNUSED s32 D_80150128;
UNUSED s32 D_8015012C;
f32 gCameraFOV[NUM_CAMERAS]; // Field-of-view for each camera
UNUSED s32 D_80150140;
UNUSED s32 D_80150144;
f32 gScreenAspect;
f32 D_8015014C;
f32 D_80150150;
UNUSED f32 D_80150154;

struct D_80150158 gD_80150158[16];
Gfx* gDisplayListHead;

struct SPTask* gGfxSPTask;
s32 D_801502A0;
s32 D_801502A4;
u16* gPhysicalFramebuffers[3];
u16 gPortFramebuffers[3][SCREEN_WIDTH * SCREEN_HEIGHT];
uintptr_t gPhysicalZBuffer;
UNUSED u32 D_801502B8;
UNUSED u32 D_801502BC;
Mat4 sBillBoardMtx; // Faces 2D actors at the camera

s32 padding[2048];

u16 D_80152308;

UNUSED OSThread paddingThread;
OSThread gIdleThread;
ALIGNED8 u8 gIdleThreadStack[STACKSIZE]; // Based on sm64 and padding between bss symbols.
OSThread gVideoThread;
ALIGNED8 u8 gVideoThreadStack[STACKSIZE];
UNUSED OSThread D_80156820;
UNUSED ALIGNED8 u8 D_8015680_Stack[STACKSIZE];
OSThread gGameLoopThread;
ALIGNED8 u8 gGameLoopThreadStack[STACKSIZE];
OSThread gAudioThread;
ALIGNED8 u8 gAudioThreadStack[STACKSIZE];
UNUSED OSThread D_8015CD30;
UNUSED ALIGNED8 u8 D_8015CD30_Stack[STACKSIZE / 2];

ALIGNED8 u8 gGfxSPTaskYieldBuffer[4352];
ALIGNED8 u32 gGfxSPTaskStack[256];
OSMesg gPIMesgBuf[32];
OSMesgQueue gPIMesgQueue;

s32 gGamestate = 0xFFFF;
// gRaceState is externed as an s32 in other files. D_800DC514 is only used in main.c, likely a developer mistake.
s32 gRaceState = RACE_INIT;
u16 D_800DC514 = 0;
u16 gDemoMode = DEMO_MODE_INACTIVE;
u16 gEnableDebugMode = DEBUG_MODE;
s32 gGamestateNext = 7; // = COURSE_DATA_MENU?;
UNUSED s32 D_800DC528 = 1;
s32 gActiveScreenMode = SCREEN_MODE_1P;
s32 gScreenModeSelection = SCREEN_MODE_1P;
UNUSED s32 D_800DC534 = 0;
s32 gPlayerCountSelection1 = 2;
bool gTourComplete = false;

s32 gModeSelection = GRAND_PRIX;
s32 D_800DC540 = 0;
s32 D_800DC544 = 0;
s32 gCCSelection = CC_150;
s32 gGlobalTimer = 0;
UNUSED s32 D_800DC550 = 0;
UNUSED s32 D_800DC554 = 0;
UNUSED s32 D_800DC558 = 0;
// Framebuffer rendering values (max 3)
u16 sRenderedFramebuffer = 0;
u16 sRenderingFramebuffer = 0;
UNUSED u16 D_800DC564 = 0;
s32 D_800DC568 = 0;
s32 D_800DC56C[8] = { 0 };
s16 sNumVBlanks = 0;
UNUSED s16 D_800DC590 = 0;
f32 gVBlankTimer = 0.0f;
f32 gCourseTimer = 0.0f;

void create_thread(OSThread* thread, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri pri) {
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}
void isPrintfInit(void);
void main_func(void) {
#ifdef VERSION_EU
    osTvType = OS_TV_PAL;
#endif
    osInitialize();
#ifdef DEBUG
    isPrintfInit(); // init osSyncPrintf
#endif
    create_thread(&gIdleThread, 1, &thread1_idle, NULL, gIdleThreadStack + ARRAY_COUNT(gIdleThreadStack), 100);
    osStartThread(&gIdleThread);
}
// extern OSViMode osViModeTable[];
/**
 * Initialize hardware, start main thread, then idle.
 */
void thread1_idle(void* arg) {
    osCreateViManager(OS_PRIORITY_VIMGR);
#ifdef VERSION_EU
    osViSetMode(&osViModeTable[OS_VI_PAL_LAN1]);
#else // VERSION_US
    if (osTvType == OS_TV_NTSC) {
        // osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    } else {
        // osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
    }
#endif
    osViBlack(true);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    wasSoftReset = (s16) osResetType;
    create_debug_thread();
    start_debug_thread();
    create_thread(&gVideoThread, 3, &thread3_video, arg, gVideoThreadStack + ARRAY_COUNT(gVideoThreadStack), 100);
    osStartThread(&gVideoThread);
    osSetThreadPri(NULL, 0);

    // Halt
    while (true) {
        ;
    }
}

void setup_mesg_queues(void) {
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, ARRAY_COUNT(gDmaMesgBuf));
    osCreateMesgQueue(&gSPTaskMesgQueue, gSPTaskMesgBuf, ARRAY_COUNT(gSPTaskMesgBuf));
    osCreateMesgQueue(&gIntrMesgQueue, gIntrMesgBuf, ARRAY_COUNT(gIntrMesgBuf));
    osViSetEvent(&gIntrMesgQueue, OS_MESG_32(MESG_VI_VBLANK), 1);
    osSetEventMesg(OS_EVENT_SP, &gIntrMesgQueue, OS_MESG_32(MESG_SP_COMPLETE));
    osSetEventMesg(OS_EVENT_DP, &gIntrMesgQueue, OS_MESG_32(MESG_DP_COMPLETE));
}

void start_sptask(s32 taskType) {
    if (taskType == M_AUDTASK) {
        gActiveSPTask = sCurrentAudioSPTask;
    } else {
        gActiveSPTask = sCurrentDisplaySPTask;
    }
    osSpTaskLoad(&gActiveSPTask->task);
    osSpTaskStartGo(&gActiveSPTask->task);
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
}

extern void Graphics_PushFrame(Gfx* data);

/**
 * Initializes the Fast3D OSTask structure.
 * Loads F3DEX or F3DLX based on the number of players
 **/
void create_gfx_task_structure(void) {
    gGfxSPTask->msgqueue = &gGfxVblankQueue;
#ifdef TARGET_N64
    gGfxSPTask->msg = (OSMesg) 2;
    gGfxSPTask->task.t.type = M_GFXTASK;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_boot = rspF3DBootStart;
    gGfxSPTask->task.t.ucode_boot_size = ((u8*) rspF3DBootEnd - (u8*) rspF3DBootStart);
    // The split-screen multiplayer racing state uses F3DLX which has a simple subpixel calculation.
    // Singleplayer race mode and all other game states use F3DEX.
    // http://n64devkit.square7.ch/n64man/ucode/gspF3DEX.htm
    if (gGamestate != RACING || gPlayerCountSelection1 == 1) {
        gGfxSPTask->task.t.ucode = gspF3DEXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DEXDataStart;
    } else {
        gGfxSPTask->task.t.ucode = gspF3DLXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DLXDataStart;
    }
    gGfxSPTask->task.t.flags = 0;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_size = SP_UCODE_SIZE;
    gGfxSPTask->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gGfxSPTask->task.t.dram_stack = (u64*) &gGfxSPTaskStack;
    gGfxSPTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxSPTask->task.t.output_buff = (u64*) &gGfxSPTaskOutputBuffer;
    gGfxSPTask->task.t.output_buff_size = (u64*) ((u8*) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer));
    gGfxSPTask->task.t.data_ptr = (u64*) gGfxPool->gfxPool;
    gGfxSPTask->task.t.data_size = (gDisplayListHead - gGfxPool->gfxPool) * sizeof(Gfx);
#endif
    func_8008C214();
#ifdef TARGET_N64
    gGfxSPTask->task.t.yield_data_ptr = (u64*) &gGfxSPTaskYieldBuffer;
    gGfxSPTask->task.t.yield_data_size = OS_YIELD_DATA_SIZE;
#else
    if (GfxDebuggerIsDebuggingRequested()) {
        GfxDebuggerDebugDisplayList(gGfxPool->gfxPool);
    }
    Graphics_PushFrame(gGfxPool->gfxPool);
#endif
}

f32 gDeltaTime = 0.0f;
f32 calculate_delta_time(void) {
    static u32 prevtime = 0;
    u32 now = osGetCount();
    f32 deltaTime;

    if (now > prevtime) {
        deltaTime = (f32) (now - prevtime) / OS_CPU_COUNTER;
    } else {
        // Handle counter reset
        deltaTime = (f32) ((0xffffffff - prevtime) + 1 + now) / OS_CPU_COUNTER;
    }

    prevtime = now;

    // Cap deltaTime to avoid large jumps (e.g., on pause/resume)
    if (deltaTime > 1.0f / 15.0f) { // Assume 15 FPS is the lowest acceptable rate
        deltaTime = 1.0f / 15.0f;
    }

    gDeltaTime = deltaTime;
}

void init_controllers(void) {
    osCreateMesgQueue(&gSIEventMesgQueue, &gSIEventMesgBuf[0], ARRAY_COUNT(gSIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &gSIEventMesgQueue, OS_MESG_32(0x33333333));
    osContInit(&gSIEventMesgQueue, &gControllerBits, gControllerStatuses);
    if ((gControllerBits & 1) == 0) {
        sIsController1Unplugged = true;
    } else {
        sIsController1Unplugged = false;
    }
}

void update_controller(s32 index) {
    struct Controller* controller = &gControllers[index];
    u16 stick;

    if (sIsController1Unplugged) {
        return;
    }

    // Prevents pause menu intereference while controlling flycam
    // Freecam only works with controller 1
    if ((CVarGetInteger("gFreecam", 0) == 1) && (gGamestate == RACING) && (index == 0)) {
        freecam_update_controller();
        return;
    }

    controller->rawStickX = gControllerPads[index].stick_x;
    controller->rawStickY = gControllerPads[index].stick_y;

    controller->rightRawStickX = gControllerPads[index].right_stick_x;
    controller->rightRawStickY = gControllerPads[index].right_stick_y;

    if ((gControllerPads[index].button & 4) != 0) {
        gControllerPads[index].button |= Z_TRIG;
    }
    controller->buttonPressed = gControllerPads[index].button & (gControllerPads[index].button ^ controller->button);
    controller->buttonDepressed = controller->button & (gControllerPads[index].button ^ controller->button);
    controller->button = gControllerPads[index].button;

    stick = 0;
    if (controller->rawStickX < -50) {
        stick |= L_JPAD;
    }
    if (controller->rawStickX > 50) {
        stick |= R_JPAD;
    }
    if (controller->rawStickY < -50) {
        stick |= D_JPAD;
    }
    if (controller->rawStickY > 50) {
        stick |= U_JPAD;
    }
    controller->stickPressed = stick & (stick ^ controller->stickDirection);
    controller->stickDepressed = controller->stickDirection & (stick ^ controller->stickDirection);
    controller->stickDirection = stick;
}

void read_controllers(void) {
    OSMesg msg;

    osContStartReadData(&gSIEventMesgQueue);
    // osRecvMesg(&gSIEventMesgQueue, &msg, OS_MESG_BLOCK);
    osContGetReadData(gControllerPads);
    update_controller(0);
    update_controller(1);
    update_controller(2);
    update_controller(3);
    gControllerFive->button = (s16) (((gControllerOne->button | gControllerTwo->button) | gControllerThree->button) |
                                     gControllerFour->button);
    gControllerFive->buttonPressed =
        (s16) (((gControllerOne->buttonPressed | gControllerTwo->buttonPressed) | gControllerThree->buttonPressed) |
               gControllerFour->buttonPressed);
    gControllerFive->buttonDepressed = (s16) (((gControllerOne->buttonDepressed | gControllerTwo->buttonDepressed) |
                                               gControllerThree->buttonDepressed) |
                                              gControllerFour->buttonDepressed);
    gControllerFive->stickDirection =
        (s16) (((gControllerOne->stickDirection | gControllerTwo->stickDirection) | gControllerThree->stickDirection) |
               gControllerFour->stickDirection);
    gControllerFive->stickPressed =
        (s16) (((gControllerOne->stickPressed | gControllerTwo->stickPressed) | gControllerThree->stickPressed) |
               gControllerFour->stickPressed);
    gControllerFive->stickDepressed =
        (s16) (((gControllerOne->stickDepressed | gControllerTwo->stickDepressed) | gControllerThree->stickDepressed) |
               gControllerFour->stickDepressed);
}

void func_80000BEC(void) {
    gPhysicalZBuffer = VIRTUAL_TO_PHYSICAL(&gZBuffer);
}

void dispatch_audio_sptask(struct SPTask* spTask) {
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, OS_MESG_PTR(spTask), OS_MESG_NOBLOCK);
}

void exec_display_list(struct SPTask* spTask) {
    osWritebackDCacheAll();
    spTask->state = SPTASK_STATE_NOT_STARTED;
    if (sCurrentDisplaySPTask == NULL) {
        sCurrentDisplaySPTask = spTask;
        sNextDisplaySPTask = NULL;
        osSendMesg(&gIntrMesgQueue, OS_MESG_32(MESG_START_GFX_SPTASK), OS_MESG_NOBLOCK);
    } else {
        sNextDisplaySPTask = spTask;
    }
}

/**
 * Set default RCP (Reality Co-Processor) settings.
 */
void init_rcp(void) {
    init_rdp();
    set_viewport();
    select_framebuffer();
    init_z_buffer();
}

/**
 * End the master display list and initialize the graphics task structure for the next frame to be rendered.
 */
void end_master_display_list(void) {
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
    create_gfx_task_structure();
}

// clear_frame_buffer from SM64, with a few edits
//! @todo Why did void* work for matching
void* clear_framebuffer(s32 color) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void rendering_init(void) {
    gGfxPool = &gGfxPools[0];
    gGfxSPTask = &gGfxPool->spTask;
    gDisplayListHead = gGfxPool->gfxPool;
    init_rcp();
    clear_framebuffer(0);
    end_master_display_list();
    exec_display_list(&gGfxPool->spTask);
    sRenderingFramebuffer++;
    gGlobalTimer++;
}

void config_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer & 1];
    gDisplayListHead = gGfxPool->gfxPool;
    gGfxSPTask = &gGfxPool->spTask;
}

/**
 * Send current master display list for rendering.
 * Tell the VI which colour framebuffer to display.
 * Yields to the VI framerate twice, locking the game at 30 FPS.
 * Selects the next framebuffer to be rendered and displayed.
 */
void display_and_vsync(void) {
    profiler_log_thread5_time(BEFORE_DISPLAY_LISTS);
    // osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    exec_display_list(&gGfxPool->spTask);
    profiler_log_thread5_time(AFTER_DISPLAY_LISTS);
#ifdef TARGET_N64
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#endif
    osViSwapBuffer((void*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[sRenderedFramebuffer]));
    profiler_log_thread5_time(THREAD5_END);
    // osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    // crash_screen_set_framebuffer(gPhysicalFramebuffers[sRenderedFramebuffer]);

    if (++sRenderedFramebuffer == 3) {
        sRenderedFramebuffer = 0;
    }
    if (++sRenderingFramebuffer == 3) {
        sRenderingFramebuffer = 0;
    }
    if (gTickVisuals) {
        gGlobalTimer++;
    }
}

void init_segment_ending_sequences(void) {
#ifdef TARGET_N64
    bzero((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osWritebackDCacheAll();
    dma_copy((u8*) SEG_ENDING, (u8*) SEG_ENDING_ROM_START, SEG_ENDING_ROM_SIZE);
    osInvalICache((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osInvalDCache((void*) SEG_ENDING, SEG_ENDING_SIZE);
#endif
}

void init_segment_racing(void) {
#ifdef TARGET_N64
    bzero((void*) SEG_RACING, SEG_RACING_SIZE);
    osWritebackDCacheAll();
    dma_copy((u8*) SEG_RACING, (u8*) SEG_RACING_ROM_START, SEG_RACING_ROM_SIZE);
    osInvalICache((void*) SEG_RACING, SEG_RACING_SIZE);
    osInvalDCache((void*) SEG_RACING, SEG_RACING_SIZE);
#endif
}

void dma_copy(u8* dest, u8* romAddr, size_t size) {

    osInvalDCache(dest, size);
    while (size > 0x100) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, 0x100, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
        size -= 0x100;
        romAddr += 0x100;
        dest += 0x100;
    }
    if (size != 0) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, size, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    }
}

/**
 * Setup main segments and framebuffers.
 */
void setup_game_memory(void) {
    //     UNUSED u32 pad[2];
    //     ptrdiff_t commonCourseDataSize; // Compressed mio0 size
    //     uintptr_t textureSegSize;
    //     ptrdiff_t textureSegStart;
    //     uintptr_t allocatedMemory;
    //     UNUSED s32 unknown_padding;

    init_segment_racing();
    // gHeapEndPtr = SEG_RACING;
    //     set_segment_base_addr(0, (void *) SEG_START);

    //     // Memory pool size of 0xAB630
    initialize_memory_pool();

    func_80000BEC();

    //     // Initialize trig tables segment
    //     osInvalDCache((void *) TRIG_TABLES, TRIG_TABLES_SIZE);
    //     osPiStartDma(&gDmaIoMesg, 0, 0, TRIG_TABLES_ROM_START, (void *) TRIG_TABLES, TRIG_TABLES_SIZE,
    //     &gDmaMesgQueue); osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    //     set_segment_base_addr(2, (void *) load_data(SEG_DATA_START, SEG_DATA_END));

    //     commonCourseDataSize = COMMON_TEXTURES_SIZE;
    //     commonCourseDataSize = ALIGN16(commonCourseDataSize);

    // #ifdef AVOID_UB
    //     textureSegStart = (ptrdiff_t) SEG_RACING - commonCourseDataSize;
    // #else
    //     textureSegStart = SEG_RACING - commonCourseDataSize;
    // #endif
    //     osPiStartDma(&gDmaIoMesg, 0, 0, COMMON_TEXTURES_ROM_START, (void *) textureSegStart, commonCourseDataSize,
    //     &gDmaMesgQueue); osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    //     textureSegSize = *(uintptr_t *)(textureSegStart + 4);
    //     textureSegSize = ALIGN16(textureSegSize);
    //     allocatedMemory = gNextFreeMemoryAddress;
    //     mio0decode((u8 *) textureSegStart, (u8 *) allocatedMemory);
    //     set_segment_base_addr(0xD, (void *) allocatedMemory);

    //     gNextFreeMemoryAddress += textureSegSize;

    //     // Common track data does not get reloaded when the race state resets.
    //     // Therefore, only reset the memory ptr to after the common track data.
    gFreeMemoryResetAnchor = gNextFreeMemoryAddress;
}

/**
 * @brief
 *
 */
void game_init_clear_framebuffer(void) {
    gGamestateNext = 0; // = START_MENU_FROM_QUIT?
    clear_framebuffer(0);
}

void display_debug_info(void) {
    u16 rotY;
    if (!gEnableDebugMode) {
        D_800DC514 = false;
    } else if (D_800DC514) {
        if ((gControllerOne->buttonPressed & R_TRIG) && (gControllerOne->button & A_BUTTON) &&
            (gControllerOne->button & B_BUTTON)) {
            D_800DC514 = false;
        }
        rotY = camera1->rot[1];
        gDebugPathCount = gScreenOneCtx->pathCounter;

        if (rotY < 0x2000) {
            func_80057A50(40, 100, "SOUTH  ", gDebugPathCount);
        } else if (rotY < 0x6000) {
            func_80057A50(40, 100, "EAST   ", gDebugPathCount);
        } else if (rotY < 0xA000) {
            func_80057A50(40, 100, "NORTH  ", gDebugPathCount);
        } else if (rotY < 0xE000) {
            func_80057A50(40, 100, "WEST   ", gDebugPathCount);
        } else {
            func_80057A50(40, 100, "SOUTH  ", gDebugPathCount);
        }

    } else if ((gControllerOne->buttonPressed & L_TRIG) && (gControllerOne->button & A_BUTTON) &&
               (gControllerOne->button & B_BUTTON)) {
        D_800DC514 = true;
    }

    if (!gEnableDebugMode) {
        gEnableResourceMeters = 0;
    } else {
        if (gEnableResourceMeters) {
            resource_display();
            if (!(gControllerOne->button & L_TRIG) && (gControllerOne->button & R_TRIG) &&
                (gControllerOne->buttonPressed & B_BUTTON)) {
                gEnableResourceMeters = 0;
            }
        } else if (!(gControllerOne->button & L_TRIG) && (gControllerOne->button & R_TRIG) &&
                   (gControllerOne->buttonPressed & B_BUTTON)) {
            gEnableResourceMeters = 1;
        }
    }
}

void process_game_tick(void) {

    if (Editor_IsPaused() == false) {
        if (D_8015011E) {
            gCourseTimer += TRACK_TIMER_ITER;
        }
        func_802909F0();
        evaluate_collision_for_players_and_actors();
        handle_a_press_for_all_players_during_race();
    }

    CM_TickCameras();

    // Editor requires this so the camera keeps moving while the game is paused.
    if (Editor_IsPaused() == true) {
        return;
    }
    
    func_80028F70(); // Player controller

    func_8028F474();
    func_80059AC8();
    update_course_actors();
    CM_TickActors();
    func_802966A0();
    if (CM_IsTourEnabled() == false) {
        func_8028FCBC();
    }
}

void race_logic_loop(void) {
    ClearMatrixPools();
    ClearObjectsMatrixPool();
    Editor_ClearMatrix();
    Editor_CleanWorld(); // Clears all actors
    gMatrixObjectCount = 0;
    gMatrixEffectCount = 0;

    if (gIsGamePaused != 0) {
        func_80290B14();
    }
    if (gIsInQuitToMenuTransition != 0) {
        func_802A38B4();
        return;
    }

    if (sNumVBlanks >= 6) {
        sNumVBlanks = 5;
    }
    else if (sNumVBlanks < 0) {
        sNumVBlanks = 1;
    }

    func_802A4EF4();

    if (gModeSelection == TIME_TRIALS) {
        replays_loop();
    }

    if (gIsGamePaused == false) {
        for (size_t i = 0; i < gTickLogic; i++) {
            process_game_tick();
        }
        if (Editor_IsPaused() == false) {
            func_80022744();
        }
    }
    func_8005A070();
    CM_TickEditor();
    profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
    sNumVBlanks = 0;
    gNumScreens = 0;
    init_rdp();
    if (D_800DC5B0 != 0) {
        select_framebuffer();
    }

    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            render_screens(RENDER_SCREEN_MODE_1P_PLAYER_ONE, 0, 0);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            if (gPlayerWinningIndex == 0) {
                // In VS Mode the winning player's viewport takes over the whole screen.
                // Rendering the winning player last places their screen above the other screens
                render_screens(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO, 1, 1);
                render_screens(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE, 0, 0);
            } else {
                render_screens(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE, 0, 0);
                render_screens(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO, 1, 1);
            }
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (gPlayerWinningIndex == 0) {
                render_screens(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO, 1, 1);
                render_screens(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE, 0, 0);
            } else {
                render_screens(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE, 0, 0);
                render_screens(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO, 1, 1);
            }
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if (gPlayerWinningIndex == 0) {
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO, 1, 1);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE, 2, 2);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR, 3, 3);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE, 0, 0);
            } else if (gPlayerWinningIndex == 1) {
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE, 0, 0);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE, 2, 2);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR, 3, 3);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO, 1, 1);
            } else if (gPlayerWinningIndex == 2) {

                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE, 0, 0);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO, 1, 1);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR, 3, 3);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE, 2, 2);
            } else {
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE, 0, 0);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO, 1, 1);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE, 2, 2);
                render_screens(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR, 3, 3);
            }
            break;
    }

    display_debug_info();

    func_802A4300();
    func_800591B4();
    func_80093E20();
#if DVDL
    display_dvdl();
#endif
    // Copies after all the main rendering is complete. This is accurate to hardware.
    // This means things like jumbotron will have the pause menu rendered in it.
    // To avoid that, this line can be moved above func_800591B4.
    FB_WriteFramebufferSliceToCPU(&gDisplayListHead, gPortFramebuffers[sRenderingFramebuffer], true);
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);

    // End of frame cleanup of actors, objects, etc.
    CM_RunGarbageCollector();
}

/**
 * mk64's game loop depends on a series of states.
 * It runs a wide branching series of code based on these states.
 * State 1) Clear framebuffer
 * State 2) Run menus
 * State 3) Process race related logic
 * State 4) Ending sequence
 * State 5) Credits
 *
 * Note that the state doesn't flip-flop at random but is permanent
 * until the state changes (ie. Exit menus and start a race).
 */

void game_state_handler(void) {
    FrameInterpolation_StartRecord();

#if DVDL
    if ((gControllerOne->button & L_TRIG) && (gControllerOne->button & R_TRIG) && (gControllerOne->button & Z_TRIG) &&
        (gControllerOne->button & A_BUTTON)) {
        gGamestateNext = CREDITS_SEQUENCE;
    } else if ((gControllerOne->button & L_TRIG) && (gControllerOne->button & R_TRIG) &&
               (gControllerOne->button & Z_TRIG) && (gControllerOne->button & B_BUTTON)) {
        gGamestateNext = ENDING;
    }
#endif
    switch (gGamestate) {
        case 7:
            game_init_clear_framebuffer();
            break;
        case START_MENU_FROM_QUIT:
        case MAIN_MENU_FROM_QUIT:
        case PLAYER_SELECT_MENU_FROM_QUIT:
        case COURSE_SELECT_MENU_FROM_QUIT:
            // Display black
            osViBlack(0);
            update_menus();
            init_rcp();
            func_80094A64(gGfxPool);
#if DVDL
            display_dvdl();
#endif
            break;
        case RACING:
            race_logic_loop();
            break;
        case ENDING:
            podium_ceremony_loop();
            break;
        case CREDITS_SEQUENCE:
            credits_loop();
            break;
    }
    FrameInterpolation_StopRecord();
}

void interrupt_gfx_sptask(void) {
    if (gActiveSPTask->task.t.type == M_GFXTASK) {
        gActiveSPTask->state = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

void receive_new_tasks(void) {
    UNUSED s32 pad;
    struct SPTask* spTask;

    while (osRecvMesg(&gSPTaskMesgQueue, (OSMesg*) &spTask, OS_MESG_NOBLOCK) != -1) {
        spTask->state = SPTASK_STATE_NOT_STARTED;
        switch (spTask->task.t.type) {
            case 2:
                sNextAudioSPTask = spTask;
                break;
            case 1:
                sNextDisplaySPTask = spTask;
                break;
        }
    }

    if (sCurrentAudioSPTask == NULL && sNextAudioSPTask != NULL) {
        sCurrentAudioSPTask = sNextAudioSPTask;
        sNextAudioSPTask = NULL;
    }
    if (sCurrentDisplaySPTask == NULL && sNextDisplaySPTask != NULL) {
        sCurrentDisplaySPTask = sNextDisplaySPTask;
        sNextDisplaySPTask = NULL;
    }
}

void set_vblank_handler(s32 index, struct VblankHandler* handler, OSMesgQueue* queue, OSMesg* msg) {
    handler->queue = queue;
    handler->msg = *msg;
    switch (index) {
        case 1:
            gVblankHandler1 = handler;
            break;
        case 2:
            gVblankHandler2 = handler;
            break;
    }
}

void start_gfx_sptask(void) {
    if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL &&
        sCurrentDisplaySPTask->state == SPTASK_STATE_NOT_STARTED) {
        profiler_log_gfx_time(TASKS_QUEUED);
        start_sptask(M_GFXTASK);
    }
}

void handle_vblank(void) {
    gVBlankTimer += V_BlANK_TIMER_ITER;
    sNumVBlanks++;

    receive_new_tasks();

    // First try to kick off an audio task. If the gfx task is currently
    // running, we need to asynchronously interrupt it -- handle_sp_complete
    // will pick up on what we're doing and start the audio task for us.
    // If there is already an audio task running, there is nothing to do.
    // If there is no audio task available, try a gfx task instead.
    if (sCurrentAudioSPTask != NULL) {
        if (gActiveSPTask != NULL) {
            interrupt_gfx_sptask();
        } else {
            profiler_log_vblank_time();
            start_sptask(M_AUDTASK);
        }
    } else {
        if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL &&
            sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
            profiler_log_gfx_time(TASKS_QUEUED);
            start_sptask(M_GFXTASK);
        }
    }

/* This is where I would put my rumble code... If I had any. */
#if ENABLE_RUMBLE
#ifdef TARGET_N64
    rumble_thread_update_vi();
#endif
#endif

    if (gVblankHandler1 != NULL) {
        osSendMesg(gVblankHandler1->queue, gVblankHandler1->msg, OS_MESG_NOBLOCK);
    }
    if (gVblankHandler2 != NULL) {
        osSendMesg(gVblankHandler2->queue, gVblankHandler2->msg, OS_MESG_NOBLOCK);
    }
}

void handle_dp_complete(void) {
    // Gfx SP task is completely done.
    if (sCurrentDisplaySPTask->msgqueue != NULL) {
        osSendMesg(sCurrentDisplaySPTask->msgqueue, sCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    }
    profiler_log_gfx_time(RDP_COMPLETE);
    sCurrentDisplaySPTask->state = SPTASK_STATE_FINISHED_DP;
    sCurrentDisplaySPTask = NULL;
}

void handle_sp_complete(void) {
    struct SPTask* curSPTask = gActiveSPTask;

    gActiveSPTask = NULL;

    if (curSPTask->state == SPTASK_STATE_INTERRUPTED) {
        // handle_vblank tried to start an audio task while there was already a
        // gfx task running, so it had to interrupt the gfx task. That interruption
        // just finished.
        if (osSpTaskYielded((OSTask*) curSPTask) == 0) {
            // The gfx task completed before we had time to interrupt it.
            // Mark it finished, just like below.
            curSPTask->state = SPTASK_STATE_FINISHED;
            profiler_log_gfx_time(RSP_COMPLETE);
        }
        // Start the audio task, as expected by handle_vblank.
        profiler_log_vblank_time();
        start_sptask(M_AUDTASK);
    } else {
        curSPTask->state = SPTASK_STATE_FINISHED;
        if (curSPTask->task.t.type == M_AUDTASK) {
            // After audio tasks come gfx tasks.
            profiler_log_vblank_time();
            if (sCurrentDisplaySPTask != NULL) {
                if (sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
                    if (sCurrentDisplaySPTask->state != SPTASK_STATE_INTERRUPTED) {
                        profiler_log_gfx_time(TASKS_QUEUED);
                    }
                    start_sptask(M_GFXTASK);
                }
            }
            sCurrentAudioSPTask = NULL;
            if (curSPTask->msgqueue != NULL) {
                osSendMesg(curSPTask->msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
            }
        } else {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the task completely finished and
            // null out sCurrentDisplaySPTask. That happens in handle_dp_complete.
            profiler_log_gfx_time(RSP_COMPLETE);
        }
    };
}

void thread3_video(UNUSED void* arg0) {
    s32 i;
    u64* framebuffer1;
    OSMesg msg;
    UNUSED s32 pad[4];

    gPhysicalFramebuffers[0] = (u16*) &gFramebuffer0;
    gPhysicalFramebuffers[1] = (u16*) &gFramebuffer1;
    gPhysicalFramebuffers[2] = (u16*) &gFramebuffer2;

    // Clear framebuffer.
    framebuffer1 = (u64*) &gFramebuffer1;
    for (i = 0; i < 19200; i++) {
        framebuffer1[i] = 0;
    }
    setup_mesg_queues();
    setup_game_memory();

    create_thread(&gAudioThread, 4, &thread4_audio, 0, gAudioThreadStack + ARRAY_COUNT(gAudioThreadStack), 20);
    osStartThread(&gAudioThread);

    create_thread(&gGameLoopThread, 5, &thread5_game_loop, 0, gGameLoopThreadStack + ARRAY_COUNT(gGameLoopThreadStack),
                  10);
    osStartThread(&gGameLoopThread);

    while (true) {
        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((u32) msg.data32) {
            case MESG_VI_VBLANK:
                handle_vblank();
                break;
            case MESG_SP_COMPLETE:
                handle_sp_complete();
                break;
            case MESG_DP_COMPLETE:
                handle_dp_complete();
                break;
            case MESG_START_GFX_SPTASK:
                start_gfx_sptask();
                break;
        }
    }
}

void func_800025D4(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

void func_80002600(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

void func_8000262C(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

void func_80002658(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

/**
 * Sets trackId to NULL if
 *
 *
 */
void update_gamestate(void) {
    switch (gGamestate) {
        case START_MENU_FROM_QUIT:
            func_80002658();
            gCurrentlyLoadedTrackAddr = NULL;
            break;
        case MAIN_MENU_FROM_QUIT:
            func_800025D4();
            gCurrentlyLoadedTrackAddr = NULL;
            break;
        case PLAYER_SELECT_MENU_FROM_QUIT:
            func_80002600();
            gCurrentlyLoadedTrackAddr = NULL;
            break;
        case COURSE_SELECT_MENU_FROM_QUIT:
            func_8000262C();
            gCurrentlyLoadedTrackAddr = NULL;
            break;
        case RACING:
            /**
             * @bug Reloading this segment makes random_u16() deterministic for player spawn order.
             * In laymens terms, random_u16() outputs the same value every time.
             */
            // init_segment_racing();
            setup_race();
            break;
        case ENDING:
            gCurrentlyLoadedTrackAddr = NULL;
            init_segment_ending_sequences();
            setup_podium_ceremony();
            break;
        case CREDITS_SEQUENCE:
            gCurrentlyLoadedTrackAddr = NULL;
            // init_segment_racing();
            init_segment_ending_sequences();
            load_credits();
            break;
    }
}

void thread5_game_loop(void) {
    setup_game_memory();
    osCreateMesgQueue(&gGfxVblankQueue, gGfxMesgBuf, 1);
    osCreateMesgQueue(&gGameVblankQueue, &gGameMesgBuf, 1);
    init_controllers();
    if (!wasSoftReset) {
        clear_nmi_buffer();
    }

    // set_vblank_handler(2, &gGameVblankHandler, &gGameVblankQueue, (OSMesg *)(OS_EVENT_SW2));
    // These variables track stats such as player wins.
    // In the event of a console reset, it remembers them.
    gNmiUnknown1 = &pAppNmiBuffer[0]; // 2  u8's, tracks number of times player 1/2 won a VS race
    gNmiUnknown2 =
        &pAppNmiBuffer[2]; // 9  u8's, 3x3, tracks number of times player 1/2/3   has placed in 1st/2nd/3rd in a VS race
    gNmiUnknown3 = &pAppNmiBuffer[11]; // 12 u8's, 4x3, tracks number of times player 1/2/3/4 has placed in 1st/2nd/3rd
                                       // in a VS race
    gNmiUnknown4 = &pAppNmiBuffer[23]; // 2  u8's, tracking number of Battle mode wins by player 1/2
    gNmiUnknown5 = &pAppNmiBuffer[25]; // 3  u8's, tracking number of Battle mode wins by player 1/2/3
    gNmiUnknown6 = &pAppNmiBuffer[28]; // 4  u8's, tracking number of Battle mode wins by player 1/2/3/4
    rendering_init();
    read_controllers();
    func_800C5CB8();
}

void thread5_iteration(void) {
    func_800CB2C4();
    calculate_delta_time();
#ifdef TARGET_N64
    while (true) {
        func_800CB2C4();
    }
#endif
    if (GfxDebuggerIsDebugging()) {
        Graphics_PushFrame(gGfxPool->gfxPool);
        return;
    }
    // Update the gamestate if it has changed (racing, menus, credits, etc.).
    if (gGamestateNext != gGamestate) {
        gGamestate = gGamestateNext;
        update_gamestate();
    }
    profiler_log_thread5_time(THREAD5_START);
    config_gfx_pool();
    FB_CreateFramebuffers();
    read_controllers();
    game_state_handler();

    // call_render_hook();

    end_master_display_list();
    display_and_vsync();
}

/**
 * Sound processing thread. Runs at 50 or 60 FPS according to osTvType.
 */
void thread4_audio(UNUSED void* arg) {
    UNUSED u32 unused[3];
    audio_init();
    osCreateMesgQueue(&sSoundMesgQueue, sSoundMesgBuf, ARRAY_COUNT(sSoundMesgBuf));
    set_vblank_handler(1, &sSoundVblankHandler, &sSoundMesgQueue, (OSMesg*) 512);

    while (true) {
        OSMesg msg;
        struct SPTask* spTask;

        osRecvMesg(&sSoundMesgQueue, &msg, OS_MESG_BLOCK);

        profiler_log_thread4_time();

        spTask = create_next_audio_frame_task();
        if (spTask != NULL) {
            dispatch_audio_sptask(spTask);
        }
        profiler_log_thread4_time();
    }
}
