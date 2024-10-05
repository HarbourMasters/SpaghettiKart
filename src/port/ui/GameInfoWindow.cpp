#include "GameInfoWindow.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <libultraship/libultraship.h>
#include "spdlog/spdlog.h"
#include <common_structs.h>
#include <defines.h>

extern "C" {
#include "../../../src/main.h"
#include "networking/networking.h"
}

namespace GameInfo {
GameInfoWindow::~GameInfoWindow() {
    SPDLOG_TRACE("destruct game info window");
}

void GameInfoWindow::InitElement() {
}

static s32 sReadyUpBool = false;

void GameInfoWindow::DrawElement() {
    const float framerate = ImGui::GetIO().Framerate;
    const float deltatime = ImGui::GetIO().DeltaTime;
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    static char username[NETWORK_USERNAME_LENGTH] = "TestUser";

    ImGui::Text("Username:");
    ImGui::InputText("##edit", username, IM_ARRAYSIZE(username));

    static char address[32] = "127.0.0.1:64010";
    static char addressCopy[32];
    strncpy(addressCopy, address, sizeof(addressCopy));
    addressCopy[sizeof(addressCopy) - 1] = '\0'; // Ensure null termination

    ImGui::Text("Host:");
    ImGui::InputText("##edit3", addressCopy, IM_ARRAYSIZE(addressCopy));

    if (ImGui::Button("Connect!")) {
        char* ip = NULL;
        uint16_t port = 0;

        char* token = strtok(addressCopy, ":");
        ip = token;

        token = strtok(NULL, ":");
        if (token != NULL) {
            port = (uint16_t) atoi(token);
        }

        gNetwork.enabled = true;
        ConnectToServer(ip, port, username);
    }

    char buttonLabel[32];

    if (sReadyUpBool) {
        strcpy(buttonLabel, "Lets Go!");
    } else {
        strcpy(buttonLabel, "Ready Up!");
    }

    if (ImGui::Button(buttonLabel)) {
        sReadyUpBool = !sReadyUpBool;
        networking_ready_up(sReadyUpBool);
    }

    if (gGamestate == RACING) {

         ImGui::Text("Player %f, %f, %f", gPlayers[0].pos[0], gPlayers[0].pos[1], gPlayers[0].pos[2]);

        for (int i = 0; i < NUM_PLAYERS; i++) {
            ImGui::Text("Player %d: type: %X, char: %d, rank: %d, hasAuthority: %d", i, gPlayers[i].type,
                        gPlayers[i].characterId, gPlayers[i].currentRank, gPlayers[i].nHasAuthority);
        }
    }
    ImGui::PopStyleColor();
}

void GameInfoWindow::UpdateElement() {
}
} // namespace GameInfo
