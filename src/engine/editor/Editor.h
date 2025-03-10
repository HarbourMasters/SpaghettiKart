#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <libultraship.h>
#include <libultra/gbi.h>

#ifdef __cplusplus

#include "ObjectPicker.h"
namespace EditorNamespace {
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
    void SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY);

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
