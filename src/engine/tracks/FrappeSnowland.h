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
    #include "data/path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture frappe_snowland_textures[];
}

class FrappeSnowland : public Track {
public:
    explicit FrappeSnowland();

    void Load() override;
    void BeginPlay() override;
    void InitTrackObjects() override;
    void TickTrackObjects() override;
    void Draw(ScreenContext*) override;
    void DrawCredits() override;
    void Waypoints(Player* player, int8_t playerId) override;
};
