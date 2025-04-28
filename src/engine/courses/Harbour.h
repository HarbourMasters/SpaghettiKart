#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "mario_raceway_vertices.h"
    #include "mario_raceway_displaylists.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    #include "mario_raceway_data.h"
    extern const course_texture test_course_textures[];
}

class Harbour : public Course {
public:
    virtual ~Harbour() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit Harbour();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void LoadTextures() override;
    virtual void BeginPlay() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual bool IsMod() override;
};
