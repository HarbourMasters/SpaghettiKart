#include "TrackBrowser.h"
#include "port/Engine.h"
#include "engine/editor/SceneManager.h"

TrackBrowser* TrackBrowser::Instance;

void TrackBrowser::FindCustomTracks() {
    auto manager = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager();

    auto ptr2 = manager->ListDirectories("tracks/*");
    if (ptr2) {
        auto dirs = *ptr2;

        for (const std::string& dir : dirs) {
            std::string name = dir.substr(dir.find_last_of('/') + 1);
            std::string sceneFile = dir + "/scene.json";
            std::string minimapFile = dir + "/minimap.png";

            // The track has a valid scene file, add it to the registry
            if (manager->HasFile(sceneFile)) {
                auto archive = manager->GetArchiveFromFile(sceneFile);

                TrackInfo info;
                info.Path = dir;
                Editor::LoadTrackInfo(info, archive, sceneFile);
                printf("Added custom track %s\n", info.Name.c_str());
                gTrackRegistry.Add(info, [info, archive]() {
                    auto track = std::make_unique<Track>();
                    track->ResourceName = info.ResourceName;
                    track->Archive = archive;
                    GetWorld()->SetCurrentTrack(std::move(track));
                });
            } else { // The track does not have a valid scene file
                const std::string file = dir + "/data_track_sections";

                // If the track has a data_track_sections file,
                // then it must at least be a valid track.
                // So lets add it as an uninitialized track.
                if (manager->HasFile(file)) {
                    TrackInfo info;

                    std::string resName = std::string("mods:") + name;
                    info.ResourceName = resName;
                    info.Name = name;
                    info.DebugName = name;

                    auto archive = manager->GetArchiveFromFile(file);
                    //mNewTracks.push_back({info, "", dir, archive});
                    auto track = std::make_unique<Track>();
                    Editor::SaveLevel(track.get()); // Write scene file so it will show up in the track browser

                    gTrackRegistry.Add(info, [info, archive]() {
                        auto track = std::make_unique<Track>();
                        track->ResourceName = info.ResourceName;
                        track->Archive = archive;
                        GetWorld()->SetCurrentTrack(std::move(track));
                    });

                } else {
                    printf("ContentBrowser.cpp: Track '%s' missing required track files. Cannot add to game\n  Missing %s/data_track_sections file\n", name.c_str(), dir.c_str());
                }
            }
        }
    }
}

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

extern "C" const char* TrackBrowser_GetMinimapTextureByIdx(size_t trackIndex) {
    return TrackBrowser::Instance->GetMinimapTextureByIdx(trackIndex);
}
