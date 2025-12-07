#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/sherbet_land/sherbet_land_vertices.h"
    #include "assets/models/tracks/sherbet_land/sherbet_land_displaylists.h"
    #include "assets/models/tracks/sherbet_land/sherbet_land_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture sherbet_land_textures[];
}

class SherbetLand : public Course {
public:
    virtual ~SherbetLand() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit SherbetLand();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual f32 GetWaterLevel(FVector pos, Collision* collision) override;
    virtual void BeginPlay() override;
    virtual void UpdateCourseObjects() override;
    virtual void RenderCourseObjects(s32 cameraId) override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) override;
    virtual void CreditsSpawnActors() override;
};
