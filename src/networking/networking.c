#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <windows.h>

#include "networking.h"

#define BUFFER_SIZE 10240

s32 gNetworkingEnabled = false;
char* gNetworkingIp = "127.0.0.1";
u32 gNetworkingPort = 64010;

NetworkClient dummyClient; // For use before server sends the real client

NetworkClient *localClient = NULL;
NetworkClient clients[NETWORK_MAX_PLAYERS];

// Global variables
SDLNet_SocketSet set;
TCPsocket client;

int isConnected = 0;

// void set_username(char* username) {
//     strcpy(localClient->username, username);
// }


void set_username(const char *username) {
    srand(time(NULL) + rand()); // Use a different seed for each call
    int random_number = rand() % 10000; // Generate a random number between 0 and 9999
    snprintf(localClient->username, sizeof(localClient->username), "%s%d", username, random_number);
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

//uint32_t count = 0;
// Original
// void networking_loop() {
//     // Check for ready sockets

//     while (set) {


//         int numReadySockets = SDLNet_CheckSockets(set, 0);
//         if (numReadySockets == -1) {
//             fprintf(stderr, "SDLNet_CheckSockets: %s\n", SDLNet_GetError());
//             return;
//         }
//         if (numReadySockets == 0) {
//             continue;
//         }

//         // Receive packets
//         if (numReadySockets > 0) {
//             if (SDLNet_SocketReady(client)) {
//                 char buffer[BUFFER_SIZE];
//                 int len = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
//                 if (len <= 0) {
//                     fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
//                     return;
//                 } else {
//                     handleReceivedData(buffer, len);
//                 }
//             }
//         }
//     }

//     // Send packets
//     //char str[20];
//     //sprintf(str, "packet: %d", +count++);
//     if (!isConnected) {
//         isConnected = 1;
//         send_str_packet(client, PACKET_JOIN, localClient->username);
//         send_str_packet(client, PACKET_MESSAGE, "a message");
//         send_str_packet(client, PACKET_MESSAGE, "another message");
//         send_int_packet(client, PACKET_READY_UP, 1, sizeof(int));
//         send_int_packet(client, PACKET_SET_CHARACTER, 2, sizeof(int));
//     } else {
//         //send_str_packet(client, PACKET_MESSAGE, str);
//     }
// }

// void networking_loop() {
//     if (!isConnected) {
//         client = SDLNet_TCP_Open(&ip);
//         if (client) {
//             isConnected = 1;
//             fprintf(stdout, "Connection to server established!\n");
//         } else {
//             fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
//             return;
//         }
//     }

//     SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
//     SDLNet_TCP_AddSocket(socketSet, client);

//     while (isConnected && client) {
//         int numReadySockets = SDLNet_CheckSockets(socketSet, 0);
//         if (numReadySockets == -1) {
//             fprintf(stderr, "SDLNet_CheckSockets: %s\n", SDLNet_GetError());
//             isConnected = 0;
//             SDLNet_TCP_Close(client);
//             return;
//         }

//         if (numReadySockets > 0) {
//             if (SDLNet_SocketReady(client)) {
//                 char buffer[BUFFER_SIZE];
//                 int len = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
//                 if (len <= 0) {
//                     fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
//                     isConnected = 0;
//                     SDLNet_TCP_Close(client);
//                     return;
//                 } else {
//                     handleReceivedData(buffer, len);
//                 }
//             }
//         }

//         // Process other game logic and handle events
//         // SDL_Event event;
//         // while (SDL_PollEvent(&event)) {
//         //     if (event.type == SDL_QUIT) {
//         //         isConnected = 0;
//         //         SDLNet_TCP_Close(client);
//         //         return;
//         //     }
//         // }

//         SDL_Delay(10);  // Add a short delay to control the loop rate
//     }

//     SDLNet_FreeSocketSet(socketSet);
// }

IPaddress ip;
HANDLE remoteThreadReceive;
DWORD threadID;
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

    if (SDLNet_ResolveHost(&ip, gNetworkingIp, gNetworkingPort) == -1) {
        printf("[GameInteractor] SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    }

    // Ensure no thread is already running
    if (remoteThreadReceive != NULL) {
        WaitForSingleObject(remoteThreadReceive, INFINITE);
        CloseHandle(remoteThreadReceive);
        remoteThreadReceive = NULL;
    }

    remoteThreadReceive = CreateThread(
        NULL,                   // default security attributes
        0,                      // default stack size
        networking_loop,      // thread function
        NULL,                   // argument to thread function
        0,                      // default creation flags
        &threadID               // receive thread identifier
    );

    if (remoteThreadReceive == NULL) {
        printf("CreateThread failed: %d\n", GetLastError());
        exit(EXIT_FAILURE);
    }

    //remoteThreadReceive = std::thread(&GameInteractor::ReceiveFromServer, this);
}


bool isRemoteInteractorConnected;
TCPsocket remoteSocket;
int isNetworkingThreadEnabled = 1;
void (*remoteConnectedHandler)(void);
void (*remoteDisconnectedHandler)(void);
bool sendInitialPackets = 0;
DWORD WINAPI networking_loop(LPVOID arg) {
    while (isNetworkingThreadEnabled) {
        while (!isConnected && isNetworkingThreadEnabled) { // && isRemoteInteractorEnabled) {
            printf("[SpaghettiOnline] Attempting to make connection to server...\n");
            remoteSocket = SDLNet_TCP_Open(&ip);

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
            if (remoteDisconnectedHandler) {
                remoteDisconnectedHandler();
            }
            printf("[SpaghettiOnline] Ending receiving thread...\n");
        }

    }
    return 0;
}


void sendInitialData(void) {
    if (!sendInitialPackets) {
        if (remoteSocket != NULL) {
            sendInitialPackets = true;
            printf("SENDING PACKETS!\n");
            localClient = &dummyClient; // Temporary until server sends the real data
            set_username("TestUser");
            send_str_packet(remoteSocket, PACKET_JOIN, localClient->username);
            send_str_packet(remoteSocket, PACKET_MESSAGE, "a message");
            send_str_packet(remoteSocket, PACKET_MESSAGE, "another message");
            send_int_packet(remoteSocket, PACKET_SET_CHARACTER, 2, sizeof(int));
            send_int_packet(remoteSocket, PACKET_READY_UP, 1, sizeof(int));
        }
    }
}

void networking_ready_up(bool value) {
    send_int_packet(remoteSocket, PACKET_READY_UP, value, sizeof(int));
}

// void networking_loop() {
//     // Check for ready sockets
//     int numReadySockets = SDLNet_CheckSockets(set, 0);
//     if (numReadySockets == -1) {
//         fprintf(stderr, "SDLNet_CheckSockets: %s\n", SDLNet_GetError());
//         return;
//     }

//     // Receive packets
//     while (numReadySockets > 0) {
//         if (SDLNet_SocketReady(client)) {
//             char buffer[BUFFER_SIZE];
//             int len = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
//             if (len <= 0) {
//                 fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
//                 return;
//             } else {
//                 handleReceivedData(buffer, len);
//             }
//         }
//         // Re-check for more ready sockets
//         numReadySockets = SDLNet_CheckSockets(set, 0);
//     }

//     // Send packets
//     //char str[20];
//     //sprintf(str, "packet: %d", count++);
//     if (!isConnected) {
//         isConnected = 1;
//         send_str_packet(client, PACKET_JOIN, localClient->username);
//         send_str_packet(client, PACKET_MESSAGE, "a message");
//         send_str_packet(client, PACKET_MESSAGE, "another message");
//         send_int_packet(client, PACKET_READY_UP, 1, sizeof(int));
//         send_int_packet(client, PACKET_SET_CHARACTER, 2, sizeof(int));
//     } else {
//         //send_str_packet(client, PACKET_MESSAGE, str)
//     }
// }

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

// void networking_init() {
//     if (SDL_Init(0) == -1) {
//         fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
//         exit(EXIT_FAILURE);
//     }

//     if (SDLNet_Init() == -1) {
//         fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
//         SDL_Quit();
//         exit(EXIT_FAILURE);
//     }

//     if (SDLNet_ResolveHost(&ip, SERVER_IP, SERVER_PORT) == -1) {
//         fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
//         SDLNet_Quit();
//         SDL_Quit();
//         exit(EXIT_FAILURE);
//     }

//     client = SDLNet_TCP_Open(&ip);
//     if (!client) {
//         fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
//         SDLNet_Quit();
//         SDL_Quit();
//         exit(EXIT_FAILURE);
//     }

//     set = SDLNet_AllocSocketSet(1);
//     if (!set) {
//         fprintf(stderr, "SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
//         SDLNet_TCP_Close(client);
//         SDLNet_Quit();
//         SDL_Quit();
//         exit(EXIT_FAILURE);
//     }

//     if (SDLNet_TCP_AddSocket(set, client) == -1) {
//         fprintf(stderr, "SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
//         SDLNet_TCP_Close(client);
//         SDLNet_FreeSocketSet(set);
//         SDLNet_Quit();
//         SDL_Quit;
//     }

//     localClient = &dummyClient; // Temporary until server sends the real data
//     set_username("TestUser");
//     printf("\nNetworking Initialized\n\n");
// }

void networking_cleanup() {
    send_str_packet(remoteSocket, PACKET_LEAVE, localClient->username);
    SDLNet_TCP_Close(remoteSocket);
    SDLNet_FreeSocketSet(set);
    SDLNet_Quit();
    SDL_Quit;
}