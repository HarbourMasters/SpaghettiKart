#include "BombKart.h"

extern "C" {
    #include "bomb_kart.h"
}

/**
 * BombKarts derive their Y position from the waypoint Y value or 'spawn_actor_on_surface' function
 */

OBombKart::OBombKart(uint16_t waypoint, uint16_t state, float unk_04, float x, float z, float unk_10, float unk_14) {}

void OBombKart::Init() {
   // BombKarts.emplace_back(this);
    //BombKarts.push_back(BombKarts(waypoint, state, unk_04, x, z, unk_10, unk_14));
}

void OBombKart::Update() {

}

void OBombKart::Render() {

}

void OBombKart::Explode() {

}