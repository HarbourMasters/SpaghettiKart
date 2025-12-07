#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/block_fort/block_fort_vertices.h"
    #include "assets/models/tracks/block_fort/block_fort_displaylists.h"
    #include "assets/models/tracks/block_fort/block_fort_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture block_fort_textures[];
}

class BlockFort : public Course {
public:
    virtual ~BlockFort() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit BlockFort();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void Waypoints(Player*, int8_t) override;
};
