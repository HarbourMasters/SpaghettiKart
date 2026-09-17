#pragma once

#include <memory>

namespace Fast {
class GfxDebugger;
}

namespace Ship {
class Context;
class Window;
class ResourceManager;
class ControlDeck;
class Config;
class Audio;
class Keystore;
class FileDrop;
class ConsoleVariable;
class Console;
class ScriptLoader;
} // namespace Ship

namespace ShipCompat {

void SetContext(std::shared_ptr<Ship::Context> context);
std::shared_ptr<Ship::Context> GetContext();

std::shared_ptr<Ship::Window> GetWindow();
std::shared_ptr<Ship::ResourceManager> GetResourceManager();
std::shared_ptr<Ship::ControlDeck> GetControlDeck();
std::shared_ptr<Ship::Config> GetConfig();
std::shared_ptr<Ship::Audio> GetAudio();
std::shared_ptr<Ship::Keystore> GetKeystore();
std::shared_ptr<Ship::FileDrop> GetFileDropMgr();
std::shared_ptr<Ship::ConsoleVariable> GetConsoleVariables();
std::shared_ptr<Ship::Console> GetConsole();
std::shared_ptr<Fast::GfxDebugger> GetGfxDebugger();
#ifdef ENABLE_SCRIPTING
std::shared_ptr<Ship::ScriptLoader> GetScriptLoader();
#endif

} // namespace ShipCompat
