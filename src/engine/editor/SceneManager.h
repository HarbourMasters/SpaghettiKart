#include <libultraship/libultraship.h>
#include "engine/courses/Course.h"

namespace Editor {
        void SaveLevel();
        void LoadLevel(std::shared_ptr<Ship::Archive> archive, Course* course, std::string sceneFile);
        void Load_AddStaticMeshActor(const nlohmann::json& actorJson);
        void SetSceneFile(std::shared_ptr<Ship::Archive> archive, std::string sceneFile);
        void LoadMinimap(std::shared_ptr<Ship::Archive> archive, Course* course, std::string filePath);

        extern std::shared_ptr<Ship::Archive> CurrentArchive; // This is used to retrieve and write the scene data file
        extern std::string SceneFile;
}
