#include <libultraship.h>
#include <SDL2/SDL_net.h>
#include "networking.h"
#include "code_800029B0.h"

void handleJoinPacket(const char* data) {
    printf("Join packet received: %s\n", data);
    // Handle join logic here
}

void handleLeavePacket(const char* data) {
    printf("Leave packet received: %s\n", data);
    // Handle leave logic here
}

void handleMessagePacket(const char* data) {
    printf("%s\n", data);
    // Handle message logic here
}

void handleMessageNumberPacket(const char* data) {
    printf("%s\n", data);
    // Handle message logic here
}

void handle_start_game() {
    gNetwork.playersLoaded = true;
    gIsGamePaused = false;
}

extern Player gPlayers[8];

#define BUFFER_SIZE 4096
void send_data_packet(TCPsocket socket, uint8_t type, const uint8_t* payload, size_t payload_size) {
    uint8_t buffer[BUFFER_SIZE];
    size_t offset = 0;

    // Write the type byte
    buffer[offset] = type;
    offset += sizeof(uint8_t);

    // Write the payload size (ensure it's within range)
    if (payload_size > UINT16_MAX) {
        fprintf(stderr, "Payload size exceeds maximum allowed size\n");
        return;
    }
    uint16_t size = (uint16_t)22;
    *(uint16_t *)(buffer+offset) = size;
    offset += sizeof(uint16_t);

    // Ensure there's enough space for player data and payload
    // size_t packet_size = sizeof(uint8_t) + sizeof(uint16_t) + payload_size;
    // if (packet_size > BUFFER_SIZE) {
    //     fprintf(stderr, "Packet size exceeds buffer size\n");
    //     return;
    // }
    *(uint16_t*)(buffer+offset) = gPlayers[0].unk_002;
    offset += sizeof(uint16_t);
    *(uint16_t*)(buffer+offset) = gPlayers[0].unk_006;
    offset += sizeof(uint16_t);

    // Pos test
    *(f32*)(buffer+offset) = gPlayers[0].pos[0];
    offset += sizeof(f32);
    *(f32*)(buffer+offset) = gPlayers[0].pos[1];
    offset += sizeof(f32);
    *(f32*)(buffer+offset) = gPlayers[0].pos[2];
    offset += sizeof(f32);

    *(int16_t*)(buffer+offset) = gPlayers[0].rotation[0];
    offset += sizeof(int16_t);
    *(int16_t*)(buffer+offset) = gPlayers[0].rotation[1];
    offset += sizeof(int16_t);
    *(int16_t*)(buffer+offset) = gPlayers[0].rotation[2];
    offset += sizeof(int16_t);

    // memcpy(buffer + offset, gPlayers, sizeof(Player));
    // offset += sizeof(Player);

    // // Write player position data
    // memcpy(buffer + offset, 0x12345678, sizeof(int32_t));
    // offset += sizeof(f32);
    // memcpy(buffer + offset, 0x11223344, sizeof(int32_t));
    // offset += sizeof(f32);
    // memcpy(buffer + offset, 0x99887766, sizeof(int32_t));
    // offset += sizeof(f32);

    // Write the payload data
    // memcpy(buffer + offset, payload, payload_size);
    // offset += payload_size;

    // Send the buffer through the socket
    int len = SDLNet_TCP_Send(socket, buffer, offset);
    if (len < (int)offset) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }
}

void send_str_packet(TCPsocket socket, uint8_t type, const char* payload) {
    char buffer[BUFFER_SIZE];
    int offset = 0;

    // Copy the integer type into the buffer
    buffer[offset] = type;
    offset += sizeof(uint8_t);

    // Copy the payload into the buffer
    int size = strlen(payload);
    if (offset + size >= BUFFER_SIZE) {
        fprintf(stderr, "Payload is too large to fit in buffer\n");
        return;
    }

    *(uint16_t*) (buffer + offset) = size;
    offset += sizeof(uint16_t);

    memcpy(buffer + offset, payload, size);
    offset += size;

    // Send the buffer through the socket
    int len = SDLNet_TCP_Send(socket, buffer, offset);
    if (len < offset) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }
}

// void send_packet(TCPsocket socket, uint8_t type, const char *payload, uint16_t size) {
//     // Ensure the buffer is large enough to hold the type, colon, and payload
//     if (sizeof(int) + 1 + size > BUFFER_SIZE) {
//         fprintf(stderr, "Sending data too big for the buffer\n");
//         return;
//     }

//     char buffer[BUFFER_SIZE];
//     int offset = 0;

//     // Copy the type into the buffer
//     buffer[offset] = type;
//     offset += sizeof(uint8_t);

//     // Copy the payload into the buffer
//     memcpy(buffer + offset, payload, size);
//     offset += size;

//     // Send the buffer through the socket
//     int len = SDLNet_TCP_Send(socket, buffer, offset);
//     if (len < offset) {
//         fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
//     }
// }

void send_int_packet(TCPsocket socket, uint8_t type, uint32_t payload, uint16_t size) {
    // Ensure the buffer is large enough to hold the type, colon, and payload
    if (sizeof(int) + 1 + size > BUFFER_SIZE) {
        fprintf(stderr, "Sending data too big for the buffer\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    int offset = 0;

    // Write the packet type into the buffer
    buffer[offset] = type;
    offset += sizeof(uint8_t);

    // Write the data size into the buffer
    *(uint16_t*) (buffer + offset) = size;
    offset += sizeof(uint16_t);

    // Write the payload into the buffer
    *(uint32_t*) (buffer + offset) = payload;
    offset += sizeof(uint32_t);

    // Send the buffer through the socket
    int len = SDLNet_TCP_Send(socket, buffer, offset);
    if (len < offset) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }
}

void send_udp_join_packet(TCPsocket socket, uint8_t type, IPaddress *ip) {
    // Ensure the buffer is large enough to hold the type, colon, and payload
    if (sizeof(int) + 1 + sizeof(IPaddress) > BUFFER_SIZE) {
        fprintf(stderr, "Sending data too big for the buffer\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    int offset = 0;

    // Write the packet type into the buffer
    buffer[offset] = type;
    offset += sizeof(uint8_t);

    // Write the data size into the buffer
    *(uint16_t*) (buffer + offset) = sizeof(IPaddress);
    offset += sizeof(uint16_t);

    // Write the payload into the buffer
    *(uint32_t*) (buffer + offset) = ip->host;
    offset += sizeof(uint32_t);

    *(uint16_t*) (buffer + offset) = ip->port;
    offset += sizeof(uint16_t);

    // Send the buffer through the socket
    int len = SDLNet_TCP_Send(socket, buffer, offset);
    if (len < offset) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }
}
