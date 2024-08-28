#include "EnvSettings.h"

namespace SF64 {
EnvSettingsData* EnvSettings::GetPointer() {
    return &mSettings;
}

size_t EnvSettings::GetPointerSize() {
    return sizeof(EnvSettingsData);
}
} // namespace SF64
