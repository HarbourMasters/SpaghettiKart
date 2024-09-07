#include <vector>
#include <memory>

class ActorManager {
public:
    std::vector<std::unique_ptr<Actor>> actors;

    void AddActor(std::unique_ptr<Actor> actor) {
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
                           [](const std::unique_ptr<Actor>& actor) { return actor->id == 0; }), // Example condition
            actors.end());
    }
};
