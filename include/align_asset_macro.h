#pragma once

uint8_t GameEngine_OTRSigCheck(char* imgData);

#if defined(_WIN32)
    #define ALIGN_ASSET(x) __declspec(align(x))
#else
    #define ALIGN_ASSET(x) __attribute__((aligned (x)))
#endif

#define LOAD_ASSET(path) (path == NULL ? NULL : (GameEngine_OTRSigCheck((const char*) path) ? ResourceGetDataByName((const char*) path) : path))
#define LOAD_ASSET_RAW(path) ResourceGetDataByName((const char*) path)
