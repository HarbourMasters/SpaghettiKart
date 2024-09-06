#include <vector>
#include <iostream>

extern "C" {
    #include "actor_types.h"
    extern uint16_t gNumActors;
}



// Define the global actor list
std::vector<Actor> gActorList;


// (Vec3f pos, Vec3s rot, Vec3f velocity, s16 actorType)


/**
 * Spawns an actor
 * 
 * @arg pos spawn position
 * @arg rot spawn rotation
 * @arg velocity initial velocity
 * @arg type actor type
 * 
 */
extern "C" void SpawnActor(Vec3f pos, Vec3s rot, Vec3f velocity, s16 type) {

    // Try running game without removing expired actors
    // if (gNumActors >= ACTOR_LIST_SIZE) {
    //     return try_remove_destructable_item(pos, rot, velocity, type);
    // }


    gActorList.push_back({type, 0, 0, 0, 0, 0, rot, 0, pos, velocity, 0});
    std::cout << "Actor ID " << id << " added.\n";
}

// Function to remove an actor by id from the list
extern "C" void remove_actor(int id) {
    for (auto it = gActorList.begin(); it != gActorList.end(); ++it) {
        if (it->id == id) {
            gActorList.erase(it);
            play_sound_before_despawn(actor);
            std::cout << "Actor ID " << id << " removed.\n";
            return;
        }
    }
    std::cout << "Actor ID " << id << " not found.\n";
}

// Function to update all actors
extern "C" void update_actors() {
    for (auto& actor : gActorList) {
        actor.update();
    }
}

// Function to render all actors
extern "C" void render_actors() {
    for (auto& actor : gActorList) {
        actor.render();
    }
}

// Function to get the number of actors in the list
extern "C" int get_actor_count() {
    return gActorList.size();
}

extern "C" Actor* get_actor_by_index(int index) {
    if (index >= 0 && index < gActorList.size()) {
        return &gActorList[index];
    } else {
        std::cout << "Invalid index: " << index << "\n";
        return nullptr; // Return null if index is out of bounds
    }
}
