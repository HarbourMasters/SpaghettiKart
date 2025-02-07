#pragma once

#define LOAD_ASSET(path) \
    (path == NULL ? NULL \
                  : (GameEngine_OTRSigCheck((const char*) path) ? ResourceGetDataByName((const char*) path) : path))
#define LOAD_ASSET_RAW(path) ResourceGetDataByName((const char*) path)

#ifdef __cplusplus
#include <vector>
#include <Fast3D/gfx_pc.h>
#include "libultraship/src/Context.h"

#define SAMPLES_HIGH 448
#define SAMPLES_LOW 432
#define AUDIO_FRAMES_PER_UPDATE 2
#define NUM_AUDIO_CHANNELS 2
#define SAMPLES_PER_FRAME (SAMPLES_HIGH * NUM_AUDIO_CHANNELS * 2)

struct CtlEntry;
struct AudioBankSample;
struct AudioSequenceData;

class GameEngine {
  public:
    static GameEngine* Instance;

    std::shared_ptr<Ship::Context> context;
    std::vector<CtlEntry*> banksTable;
    std::vector<std::string> sequenceTable;
    std::vector<AudioSequenceData*> audioSequenceTable;

    ImFont* fontStandard;
    ImFont* fontStandardLarger;
    ImFont* fontStandardLargest;
    ImFont* fontMono;
    ImFont* fontMonoLarger;
    ImFont* fontMonoLargest;

    std::unordered_map<std::string, uint8_t> bankMapTable;
    GameEngine();
    static void Create();

    void AudioInit();
    static void HandleAudioThread();
    static void StartAudioFrame();
    static void EndAudioFrame();
    static void AudioExit();

    void StartFrame() const;
    static void RunCommands(Gfx* Commands);
    void ProcessFrame(void (*run_one_game_iter)()) const;
    static void Destroy();
    static void ProcessGfxCommands(Gfx* commands);
    static uint8_t GetBankIdByName(const std::string& name);
    float OTRGetAspectRatio(void);
    float OTRGetDimensionFromLeftEdge(float v);
    float OTRGetDimensionFromRightEdge(float v);
    int16_t OTRGetRectDimensionFromLeftEdge(float v);
    int16_t OTRGetRectDimensionFromRightEdge(float v);
    uint32_t OTRGetGameRenderWidth();
    uint32_t OTRGetGameRenderHeight();
    uint32_t OTRCalculateCenterOfAreaFromRightEdge(int32_t center);
    uint32_t OTRCalculateCenterOfAreaFromLeftEdge(int32_t center);
  private:
    ImFont* CreateFontWithSize(float size, std::string fontPath = "");
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
void GameEngine_ProcessGfxCommands(Gfx* commands);
uint32_t GameEngine_GetSampleRate();
uint32_t GameEngine_GetSamplesPerFrame();
float GameEngine_GetAspectRatio();
struct CtlEntry* GameEngine_LoadBank(uint8_t bankId);
uint8_t GameEngine_IsBankLoaded(uint8_t bankId);
void GameEngine_UnloadBank(uint8_t bankId);
struct AudioSequenceData* GameEngine_LoadSequence(uint8_t seqId);
uint32_t GameEngine_GetSequenceCount();
uint8_t GameEngine_IsSequenceLoaded(uint8_t seqId);
void GameEngine_UnloadSequence(uint8_t seqId);
// bool GameEngine_OTRSigCheck(char* imgData); -> align_asset_macro.h
float OTRGetAspectRatio(void);
float OTRGetDimensionFromLeftEdge(float v);
float OTRGetDimensionFromRightEdge(float v);
int16_t OTRGetRectDimensionFromLeftEdge(float v);
int16_t OTRGetRectDimensionFromRightEdge(float v);
uint32_t OTRGetGameRenderWidth(void);
uint32_t OTRGetGameRenderHeight(void);
uint32_t OTRCalculateCenterOfAreaFromRightEdge(int32_t center);
uint32_t OTRCalculateCenterOfAreaFromLeftEdge(int32_t center);
#ifdef __cplusplus
}
#endif

