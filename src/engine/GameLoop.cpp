
// Example of how this system would be used inside the game loop.

    ActorManager actorManager;

    // Add a BananaActor
    std::unique_ptr<Actor> bananaActor = std::make_unique<BananaActor>(1);
    bananaActor->Init();
    actorManager.AddActor(std::move(bananaActor));

    // Add another actor type, if desired
    // std::unique_ptr<Actor> anotherActor = std::make_unique<AnotherActor>(2);
    // anotherActor->Init();
    // actorManager.AddActor(std::move(anotherActor));

    // Main loop
    for (int i = 0; i < 10; i++) {
        actorManager.UpdateActors();
        actorManager.RenderActors();

        // Optionally remove actors based on some condition
        actorManager.RemoveExpiredActors();
    }
