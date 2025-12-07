#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/big_donut/big_donut_vertices.h"
    #include "assets/models/tracks/big_donut/big_donut_displaylists.h"
    #include "assets/models/tracks/big_donut/big_donut_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture big_donut_textures[];
}

class BigDonut : public Course {
public:
    virtual ~BigDonut() = default;

    explicit BigDonut();

    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void Waypoints(Player* player, int8_t playerId) override;
    virtual void Destroy() override;
};
