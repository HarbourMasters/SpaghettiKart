#ifndef NETWORKING_H
#define NETWORKING_H

#include <libultraship.h>
#include <common_structs.h>
#include <SDL2/SDL_net.h>

#define NETWORK_MAX_PLAYERS 8
#define NETWORK_USERNAME_LENGTH 32

enum {
    PACKET_JOIN,
    PACKET_LEAVE,
    PACKET_MESSAGE,
    PACKET_LOADED,
    PACKET_READY_UP,
    PACKET_SET_CHARACTER,
    PACKET_COURSE_VOTE,
    PACKET_SET_COURSE,
    PACKET_PLAYER_ASSIGN_SLOTS,
    PACKET_START_SESSION,
    PACKET_PLAYER,
    PACKET_ACTOR,
    PACKET_OBJECT
};

typedef struct {
    bool enabled; // Enables networking
    TCPsocket tcpSocket;
    IPaddress address;
    Player* localPlayer;
    bool isConnected;
    bool loaded;
    bool playersLoaded; // Are all players loaded?
    bool gameStarted;
    uint32_t cupVote;
    uint32_t character;
} Network;

extern Network gNetwork;

typedef struct {
    char username[NETWORK_USERNAME_LENGTH];
    s32 slot;
    s32 isPlayer;
    s32 isAI;
    s32 character;
    s32 hasAuthority;
} NetworkClient;

extern NetworkClient dummyClient;
extern NetworkClient* localClient;
extern NetworkClient clients[];

/* Main Networking */
void ConnectToServer(char* ip, uint16_t port, char* username);
void networking_init(char* ip, uint16_t port);
void networking_update(void);
void networking_ready_up(bool);
void networking_cleanup(SDLNet_SocketSet);
int networking_loop(void*);
void handleReceivedData(const char*, size_t);
void set_username(const char* username);
void network_character_vote(uint32_t course);
void network_cup_vote(uint32_t course);
void networking_disconnect(void);

/* Start Game */
void spawn_network_players(f32*, f32*, f32);
s32 network_all_players_loaded(void);
void set_course(const char* data);
void networking_start_session(const char* data);

/* Replication */
void replicate_player(const char* data);
void assign_player_slots(const char* data);

/* Packets */
void send_int_packet(TCPsocket socket, uint8_t type, uint32_t payload, uint16_t size);
void handleJoinPacket(const char* data);
void handleLeavePacket(const char* data);
void handleMessagePacket(const char* data);

void handle_start_game(void);
void send_str_packet(TCPsocket, uint8_t, const char*);

#endif // NETWORKING_H
