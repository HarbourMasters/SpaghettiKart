#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "GameObject.h"

#ifdef __cplusplus

#include "ObjectPicker.h"
namespace Editor {
    class ObjectPicker;

    class Editor {
public:
    Editor();

    ObjectPicker eObjectPicker;
    std::vector<GameObject*> eGameObjects;

    void Tick();
    void Draw();
	void Load();
    GameObject* AddObject(const char* name, FVector* pos, IRotator* rot, FVector* scale, Gfx* model, float collScale, GameObject::CollisionType collision, float boundingBoxSize, int32_t* despawnFlag, int32_t despawnValue);
    void AddLight(const char* name, FVector* pos, s8* rot);
    void ClearObjects();
    void RemoveObject();
    void SelectObjectFromSceneExplorer(GameObject* object);
    void SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY);
    void ClearMatrixPool();
    void DeleteObject();
    bool bEditorEnabled = false;

private:
    bool _draw = false;
    Vec3f _ray;

    s32 Inverse(MtxF* src, MtxF* dest);
    void Copy(MtxF* src, MtxF* dest);
    void Clear(MtxF* mf);
};
}
#endif

void SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY);

#endif // __EDITOR_H__
