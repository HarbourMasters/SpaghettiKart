#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
#include "assets/big_donut_vertices.h"
#include "assets/big_donut_displaylists.h"
#include "assets/big_donut_data.h"
#include "course_offsets.h"
#include "camera.h"
#include "data/some_data.h"
#include "objects.h"
#include "path_spawn_metadata.h"
extern const course_texture big_donut_textures[];
}

class BigDonut : public Course {
  public:
    virtual ~BigDonut() = default; // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit BigDonut();

    //    virtual void Load(const char* courseVtx,
    //                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void LoadTextures() override;
    virtual void SpawnActors() override;
    virtual void SpawnVehicles() override;
    // virtual void InitClouds() override;
    virtual void MinimapSettings() override;
    virtual void InitCourseObjects() override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void MinimapFinishlinePosition() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;
    virtual void Collision() override;
    virtual void Waypoints(Player* player, int8_t playerId) override;
    virtual void Destroy() override;
};
