#include <libultraship.h>
#include <defines.h>
#include "main.h"
#include "spawn_players.h"
#include "networking.h"
#include "code_800029B0.h"
#include "menus.h"
#include "audio/external.h"
#include "code_80091750.h"

// PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN
// PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI

void assign_player_control_types(void) {
    printf("Assign player control types\n");
    for (size_t i = 0; i < NETWORK_MAX_PLAYERS; i++) {
        s32 playerIndex = clients[i].slot;

        gPlayers[playerIndex].nCharacter = clients[i].character;
        gPlayers[playerIndex].nStartingRank = clients[i].slot;

        // printf("CHAR  %d\n", clients[i].character);
        // printf("CHAR2 %d\n", gPlayers[playerIndex].nCharacter);

        if (clients[i].isAI) {
            gPlayers[playerIndex].nControlFlags = 0;
        } else if (clients[i].hasAuthority) {
            // printf("PLAYER SLOT %d, INDEX %d\n", clients[i].slot, clients[i].character);

            // Set player to the camera
            // cameras[0].playerId = clients[i].slot;
            // D_800DC5EC->player = &gPlayers[clients[i].slot];
            // printf("PLAYER SLOT %d, INDEX %d\n", clients[i].slot, clients[i].character);
            gPlayers[playerIndex].nControlFlags =
                PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE; // Requires server replication to move.
        }

        // printf("Player Slot %d, character %d\n", playerIndex, clients[i].character);
    }

    // Get and Set player
    // gNetwork.localPlayer = &gPlayers[i];
}

void network_character_vote(uint32_t course) {
    send_int_packet(gNetwork.tcpSocket, PACKET_SET_CHARACTER, course, sizeof(uint32_t));
}

void network_cup_vote(uint32_t course) {
    send_int_packet(gNetwork.tcpSocket, PACKET_COURSE_VOTE, course, sizeof(uint32_t));
}

void set_course(const char* data) {
    if (data != NULL) {
        gCupSelection = data[0];
        gCurrentCourseId = gCupCourseOrder[gCupSelection][CUP_COURSE_ONE];
    }
}

s32 testa = 0;

void assign_player_slots(const char* data) {
    if (testa) {
        return;
    }
    testa = 1;
    int offset = 0;
    printf("Assign Player Slots\n");
    for (int i = 0; i < NETWORK_MAX_PLAYERS; ++i) {
        // Read username

        int usernameLength = *(int*) (data + offset);
        offset += sizeof(int);

        memcpy(clients[i].username, data + offset, usernameLength);
        offset += usernameLength;

        // Read slot
        clients[i].slot = *(int*) (data + offset);
        offset += sizeof(int);

        // Read isPlayer
        clients[i].isPlayer = *(int*) (data + offset);
        offset += sizeof(int);

        // Read IsAI
        clients[i].isAI = *(int*) (data + offset);
        offset += sizeof(int);

        // Read character
        clients[i].character = *(int*) (data + offset);
        offset += sizeof(int);

        // gCharacterSelections[i] = clients[i].character;

        // Read hasAuthority
        clients[i].hasAuthority = *(int*) (data + offset);

        if (clients[i].hasAuthority) {
            localClient = &clients[i];
            gPlayers[clients[i].slot].nHasAuthority = true;
            printf("You have been assigned slot %d\n", clients[i].slot + 1);
        }

        offset += sizeof(int);
    }
}

s32 sessionBool = 0;

// Duplicate of setup_race
void networking_start_session(const char* data) {
    if (sessionBool) {
        return;
    }
    sessionBool = 1;
    printf("Start session\n");
    // gIsInQuitToMenuTransition = 1;
    // gQuitToMenuTransitionCounter = 5;
    gGotoMode = RACING; // Enter race state
    gGamestateNext = RACING;
    gCCSelection = CC_150;
    // gCupSelection = FLOWER_CUP;
    gCupCourseSelection = CUP_COURSE_ONE;
    gScreenModeSelection = SCREEN_MODE_1P;
    gModeSelection = GRAND_PRIX;
    gPlayerCount = 1;
    // gCurrentCourseId = 8;
    gDebugMenuSelection = DEBUG_MENU_EXITED;
    func_8009E1C0();
    func_800CA330(0x19);
}

// Wait for all players to load
s32 network_all_players_loaded() {
    if (gNetwork.gameStarted) {
        return;
    }
    if (!gNetwork.loaded) {
        gNetwork.loaded = true;
        send_int_packet(gNetwork.tcpSocket, PACKET_LOADED, true, sizeof(int));
    }
    if (gNetwork.playersLoaded) {
        gNetwork.gameStarted = true;
        gIsGamePaused = false;
    } else {
        gIsGamePaused = true;
    }
}

s32 currentNetworkPlayers = 0;

