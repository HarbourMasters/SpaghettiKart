#include "MultiplayerWindow.h"
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
#include "actor_types.h"
#include "code_800029B0.h"
extern struct Actor gActorList[];
extern char* gCupNames[];
extern char* D_800E76A8[];
extern Network gNetwork;
extern Camera cameras[];
extern UnkStruct_800DC5EC* D_800DC5EC;
}

namespace Multiplayer {
MultiplayerWindow::~MultiplayerWindow() {
    SPDLOG_TRACE("destruct multiplayer window");
}

void MultiplayerWindow::InitElement() {
}

static s32 sConnectedBool = false;
static s32 sReadyUpBool = false;
static s32 sCharacter = 0;
static s32 sCup = 0;

void MultiplayerWindow::DrawElement() {
    const float framerate = ImGui::GetIO().Framerate;
    const float deltatime = ImGui::GetIO().DeltaTime;
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    if (!gNetwork.isConnected) {

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

    } else {

        ImGui::Spacing();
        ImGui::Text("Select Character:");
        ImGui::Spacing();

        for (size_t i = 0; i < NUM_PLAYERS; i++) {
            if (D_800E76A8[i] == NULL || D_800E76A8[i] == "ーーーー") {
                break;
            }
            ImGui::SameLine();
            if (ImGui::Button(D_800E76A8[i])) {
                network_character_vote(i);
                sCharacter = i;
            }
        }
        ImGui::Spacing();
        ImGui::Text("-- %s --", D_800E76A8[sCharacter]);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Course Vote:");

        for (size_t i = 0; i < NUM_CUPS - 1; i++) {
            if (ImGui::Button(gCupNames[i])) {
                network_cup_vote(i);
                sCup = i;
            }
            ImGui::SameLine();
        }
        ImGui::Spacing();
        ImGui::Text("-- %s --", gCupNames[sCup]);

        if (ImGui::Button("test", ImVec2(-1, 0))) {
            // Add disconnect logic here
            cameras[0].playerId = 2;
            D_800DC5EC->player = &gPlayers[2];
        }

        for (size_t i = 0; i < NETWORK_MAX_PLAYERS; i++) {
            ImGui::Text("Slot %d: char: %d, hasAuthority: %d", clients[i].slot, clients[i].character,
                        gPlayers[i].nHasAuthority);
        }

        /*  Stick UI to the bottom of the panel  */

        float windowHeight = ImGui::GetWindowHeight();

        // Move the cursor to a position relative to the bottom of the window
        ImGui::SetCursorPosY(windowHeight -
                             ImGui::GetFrameHeightWithSpacing() * 2); // Adjust Y position for two buttons

        char buttonLabel[32];

        if (sReadyUpBool) {
            strcpy(buttonLabel, "Lets Go!");
        } else {
            strcpy(buttonLabel, "Ready Up!");
        }

        // First button (Ready Up / Let's Go)
        if (ImGui::Button(buttonLabel, ImVec2(-1, 0))) {
            sReadyUpBool = !sReadyUpBool;
            networking_ready_up(sReadyUpBool);
        }

        // Add some vertical spacing between the buttons
        ImGui::Spacing();

        // Second button (Disconnect)
        if (ImGui::Button("Disconnect", ImVec2(-1, 0))) {
            // Add disconnect logic here
            networking_disconnect();
        }
    }

    ImGui::PopStyleColor();
}

void MultiplayerWindow::UpdateElement() {
}
} // namespace Multiplayer
