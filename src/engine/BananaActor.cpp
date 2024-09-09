#include <libultraship.h>

#include "GameActor.h"
#include "structs.h"

extern "C" {
    void update_actor_banana(BananaActor*);
    void render_actor_banana(Camera*, float[4][4], struct BananaActor*);
}

class ABanana : public GameActor  {
public:
    uint8_t uuid[16];
    s16 type;
    s16 flags;
    s16 unk_04;
    s16 state;
    s16 parentIndex;
    s16 bananaId;  // Banana identifier
    f32 boundingBoxSize;
    s16 playerId;
    s16 elderIndex;
    s16 youngerIndex;
    s16 unk_16;
    float pos[3];
    float velocity[3];
    //Collision unk30;  // Use the globally defined Collision type here

    //GameActor()

    // Virtual functions to be overridden by derived classes
    virtual void Init() override {
        
    }
    
    virtual void Update() override { 
        update_actor_banana(reinterpret_cast<BananaActor*>(this));
    }
    virtual void Render(Camera *camera) override {
        render_actor_banana(camera, NULL, reinterpret_cast<BananaActor*>(this));
    }
    virtual void Collision() override { }
    virtual void Expire() override { }
    virtual void Destroy() override { }
};
