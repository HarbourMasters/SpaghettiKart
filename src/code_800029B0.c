#include <libultraship.h>
#include <macros.h>
#include <mk64.h>
#include <stubs.h>

#include "code_800029B0.h"
#include "TrackBrowser.h"
#include "memory.h"
#include "waypoints.h"
#include "actors.h"
#include "actor_types.h"
#include "math_util.h"
#include "audio/external.h"
#include <defines.h>
#include "collision.h"
#include "memory.h"
#include "menu_items.h"
#include "skybox_and_splitscreen.h"
#include "code_8006E9C0.h"
#include "spawn_players.h"
#include "replays.h"
#include "render_courses.h"
#include "main.h"
#include "courses/all_course_data.h"
#include "courses/all_course_packed.h"
#include "menus.h"
#include <assets/textures/other_textures.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_data.h>
#include <assets/models/tracks/moo_moo_farm/moo_moo_farm_data.h>
#include "port/Game.h"
#include "engine/CoreMath.h"

extern s32 D_802BA038;
extern s16 D_802BA048;
s16 gCurrentCourseId = 0;
uintptr_t gCurrentlyLoadedTrackAddr = NULL;
u16 D_800DC5A8 = 0;
s32 D_800DC5AC = 0;
u16 D_800DC5B0 = 1;
u16 D_800DC5B4 = 0;
u16 D_800DC5B8 = 0;
bool bFog = false;
u16 gIsInQuitToMenuTransition = 0;
u16 gQuitToMenuTransitionCounter = 0;
u16 D_800DC5C8 = 0;
UNUSED u16 D_800DC5CC = 0;
s32 D_800DC5D0 = 0;
s32 D_800DC5D4 = 0;
s32 D_800DC5D8 = 0;
s32 D_800DC5DC = 64;

s32 D_800DC5E0 = 32;

// This is tracking which credit "state" we're in, decides which credits are shown (and probably other stuff)
u16 D_800DC5E4 = 0;

//! @todo gPlayerWinningIndex (D_800DC5E8) accessed as word, D_800DC5EB as u8
s32 gPlayerWinningIndex = 0;

ALIGNED16 ScreenContext gScreenContexts[4] = { 0 };
ScreenContext* gScreenOneCtx = &gScreenContexts[0];
ScreenContext* gScreenTwoCtx = &gScreenContexts[1];
ScreenContext* gScreenThreeCtx = &gScreenContexts[2];
ScreenContext* gScreenFourCtx = &gScreenContexts[3];
u16 gIsGamePaused = false; // true if the game is paused and false if the game is not paused
u8* pAppNmiBuffer = (u8*) &osAppNmiBuffer;

s32 gIsMirrorMode = 0;
void set_mirror_mode(s32 mirror) {
    gIsMirrorMode = mirror;
}
Vec3f gVtxStretch = {1.0f, 1.0f, 1.0f};
Lights1 D_800DC610[] = {
    gdSPDefLights1(175, 175, 175, 255, 255, 255, 0, 0, 120),
    //! @todo impl lighting in custom track origin value 115 instead of 209. Hack fix for lighting for now
    gdSPDefLights1(209, 209, 209, 255, 255, 255, 0, 0, 120),
};
UNUSED s32 pad_800029B0 = 0x80000000;
s16 gCreditsCourseId = TRACK_LUIGI_RACEWAY;
s16 gPlaceItemBoxes = 1;

// Technically a pointer to an array, but declaring it so creates regalloc issues.
CollisionTriangle* gCollisionMesh;
u16* gCollisionIndices;
u16 gCollisionMeshCount; // Number of entries in gCollisionMesh
u16 gNumCollisionTriangles;
u32 D_8015F58C;

Vec3f D_8015F590;
s32 D_8015F59C;
s32 D_8015F5A0;
s32 D_8015F5A4;
s32 code_800029B0_bss_pad[48];
Vtx* vtxBuffer[32];

s16 gTrackMaxX;
s16 gTrackMinX;

s16 gTrackMaxY; // s16 or u16?
s16 gTrackMinY;

s16 gTrackMaxZ;
s16 gTrackMinZ;

