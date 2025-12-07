#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/yoshi_valley/yoshi_valley_vertices.h"
    #include "assets/models/tracks/yoshi_valley/yoshi_valley_displaylists.h"
    #include "assets/models/tracks/yoshi_valley/yoshi_valley_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture yoshi_valley_textures[];
}

class YoshiValley : public Course {
public:
    virtual ~YoshiValley() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit YoshiValley();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void InitCourseObjects() override;
    virtual void UpdateCourseObjects() override;
    virtual void RenderCourseObjects(s32 cameraId) override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void ScrollingTextures() override;
    virtual void Waypoints(Player* player, int8_t playerId) override;
    virtual void CreditsSpawnActors() override;
    virtual void Destroy() override;
};
