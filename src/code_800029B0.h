#ifndef CODE_800029B0_H
#define CODE_800029B0_H

#include <macros.h>
#include <libultraship.h>
#include <actor_types.h>
#include "camera.h"
#include "engine/CoreMath.h"

typedef struct {
    /* 0x00 */ struct Controller* controllers; // gControllers ptr 800F6910
    /* 0x04 */ Camera* camera;                 // The active camera
    /*      */ Camera* raceCamera;
    /*      */ Camera* lookBehindCamera;       // The lookBehind camera
               Camera* freeCamera;
    /*      */ Camera* pendingCamera;          // The camera to switch too next frame
    /* 0x08 */ Player* player;                 // Player ptr 800F6990
    /* 0x0C */ s32* unkC;                      // unk struct?
    /* 0x10 */ Vp viewport;
    /* 0x20 */ s32 pad[2];
    /* 0x28 */ s16 cameraHeight;
    /* 0x2A */ s16 unk;
    /* 0x2C */ s16 screenWidth;
    /* 0x2E */ s16 screenHeight;
    /* 0x30 */ s16 screenStartX;
    /* 0x32 */ s16 screenStartY;
    /* 0x34 */ s16 unk3C;
    /* 0x36 */ s16 playerDirection;
    /* 0x38 */ s16 pathCounter;
    /* 0x3A */ s16 unk42;
    /* 0x3C */ s32 pad2;
} ScreenContext;

/* Function Prototypes */

void func_800029B0(void);
void setup_race(void);
void setup_editor(void);
void func_80002DAC(void);
void clear_nmi_buffer(void);
void credits_spawn_actors(void);

extern s16 gCurrentCourseId; // D_800DC5A0
extern uintptr_t gCurrentlyLoadedTrackAddr;
extern u16 D_800DC5A8;
extern s32 D_800DC5AC;
extern u16 D_800DC5B0;
extern u16 D_800DC5B4;
extern u16 D_800DC5B8;
extern bool bFog;
extern u16 gIsInQuitToMenuTransition;
extern u16 gQuitToMenuTransitionCounter;
extern u16 D_800DC5C8;
extern u16 D_800DC5CC;
extern s32 D_800DC5D0;
extern s32 D_800DC5D4;
extern s32 D_800DC5D8;
extern s32 D_800DC5DC;
extern s32 D_800DC5E0;
// This is tracking which credit "state" we're in, decides which credits are shown (and probably other stuff)
extern u16 D_800DC5E4;
extern s32 gPlayerWinningIndex;

extern ScreenContext gScreenContexts[4];
extern ScreenContext* gScreenOneCtx;
extern ScreenContext* gScreenTwoCtx;
extern ScreenContext* gScreenThreeCtx;
extern ScreenContext* gScreenFourCtx;
extern u16 gIsGamePaused;
extern u8* pAppNmiBuffer;
extern s32 gIsMirrorMode; // D_800DC604
extern void set_mirror_mode(s32 mirror);
extern s16 gCreditsCourseId;
extern s16 gPlaceItemBoxes;
extern Vec3f gVtxStretch;

extern CollisionTriangle* gCollisionMesh;
extern u16* gCollisionIndices;
extern u16 gCollisionMeshCount;
extern u16 gNumCollisionTriangles;
extern u32 D_8015F58C;

extern Vec3f D_8015F590;
extern s32 D_8015F59C;
extern s32 D_8015F5A0;
extern s32 D_8015F5A4;

extern Vtx* vtxBuffer[];
extern s16 gTrackMaxX;
extern s16 gTrackMinX;

extern s16 gTrackMaxY;
extern s16 gTrackMinY;

extern s16 gTrackMaxZ;
extern s16 gTrackMinZ;
extern s16 D_8015F6F4;
extern s16 D_8015F6F6;
extern u16 D_8015F6F8;
extern s16 D_8015F6FA;
extern s16 D_8015F6FC;
extern u16 gNumSpawnedShells;

extern u16 D_8015F700;
extern u16 D_8015F702;
extern f32 D_8015F704;
extern Vec3f D_8015F708;
extern UNUSED u32 D_8015F718[3];
extern size_t gFreeMemorySize;
extern uintptr_t gNextFreeMemoryAddress;
extern uintptr_t gHeapEndPtr;

/**
 * This repoints gNextFreeMemoryAddress to the point in the memory pool just after a track was loaded.
 * This allows players to retry or reset a track without reloading the whole track.
 * Memory allocated after track load is not zeroed or reset. But should get overwritten by future allocations.
 *
 * This is a relatively unsafe way to architect a memory pool as old memory could accidentally be used if future allocations do not zero or fully overwrite their free memory.
 */
extern uintptr_t gFreeMemoryCourseAnchor;
extern uintptr_t gFreeMemoryResetAnchor;
extern Vec3f D_8015F738;
extern Vec3f D_8015F748;
extern Vec3f D_8015F758;
extern Vec3f D_8015F768;
extern Vec3f D_8015F778;

extern f32 gTrackDirection;
extern s32 gNumScreens;

extern s32 D_8015F790[];
extern u16 D_8015F890;
extern u16 D_8015F892;
extern u16 D_8015F894;
extern f32 gTimePlayerLastTouchedFinishLine[];

extern u8* gNmiUnknown1;
extern u8* gNmiUnknown2;
extern u8* gNmiUnknown3;
extern u8* gNmiUnknown4;
extern u8* gNmiUnknown5;
extern u8* gNmiUnknown6;

extern Vec3f D_8015F8D0;
extern s32 D_8015F8DC;

extern s32 D_8015F8E0;
extern f32 gWaterLevel;
extern f32 gWaterVelocity;
extern s16 gPlayerPositionLUT[]; // Player index at each position
extern u16 gNumPermanentActors;

extern UNUSED u8 D_80162578[];
extern s16 gDebugPathCount;
extern s16 sIsController1Unplugged;
extern struct RGBA8 gFogColour;
extern uintptr_t D_801625F8;
extern f32 D_801625FC;

#endif
