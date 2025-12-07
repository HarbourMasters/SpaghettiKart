#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/frappe_snowland/frappe_snowland_vertices.h"
    #include "assets/models/tracks/frappe_snowland/frappe_snowland_displaylists.h"
    #include "assets/models/tracks/frappe_snowland/frappe_snowland_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture frappe_snowland_textures[];
}

class FrappeSnowland : public Course {
public:
    virtual ~FrappeSnowland() = default;

    explicit FrappeSnowland();

    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void InitClouds() override;
    virtual void UpdateClouds(s32 sp1C, Camera* camera) override;
    virtual void InitCourseObjects() override;
    virtual void UpdateCourseObjects() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void Waypoints(Player* player, int8_t playerId) override;
};
