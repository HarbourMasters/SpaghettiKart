#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
#include "assets/frappe_snowland_vertices.h"
#include "assets/frappe_snowland_displaylists.h"
#include "assets/frappe_snowland_data.h"
#include "course_offsets.h"
#include "camera.h"
#include "data/some_data.h"
#include "objects.h"
#include "path_spawn_metadata.h"
extern const course_texture frappe_snowland_textures[];
}

class FrappeSnowland : public Course {
  public:
    virtual ~FrappeSnowland() = default; // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit FrappeSnowland();

    //    virtual void Load(const char* courseVtx,
    //                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void LoadTextures() override;
    virtual void SpawnActors() override;
    virtual void SpawnVehicles() override;
    virtual void InitClouds() override;
    virtual void UpdateClouds(s32 sp1C, Camera* camera) override;
    virtual void MinimapSettings() override;
    virtual void InitCourseObjects() override;
    virtual void UpdateCourseObjects() override;
    virtual void RenderCourseObjects(s32 cameraId) override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void MinimapFinishlinePosition() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;
    virtual void Collision() override;
    virtual void ScrollingTextures() override;
    virtual void Waypoints(Player* player, int8_t playerId) override;
    virtual void Destroy() override;
};