s16 D_8015F6F4;
s16 D_8015F6F6;
u16 D_8015F6F8;
s16 D_8015F6FA;
s16 D_8015F6FC;
/**
 * Incremented by one every time the player spawns a new shell actor.
 * A cleaning routine runs once twenty-one shells have spawned.
 */
u16 gNumSpawnedShells;

u16 D_8015F700;
u16 D_8015F702;
f32 D_8015F704;
Vec3f D_8015F708;
UNUSED u32 D_8015F718[3]; // Likely held ptrs to segmented data.
size_t gFreeMemorySize;
uintptr_t gNextFreeMemoryAddress;
uintptr_t gHeapEndPtr;
uintptr_t gFreeMemoryCourseAnchor; // Reset gNextFreeMemoryAddress to just after courses were loaded
uintptr_t gFreeMemoryResetAnchor;
Vec3f D_8015F738;
Vec3f D_8015F748;
Vec3f D_8015F758;
Vec3f D_8015F768;
Vec3f D_8015F778;

f32 gTrackDirection; // Extra mode, flips vertices.
s32 gNumScreens;      // Set to zero in single player mode
s32 D_8015F790[64];   // Unknown data, potentially not used.
u16 D_8015F890;
u16 D_8015F892;
u16 D_8015F894;
// Indexed by Player ID. Track time in seconds since player has last crossed the finish line
f32 gTimePlayerLastTouchedFinishLine[NUM_PLAYERS];

u8* gNmiUnknown1;
u8* gNmiUnknown2;
u8* gNmiUnknown3;
u8* gNmiUnknown4;
u8* gNmiUnknown5;
u8* gNmiUnknown6;

Vec3f D_8015F8D0;
s32 D_8015F8DC;

s32 D_8015F8E0;
f32 gWaterLevel;
f32 gWaterVelocity;
s16 gPlayerPositionLUT[8]; // Player index at each position
u16 gNumPermanentActors;
s32 code_800029B0_bss_pad2[44];

struct Actor gActorList[100];
//! @warning todo: Is this apart of the actor array?
UNUSED u8 D_80162578[sizeof(struct Actor)];

s16 gDebugPathCount;
s16 sIsController1Unplugged;
struct RGBA8 gFogColour;
uintptr_t D_801625F8;
f32 D_801625FC;

void update_music_volume(s32 target){
    float volume = (float) target / 127.0f;
    CVarSetFloat("gMainMusicVolume", volume);
    audio_set_player_volume(SEQ_PLAYER_LEVEL, volume);
}

void func_800029B0(void) {
    switch (D_800DC5A8) {
        case 0:
            update_music_volume(127);
            break;
        case 1:
            update_music_volume(75);
            break;
        case 2:
            update_music_volume(0);
            break;
    }
}

