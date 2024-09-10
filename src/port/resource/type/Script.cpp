#include "Script.h"

namespace SF64 {
uint16_t** Script::GetPointer() {
    return mScripts.data();
}

size_t Script::GetPointerSize() {
    return sizeof(uint16_t*) * mScripts.size();
}

uint16_t* ScriptCMDs::GetPointer() {
    return mCommands.data();
}

size_t ScriptCMDs::GetPointerSize() {
    return sizeof(uint16_t) * mCommands.size();
}
} // namespace SF64
