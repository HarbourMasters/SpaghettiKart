#pragma once

#include "port/audio/HMAS.h"

static const char game_asset_file[] = "mk64.o2r";
static const char engine_asset_file[] = "spaghetti.o2r";

#define LOAD_ASSET(path) \
(path == NULL ? NULL \
  : (GameEngine_OTRSigCheck((const char*) path) ? ResourceGetDataByName((const char*) path) : path))
#define LOAD_ASSET_RAW(path) ResourceGetDataByName((const char*) path)

#ifdef __cplusplus
#include <vector>
#include <SDL2/SDL.h>
#include <fast/Fast3dWindow.h>
#include <fast/interpreter.h>
#include "ship/Context.h"
#include <unordered_map>

#ifndef IDYES
#define IDYES 6
#endif
#ifndef IDNO
#define IDNO 7
#endif

#define SAMPLES_HIGH 448
#define SAMPLES_LOW 432
#define AUDIO_FRAMES_PER_UPDATE 2
#define NUM_AUDIO_CHANNELS 2
#define SAMPLES_PER_FRAME (SAMPLES_HIGH * NUM_AUDIO_CHANNELS * 2)

Fast::Interpreter* GetInterpreter();

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
    std::vector<std::string> archiveFiles;

    ImFont* fontStandard;
    ImFont* fontStandardLarger;
    ImFont* fontStandardLargest;
    ImFont* fontMono;
    ImFont* fontMonoLarger;
    ImFont* fontMonoLargest;

    HMAS* gHMAS;

    std::unordered_map<std::string, uint8_t> bankMapTable;
    GameEngine();
    static bool GenAssetFile();
    static void Create();

    void AudioInit();
    static void HandleAudioThread();
    static void StartAudioFrame();
    static void EndAudioFrame();
    static void AudioExit();



    static uint32_t GetInterpolationFPS();
    static uint32_t GetInterpolationFrameCount();
    void StartFrame() const;
    static void RunCommands(Gfx* pool, const std::vector<std::unordered_map<Mtx*, MtxF>>& mtx_replacements);
    void ProcessFrame(void (*run_one_game_iter)()) const;
    static void Destroy();
    static void ProcessGfxCommands(Gfx* pool);
    static uint8_t GetBankIdByName(const std::string& name);
    static int ShowYesNoBox(const char* title, const char* box);
    static void ShowMessage(const char* title, const char* message, SDL_MessageBoxFlags type = SDL_MESSAGEBOX_ERROR);
    float OTRGetAspectRatio(void);
    float OTRGetDimensionFromLeftEdge(float v);
    float OTRGetDimensionFromRightEdge(float v);
    int16_t OTRGetRectDimensionFromLeftEdge(float v);
    int16_t OTRGetRectDimensionFromRightEdge(float v);
    uint32_t OTRGetGameRenderWidth();
    uint32_t OTRGetGameRenderHeight();
    uint32_t OTRGetGameViewportWidth();
    uint32_t OTRGetGameViewportHeight();
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
uint32_t OTRGetGameViewportWidth();
uint32_t OTRGetGameViewportHeight();
uint32_t OTRCalculateCenterOfAreaFromRightEdge(int32_t center);
uint32_t OTRCalculateCenterOfAreaFromLeftEdge(int32_t center);
int32_t GameEngine_ResourceGetTexTypeByName(const char* name);
#ifdef __cplusplus
}
#endif

