#pragma once

#include <libultraship.h>

#include "RegisterContent.h"
#include "World.h"


class OPodium : public OObject {
public:
    enum Behaviour : uint16_t {
    };

    explicit OPodium(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static OPodium* Spawn(const FVector& pos) {
        SpawnParams params = {
            .Name = "mk:podium",
            .Location = pos,
        };
        return dynamic_cast<OPodium*>(AddObjectToWorld<OPodium>(params));
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_8008629C(s32 objectIndex, s32 arg1);
    void func_80086424(s32 objectIndex);
    void func_80086528(s32 objectIndex, s32 arg1);

private:
    s32 _podium1Index;
    s32 _podium2Index;
    s32 _podium3Index;
    FVector _pos;
};
