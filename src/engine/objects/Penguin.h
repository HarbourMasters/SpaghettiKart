#pragma once

#include <libultraship.h>
#include "RegisterContent.h"
#include "engine/World.h"
#include "engine/objects/Object.h"

class OPenguin : public OObject {
public:
    enum PenguinType : int16_t {
        CHICK,
        ADULT,
        CREDITS,
        EMPEROR
    };

    enum Behaviour : int16_t {
        DISABLED,
        STRUT, // Emperor penguin
        CIRCLE, // Waddle in a circle
        SLIDE3,
        SLIDE4,
        UNK, // unused
        SLIDE6,
    };

    explicit OPenguin(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static OPenguin* Spawn(FVector pos, u16 direction, u16 mirrorModeAngleOffset, f32 diameter, PenguinType type, Behaviour behaviour) {
        IRotator rot;
        rot.Set(0, direction, mirrorModeAngleOffset);
        SpawnParams params = {
            .Name = "mk:penguin",
            .Type = type,
            .Behaviour = behaviour,
            .Location = pos,
            .Rotation = rot,
            .Speed = diameter, // Diameter of the walking circle
        };
        return dynamic_cast<OPenguin*>(AddObjectToWorld<OPenguin>(params));
    }

    PenguinType Type = PenguinType::CHICK;
    Behaviour SpawnBhv = Behaviour::STRUT;

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void Reset() override;
    virtual void DrawEditorProperties() override;
private:
    void Behaviours(s32 objectIndex);
    void EmperorPenguin(s32 objectIndex);
    void func_80085080(s32 objectIndex);
    void func_8008502C(s32 objectIndex);
    void func_80084D2C(s32 objectIndex, s32 arg1);

    void InitEmperorPenguin(s32 objectIndex);
    void OtherPenguin(s32 objectIndex);
    void InitOtherPenguin(s32 objectIndex);

    static bool _toggle;
};
