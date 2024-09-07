#include <cstdint>
#include <iostream>

class Actor {
public:
    uint32_t id;
    virtual ~Actor() = default;  // Make sure to have a virtual destructor in a base class

    Actor(uint32_t actorId) : id(actorId) {}

    // Virtual functions to be overridden by derived classes
    virtual void Init() { std::cout << "Initializing Actor " << id << "\n"; }
    virtual void Update() { std::cout << "Updating Actor " << id << "\n"; }
    virtual void Render() { std::cout << "Rendering Actor " << id << "\n"; }
    virtual void Expire() { std::cout << "Expiring Actor " << id << "\n"; }
    virtual void Destroy() { std::cout << "Destroying Actor " << id << "\n"; }
};
