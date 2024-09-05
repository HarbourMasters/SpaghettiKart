#include <libultraship.h>
#include "networking.h"
#include "main.h"

void replicate_player(const char* data) {
    u32 idx = clients[data[0]].slot;
    data += sizeof(u32);

    if (gSwappedSlot != -1 && idx == 0) {
        idx = gSwappedSlot;
    }
    //printf("Swapped slot: %d", idx);

    printf("player rpe: %f %f %f\n", (float)data[0x14], (float)data[0x18], (float)data[0x1C]);

    // gPlayers[idx].unk_002 = *(uint16_t*)data;
    // data += sizeof(uint16_t);
    // gPlayers[idx].unk_006 = *(uint16_t*)data;
    // data += sizeof(uint16_t);

    gPlayers[idx].pos[0] = *(f32*)data;
    data += sizeof(f32);
    gPlayers[idx].pos[1] = *(f32*)data;
    data += sizeof(f32);
    gPlayers[idx].pos[2] = *(f32*)data;
    data += sizeof(f32);

    gPlayers[idx].rotation[0] = *(int16_t*)data;
    data += sizeof(int16_t);
    gPlayers[idx].rotation[1] = *(int16_t*)data;
    data += sizeof(int16_t);
    gPlayers[idx].rotation[2] = *(int16_t*)data;


    //memcpy(&gPlayers[idx], data, sizeof(Player));

    // if (idx > NETWORK_MAX_PLAYERS) {
    //     memcpy(&gPlayers[idx], data, sizeof(Player));
    // }
}

void send_player(void) { // udp
    //send_data_packet(gNetwork.socket, PACKET_PLAYER, &gPlayerOne, sizeof(Player));
    sendNetworkPlayerPacket();
}

void ActorReplication() {
}

void ObjectReplication() {
}
