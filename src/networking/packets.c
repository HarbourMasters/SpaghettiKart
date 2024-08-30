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

#define BUFFER_SIZE 1024
void send_data_packet(TCPsocket socket, int type, const uint8_t* payload, size_t payload_size) {
    // uint8_t buffer[BUFFER_SIZE];
    // size_t packet_size = sizeof(type) + payload_size;
    // if (packet_size > BUFFER_SIZE) {
    //     fprintf(stderr, "Packet size exceeds buffer size\n");
    //     return;
    // }

    // memcpy(buffer, &type, sizeof(type));
    // memcpy(buffer + sizeof(type), payload, payload_size);

    // int len = SDLNet_TCP_Send(socket, buffer, packet_size);
    // if (len < packet_size) {
    //     fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    // }
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
