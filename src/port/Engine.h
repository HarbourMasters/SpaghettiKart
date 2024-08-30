#pragma once

#define LOAD_ASSET(path) \
    (path == NULL ? NULL \
                  : (GameEngine_OTRSigCheck((const char*) path) ? ResourceGetDataByName((const char*) path) : path))
#define LOAD_ASSET_RAW(path) ResourceGetDataByName((const char*) path)

#ifdef __cplusplus
#include <vector>
#include <Fast3D/gfx_pc.h>
#include "libultraship/src/Context.h"

#define SAMPLES_HIGH 544
#define SAMPLES_LOW 528
#define AUDIO_FRAMES_PER_UPDATE 2
#define NUM_AUDIO_CHANNELS 2
#define SAMPLES_PER_FRAME (SAMPLES_HIGH * NUM_AUDIO_CHANNELS * 2)

class GameEngine {
  public:
    static GameEngine* Instance;

    std::shared_ptr<Ship::Context> context;

    GameEngine();
    static void Create();
    void StartFrame() const;
    static void RunCommands(Gfx* Commands);
    void ProcessFrame(void (*run_one_game_iter)()) const;
    static void Destroy();
    static void ProcessGfxCommands(Gfx* commands);
    float OTRGetAspectRatio(void);
    float OTRGetDimensionFromLeftEdge(float v);
    float OTRGetDimensionFromRightEdge(float v);
    int16_t OTRGetRectDimensionFromLeftEdge(float v);
    int16_t OTRGetRectDimensionFromRightEdge(float v);
    uint32_t OTRGetGameRenderWidth();
    uint32_t OTRGetGameRenderHeight();
};
#else
void GameEngine_ProcessGfxCommands(Gfx* commands);
float GameEngine_GetAspectRatio();
uint8_t GameEngine_OTRSigCheck(char* imgData);
float OTRGetAspectRatio(void);
float OTRGetDimensionFromLeftEdge(float v);
float OTRGetDimensionFromRightEdge(float v);
int16_t OTRGetRectDimensionFromLeftEdge(float v);
int16_t OTRGetRectDimensionFromRightEdge(float v);
uint32_t OTRGetGameRenderWidth(void);
uint32_t OTRGetGameRenderHeight(void);
#endif
