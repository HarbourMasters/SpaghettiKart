#ifndef NETWORKING_H
#define NETWORKING_H

#include <libultraship.h>
#include <common_structs.h>
#include <SDL2/SDL_net.h>
#include <windows.h>

#define NETWORK_MAX_PLAYERS 8
#define NETWORK_USERNAME_LENGTH 64

enum {
    PACKET_JOIN,
    PACKET_LEAVE,
    PACKET_MESSAGE,
    PACKET_LOADED,
    PACKET_READY_UP,
    PACKET_SET_CHARACTER,
    PACKET_COURSE,
    PACKET_PLAYER_ASSIGN_SLOTS,
    PACKET_START_SESSION,
    PACKET_PLAYER,
    PACKET_ACTOR,
    PACKET_OBJECT
};

typedef struct {
    char username[NETWORK_USERNAME_LENGTH];
    s32 slot;
    s32 isPlayer;
    s32 isAI;
    s32 character;
    s32 hasAuthority;
} NetworkClient;

extern Player *nLocalPlayer;
extern TCPsocket client;
extern TCPsocket remoteSocket;
extern NetworkClient dummyClient;
extern NetworkClient *localClient;
extern NetworkClient clients[];

extern s32 nAllPlayersLoaded;

/* Main Networking */
void networking_init(void);
void networking_update(void);
void networking_cleanup(void);
DWORD WINAPI networking_loop(LPVOID);
void handleReceivedData(const char *, size_t);
void sendInitialData(void);

/* Start Game */
void spawn_network_players(f32*,f32*,f32);
s32 network_all_players_loaded(void);
void set_course(const char *data);
void networking_start_session(const char *data);


/* Replication */
void replicate_player(const char* data);
void assign_player_slots(const char* data);

/* Packets */
void handleJoinPacket(const char *data);
void handleLeavePacket(const char *data);
void handleMessagePacket(const char *data);


#endif // NETWORKING_H