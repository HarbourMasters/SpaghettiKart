#pragma once

#include <libultraship.h>
#include "Object.h"

#include "RegisterContent.h"
#include "World.h"

class OTrashBin : public OObject {
public:

    enum Behaviour : int16_t {
        STATIC, // The lid stays shut
        MUNCHING // The lid opens/closes in a scary munching manner
    };

    // This is simply a helper function to keep Spawning code clean
    static OTrashBin* Spawn(const FVector& pos, const IRotator& rot, f32 scale, OTrashBin::Behaviour bhv) {
        SpawnParams params = {
            .Name = "mk:trash_bin",
            .Behaviour = bhv,
            .Location = pos,
            .Rotation = rot,
            .Scale = FVector(0, scale, 0),
        };
        return dynamic_cast<OTrashBin*>(AddObjectToWorld<OTrashBin>(params));
    }

    explicit OTrashBin(const SpawnParams& params);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_8007E00C(s32 objectIndex);
    void init_bb_trash_bin(s32);

private:
    static Gfx BinMod[];
    static Vtx BinVtx[];
    static Vtx BinVtx2[];

    Behaviour _bhv;
    FVector _pos;
    IRotator _rot;
    float _scale;
    size_t _idx;
    bool _drawBin = false;
};
