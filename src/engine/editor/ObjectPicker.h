#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "Collision.h"
#include "engine/objects/Flagpole.h"
#include "Gizmo.h"

class ObjectPicker {
    public:
        ObjectPicker();
    
        void SelectObject(std::vector<GameObject>& objects);
        void DragHandle();
        void Draw();
        void FindObject(Ray ray, std::vector<GameObject>& objects);
        void Load();
        void Tick();
        Gizmo eGizmo;
    private:
        bool _draw = false;
        Vec3f _ray;
        GameObject* _selected;
        GameObject* _lastSelected;
        s32 Inverse(MtxF* src, MtxF* dest);
        void Copy(MtxF* src, MtxF* dest);
        void Clear(MtxF* mf);
};
