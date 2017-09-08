#include "client/Client.h"
#include "game/Game.h"
#include "Log.h"

int main() {
    Log_Init();
    Log_Info("hello");

    // Initialise the client systems
    client::Client *client = new client::Client();
    client->Init();

    // Initialise the game systems
    game::Game *game = new game::Game();
    game->Init(client, "menu");

    // Run until we're told to stop!
    bool running = true;
    int64_t freq, current;
    int64_t last = client->GetTicks();
    freq = client->GetFrequency() / 1000;   // Get ticks per millisecond
    while(running) {
        // Run entire frame
        if(!client->BeginFrame()) {
            running = false;
        }
        current = client->GetTicks();
        game->RunFrame(client->GetRenderer(), (current - last) / freq);
        last = current;
        client->EndFrame();
    }

    // Shutdown the game systems
    game->Shutdown();
    delete game;

    // Shutdown the client systems
    client->Shutdown();
    delete client;

    Log_Info("goodbye");
    Log_Shutdown();

    return 0;
}
