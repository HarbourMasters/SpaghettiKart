#include <libultraship.h>
#include "Course.h"

namespace Editor {
        void SaveLevel();
        void LoadLevel(std::string sceneFile);
        void Load_AddStaticMeshActor(const nlohmann::json& actorJson);
        void SetSceneFile(std::string sceneFile);

        extern std::shared_ptr<Ship::Archive> CurrentArchive; // This is used to retrieve and write the scene data file
        extern std::string SceneFile;
}