void setup_race(void) {
    struct Controller* controller;
    int i;

    LUSLOG_DEBUG("Setup Race!", 0);
    LUSLOG_DEBUG("Game Speed: %d", gTickLogic);

    gPlayerCountSelection1 = gPlayerCount;
    if (gGamestate != RACING) {
        set_mirror_mode(0);
    }
    if (gIsMirrorMode) {
        gTrackDirection = -1.0f;
    } else {
        gTrackDirection = 1.0f;
    }
    if (gModeSelection == GRAND_PRIX) {
        gCurrentCourseId = gCupCourseOrder[gCupSelection][gCourseIndexInCup];
        // Skip for debug menu
        if (gMenuSelection != START_MENU) {
            TrackBrowser_SetTrackFromCup();
        }
    }
    gActiveScreenMode = gScreenModeSelection;
    if (CM_GetTrack() != gCurrentlyLoadedTrackAddr) {
        D_80150120 = 0;
        gCurrentlyLoadedTrackAddr = CM_GetTrack();
        gNextFreeMemoryAddress = gFreeMemoryResetAnchor;
        load_track(gCurrentCourseId);
        gFreeMemoryCourseAnchor = gNextFreeMemoryAddress;
    } else {
        gNextFreeMemoryAddress = gFreeMemoryCourseAnchor;
    }

    // Cow related
    D_8015F702 = 0;
    D_8015F700 = 200;

    func_80005310();
    CM_CleanCameras();
    spawn_players_and_cameras();
    load_kart_textures();
    init_hud();
    gRaceState = RACE_INIT;
    gNumSpawnedShells = 0;
    D_800DC5B8 = 0;
    D_80152308 = 0;
    D_802BA038 = -1;
    D_802BA048 = 0;
    set_screen();
    func_802A4D18();
    func_80091FA4();
    init_actors_and_load_textures();

    // Set finishline position. This is now done in files in src/engine/tracks/*
    // if (gModeSelection != BATTLE) {
    //     D_8015F8D0[1] = (f32) (gCurrentTrackPath->posY - 15);
    //     D_8015F8D0[2] = gCurrentTrackPath->posZ;

    //     if (IsToadsTurnpike()) {
    //         D_8015F8D0[0] = (gIsMirrorMode != 0) ? D_80164490->posX + 138.0f : D_80164490->posX - 138.0f;
    //     } else if (IsWarioStadium()) {
    //         D_8015F8D0[0] = (gIsMirrorMode != 0) ? D_80164490->posX + 12.0f : D_80164490->posX - 12.0f;
    //     } else {
    //         D_8015F8D0[0] = gCurrentTrackPath->posX;
    //     }
    // }
    if (!gDemoMode) {
        //! @warning this used to be gCurrentCourseId + 4
        // Hopefully this is equivallent.
        func_800CA008(gPlayerCountSelection1 - 1, TrackBrowser_GetTrackIndex() + 4);
        func_800CB2C4();
    }

    controller = gControllerOne;

    for (i = 0; i < 7; i++, controller++) {
        controller->rawStickX = 0;
        controller->rawStickY = 0;
        controller->buttonPressed = 0;
        controller->buttonDepressed = 0;
        controller->button = 0;
    }
}

void setup_editor(void) {
    LUSLOG_DEBUG("Setup Editor!", 0);
    LUSLOG_DEBUG("Game Speed: %d", gTickLogic);

    gPlayerCountSelection1 = 1;
    if (gGamestate != RACING) {
        set_mirror_mode(0);
    }

    gActiveScreenMode = gScreenModeSelection;
    if (CM_GetTrack() != gCurrentlyLoadedTrackAddr) {
        D_80150120 = 0;
        gCurrentlyLoadedTrackAddr = CM_GetTrack();
        gNextFreeMemoryAddress = gFreeMemoryResetAnchor;
        load_track(gCurrentCourseId);
        gFreeMemoryCourseAnchor = gNextFreeMemoryAddress;
    } else {
        gNextFreeMemoryAddress = gFreeMemoryCourseAnchor;
    }

    if (gIsMirrorMode) {
        gTrackDirection = -1.0f;
    } else {
        gTrackDirection = 1.0f;
    }

    // Cow related
    D_8015F702 = 0;
    D_8015F700 = 200;

    func_80005310();
    CM_CleanCameras();
    spawn_players_and_cameras();
    load_kart_textures();
    init_hud();
    gRaceState = RACE_INIT;
    gNumSpawnedShells = 0;
    D_800DC5B8 = 0;
    D_80152308 = 0;
    D_802BA038 = -1;
    D_802BA048 = 0;
    set_editor_screen();
    func_802A4D18();
    func_80091FA4();
    init_actors_and_load_textures();
}

void func_80002DAC(void) {

    CM_SomeSounds();
}

/**
 * The nmi buffer allows developers to save 64 bytes of data through a soft reset.
 */
void clear_nmi_buffer(void) {
    s32 i;
    for (i = 0; i < 16; i++) {
        osAppNmiBuffer[i] = 0;
    }
}

/**
 * Also spawns water features
 */
void credits_spawn_actors(void) {
    Vec3f position;
    Vec3f velocity = { 0, 0, 0 };
    Vec3s rotation = { 0, 0, 0 };

    bFog = false;
    D_800DC5C8 = 0;
    gNumActors = 0;
    set_mirror_mode(0);
    gTrackDirection = 1.0f;

    gPlayerCountSelection1 = 1;

    // Stupid hack to sync segment 3 memory allocations with hard-coded address in data.
    gNextFreeMemoryAddress += 0x9000;
    destroy_all_actors();
    CM_CleanWorld();
    CM_CreditsSpawnActors();
    CM_BeginPlay();

    gNumPermanentActors = gNumActors;
}
