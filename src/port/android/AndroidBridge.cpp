/**
 * JNI surface for the Android app.
 *
 * Two unrelated things live here because they are the only places the Java/Kotlin
 * side reaches into native code:
 *
 *  - the on-screen controller, which is fed into SDL as a virtual joystick, and
 *  - game asset generation, which runs Torch against the user's ROM.
 *
 * The asset generation entry point is deliberately independent of libultraship:
 * it is called from the launcher activity, before SDL exists, so it cannot use
 * Ship::Context to discover paths. The launcher passes them in explicitly.
 */
#ifdef __ANDROID__

#include <jni.h>
#include <android/log.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "Companion.h"
#include "ship/Context.h"
#include "ship/window/Window.h"
#include "ship/window/gui/Gui.h"

namespace {

constexpr const char* kLogTag = "SpaghettiKart";

std::string ToStdString(JNIEnv* env, jstring value) {
    if (value == nullptr) {
        return {};
    }
    const char* chars = env->GetStringUTFChars(value, nullptr);
    std::string result = chars != nullptr ? chars : "";
    if (chars != nullptr) {
        env->ReleaseStringUTFChars(value, chars);
    }
    return result;
}

// The on-screen controller is exposed to the game as an ordinary SDL game
// controller so it flows through libultraship's normal input mapping.
SDL_Joystick* gVirtualJoystick = nullptr;
int gVirtualJoystickId = -1;

} // namespace

extern "C" {

JNIEXPORT void JNICALL Java_com_izzy_kart_MainActivity_attachController(JNIEnv*, jobject) {
    if (gVirtualJoystick != nullptr) {
        return;
    }

    gVirtualJoystickId = SDL_JoystickAttachVirtual(SDL_JOYSTICK_TYPE_GAMECONTROLLER, 6, 18, 0);
    if (gVirtualJoystickId == -1) {
        __android_log_print(ANDROID_LOG_ERROR, kLogTag, "Could not attach the on-screen controller: %s",
                            SDL_GetError());
        return;
    }

    gVirtualJoystick = SDL_JoystickOpen(gVirtualJoystickId);
    if (gVirtualJoystick == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, kLogTag, "Could not open the on-screen controller: %s", SDL_GetError());
        SDL_JoystickDetachVirtual(gVirtualJoystickId);
        gVirtualJoystickId = -1;
    }
}

JNIEXPORT void JNICALL Java_com_izzy_kart_MainActivity_detachController(JNIEnv*, jobject) {
    if (gVirtualJoystick == nullptr) {
        return;
    }

    SDL_JoystickClose(gVirtualJoystick);
    SDL_JoystickDetachVirtual(gVirtualJoystickId);
    gVirtualJoystick = nullptr;
    gVirtualJoystickId = -1;
}

JNIEXPORT void JNICALL Java_com_izzy_kart_MainActivity_setButton(JNIEnv*, jobject, jint button, jboolean value) {
    if (gVirtualJoystick == nullptr) {
        return;
    }

    // Negative ids address an axis instead, so the triggers can be driven by a
    // plain on/off button in the overlay.
    if (button < 0) {
        SDL_JoystickSetVirtualAxis(gVirtualJoystick, -button, value ? SDL_MAX_SINT16 : -SDL_MAX_SINT16);
    } else {
        SDL_JoystickSetVirtualButton(gVirtualJoystick, button, value);
    }
}

/**
 * Whether libultraship's menu is currently up.
 *
 * The overlay needs this because the menu is not only opened by its own Menu
 * button — a keyboard, a gamepad, or the menu closing itself all change it
 * behind the overlay's back. Reading the engine's state instead of mirroring it
 * in Java is what keeps the two from drifting apart.
 *
 * Called from the UI thread while the game thread renders. It reads a bool the
 * game thread may be writing, which is worth accepting here: the value only
 * drives which overlay buttons are shown, and a torn read self-corrects on the
 * next poll.
 */
