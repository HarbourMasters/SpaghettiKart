#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/objects/Flagpole.h"
#include "Gizmo.h"

class ObjectPicker {
    public:
        ObjectPicker();
    
        void SelectObject();
        void Draw();
        void FindObject(FVector ray);
    private:
        bool _draw = false;
        Vec3f _ray;
        AActor* _selected;
        AActor* _lastSelected;
        s32 Inverse(MtxF* src, MtxF* dest);
        void Copy(MtxF* src, MtxF* dest);
        void Clear(MtxF* mf);
    
        void DrawObj(float length);
    
        Gizmo eGizmo;  

};