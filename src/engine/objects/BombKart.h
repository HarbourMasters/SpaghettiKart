#pragma once

#include <libultraship.h>
#include "RegisterContent.h"
#include "engine/objects/Object.h"

#include "World.h"

extern "C" {
#include "common_structs.h"
}

/**
 * Used in VS mode
 *
 * This differs from the other vehicle classes in that it does not get added to the standard actor list
 * So this is sort of its own thing. Draw call in different place too.
 */
class OBombKart : public OObject {
  public:
    enum States : uint16_t { // 0,1,3,5
        DISABLED,
        COUNTERCLOCKWISE,
        CLOCKWISE,
        STATIONARY,
        CHASE,
        EXPLODE,
        PODIUM_CEREMONY,
    };

    const char* Type;

    Vec3f Pos;
    Vec3f WheelPos[4]; //! @todo Turn WheelPos into a struct
    f32 Unk_3C;
    u16 SomeRot;       // Some angle
    u16 WaypointIndex; // The waypoint the kart circles
    States State = States::DISABLED;
    u16 BounceTimer = 0;
    u16 CircleTimer = 0;
    u16 Unk_4A = 0;
    s16 Unk_4C = 1;
    f32 CenterY; // Center of the circle
    Collision _Collision;


    // This is simply a helper function to keep Spawning code clean
    // Spawn object at a position
    static OBombKart* Spawn(FVector pos, uint16_t behaviour, f32 unk_3C) {
        SpawnParams params = {
            .Name = "mk:bomb_kart",
            .Behaviour = behaviour,
            .Location = pos,
            .Speed = unk_3C, // Only used for podium ceremony. Arbitrarily chose Speed for this
            .SpeedB = 2.7f, // Chase speed
        };
        return dynamic_cast<OBombKart*>(AddObjectToWorld<OBombKart>(params));
    }

    // Spawn object at a point along the tracks path
    static OBombKart* Spawn(uint32_t pathIndex, uint32_t pathPoint, uint16_t behaviour, f32 unk_3C) {
        SpawnParams params = {
            .Name = "mk:bomb_kart",
            .Behaviour = behaviour,
            .PathIndex = pathIndex,
            .PathPoint = pathPoint,
            .Speed = unk_3C, // Only used for podium ceremony. Arbitrarily chose Speed for this
        };
        return dynamic_cast<OBombKart*>(AddObjectToWorld<OBombKart>(params));
    }

    // Set waypoint to NULL if using a spawn position and not a waypoint.
    explicit OBombKart(const SpawnParams& params);

    ~OBombKart() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void Translate(FVector pos) override;
    virtual void DrawEditorProperties() override;
    void DrawBattle(s32 cameraId);
    void SomeRender(Vec3f arg1);
    void LoadMtx();
    void Waypoint(s32 screenId);

    OBombKart::States Behaviour = OBombKart::States::COUNTERCLOCKWISE;
    float SpeedB = 2.7f;
  private:
    static size_t _count;
    s32 _idx;
    Player* FindTarget();
    void Chase(Player*, Vec3f pos);

    Player* _target = NULL;
};
