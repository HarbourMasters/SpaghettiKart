#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/toads_turnpike/toads_turnpike_vertices.h"
    #include "assets/models/tracks/toads_turnpike/toads_turnpike_displaylists.h"
    #include "assets/models/tracks/toads_turnpike/toads_turnpike_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture toads_turnpike_textures[];
}

class ToadsTurnpike : public Course {
public:
    virtual ~ToadsTurnpike() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit ToadsTurnpike();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void InitClouds() override;
    virtual void UpdateClouds(s32, Camera*) override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void Destroy() override;
private:
    size_t _numTrucks = 7;
    size_t _numBuses = 7;
    size_t _numTankerTrucks = 7;
    size_t _numCars = 7;
};
