#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/mario_raceway/mario_raceway_vertices.h"
    #include "assets/models/tracks/mario_raceway/mario_raceway_displaylists.h"
    #include "assets/models/tracks/mario_raceway/mario_raceway_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
}

class MarioRaceway : public Course {
public:
    virtual ~MarioRaceway() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit MarioRaceway();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void InitCourseObjects() override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void SetStaffGhost() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void CreditsSpawnActors() override;
    virtual void Destroy() override;
};
