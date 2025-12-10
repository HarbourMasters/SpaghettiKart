#include "TrackBrowser.h"

TrackBrowser* TrackBrowser::Instance;

extern "C" void TrackBrowser_SetTrack(const char* name) {
    TrackBrowser::Instance->SetTrack(std::string(name));
}

extern "C" void TrackBrowser_SetTrackFromCup() {
    TrackBrowser::Instance->SetTrack(GetWorld()->GetCurrentCup()->GetTrack());
}

extern "C" void TrackBrowser_NextTrack(void) {
    TrackBrowser::Instance->NextTrack();
}

extern "C" void TrackBrowser_PreviousTrack(void) {
    TrackBrowser::Instance->PreviousTrack();
}

extern "C" size_t TrackBrowser_GetTrackIndex(void) {
    return TrackBrowser::Instance->GetTrackIndex();
}

extern "C" const char* TrackBrowser_GetTrackName(void) {
    return TrackBrowser::Instance->GetTrackName();
}

extern "C" const char* TrackBrowser_GetTrackDebugName(void) {
    return TrackBrowser::Instance->GetTrackDebugName();

}

extern "C" const char* TrackBrowser_GetTrackLength(void) {
    return TrackBrowser::Instance->GetTrackLength();
}

extern "C" void TrackBrowser_SetTrackByIdx(size_t trackIndex) {
    TrackBrowser::Instance->SetTrackByIdx(trackIndex);
}

extern "C" const char* TrackBrowser_GetTrackNameByIdx(size_t trackIndex) {
    return TrackBrowser::Instance->GetTrackNameByIdx(trackIndex);
}

extern "C" const char* TrackBrowser_GetTrackDebugNameByIdx(size_t trackIndex) {
    return TrackBrowser::Instance->GetTrackDebugNameByIdx(trackIndex);
}

extern "C" const char* TrackBrowser_GetTrackLengthByIdx(size_t trackIndex) {
    return TrackBrowser::Instance->GetTrackLengthByIdx(trackIndex);
}
