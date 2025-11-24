#pragma once

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
    virtual void Clean();
protected:
    World& WorldContext;
};
