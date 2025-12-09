#pragma once

#include <libultraship.h>
#include "Track.h"

extern "C" {
    #include "assets/models/tracks/frappe_snowland/frappe_snowland_vertices.h"
    #include "assets/models/tracks/frappe_snowland/frappe_snowland_displaylists.h"
    #include "assets/models/tracks/frappe_snowland/frappe_snowland_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture frappe_snowland_textures[];
}

class FrappeSnowland : public Track {
public:
    explicit FrappeSnowland();

    virtual void Load() override;
    virtual void BeginPlay() override;
    virtual void InitClouds() override;
    virtual void TickClouds(s32 sp1C, Camera* camera) override;
    virtual void InitTrackObjects() override;
    virtual void TickTrackObjects() override;
    virtual void Draw(ScreenContext*) override;
    virtual void DrawCredits() override;    
    virtual void Waypoints(Player* player, int8_t playerId) override;
};
