#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/luigi_raceway_vertices.h"
    #include "assets/luigi_raceway_displaylists.h"
    #include "assets/luigi_raceway_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture luigi_raceway_textures[];
}

class LuigiRaceway : public Course {
public:
    virtual ~LuigiRaceway() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit LuigiRaceway();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void LoadTextures() override;
    virtual void SpawnActors() override;
    virtual void Init() override;
    virtual void MinimapSettings() override;
    virtual void InitCourseObjects() override;
    virtual void UpdateCourseObjects() override;
    virtual void RenderCourseObjects(s32 cameraId) override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void MinimapFinishlinePosition() override;
    virtual void SetStaffGhost() override;
    virtual void BeginPlay() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void Collision() override;
    virtual void SpawnBombKarts() override;
    virtual void GenerateCollision() override;
    virtual void Destroy() override;
};
