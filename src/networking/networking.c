#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "networking.h"

#define SERVER_PORT 64010
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

NetworkClient dummyClient; // For use before server sends the real client

NetworkClient *localClient = NULL;
NetworkClient clients[NETWORK_MAX_PLAYERS];

// Global variables
SDLNet_SocketSet set;
TCPsocket client;

int isConnected = 0;

void set_username(char* username) {
    strcpy(localClient->username, username);
}

void send_json_string(TCPsocket socket, const char *str) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\"%s\"", str); // Format as JSON string
    int len = SDLNet_TCP_Send(socket, buffer, strlen(buffer));
    if (len < strlen(buffer)) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }
}

// void send_str_packet(TCPsocket socket, int type, const char *payload) {
//     char buffer[BUFFER_SIZE];
//     snprintf(buffer, BUFFER_SIZE, "%d:%s\n", type, payload); // Format as TYPE:Payload
//     int len = SDLNet_TCP_Send(socket, buffer, strlen(buffer));
//     if (len < strlen(buffer)) {
//         fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
//     }
// }

void networking_loop() {
    // Check for ready sockets
    int numReadySockets = SDLNet_CheckSockets(set, 0);
    if (numReadySockets == -1) {
        fprintf(stderr, "SDLNet_CheckSockets: %s\n", SDLNet_GetError());
        return;
    }

    // Receive packets
    if (numReadySockets > 0) {
        if (SDLNet_SocketReady(client)) {
            char buffer[BUFFER_SIZE];
            int len = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
            if (len <= 0) {
                fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                return;
            } else {
                handleReceivedData(buffer, len);
            }
        }
    }

    // Send packets
    if (!isConnected) {
        isConnected = 1;
        send_str_packet(client, PACKET_JOIN, localClient->username);
        send_str_packet(client, PACKET_MESSAGE, "a message");
        send_str_packet(client, PACKET_MESSAGE, "another message");
        send_int_packet(client, PACKET_READY_UP, 1, sizeof(int));
        send_int_packet(client, PACKET_SET_CHARACTER, 2, sizeof(int));
    } else {
        //send_str_packet(client, PACKET_MESSAGE, "another message2");
    }
}

void handleReceivedData(const char *buffer, size_t bufSize) {
    if (bufSize < 4) {
        printf("Malformed packet received: too short\n");
        return;
    }

    uint8_t type = (uint8_t)buffer[0];
    uint16_t length = (uint16_t)buffer[1] | ((uint16_t)buffer[2] << 8);

    // Validate buffer size
    if (bufSize < 3 + length) {
        printf("Malformed packet received: declared length exceeds buffer size\n");
        return;
    }

    // Point to the data
    const char *data = buffer + 3;

    switch (type) {
        case PACKET_JOIN:
            handleJoinPacket(data);
            break;
        case PACKET_LEAVE:
            handleLeavePacket(data);
            break;
        case PACKET_MESSAGE:
            printf("Recived Msg\n");
            handleMessagePacket(data);
            break;
        case PACKET_LOADED:
            handle_start_game(data);
            break;
        case PACKET_PLAYER:
            replicate_player(data);
            break;
        case PACKET_COURSE:
            set_course(data);
            break;
        case PACKET_PLAYER_ASSIGN_SLOTS:
            printf("\n\n\nASSIGN SLOT\n\n\n");
            assign_player_slots(data);
            break;
        case PACKET_START_SESSION:
            networking_start_session(data);
            break;
        default:
            printf("Unknown packet type received: %d\n", type);
            break;
    }
}

void networking_init() {
    if (SDL_Init(0) == -1) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (SDLNet_Init() == -1) {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, SERVER_IP, SERVER_PORT) == -1) {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    client = SDLNet_TCP_Open(&ip);
    if (!client) {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    set = SDLNet_AllocSocketSet(1);
    if (!set) {
        fprintf(stderr, "SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
        SDLNet_TCP_Close(client);
        SDLNet_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    if (SDLNet_TCP_AddSocket(set, client) == -1) {
        fprintf(stderr, "SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
        SDLNet_TCP_Close(client);
        SDLNet_FreeSocketSet(set);
        SDLNet_Quit();
        SDL_Quit;
    }

    localClient = &dummyClient; // Temporary until server sends the real data
    set_username("TestUser");
    printf("\nNetworking Initialized\n\n");
}

void networking_cleanup() {
    send_str_packet(client, PACKET_LEAVE, localClient->username);
    SDLNet_TCP_Close(client);
    SDLNet_FreeSocketSet(set);
    SDLNet_Quit();
    SDL_Quit;
}