#include "ContentBrowser.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "ship/Context.h"
#include "port/Engine.h"
#include "SpawnParams.h"

#include <imgui.h>
#include <map>
#include <filesystem>
#include <iostream>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>
#include "CoreMath.h"
#include "World.h"
#include "AllActors.h"
#include "port/Game.h"
#include "src/engine/editor/SceneManager.h"
#include "engine/TrackBrowser.h"

#include "World.h"

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "collision.h"
}

namespace Editor {
    bool bIsTrainWindowOpen = false; // Global because member variables do not work in lambdas

    ContentBrowserWindow::~ContentBrowserWindow() {
        SPDLOG_TRACE("destruct content browser window");
    }

    void ContentBrowserWindow::DrawElement() {
        if (ImGui::Button(ICON_FA_REFRESH)) {
            Refresh = true;
        }

        // Query content in o2r and add them to Content
        if (Refresh) {
            Refresh = false;
            Content.clear();
            TrackBrowser::Instance->Refresh(gTrackRegistry);
            FindContent();
            return;
        }

        ImGui::BeginChild("LeftPanel", ImVec2(120, 0), true, ImGuiWindowFlags_None);

        ContentBrowserWindow::FolderButton("Tracks", TrackContent);
        ContentBrowserWindow::FolderButton("Actors", ActorContent);
        ContentBrowserWindow::FolderButton("Custom", CustomContent);
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true, ImGuiWindowFlags_None);
        
        // Search bar
        ImGui::InputTextWithHint("##GlobalSearch", "Search name or #tag", mSearchBuffer, IM_ARRAYSIZE(mSearchBuffer));
        ImGui::NewLine();

        std::string search = ToLower(std::string(mSearchBuffer));
        
        if (TrackContent) {
            AddTrackContent(search);
        }

        if (ActorContent) {
            AddActorContent(search);
        }

        if (CustomContent) {
            AddCustomContent(search);
        }
        ImGui::EndChild();
    }

    void ContentBrowserWindow::FolderButton(const char* label, bool& contentFlag, const ImVec2& size) {
        std::string buttonText = fmt::format("{0} {1}", contentFlag ? ICON_FA_FOLDER_OPEN_O : ICON_FA_FOLDER_O, label);
        if (ImGui::Button(buttonText.c_str(), size)) {
            TrackContent = false;
            ActorContent = false;
            CustomContent = false;
            contentFlag = !contentFlag;
        }
    }

    void ContentBrowserWindow::AddTrackContent(std::string search) {
        size_t i_track = 0;
        for (const TrackInfo* info : gTrackRegistry.GetAllInfo()) {
            if (!info) { continue; }

            if (!search.empty() &&
                ToLower(info->Name).find(search) == std::string::npos) {
                continue;
            }

            std::string label = fmt::format("{}##{}", info->Name, i_track);
            if (ImGui::Button(label.c_str())) {
                //gGamestateNext = RACING;
                gGotoMode = RACING;
                gIsInQuitToMenuTransition = 1;
                gQuitToMenuTransitionCounter = 5;
                TrackBrowser::Instance->SetTrack(info->ResourceName);
                break;
            }

            if ((i_track != 0) && (i_track % 8 == 0)) {
            } else {
                ImGui::SameLine();
            }

            i_track += 1;
        }
    }

