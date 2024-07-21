#include <libultraship.h>
#include <defines.h>
#include "main.h"
#include "spawn_players.h"
#include "networking.h"
#include "code_800029B0.h"
#include "menus.h"

Player *nLocalPlayer = &gPlayerFive;

s32 nAllPlayersLoaded = true;
s32 nGameStarted = false;
s32 localPlayerLoaded = false;

// PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN
// PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI

void assign_player_control_types(void) {
    printf("Start network game\n");
    for (size_t i = 0; i < NETWORK_MAX_PLAYERS; i++) {
        s32 playerIndex = clients[i].slot;

        if (clients[i].isAI) {
            gPlayers[playerIndex].nControlFlags = 0;
        } else if (clients[i].hasAuthority) {
            gPlayers[playerIndex].nControlFlags = PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE; // Requires server replication to move.

        }
        gPlayers[playerIndex].nCharacter = clients[i].character;
        gPlayers[playerIndex].nStartingRank = clients[i].slot;

        printf("Player Slot %d, character %d\n", playerIndex, clients[i].character);
    }

    // Get and Set player
    // nLocalPlayer = &gPlayers[i];
}

void set_course(const char* data) {
    if (data != NULL) {
        gCurrentCourseId = 0; //*(s16 *) data;
    }
}

void assign_player_slots(const char* data) {
    int offset = 0;
    printf("Assign Player Slots\n");
    for (int i = 0; i < NETWORK_MAX_PLAYERS; ++i) {
        // Read username

        int usernameLength = *(int*)(data + offset);
        offset += sizeof(int);

        memcpy(clients[i].username, data + offset, usernameLength);
        offset += usernameLength;

        // Read slot
        clients[i].slot = *(int*)(data + offset);
        offset += sizeof(int);

        // Read isPlayer
        clients[i].isPlayer = *(int*)(data + offset);
        offset += sizeof(int);

        // Read IsAI
        clients[i].isAI = *(int*)(data + offset);
        offset += sizeof(int);

        // Read character
        clients[i].character = *(int*)(data + offset);
        offset += sizeof(int);

        gCharacterSelections[i] = clients[i].character;

        // Read hasAuthority
        clients[i].hasAuthority = *(int*)(data + offset);

        if (clients[i].hasAuthority) {
            localClient = &clients[i];
            printf("You have been assigned slot %d\n",  clients[i].slot);
        }

        offset += sizeof(int);
    }

}

// Duplicate of setup_race
void networking_start_session(const char *data) {
    printf("Start session\n");
    //gIsInQuitToMenuTransition = 1;
    //gQuitToMenuTransitionCounter = 5;
    gGotoMode = RACING; // Enter race state
    gGamestateNext = RACING;
    gCCSelection = CC_150;
    gCupSelection = FLOWER_CUP;
    gCupCourseSelection = CUP_COURSE_FOUR;
    gScreenModeSelection = SCREEN_MODE_1P;
    gModeSelection = GRAND_PRIX;
    gPlayerCount = 1;
    gCurrentCourseId = 0;
}

// Wait for all players to load
s32 network_all_players_loaded() {
    if (nGameStarted) { return; }
    if (!localPlayerLoaded) {
        localPlayerLoaded = true;
        send_int_packet(client, PACKET_LOADED, true);
    }
    if (nAllPlayersLoaded) {
        nGameStarted = true;
        gIsGamePaused = false;
    } else {
        gIsGamePaused = true;
    }
}

void spawn_network_players(f32 *arg0, f32 *arg1, f32 arg2) {

    assign_player_control_types();
    s32 playerIndex = clients[0].slot;

    printf("----- Spawn Players ------\n");
    for (size_t i = 0; i < NETWORK_MAX_PLAYERS; i++) {
        printf("Spawned %s, in slot %d as char %d with flags 0x%X\n", clients[i].username, clients[i].slot, gPlayers[playerIndex].nCharacter, gPlayers[playerIndex].nControlFlags);
        printf("Direct Character: %d\n", clients[i].character);
        
        if (gPlayers[playerIndex].nCharacter == -1) {
            printf("\n\nCORRUPT PLAYER DATA\n\n");
        } else {
            spawn_player(&gPlayers[playerIndex], playerIndex, arg0[D_80165270[playerIndex]], arg1[D_80165270[playerIndex]] + 250.0f, arg2, 32768.0f, gPlayers[playerIndex].nCharacter, gPlayers[playerIndex].nControlFlags);
        }
        //           This index isn't right like this ^
    }
    printf("--------------------------\n");
    // gPlayers[i].nControlFlags
    // spawn_player(&gPlayers[clients[0].slot], clients[0].slot, arg0[D_80165270[0]], arg1[D_80165270[0]] + 250.0f, arg2, 32768.0f, gCharacterSelections[0], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    // spawn_player(&gPlayers[clients[1].slot],     clients[1].slot, arg0[D_80165270[1]], arg1[D_80165270[1]] + 250.0f, arg2, 32768.0f, chooseKartAIPlayers[0], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[2].slot],   clients[2].slot, arg0[D_80165270[3]], arg1[D_80165270[2]] + 250.0f, arg2, 32768.0f, chooseKartAIPlayers[1], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[3].slot],    clients[3].slot, arg0[D_80165270[2]], arg1[D_80165270[3]] + 250.0f, arg2, 32768.0f, chooseKartAIPlayers[2], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[4].slot],    clients[4].slot, arg0[D_80165270[5]], arg1[D_80165270[4]] + 250.0f, arg2, 32768.0f, chooseKartAIPlayers[3], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[5].slot],     clients[5].slot, arg0[D_80165270[4]], arg1[D_80165270[5]] + 250.0f, arg2, 32768.0f, chooseKartAIPlayers[4], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[6].slot],   clients[6].slot, arg0[D_80165270[7]], arg1[D_80165270[6]] + 250.0f, arg2, 32768.0f, chooseKartAIPlayers[5], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    // spawn_player(&gPlayers[clients[7].slot],   clients[7].slot, arg0[D_80165270[6]], arg1[D_80165270[7]] + 250.0f, arg2, 32768.0f, chooseKartAIPlayers[6], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);

}