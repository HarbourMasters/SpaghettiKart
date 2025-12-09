#include <libultraship.h>
#include <string>

class TrackInfo {
public:
    std::string mResourceName;
    const char* GetName() { return mName; }
    const char* GetDebugName() { return mDebugName; }
    const char* GetLength() { return mLength; }

private:
    char mName[128];
    char mDebugName[128];
    char mLength[128]; // The length of the track in meters. Really just a random value set by the track maker.

    void SetText(char* name, const char* title, size_t bufferSize) {
        // Copy the title into the name buffer, ensuring it's null-terminated and within bounds
        std::strncpy(name, title, bufferSize - 1);
        name[bufferSize - 1] = '\0';  // Ensure the string is null-terminated
    }

}
