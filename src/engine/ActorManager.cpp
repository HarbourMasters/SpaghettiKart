//#include <vector>
//#include <memory>
#include "ActorManager.h"
#include "GameActor.h"

class ActorManager {
public:
    void AddActor(std::unique_ptr<GameActor> actor) {
        actors.push_back(std::move(actor));
    }

    void UpdateActors() {
        for (auto& actor : actors) {
            actor->Update();
        }
    }

    void RenderActors() {
        for (auto& actor : actors) {
            actor->Render();
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

    void ActorManager_RenderActors() {
        gActorManager.RenderActors();
    }

    void ActorManager_RemoveExpiredActors() {
        gActorManager.RemoveExpiredActors();
    }
}
