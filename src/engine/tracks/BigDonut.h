#pragma once

#include <libultraship.h>
#include "Track.h"

extern "C" {
    #include "assets/models/tracks/big_donut/big_donut_vertices.h"
    #include "assets/models/tracks/big_donut/big_donut_displaylists.h"
    #include "assets/models/tracks/big_donut/big_donut_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "data/path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture big_donut_textures[];
}

class BigDonut : public Track {
public:
    explicit BigDonut();

    void Load() override;
    void BeginPlay() override;
    void Draw(ScreenContext*) override;
    void DrawCredits() override;
    void Waypoints(Player* player, int8_t playerId) override;
    void Destroy() override;
};
