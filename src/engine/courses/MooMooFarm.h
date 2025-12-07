#pragma once

#include <libultraship.h>
#include "Course.h"
#include "engine/objects/Mole.h"

extern "C" {
    #include "assets/models/tracks/moo_moo_farm/moo_moo_farm_vertices.h"
    #include "assets/models/tracks/moo_moo_farm/moo_moo_farm_displaylists.h"
    #include "assets/models/tracks/moo_moo_farm/moo_moo_farm_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture moo_moo_farm_textures[];
}

class OMole;

class MooMooFarm : public Course {
public:
    virtual ~MooMooFarm() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit MooMooFarm();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void CreditsSpawnActors() override;
    virtual void Destroy() override;
};
