#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "ObjectPicker.h"

class ObjectPicker;

class Editor {
public:
    Editor();


    ObjectPicker eObjectPicker;
    std::vector<GameObject> eGameObjects;

    void Tick();
    void Draw();
    void MouseClick();
	void Load();
    void AddObject(const char* name, FVector* pos, Gfx* model, float scale, CollisionType collision, float boundingBoxSize, int32_t* despawnFlag, int32_t despawnValue);
    void ClearObjects();
    void RemoveObject();
    void SelectObjectFromSceneExplorer(GameObject* object);
private:
    bool _draw = false;
    Vec3f _ray;

    s32 Inverse(MtxF* src, MtxF* dest);
    void Copy(MtxF* src, MtxF* dest);
    void Clear(MtxF* mf);

};
