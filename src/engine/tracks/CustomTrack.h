#include <libultraship/libultraship.h>
#include "Track.h"

class CustomTrack : public Track {
public:
    explicit CustomTrack();

    virtual void Load() override;
    virtual void BeginPlay() override;
    virtual void Draw(ScreenContext*) override;
    virtual void DrawTransparency(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) override;
    virtual void Tick() override;
    virtual bool IsMod() override { return true; }
    void InvertTriangleWindingModdedByName(const char* name);
    void ParseMeshForCollision(TrackSections* sections, size_t numMesh);
    void TestPath();

protected:

enum class SurfaceClip : uint16_t {
    CLIP_NONE,
    CLIP_DEFAULT,
    CLIP_SINGLE_SIDED_WALL,
    CLIP_SURFACE,
    CLIP_DOUBLE_SIDED_WALL
};

enum class DrawLayer : uint16_t {
    DRAW_INVISIBLE,
    DRAW_OPAQUE,
    DRAW_TRANSLUCENT,
    DRAW_TRANSLUCENT_NO_ZBUFFER
};
    // Containers that hold meshes that get drawn
    std::vector<uint64_t> mOpaqueItems;
    std::vector<TrackSections> mTranslucentItems;
    std::vector<TrackSections> mTranslucentNoZBufferItems;
};
