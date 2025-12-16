#ifndef MAIN_H
#define MAIN_H

#include "mk64.h"
#include "common_structs.h"

// Message IDs
#define MESG_SP_COMPLETE 100
#define MESG_DP_COMPLETE 101
#define MESG_VI_VBLANK 102
#define MESG_START_GFX_SPTASK 103
#define MESG_NMI_REQUEST 104

#define MTX_HUD_POOL_SIZE 1600

#ifdef AVOID_UB
#define MTX_HUD_POOL_SIZE_MAX MTX_HUD_POOL_SIZE
#else
#define MTX_HUD_POOL_SIZE_MAX MTX_HUD_POOL_SIZE - 50
#endif

#define MTX_OBJECT_POOL_SIZE 512

//! @todo Verify with proper documentation
// functions called by mtxShadow multiply by 8
// 8 Kart AI Players * 4 real players in coop
#define MTX_SHADOW_POOL_SIZE 8 * 4

//! @todo Verify with proper documentation
// functions called by mtxKart multiply by 8
// 8 Kart AI Players * 4 real players in coop
#define MTX_KART_POOL_SIZE 8 * 4

#define MTX_EFFECT_POOL_SIZE 660

// func_80095BD0 sets an OOB pool size max check (760)
#ifdef AVOID_UB
#define MTX_EFFECT_POOL_SIZE_MAX MTX_EFFECT_POOL_SIZE
#else
#define MTX_EFFECT_POOL_SIZE_MAX MTX_EFFECT_POOL_SIZE + 100
#endif

#define GFX_POOL_SIZE 150000

struct GfxPool {
    /* 0x00000 */ Mtx mtxScreen;                       // Matrix for skybox and startup logo
    /* 0x00040 */ Mtx mtxPersp[4];                     // Matrix for perspective screen modes
    /* 0x00140 */ Mtx mtxOrtho;                        // Matrix for ortho hud screen modes
    /* 0x00180 */ Mtx mtxUnk;                          // Matrix unused
    /* 0x001C0 */ Mtx mtxLookAt[4];                    // Matrix for lookat screen modes
    /* 0x002C0 */ Mtx mtxHud[MTX_HUD_POOL_SIZE];       // Matrix hud elements and 2D related effects
    /* 0x0CAC0 */ Mtx mtxObject[MTX_OBJECT_POOL_SIZE]; // Matrix track objects
    /* 0x0EAC0 */ Mtx mtxShadow[MTX_SHADOW_POOL_SIZE]; // Matrix shadow characters
    /* 0x0F2C0 */ Mtx mtxKart[MTX_KART_POOL_SIZE];     // Matrix kart characters
    /* 0x0FAC0 */ Mtx mtxEffect[MTX_EFFECT_POOL_SIZE]; // Matrix misc effects
    /* 0x19FC0 */ Mtx mtxArr[4];                       // Matrix unused array
    /* 0x1A0C0 */ Gfx gfxPool[GFX_POOL_SIZE];
    /* 0x28B20 */ struct SPTask spTask;
}; // size = 0x28B70

typedef struct {
    u16 triangle; // Index for gCollisionIndices which has indexes for gCollisionMesh
    u16 numTriangles;
} CollisionGrid;

void create_thread(OSThread*, OSId, void (*entry)(void*), void*, void*, OSPri);
void main_func(void);
void thread1_idle(void*);
void setup_mesg_queues(void);
void start_sptask(s32);
void create_gfx_task_structure(void);
void init_controllers(void);
void update_controller(s32);
void read_controllers(void);
void func_80000BEC(void);
void dispatch_audio_sptask(struct SPTask*);
void exec_display_list(struct SPTask*);
void init_rcp(void);
void end_master_display_list(void);
void* clear_framebuffer(s32);
void rendering_init(void);
void config_gfx_pool(void);
void display_and_vsync(void);
void init_segment_ending_sequences(void);
void init_segment_racing(void);
void dma_copy(u8*, u8*, size_t);
void setup_game_memory(void);
void game_init_clear_framebuffer(void);
void race_logic_loop(void);
void game_state_handler(void);
void interrupt_gfx_sptask(void);
void receive_new_tasks(void);
void set_vblank_handler(s32, struct VblankHandler*, OSMesgQueue*, OSMesg*);
void start_gfx_sptask(void);
void handle_vblank(void);
void handle_dp_complete(void);
void handle_sp_complete(void);
void thread3_video(void*);
void func_800025D4(void);
void func_80002600(void);
void func_8000262C(void);
void func_80002658(void);
void update_gamestate(void);
void thread5_game_loop(void);
void thread5_iteration(void);
void thread4_audio(void*);
extern f32 gDeltaTime;

