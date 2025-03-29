#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include <libultra/types.h>
#include "../CoreMath.h"
#include "EditorMath.h"
#include <vector>

extern "C" {
#include "common_structs.h"
}

struct Triangle;

namespace Editor {
    class GameObject {
public:
        enum class CollisionType {
            VTX_INTERSECT,
            BOUNDING_BOX,
            BOUNDING_SPHERE
        };

        GameObject(const char* name, FVector* pos, IRotator* rot, FVector* scale, Gfx* model, std::vector<Triangle> triangles, CollisionType collision, float boundingBoxSize, int32_t* despawnFlag, int32_t despawnValue);
        GameObject(FVector* pos, Vec3s* rot);
        GameObject();
        virtual void Tick();
        virtual void Draw();
        virtual void Load() {};

        const char* Name;
        FVector* Pos;
        IRotator* Rot;
        FVector* Scale;
        Gfx* Model;
        std::vector<Triangle> Triangles;
        CollisionType Collision;
        float BoundingBoxSize;
        int32_t* DespawnFlag;
        int32_t DespawnValue;

    };
}
