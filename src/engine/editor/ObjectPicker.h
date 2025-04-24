#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "Collision.h"
#include "Gizmo.h"
#include "GameObject.h"

namespace Editor {
    class ObjectPicker {
    public:
        void SelectObject(std::vector<GameObject*> objects);
        void DragHandle();
        void Draw();
        void FindObject(Ray ray, std::vector<GameObject*> objects);
        void Load();
        void Tick();
        Gizmo eGizmo;
        GameObject* _selected;
    private:
        bool _draw = false;
        GameObject* _lastSelected;
        s32 Inverse(MtxF* src, MtxF* dest);
        void Copy(MtxF* src, MtxF* dest);
        void Clear(MtxF* mf);
        bool Debug = false;
    };
}
