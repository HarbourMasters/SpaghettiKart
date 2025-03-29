#include <libultraship/libultraship.h>
#include "GameObject.h"

namespace Editor {

    GameObject::GameObject(const char* name, FVector* pos, IRotator* rot, FVector* scale, Gfx* model, std::vector<Triangle> triangles, CollisionType collision, float boundingBoxSize, int32_t* despawnFlag, int32_t despawnValue) {
        Name = name;
        Pos = pos;
        Rot = rot;
        Scale = scale;
        Model = model;
        Triangles = triangles;
        Collision = collision;
        BoundingBoxSize = boundingBoxSize;
        DespawnFlag = despawnFlag;
        DespawnValue = despawnValue;
    }

    GameObject::GameObject(FVector* pos, Vec3s* rot) {
        //Pos = pos;
        //Rot = rot;
    }

    GameObject::GameObject() {};

    void GameObject::Draw(){};

    void GameObject::Tick(){};

} // namespace Editor
