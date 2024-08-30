#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "networking.h"
#include "main.h"

#define UDP_PORT 64000
#define BUFFER_SIZE 1024

UDPNetwork gNetworkUDP = {
    .enabled = false,
    .socket = NULL,
    .address = { 0, 64011 },
    .localPlayer = &gPlayerOne,
    .isConnected = false,
    .loaded = false, // local player loaded?
    .playersLoaded = false,
    .gameStarted = false,
};

typedef struct {
    uint8_t type;
    uint32_t size;
    uint8_t data[4096];
} UdpPacket;

IPaddress serverIP;
UdpPacket* udpPacket;
UdpPacket* recvPacket;

SDL_Thread* sUDPNetworkThread;

// Function prototypes
void handleNetworkPlayerPacket(UdpPacket* packet);
void sendNetworkPlayerPacket(UdpPacket* packet);
void udp_server_cleanup(void);
void udp_server_loop(void);

void start_udp_client(const char* ip, uint16_t port) {
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialize SDLNet for UDP
    if (SDLNet_ResolveHost(&gNetworkUDP.address, ip, port) == -1) {
        printf("SDLNet_ResolveHost failed: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return;
    }

    // Create the UDP socket
    gNetworkUDP.socket = SDLNet_UDP_Open(0); // Bind to any available port
    if (!gNetworkUDP.socket) {
        printf("SDLNet_UDP_Open failed: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return;
    }

    // Create the socket set to monitor for incoming data
    SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
    if (!socketSet) {
        printf("SDLNet_AllocSocketSet failed: %s\n", SDLNet_GetError());
        SDLNet_UDP_Close(socketSet);
        return;
    }

    SDLNet_UDP_AddSocket(socketSet, gNetworkUDP.socket);

    // Start the UDP server thread
    sUDPNetworkThread = SDL_CreateThread(udp_server_loop, "UDPNetworkThread", NULL);
    if (!sUDPNetworkThread) {
        printf("SDL_CreateThread failed: %s\n", SDL_GetError());
        udp_server_cleanup();
        return;
    }

    printf("UDP Server started on %s:%d\n", ip, port);
}

void udp_server_loop(void) {
    while (1) {
        // Wait for incoming data
        if (SDLNet_UDP_Recv(gNetworkUDP.socket, recvPacket)) {
            UdpPacket* packet = (UdpPacket*) recvPacket;

            switch(packet->type) {
                // case PACKET_PLAYER:
                //     handleNetworkPlayerPacket(packet->data);
                //     break;
                case PACKET_PLAYER:
                    replicate_player(packet->data);
                    break;
                default:
                    printf("Unknown packet type: %d\n", packet->type);
                    break;
            }

            // Send a response if necessary
        }
        SDL_Delay(10); // Prevent CPU overload
        sendNetworkPlayerPacket(PACKET_PLAYER);
    }
}

void handleNetworkPlayerPacket(UdpPacket* packet) {
    printf("Received packet from player 0x%X)\n", packet->data);
}

void sendNetworkPlayerPacket(UdpPacket* packet) {
    // gNetworkUDP->address.host = dest->host;
    // gNetworkUDP->address.port = dest->port;
    // gNetworkUDP->len = sizeof(UdpPacket);
    // memcpy(udpPacket->data, packet, sizeof(UdpPacket));

    //unsigned data

    //memcpy();

    UDPpacket test = {
        .channel = -1,
        .data = (uint8_t*) &gPlayers[0],
        .len = sizeof(Player),
        .maxlen = 4096,
        .status = 1,
        .address = gNetworkUDP.address,
    };

    // Send the packet
    if (SDLNet_UDP_Send(gNetworkUDP.socket, -1, &test) == 0) {
        printf("SDLNet_UDP_Send failed: %s\n", SDLNet_GetError());
    }
}

void udp_server_cleanup() {
    SDLNet_FreePacket(udpPacket);
    SDLNet_FreePacket(recvPacket);
    SDLNet_UDP_Close(gNetworkUDP.socket);
    SDLNet_Quit();
}
