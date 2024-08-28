#include <libultraship.h>
#include "networking.h"
#include "main.h"

void replicate_player(const char* data) {
    u32 idx = clients[data[0]].slot;

    if (gSwappedSlot != -1 && idx == 0) {
        idx = gSwappedSlot;
    }
    printf("Swapped slot: %d", idx);


    data += sizeof(u32);
    printf("pos0; %f ", (f32)*data);
    gPlayers[idx].pos[0] = (f32) *data;
    data += sizeof(f32);
    printf("%f ", (f32)*data);
    gPlayers[idx].pos[1] = (f32) *data;
    data += sizeof(f32);
    printf("%f\n", (f32)*data);

    gPlayers[idx].pos[2] = (f32) *data;


    // if (idx > NETWORK_MAX_PLAYERS) {
    //     memcpy(&gPlayers[idx], data, sizeof(Player));
    // }
}

void send_player(void) {
    send_data_packet(gNetwork.tcpSocket, PACKET_PLAYER, &gPlayerOne->pos, sizeof(Vec3f));
}

void ActorReplication() {
}

void ObjectReplication() {
}
