#pragma once

#include <libultraship.h>
#include "Track.h"

extern "C" {
    #include "assets/models/tracks/sherbet_land/sherbet_land_vertices.h"
    #include "assets/models/tracks/sherbet_land/sherbet_land_displaylists.h"
    #include "assets/models/tracks/sherbet_land/sherbet_land_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "data/path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture sherbet_land_textures[];
}

class SherbetLand : public Track {
public:
    // Constructor
    explicit SherbetLand();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    void Load() override;
    f32 GetWaterLevel(FVector pos, struct Collision* collision) override;
    void BeginPlay() override;
    void TickTrackObjects() override;
    void DrawTrackObjects(s32 cameraId) override;
    void Draw(ScreenContext*) override;
    void DrawCredits() override;
    void DrawTransparency(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot,
                          uint16_t playerDirection) override;
    void CreditsSpawnActors() override;
};