void ContentBrowserWindow::AddActorContent(std::string search) {

    bool isTagSearch = false;
    std::string tagToSearch;

    if (!search.empty() && search[0] == '#') {
        isTagSearch = true;
        tagToSearch = ToLower(search.substr(1)); // Remove the #
    } else {
        search = ToLower(search);
    }
    

    FVector pos = GetPositionAheadOfCamera(300.0f);
    SpawnParams params;
    params.Location = pos;

    size_t i_actor = 0;

    for (const auto* actorInfo : gActorRegistry.GetAllInfo()) {
        if (!actorInfo) continue;

        // Filtering
        bool show = false;
        if (isTagSearch) {
            // Check tags case-insensitively
            for (const auto& tag : actorInfo->Tags) {
                if (ToLower(tag) == tagToSearch) {
                    show = true;
                    break;
                }
            }
        } else if (!search.empty()) {
            if (ToLower(actorInfo->Name).find(search) != std::string::npos) {
                show = true;
            }
        } else {
            show = true; // No search --> show all
        }

        if (!show) continue;

        if ((i_actor != 0) && (i_actor % 8 == 0)) {
        } else {
            ImGui::SameLine();
        }

        std::string label = fmt::format("{}##{}", actorInfo->Name, i_actor);
        if (ImGui::Button(label.c_str())) {
            gActorRegistry.Invoke(actorInfo->ResourceName, params);
        }

        i_actor += 1;
    }

    // Add a couple actor models (always shown, bypass search)
    auto addShipButton = [&](int id, const char* name, int type) {
        ImGui::PushID(id);
        if (ImGui::Button(name)) {
            params.Type = static_cast<int16_t>(type);
            gActorRegistry.Invoke("hm:ship", params);
        }
        ImGui::PopID();
    };

    addShipButton(i_actor++, "Ship 2 (HM64)", AShip::Skin::SHIP2);
    addShipButton(i_actor++, "Ship 3 (HM64)", AShip::Skin::SHIP3);

    ContentBrowserWindow::TrainWindow();
}


    void ContentBrowserWindow::AddCustomContent(std::string search) {
        FVector pos = GetPositionAheadOfCamera(300.0f);

        size_t i_custom = 0;
        for (const auto& file : Content) {
            std::string name = ToLower(file);
            if (!search.empty() &&
                name.find(search) == std::string::npos) {
                continue;
            }

            if ((i_custom != 0) && (i_custom % 5 == 0)) {
            } else {
                ImGui::SameLine();
            }

            std::string label = fmt::format("{}##{}", file, i_custom);
            if (ImGui::Button(label.c_str())) {
                int coll;
                //printf("ContentBrowser.cpp: name: %s\n", test.c_str());
                std::string name = file.substr(file.find_last_of('/') + 1);
                auto actor = GetWorld()->AddStaticMeshActor(name, FVector(pos), IRotator(0, 0, 0), FVector(1, 1, 1), "__OTR__" + file, &coll);
                // This is required because ptr gets cleaned up.
                actor->Model = "__OTR__" + file;

            }
            i_custom += 1;
        }
    }

    void ContentBrowserWindow::FindContent() {
        auto ptr = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->ListFiles({"hmintro/*", "*tracks/*","actors/*", "objects/*"}, {""});
        if (ptr) {
            auto files = *ptr;
            for (const auto& file : files) {
                if (file.find("/mat_") != std::string::npos) {
                    continue;
                } else if (file.size() >= 6 && file.substr(file.size() - 6, 5) == "_tri_" && isdigit(file.back())) {
                    // ends with _tri_#
                    continue;
                } else if (file.find("_vtx_") != std::string::npos) {
                    // Has _vtx_
                    continue;
                } else if (file.find("_vertices") != std::string::npos) {
                    // Has _vertices
                    continue;
                } else if (file.find("_spawns") != std::string::npos) {
                    // has _spawns
                    continue;
                } else if (file.find("_waypoints") != std::string::npos) {
                    // has _waypoints
                    continue;
                } else if (file.find('.') != std::string::npos) {
                    // File has an extension
                    continue;
                }

                Content.push_back(file);
            }
        }
    }

    /**  Actors that need config windows before spawning  **/

    ATrain* ContentBrowserWindow::TrainWindow() {
        if (!bIsTrainWindowOpen) {
            return nullptr;
        }

        // Setup train window size and position
        // Set window size constraints (min, max)
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(FLT_MAX, FLT_MAX));

        // Get the main viewport to find the center of the screen
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Center the window
        ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // Optional: auto-resize to content
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Appearing); // Initial size only

        if (ImGui::Begin("Spawn Train")) {
            static int32_t numCarriages = 4;
            static ATrain::TenderStatus tender = ATrain::TenderStatus::HAS_TENDER; // Can only disable tender if using no passenger cars
            static int32_t pathIndex = 0;
            static int32_t pathPoint = 0;
            static ATrain::SpawnMode spawnMode = ATrain::SpawnMode::AUTO;

            // Num Carriage Input
            if (ImGui::InputInt("Carriages", &numCarriages)) {
                // Clamp to uint32_t range (only lower bound needed if assuming positive values)
                if (numCarriages < 0) numCarriages = 0;
            }

            // Setup for has tender settings
            if (numCarriages > 0) {
                ImGui::BeginDisabled();
                // Tender is required if there are any carriages
                tender = ATrain::TenderStatus::HAS_TENDER;
            }

            // Convert enum to bool
            bool hasTender = (tender == ATrain::TenderStatus::HAS_TENDER);
            if (ImGui::Checkbox("Has Tender", &hasTender)) {
                tender = hasTender ? ATrain::TenderStatus::HAS_TENDER : ATrain::TenderStatus::NO_TENDER;
            }

            if (numCarriages > 0) {
                ImGui::EndDisabled();
            }

            // Set Spawn Mode
            bool localSpawnMode = (spawnMode == ATrain::SpawnMode::AUTO);
            if (ImGui::Checkbox("Place Train Automatically", &localSpawnMode)) {
                spawnMode = localSpawnMode ? ATrain::SpawnMode::AUTO : ATrain::SpawnMode::POINT;
            }

            // Set PathIndex and PathPoint
            if (spawnMode == ATrain::SpawnMode::POINT) {
                // PathIndex and PathPoint
                if (ImGui::InputInt("Path Index", &pathIndex)) {
                    // Clamp to uint32_t range (only lower bound needed if assuming positive values)
                    if (pathIndex < 0) pathIndex = 0;
                }

                if (ImGui::InputInt("Path Point", &pathPoint)) {
                    // Clamp to uint32_t range (only lower bound needed if assuming positive values)
                    if (pathPoint < 0) pathPoint = 0;
                }
            }

            if (ImGui::Button("Spawn")) {
                bIsTrainWindowOpen = false;
                return ATrain::Spawn(tender, numCarriages, 2.5f, (uint32_t)pathIndex, (uint32_t)pathPoint, spawnMode);
            }
        }
        ImGui::End();
    }
}