JNIEXPORT jboolean JNICALL Java_com_izzy_kart_MainActivity_isMenuOpen(JNIEnv*, jobject) {
    auto* context = Ship::Context::GetRawInstance();
    if (context == nullptr) {
        return JNI_FALSE;
    }

    auto window = context->GetWindow();
    if (window == nullptr) {
        return JNI_FALSE;
    }

    auto gui = window->GetGui();
    if (gui == nullptr) {
        return JNI_FALSE;
    }

    return gui->GetMenuOrMenubarVisible() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_com_izzy_kart_MainActivity_setAxis(JNIEnv*, jobject, jint axis, jshort value) {
    if (gVirtualJoystick == nullptr) {
        return;
    }

    SDL_JoystickSetVirtualAxis(gVirtualJoystick, axis, value);
}

/**
 * Runs Torch over the ROM at romPath and writes mk64.o2r into destDir.
 *
 * sourceDir is the directory holding config.yml and yamls/ (unpacked from the
 * APK by the launcher). Returns null on success, or a message describing what
 * went wrong.
 */
JNIEXPORT jstring JNICALL Java_com_izzy_kart_GameAssets_nativeGenerateGameArchive(JNIEnv* env, jobject,
                                                                                 jstring jRomPath, jstring jSourceDir,
                                                                                 jstring jDestDir) {
    const std::string romPath = ToStdString(env, jRomPath);
    const std::string sourceDir = ToStdString(env, jSourceDir);
    const std::string destDir = ToStdString(env, jDestDir);

    const auto fail = [env](const std::string& message) {
        __android_log_print(ANDROID_LOG_ERROR, kLogTag, "Asset generation failed: %s", message.c_str());
        return env->NewStringUTF(message.c_str());
    };

    std::ifstream rom(romPath, std::ios::binary);
    if (!rom.is_open()) {
        return fail("Could not open the ROM at " + romPath);
    }

    std::vector<uint8_t> romData((std::istreambuf_iterator<char>(rom)), std::istreambuf_iterator<char>());
    rom.close();

    if (romData.empty()) {
        return fail("The ROM at " + romPath + " is empty.");
    }

    __android_log_print(ANDROID_LOG_INFO, kLogTag, "Extracting %zu bytes of ROM data into %s", romData.size(),
                        destDir.c_str());

    // Companion registers itself as a singleton and is read back through that
    // pointer all over Torch, so it has to be assigned before Init runs.
    delete Companion::Instance;
    Companion::Instance = new Companion(std::move(romData), ArchiveType::O2R, false, sourceDir, destDir);
    Companion::Instance->SetAdditionalFiles({ "meta/mods.toml" });

    std::string extractError;
    try {
        Companion::Instance->Init(ExportType::Binary);
    } catch (const std::exception& error) {
        extractError = std::string("Torch could not extract the ROM: ") + error.what();
    } catch (...) {
        extractError = "Torch could not extract the ROM.";
    }

    // The game runs in this same process, and Companion is still holding the
    // ROM plus every decoded asset. Hand that memory back before it has to
    // compete with the game for it — including when the extraction threw, since
    // the launcher stays alive to let the user try again.
    delete Companion::Instance;
    Companion::Instance = nullptr;

    if (!extractError.empty()) {
        return fail(extractError);
    }

    // Process() logs and returns rather than throwing for a few failure modes
    // (missing config, unrecognised ROM), so confirm the archive really landed.
    std::error_code error;
    const std::filesystem::path archive = std::filesystem::path(destDir) / "mk64.o2r";
    if (!std::filesystem::exists(archive, error) || std::filesystem::file_size(archive, error) == 0) {
        return fail("Torch finished without producing mk64.o2r. Check that the ROM is Mario Kart 64 (US).");
    }

    __android_log_print(ANDROID_LOG_INFO, kLogTag, "Wrote %s", archive.c_str());
    return nullptr;
}

} // extern "C"

#endif // __ANDROID__