extern struct VblankHandler* gVblankHandler1;
extern struct VblankHandler* gVblankHandler2;

extern struct SPTask* gActiveSPTask;
extern struct SPTask* sCurrentAudioSPTask;
extern struct SPTask* sCurrentDisplaySPTask;
extern struct SPTask* sNextAudioSPTask;
extern struct SPTask* sNextDisplaySPTask;

extern struct Controller gControllers[];
extern struct Controller* gControllerOne;
extern struct Controller* gControllerTwo;
extern struct Controller* gControllerThree;
extern struct Controller* gControllerFour;
extern struct Controller* gControllerFive;
extern struct Controller* gControllerSix;
extern struct Controller* gControllerSeven;
extern struct Controller* gControllerEight;

extern Player gPlayers[];
extern Player* gPlayerOne;
extern Player* gPlayerTwo;
extern Player* gPlayerThree;
extern Player* gPlayerFour;
extern Player* gPlayerFive;
extern Player* gPlayerSix;
extern Player* gPlayerSeven;
extern Player* gPlayerEight;

extern struct GfxPool gGfxPools[];
extern struct GfxPool* gGfxPool;

extern struct VblankHandler gGameVblankHandler;
extern struct VblankHandler sSoundVblankHandler;
extern OSMesgQueue gDmaMesgQueue, gGameVblankQueue, gGfxVblankQueue, unused_gMsgQueue, gIntrMesgQueue, gSPTaskMesgQueue;
extern OSMesgQueue sSoundMesgQueue;
extern OSMesg sSoundMesgBuf[1];
extern OSMesg gDmaMesgBuf[1], gGameMesgBuf;
extern OSMesg gGfxMesgBuf[];
extern OSMesg gIntrMesgBuf[16], gSPTaskMesgBuf[16];
extern OSMesg gMainReceivedMesg;
extern OSIoMesg gDmaIoMesg;
extern OSMesgQueue gSIEventMesgQueue;
extern OSMesg gSIEventMesgBuf[3];

extern OSContStatus gControllerStatuses[];

extern OSContPad gControllerPads[];
extern u8 gControllerBits;

extern CollisionGrid gCollisionGrid[];
extern u16 gNumActors;
extern u16 gMatrixObjectCount;
extern s32 gTickLogic;
extern s32 gTickVisuals;
extern f32 D_80150118;
extern u16 wasSoftReset;
extern u16 D_8015011E;

extern s32 D_80150120;
extern s32 gGotoMode;

extern f32 gScreenAspect;
extern f32 D_8015014C;
extern f32 D_80150150;

extern struct D_80150158 gD_80150158[];
extern Gfx* gDisplayListHead;
extern struct SPTask* gGfxSPTask;
extern s32 D_801502A0;
extern s32 D_801502A4;
extern u16* gPhysicalFramebuffers[];
extern u16 gPortFramebuffers[3][SCREEN_WIDTH * SCREEN_HEIGHT];
extern uintptr_t gPhysicalZBuffer;
extern Mat4 sBillBoardMtx;

extern s32 padding[];

extern u16 D_80152308;

extern OSThread gIdleThread;
extern u8 gIdleThreadStack[];
extern OSThread gVideoThread;
extern u8 gVideoThreadStack[];
extern OSThread gGameLoopThread;
extern u8 gGameLoopThreadStack[];
extern OSThread gAudioThread;
extern u8 gAudioThreadStack[];

extern u8 gGfxSPTaskYieldBuffer[];
extern u32 gGfxSPTaskStack[];
extern OSMesg gPIMesgBuf[];
extern OSMesgQueue gPIMesgQueue;
void race_logic_loop(void);
extern s32 gGamestate;

extern s32 gRaceState;

extern u16 D_800DC514;
extern u16 gDemoMode;
extern u16 gEnableDebugMode;
extern s32 gGamestateNext;
extern s32 gActiveScreenMode;
extern s32 gScreenModeSelection;
extern s32 gPlayerCountSelection1;
extern bool gTourComplete;

extern s32 gModeSelection;
extern s32 D_800DC540;
extern s32 D_800DC544;
extern s32 gCCSelection;
extern s32 gGlobalTimer;
extern u16 sRenderedFramebuffer;
extern u16 sRenderingFramebuffer;
extern s32 D_800DC568;
extern s32 D_800DC56C[];
extern s16 sNumVBlanks;
extern f32 gVBlankTimer;
extern f32 gCourseTimer;

// end of definition of main.c variables

#endif
