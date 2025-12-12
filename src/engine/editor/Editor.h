#ifndef EDITOR_H
#define EDITOR_H

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>


#ifdef __cplusplus
#include "GameObject.h"
extern "C" {
#include "camera.h"
}

#include "ObjectPicker.h"
namespace Editor {
    class ObjectPicker;

    class Editor {
public:
    static Editor* Instance;
    Editor();
    ~Editor();

    ObjectPicker eObjectPicker;
    std::vector<GameObject*> eGameObjects;
    
	void Load();
    void Enable();
    void Disable();
    bool IsEnabled();
    void Play();
    void Pause();
    bool IsPaused();
    void TogglePlayState();
    void Tick();
    void Draw();
    void GenerateCollision();
    GameObject* AddObject(FVector pos, IRotator rot, FVector scale, const char* model, float collScale, GameObject::CollisionType collision, float boundingBoxSize);
    void AddLight(const char* name, FVector* pos, s8* rot);
    void ClearObjects();
    void ResetGizmo();
    void RemoveObject();
    void SelectObjectFromSceneExplorer(std::variant<AActor*, OObject*, GameObject*> object);
    void SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY);
    void ClearMatrixPool();
    void DeleteObject();
private:
    bool bIsEditorPaused = false;
    bool bEditorEnabled = false;
    bool _draw = false;
    Vec3f _ray;

    s32 Inverse(MtxF* src, MtxF* dest);
    void Copy(MtxF* src, MtxF* dest);
    void Clear(MtxF* mf);
};
} // namespace Editor
#endif // __cplusplus


#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void Editor_Launch(const char* resourceName);
EXTERN_C void Editor_SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY);
EXTERN_C bool Editor_IsEnabled();
EXTERN_C bool Editor_IsPaused();

#endif // EDITOR_H
