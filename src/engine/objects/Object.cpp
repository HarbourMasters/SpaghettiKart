#include <libultraship.h>
#include "Object.h"

#include "World.h"

extern "C" {
    #include "camera.h"
}


    //GameActor()

OObject::OObject() {
}
OObject::OObject(SpawnParams params) {
    ResourceName = "mk:object"; // This needs to be overridden in derived classes
    SpawnPos = params.Location.value_or(FVector{0.0f, 0.0f, 0.0f});
    SpawnRot = params.Rotation.value_or(IRotator{0, 0, 0});
    SpawnScale = params.Scale.value_or(FVector(0, 0, 0));
    Speed = params.Speed.value_or(0.0f);
}

void OObject::SetSpawnParams(SpawnParams& params) {
    params.Name = ResourceName;
    params.Location = SpawnPos;
    params.Rotation = SpawnRot;
    params.Scale = SpawnScale;
    params.Speed = Speed;
}

// Virtual functions to be overridden by derived classes
void OObject::Tick() { }
void OObject::Tick60fps() {}
void OObject::Draw(s32 cameraId) { }
void OObject::Expire() { }
void OObject::Destroy() {
    bPendingDestroy = true;
}
void OObject::Reset() { }

FVector OObject::GetLocation() const {
    if (_objectIndex != -1) {
        Object* object = &gObjectList[_objectIndex];
        return FVector(object->pos[0], object->pos[1], object->pos[2]);
    }
    printf("Editor tried to get null OObject\n");
    return FVector(0, 0, 0);
};

IRotator OObject::GetRotation() const {
    if (_objectIndex != -1) {
        Object* object = &gObjectList[_objectIndex];
        return IRotator(object->orientation[0], object->orientation[1], object->orientation[2]);
    }
    printf("Editor tried to get null OObject\n");
    return IRotator(0, 0, 0);
}

FVector OObject::GetScale() const {
    if (_objectIndex != -1) {
        Object* object = &gObjectList[_objectIndex];
        return FVector(object->sizeScaling, object->sizeScaling, object->sizeScaling);
    }
    printf("Editor tried to get null OObject\n");
    return FVector(0, 0, 0);
}

void OObject::Translate(FVector pos) {
    if (_objectIndex != -1) {
        SpawnPos = pos;

        Object* object = &gObjectList[_objectIndex];

        object->pos[0] = pos.x;
        object->pos[1] = pos.y;
        object->pos[2] = pos.z;
        object->origin_pos[0] = pos.x;
        object->origin_pos[1] = pos.y;
        object->origin_pos[2] = pos.z;
    } else {
        printf("Editor tried to translate null OObject\n");
    }
}
void OObject::Rotate(IRotator rot) {
    if (_objectIndex != -1) {
        SpawnRot = rot;
        Object* object = &gObjectList[_objectIndex];
        object->orientation[0] = rot.pitch;
        object->orientation[1] = rot.yaw;
        object->orientation[2] = rot.roll;
    } else {
        printf("Editor tried to rotate null OObject\n");
    }
}

void OObject::SetScale(FVector scale) {
    SpawnScale = scale;
}
