#pragma once

#include <libultraship.h>
#include "engine/registry/RegisterContent.h"
#include "engine/Actor.h"
#include "engine/CoreMath.h"

class AWarioSign : public AActor {
public:
  ~AWarioSign() override {
      _count -= 1;
  };
    explicit AWarioSign(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static AWarioSign* Spawn(FVector pos, IRotator rot, FVector velocity, FVector scale) {
        SpawnParams params = {
            .Name = "mk:wario_sign",
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
            .Velocity = velocity,
            .Speed = 182,
        };
        return dynamic_cast<AWarioSign*>(AddActorToWorld<AWarioSign>(params));
    }

    bool IsMod() override;
    void Tick() override;
    void Draw(Camera*) override;

  private:
    static size_t _count;
    size_t _idx;
};
