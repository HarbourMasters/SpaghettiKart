//#include <vector>
//#include <memory>
#include "ActorManager.h"
#include "GameActor.h"
#include "ABanana.h"
#include "structs.h"

class ActorManager {
public:
    GameActor* AddActor(std::unique_ptr<GameActor> actor) {
        GameActor* rawPtr = actor.get();
        actors.push_back(std::move(actor));
        return rawPtr;
    }

    void UpdateActors() {
        for (auto& actor : actors) {
            actor->Update();
        }
    }

    void RenderActors(Camera *camera) {
        for (auto& actor : actors) {
            actor->Render(camera);
        }
    }

    void RemoveExpiredActors() {
        actors.erase(
            std::remove_if(actors.begin(), actors.end(),
                           [](const std::unique_ptr<GameActor>& actor) { return actor->uuid == 0; }), // Example condition
            actors.end());
    }
private:
    std::vector<std::unique_ptr<GameActor>> actors;
};

ActorManager gActorManager;

extern "C" {

    void ActorManager_AddActor() {
        //gActorManager.AddActor(std::unique_ptr<GameActor>(actor));
    }

    void ActorManager_UpdateActors() {
        gActorManager.UpdateActors();
    }

    void ActorManager_RenderActors(Camera* camera) {
        gActorManager.RenderActors(camera);
    }

    void ActorManager_RemoveExpiredActors() {
        gActorManager.RemoveExpiredActors();
    }

    struct BananaActor *ActorManager_SpawnBanana(const float startingPos[3], const s16 startingRot[3], const float startingVelocity[3]) {
        auto newBanana = dynamic_cast<struct BananaActor*>(gActorManager.AddActor(std::make_unique<ABanana>(startingPos, startingRot, startingVelocity)));
        return newBanana;
    }
}
