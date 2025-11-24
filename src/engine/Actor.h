#pragma once

#include <libultraship.h>
#include "engine/SpawnParams.h"
#include "engine/editor/EditorMath.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "camera.h"
#include "common_structs.h"
}

class AActor {
public:
    /* 0x00 */ s16 Type = 0;
    /* 0x02 */ s16 Flags;
    /* 0x04 */ s16 Unk_04;
    /* 0x06 */ s16 State;
    /* 0x08 */ f32 Unk_08;
    /* 0x0C */ f32 BoundingBoxSize;
    /* 0x10 */ Vec3s Rot = {0, 0, 0};
    /* 0x16 */ s16 Unk_16;
    /* 0x18 */ Vec3f Pos;
    /* 0x24 */ Vec3f Velocity = {0, 0, 0};
    /* 0x30 */ Collision Unk30;
    /* 0x   */ const char* Model = "";
    uint8_t uuid[16];
    const char* Name = "";
    const char* ResourceName = "";
    FVector SpawnPos = {0.0f, 0.0f, 0.0f};
    IRotator SpawnRot = {0, 0, 0};
    FVector SpawnScale = {1.0f, 1.0f, 1.0f};

    FVector Scale = {1, 1, 1};
    float Speed = 0.0f;
    std::vector<Triangle> Triangles;

    bool bPendingDestroy = false;

    virtual ~AActor() = default;  // Virtual destructor for proper cleanup in derived classes

    explicit AActor();
    explicit AActor(SpawnParams params);

    /**
     * Make sure you call this in derived classes!
     * Usage:
     * MyActor::SetSpawnParams(SetSpawnParams& params) {
     *     AActor::SetSpawnParams(params); // Calls default implementation
     * }
     */
    virtual void SetSpawnParams(SpawnParams& params);
    virtual void BeginPlay();
    virtual void Tick();
    virtual void Draw(Camera* camera);
    virtual void Collision(Player* player, AActor* actor);
    virtual void VehicleCollision(s32 playerId, Player* player);
    void SetLocation(FVector pos);

    virtual void Destroy();
    virtual bool IsMod();

    /** Editor functions **/
    FVector GetLocation() const;
    IRotator GetRotation() const;
    FVector GetScale() const;
    void Translate(FVector pos);
    void Rotate(IRotator rot);
    void SetScale(FVector scale);
    virtual void DrawEditorProperties() { DrawDefaultEditorProperties(); };
};
