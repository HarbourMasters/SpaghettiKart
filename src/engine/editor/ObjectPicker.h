#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "Collision.h"
#include "Gizmo.h"
#include "GameObject.h"
#include "engine/Matrix.h"

namespace TrackEditor {
    class ObjectPicker {
    public:
        void SelectObject(std::vector<GameObject*> objects);
        void DragHandle();
        void Draw();
        void FindObject(Ray ray, std::vector<GameObject*> objects);
        void Load();
        void Tick();
        Gizmo eGizmo;
        std::variant<AActor*, OObject*, GameObject*> _selected;
    private:
        bool _draw = false;
        GameObject* _lastSelected;
        s32 Inverse(MtxF* src, MtxF* dest);
        void Copy(MtxF* src, MtxF* dest);
        void Clear(MtxF* mf);
        // actor, distance from camera
        std::pair<AActor*, float> CheckAActorRay(Ray ray);
        std::pair<OObject*, float> CheckOObjectRay(Ray ray);
        std::pair<GameObject*, float> CheckEditorObjectRay(Ray ray);
        bool Debug = false;
    };
}
