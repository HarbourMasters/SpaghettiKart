#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <libultraship/libultra/gbi.h>

void add_triangle_to_collision_mesh(Vtx* vtx1, Vtx* vtx2, Vtx* vtx3, Vtx** outVtx1, Vtx** outVtx2, Vtx** outVtx3);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class World;

/**
 * This may eventually become GameMode class
 * 
 * The RaceManager orchestrates the event lifecycle
 * It defines when a race begins, actors spawn, and the race ends.
 * 
 * It does not define the game mode itself.
 * 
 * You must call the base function if you want the
 * default functionality to run
 * Example:
 * MyRaceManager::BeginPlay() {
 *     RaceManager::BeginPlay() // <-- Calls default functionality
 *     // My code here
 * }
 */
class RaceManager {
public:
    RaceManager(World& world);
    virtual void Load(); // virtual required here in the base class to allow inherited classes to override
    virtual void PreInit();
    virtual void BeginPlay();
    virtual void PostInit();
protected:
    World& WorldContext;
};

#endif