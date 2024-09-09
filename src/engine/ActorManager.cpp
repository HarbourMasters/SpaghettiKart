//#include <vector>
//#include <memory>
#include "ActorManager.h"
#include "GameActor.h"
#include "ABanana.h"
#include "structs.h"

class ActorManager {
public:
    Actor* AddActor(std::unique_ptr<GameActor> actor) {
        GameActor* rawPtr = actor.get();
        actors.push_back(std::move(actor));
        return rawPtr->a;
    }

    struct BananaActor* SpawnBanana(uint16_t playerId, const Vec3f startingPos, const s16 startingRot[3], const Vec3f startingVelocity) {
        return reinterpret_cast<struct BananaActor*>(AddActor(std::make_unique<ABanana>(playerId, startingPos, startingRot, startingVelocity)));
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

    Actor* GetActorByIndex(size_t index) {
        if (index < actors.size()) {
            // Assuming GameActor::a is accessible, use reinterpret_cast if needed
            return reinterpret_cast<Actor*>(actors[index]->a);
        }
        return nullptr; // Or handle the error as needed
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

    struct BananaActor *ActorManager_SpawnBanana(uint16_t playerId, const float startingPos[3], const s16 startingRot[3], const float startingVelocity[3]) {
        return gActorManager.SpawnBanana(playerId, startingPos, startingRot, startingVelocity);
    }

    Actor* ActorManager_GetActorByIndex(size_t index) {
        return gActorManager.GetActorByIndex(index);
    }
}