void spawn_network_players(f32* arg0, f32* arg1, f32 arg2) {
    if (currentNetworkPlayers) {
        return;
    }
    currentNetworkPlayers = 1;

    assign_player_control_types();

    u32 posX[] = { 0, 1, 3, 2, 5, 4, 7, 6 };
    u32 posZ[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    u32 pos[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    printf("----- Spawn Players ------\n");
    // for (size_t i = 0; i < NETWORK_MAX_PLAYERS; i++) {
    // s32 playerIndex = clients[i].slot;
    // printf("Spawned %s, in slot %d as char %d with flags 0x%X", clients[i].username, clients[i].slot,
    // gPlayers[playerIndex].nCharacter, gPlayers[playerIndex].nControlFlags);

    // if (gPlayers[playerIndex].nCharacter == -1) {
    //     printf("\n\nCORRUPT PLAYER DATA\n\n");
    // } else {
    // printf(" X: %d, Z: %d\n", arg0[D_80165270[posX[playerIndex]]],  arg1[D_80165270[posZ[playerIndex]]] + 250.0f) ;
    // spawn_player(&gPlayers[clients[i].slot], clients[i].slot, arg0[D_80165270[posX[clients[i].slot]]],
    // arg1[D_80165270[posZ[clients[i].slot]]] + 250.0f, arg2, 32768.0f, clients[i].character,
    // gPlayers[playerIndex].nControlFlags);
    // }
    //            This index isn't right like this ^

    // if localClient.Slots

    for (size_t i = 0; i < 8; i++) {
        printf("a %d, %d\n", clients[i].slot, clients[i].character);
    }

    for (size_t i = 0; i < NETWORK_MAX_PLAYERS; i++) {
        if (clients[i].hasAuthority) {
            spawn_player(&gPlayers[0], i, arg0[clients[i].slot], arg1[clients[i].slot] + 250.0f, arg2, 32768.0f,
                         clients[i].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        }
    }

    for (size_t i = 0; i < NETWORK_MAX_PLAYERS - 1; i++) {
        if (!clients[i].hasAuthority) {
            spawn_player(&gPlayers[clients[i].slot], i, arg0[clients[i].slot], arg1[clients[i].slot] + 250.0f, arg2,
                         32768.0f, clients[i].character,
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
        }
    }

    // spawn_player(&gPlayers[clients[0].slot], 0, arg0[clients[0].slot], arg1[clients[0].slot] + 250.0f, arg2,
    // 32768.0f, clients[0].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    // spawn_player(&gPlayers[clients[1].slot],     1, arg0[clients[1].slot], arg1[clients[1].slot] + 250.0f, arg2,
    // 32768.0f, clients[1].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[2].slot],   2, arg0[clients[2].slot], arg1[clients[2].slot] + 250.0f, arg2,
    // 32768.0f, clients[2].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[3].slot],    3, arg0[clients[3].slot], arg1[clients[3].slot] + 250.0f, arg2,
    // 32768.0f, clients[3].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[4].slot],    4, arg0[clients[4].slot], arg1[clients[4].slot] + 250.0f, arg2,
    // 32768.0f, clients[4].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[5].slot],     5, arg0[clients[5].slot], arg1[clients[5].slot] + 250.0f, arg2,
    // 32768.0f, clients[5].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[6].slot],   6, arg0[clients[6].slot], arg1[clients[6].slot] + 250.0f, arg2,
    // 32768.0f,  clients[6].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[7].slot],   7, arg0[clients[7].slot], arg1[clients[7].slot] + 250.0f, arg2,
    // 32768.0f, clients[7].character, PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);

    //}
    printf("--------------------------\n");
    // gPlayers[i].nControlFlags
    // spawn_player(&gPlayers[clients[0].slot], clients[0].slot, arg0[D_80165270[0]], arg1[D_80165270[0]] + 250.0f,
    // arg2, 32768.0f, gCharacterSelections[0], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    // spawn_player(&gPlayers[clients[1].slot],     clients[1].slot, arg0[D_80165270[1]], arg1[D_80165270[1]] + 250.0f,
    // arg2, 32768.0f, chooseKartAIPlayers[0], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[2].slot],   clients[2].slot, arg0[D_80165270[3]], arg1[D_80165270[2]] + 250.0f,
    // arg2, 32768.0f, chooseKartAIPlayers[1], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[3].slot],    clients[3].slot, arg0[D_80165270[2]], arg1[D_80165270[3]] + 250.0f,
    // arg2, 32768.0f, chooseKartAIPlayers[2], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[4].slot],    clients[4].slot, arg0[D_80165270[5]], arg1[D_80165270[4]] + 250.0f,
    // arg2, 32768.0f, chooseKartAIPlayers[3], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[5].slot],     clients[5].slot, arg0[D_80165270[4]], arg1[D_80165270[5]] + 250.0f,
    // arg2, 32768.0f, chooseKartAIPlayers[4], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[6].slot],   clients[6].slot, arg0[D_80165270[7]], arg1[D_80165270[6]] + 250.0f,
    // arg2, 32768.0f, chooseKartAIPlayers[5], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[7].slot],   clients[7].slot, arg0[D_80165270[6]], arg1[D_80165270[7]] + 250.0f,
    // arg2, 32768.0f, chooseKartAIPlayers[6], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
}
