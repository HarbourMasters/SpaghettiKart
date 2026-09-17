#include "ShipCompat.h"

#include "ship/core/Context.h"
#include "ship/window/Window.h"
#include "ship/resource/ResourceManager.h"
#include "ship/controller/controldeck/ControlDeck.h"
#include "ship/config/Config.h"
#include "ship/audio/Audio.h"
#include "ship/security/Keystore.h"
#include "ship/window/FileDrop.h"
#include "ship/config/ConsoleVariable.h"
#include "ship/debug/Console.h"
#include "ship/log/Logger.h"
#include "fast/debug/GfxDebugger.h"
#ifdef ENABLE_SCRIPTING
#include "ship/scripting/ScriptLoader.h"
#endif

namespace ShipCompat {

static std::shared_ptr<Ship::Context> sContext;

void SetContext(std::shared_ptr<Ship::Context> context) {
    sContext = std::move(context);
}

std::shared_ptr<Ship::Context> GetContext() {
    return sContext;
}

template <typename T> static std::shared_ptr<T> Get() {
    return sContext ? sContext->GetChildren().GetFirst<T>() : nullptr;
}

std::shared_ptr<Ship::Window> GetWindow() {
    return Get<Ship::Window>();
}
std::shared_ptr<Ship::ResourceManager> GetResourceManager() {
    return Get<Ship::ResourceManager>();
}
std::shared_ptr<Ship::ControlDeck> GetControlDeck() {
    return Get<Ship::ControlDeck>();
}
std::shared_ptr<Ship::Config> GetConfig() {
    return Get<Ship::Config>();
}
std::shared_ptr<Ship::Audio> GetAudio() {
    return Get<Ship::Audio>();
}
std::shared_ptr<Ship::Keystore> GetKeystore() {
    return Get<Ship::Keystore>();
}
std::shared_ptr<Ship::FileDrop> GetFileDropMgr() {
    return Get<Ship::FileDrop>();
}
std::shared_ptr<Ship::ConsoleVariable> GetConsoleVariables() {
    return Get<Ship::ConsoleVariable>();
}
std::shared_ptr<Ship::Console> GetConsole() {
    return Get<Ship::Console>();
}
std::shared_ptr<Fast::GfxDebugger> GetGfxDebugger() {
    return Get<Fast::GfxDebugger>();
}

#ifdef ENABLE_SCRIPTING
std::shared_ptr<Ship::ScriptLoader> GetScriptLoader() {
    return Get<Ship::ScriptLoader>();
}
#endif

} // namespace ShipCompat
