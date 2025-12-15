#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include <libultra/types.h>
#include "../CoreMath.h"
#include "EditorMath.h"
#include <vector>
#include "engine/SpawnParams.h"

#include "src/port/ui/DefaultProperties.h"

extern "C" {
#include "common_structs.h"
}

struct Triangle;

namespace TrackEditor {
    class GameObject {
public:
        enum class CollisionType {
            VTX_INTERSECT,
            BOUNDING_BOX,
            BOUNDING_SPHERE
        };

        GameObject(FVector pos, IRotator rot, FVector scale, const char* model, std::vector<Triangle> triangles, CollisionType collision, float boundingBoxSize);
        GameObject();
        virtual void Tick();
        virtual void Draw();
        virtual void Load() {};
        FVector GetLocation() const;
        IRotator GetRotation() const;
        FVector GetScale() const;
        void Translate(FVector pos);
        void Rotate(IRotator rot);
        void SetScale(FVector scale);
        void Destroy() {};

        const char* Name;
        const char* ResourceName;
        FVector SpawnPos = {0.0f, 0.0f, 0.0f};
        IRotator SpawnRot = {0, 0, 0};
        FVector SpawnScale = {1.0f, 1.0f, 1.0f};
        float Speed;
        FVector Pos;
        IRotator Rot;
        FVector Scale;
        const char* Model = "";
        std::vector<Triangle> Triangles;
        CollisionType Collision;
        float BoundingBoxSize;
        virtual void DrawEditorProperties() { DrawDefaultEditorProperties(); };
    };
}
