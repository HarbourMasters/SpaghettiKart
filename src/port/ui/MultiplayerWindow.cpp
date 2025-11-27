#include "MultiplayerWindow.h"
#include "UIWidgets.h"
#include "PortMenu.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <libultraship/libultraship.h>
#include "spdlog/spdlog.h"
#include <common_structs.h>
#include <defines.h>

extern "C" {
#include "main.h"
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

namespace GameUI {
extern std::shared_ptr<PortMenu> mPortMenu;
namespace Multiplayer {

static s32 sConnectedBool = false;
static s32 sReadyUpBool = false;
static s32 sCharacter = 0;
static s32 sCup = 0;

void DrawColumn1(WidgetInfo& info) {
    if (gNetwork.isConnected) {
        ImGui::BeginDisabled();
    }

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
    if (gNetwork.isConnected) {
        ImGui::EndDisabled();
    }
}

void DrawColumn2(WidgetInfo& info) {
    if (!gNetwork.isConnected) {
        ImGui::BeginDisabled();
    }

    ImGui::Text("Select Character:");
    ImGui::Spacing();

    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        // These are euc-jp characters that look sort of like a hyphen -
        if (D_800E76A8[i] == NULL || D_800E76A8[i] == "\xA1\xBC\xA1\xBC\xA1\xBC\xA1\xBC") {
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
    if (!gNetwork.isConnected) {
        ImGui::EndDisabled();
    }
}

void RegisterMultiplayerWidgets() {
    // mPortMenu->AddSidebarEntry("Enhancements", "Multiplayer", 2);
    // WidgetPath path = { "Enhancements", "Multiplayer", SECTION_COLUMN_1 };
    // mPortMenu->AddWidget(path, "Multi Custom 1", WIDGET_CUSTOM)
    //     .CustomFunction(DrawColumn1);
    // path.column = SECTION_COLUMN_2;
    // mPortMenu->AddWidget(path, "Multi Custom 2", WIDGET_CUSTOM)
    //     .CustomFunction(DrawColumn2);
}

static RegisterMenuInitFunc initFunc(RegisterMultiplayerWidgets);
} // namespace Multiplayer
} // namespace GameUI
