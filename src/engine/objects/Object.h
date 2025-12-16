#pragma once

#include <libultraship.h>
#include "engine/SpawnParams.h"

// Editor
#include "engine/editor/EditorMath.h"

extern "C" {
    #include "camera.h"
    #include "objects.h"
}

class OObject {
public:
    uint8_t uuid[16];
    Object o;
    const char* Name = "";
    const char* ResourceName = "";
    bool bPendingDestroy = false;
    s32 _objectIndex = -1;
    const char* Model = "";

    FVector SpawnPos = {0.0f, 0.0f, 0.0f};
    IRotator SpawnRot = {0, 0, 0};
    FVector SpawnScale = {1.0f, 1.0f, 1.0f};
    float Speed = 0.0f;

    std::vector<Triangle> Triangles;
    virtual ~OObject() = default;

    explicit OObject();
    explicit OObject(SpawnParams params);

    virtual void SetSpawnParams(SpawnParams& params);
    virtual void Tick();
    virtual void Tick60fps();
    virtual void Draw(s32 cameraId);
    virtual void Expire();
    virtual void Destroy(); // Mark object for deletion at the start of the next frame
    virtual void Reset();
    FVector GetLocation() const;
    IRotator GetRotation() const;
    FVector GetScale() const;
    virtual void Translate(FVector pos);
    void Rotate(IRotator rot);
    void SetScale(FVector scale);
    virtual void DrawEditorProperties() { DrawDefaultEditorProperties(); };
};
