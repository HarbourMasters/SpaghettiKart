#include <libultraship/libultraship.h>
#include "GameObject.h"

namespace TrackEditor {

    GameObject::GameObject(FVector pos, IRotator rot, FVector scale, const char* model, std::vector<Triangle> triangles, CollisionType collision, float boundingBoxSize) {
        Pos = pos;
        Rot = rot;
        Scale = scale;
        Model = model;
        Triangles = triangles;
        Collision = collision;
        BoundingBoxSize = boundingBoxSize;
    }

    GameObject::GameObject() {};

    void GameObject::Draw(){};

    void GameObject::Tick(){};

    FVector GameObject::GetLocation() const {
        return Pos;
    };
    IRotator GameObject::GetRotation() const {
        return Rot;
    }
    FVector GameObject::GetScale() const {
        return Scale;
    }
    void GameObject::Translate(FVector pos) {
        Pos = pos;
    };
    void GameObject::Rotate(IRotator rot) {
        Rot = rot;
    };
    void GameObject::SetScale(FVector scale) {
        Scale = scale;
    };

} // namespace TrackEditor
