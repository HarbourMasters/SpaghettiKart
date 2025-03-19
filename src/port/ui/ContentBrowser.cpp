#include "ContentBrowser.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"
#include "port/Engine.h"

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

namespace Editor {

    ContentBrowserWindow::~ContentBrowserWindow() {
        SPDLOG_TRACE("destruct content browser window");
    }

    void ContentBrowserWindow::DrawElement() {
        static bool refresh = true;
        // List the available mods/o2r files
        // for (size_t i = 0; i < GameEngine::Instance->archiveFiles.size(); i++) {
        //     auto str = GameEngine::Instance->archiveFiles[i];
        //     if (str.starts_with("./mods/")) {
        //         ImGui::Text(GameEngine::Instance->archiveFiles[i].c_str());
        //     }
        // }

        if (ImGui::Button(ICON_FA_REFRESH)) {
            refresh = true;
        }

        // Query content in o2r and add them to Content
        if (refresh) {
            refresh = false;
            FindContent();
            return;
        }

        // Display entries in Content
        for (const auto& file : Content) {
            if (ImGui::Button(file.c_str())) {
                int coll;
                //printf("ContentBrowser.cpp: name: %s\n", test.c_str());
                printf("TEST %s\n", file.c_str());
                std::string name = file.substr(file.find_last_of('/') + 1);
                printf("NAME %s\n", name.c_str());
                auto actor = gWorldInstance.AddStaticMeshActor(name, FVector(50, 100, 0), IRotator(0, 90, 0), FVector(1, 1, 1), "__OTR__" + file, &coll);
                // This is required because ptr gets cleaned up.
                actor->Model = "__OTR__" + file;

            }
        }
    }

    void ContentBrowserWindow::FindContent() {
        std::list<std::string> myList = {"tracks/*", "actors/*", "objects/*"};
        std::list<std::string> myList2 = {""};

        Content.clear();

        auto ptr = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->ListFiles(myList, myList2);
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
                } else if (file.find('.') != std::string::npos) {
                    // File has an extension
                    continue;
                }

                Content.push_back(file);
            }
        }
    }
}
