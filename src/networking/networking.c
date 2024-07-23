#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <windows.h>

#include "networking.h"

#define BUFFER_SIZE 10240

s32 gNetworkingEnabled = false;

NetworkClient dummyClient; // For use before server sends the real client

NetworkClient *localClient = NULL;
NetworkClient clients[NETWORK_MAX_PLAYERS];

// Global variables
SDLNet_SocketSet set;
TCPsocket client;

bool isConnected = 0;

IPaddress address;
HANDLE sNetworkThread;
DWORD threadID;
bool threadStarted = false;

TCPsocket remoteSocket;
int isNetworkingThreadEnabled = true;
void (*remoteConnectedHandler)(void);

void ConnectToServer(char* ip, uint16_t port, char *username) {
    if (!threadStarted) {
        threadStarted = true;
        networking_init(ip, port);

        SDL_Delay(20);

        localClient = &dummyClient; // Temporary until server sends the real data
        set_username(username);
        send_str_packet(remoteSocket, PACKET_JOIN, localClient->username);
        send_int_packet(remoteSocket, PACKET_SET_CHARACTER, 2, sizeof(uint32_t));
        send_str_packet(remoteSocket, PACKET_MESSAGE, "a message");
        send_str_packet(remoteSocket, PACKET_MESSAGE, "another message");
    }
}

void set_username(const char *username) {
    //srand(time(NULL) + rand()); // Use a different seed for each call
    //int random_number = rand() % 10000; // Generate a random number between 0 and 9999
    //snprintf(localClient->username, sizeof(localClient->username), "%s%d", username, random_number);

    strncpy(localClient->username, username, sizeof(localClient->username) - 1);
    localClient->username[sizeof(localClient->username) - 1] = '\0';  // Ensure null-termination
}

void networking_init(char* ip, uint16_t port) {
    if (SDL_Init(0) == -1) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (SDLNet_Init() == -1) {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    if (SDLNet_ResolveHost(&address, ip, port) == -1) {
        printf("[GameInteractor] SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    }

    // Ensure no thread is already running
    if (sNetworkThread != NULL) {
        WaitForSingleObject(sNetworkThread, INFINITE);
        CloseHandle(sNetworkThread);
        sNetworkThread = NULL;
    }

    sNetworkThread = CreateThread(
        NULL,                   // default security attributes
        0,                      // default stack size
        networking_loop,      // thread function
        NULL,                   // argument to thread function
        0,                      // default creation flags
        &threadID               // receive thread identifier
    );

    if (sNetworkThread == NULL) {
        printf("CreateThread failed: %d\n", GetLastError());
        exit(EXIT_FAILURE);
    }

    //sNetworkThread = std::thread(&GameInteractor::ReceiveFromServer, this);
}

DWORD WINAPI networking_loop(LPVOID arg) {
    while (isNetworkingThreadEnabled) {
        while (!isConnected && isNetworkingThreadEnabled) { // && isRemoteInteractorEnabled) {
            printf("[SpaghettiOnline] Attempting to make connection to server...\n");
            remoteSocket = SDLNet_TCP_Open(&address);

            if (remoteSocket) {
                isConnected = true;
                printf("[SpaghettiOnline] Connection to server established!\n");

                if (remoteConnectedHandler) {
                    remoteConnectedHandler();
                }
                break;
            }
        }

        SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
        if (remoteSocket) {
            SDLNet_TCP_AddSocket(socketSet, remoteSocket);
        }

        // Listen to socket messages
        while (isConnected && remoteSocket && isNetworkingThreadEnabled) { // && isRemoteInteractorEnabled) {
            // we check first if socket has data, to not block in the TCP_Recv
            int socketsReady = SDLNet_CheckSockets(socketSet, 0);

            if (socketsReady == -1) {
                printf("[SpaghettiOnline] SDLNet_CheckSockets: %s\n", SDLNet_GetError());
                break;
            }

            if (socketsReady == 0) {
                continue;
            }

            char remoteDataReceived[512];
            memset(remoteDataReceived, 0, sizeof(remoteDataReceived));
            int len = SDLNet_TCP_Recv(remoteSocket, &remoteDataReceived, sizeof(remoteDataReceived));
            if (!len || !remoteSocket || len == -1) {
                printf("[SpaghettiOnline] SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                break;
            }

            handleReceivedData(remoteDataReceived, len); //HandleRemoteData(remoteDataReceived);

            // receivedData.append(remoteDataReceived, len);

            // // Process all complete packets
            // size_t delimiterPos = receivedData.find('\0');
            // while (delimiterPos != std::string::npos) {
            //     // Extract the complete packet until the delimiter
            //     std::string packet = receivedData.substr(0, delimiterPos);
            //     // Remove the packet (including the delimiter) from the received data
            //     receivedData.erase(0, delimiterPos + 1);
            //     HandleRemoteJson(packet);
            //     // Find the next delimiter
            //     delimiterPos = receivedData.find('\0');
            // }
        }

        if (isConnected) {
            SDLNet_TCP_Close(remoteSocket);
            isConnected = false;
            if (networking_cleanup) {
                networking_cleanup();
            }
            printf("[SpaghettiOnline] Ending receiving thread...\n");
        }

    }
    return 0;
}

void networking_ready_up(bool value) {
    send_int_packet(remoteSocket, PACKET_READY_UP, value, sizeof(int));
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

void networking_cleanup(void) {
    send_str_packet(remoteSocket, PACKET_LEAVE, localClient->username);
    SDLNet_TCP_Close(remoteSocket);
    SDLNet_FreeSocketSet(set);
    SDLNet_Quit();
    SDL_Quit;
}