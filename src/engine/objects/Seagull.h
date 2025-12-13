#pragma once

#include <libultraship.h>
#include "Object.h"

#include "RegisterContent.h"
#include "engine/World.h"

//! @todo unk_0D5 needs to be a struct variable probably. What does it do? Behaviour?
class OSeagull : public OObject {
public:
    explicit OSeagull(const SpawnParams& params);

    ~OSeagull() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    // This is simply a helper function to keep Spawning code clean
    static OSeagull* Spawn(const FVector& pos) {
        SpawnParams params = {
            .Name = "mk:seagull",
            .Location = pos,
        };
        return dynamic_cast<OSeagull*>(AddObjectToWorld<OSeagull>(params));
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;

    void func_800552BC(s32 objectIndex);

    void func_8008275C(s32 objectIndex);
    void func_8008241C(s32 objectIndex, s32 arg1);
    void func_80082714(s32 objectIndex, s32 arg1);
private:
    static size_t _count;
    s32 _idx;
    bool _toggle;

    SplineData *spline;
};
