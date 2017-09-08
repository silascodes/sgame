#include <string>
#include "Log.h"
#include "Game.h"
#include "util/Serializer.h"
#include "util/Deserializer.h"

namespace game {

    void Game::Init(client::Client *client, const std::string &initialState) {
        Log_Info(initialState.c_str());

        // Initialise everything
        this->client = client;
        this->statePush = "";
        this->statePop = false;

        // Start the first state
        this->PushState(initialState);
        this->PerformPushState();
    }

    void Game::Shutdown() {
        Log_Info("");

        // Pop and clean up all our states
        while(!this->states.empty()) {
            this->PerformPopState();
        }
    }

    void Game::RunFrame(graphics::Renderer *renderer, int elapsedMs) {
        // If we need to perform a state change, do it
        if(!this->statePush.empty()) {
            this->PerformPushState();
        }
        else if(this->statePop) {
            this->PerformPopState();
        }

        //this->SaveGame("test.sav");

        // If we have an active state, call its RunFrame function
        if(!this->states.empty()) {
            this->states.back()->RunFrame(renderer, elapsedMs);
        }
    }

    void Game::PushState(const std::string &name) {
        // Check if a state pop has already been queued, if so bail
        if(this->statePop) {
            Log_Warning("state push to %s requested but pop already queued", name.c_str());
            return;
        }

        // Check if a state push has already been queued, if so bail
        if(!this->statePush.empty()) {
            Log_Warning("state push to %s requested but state push to to %s already queued", name.c_str(),
                        this->statePush.c_str());
            return;
        }

        // Store the new state name (we will perform push later)
        this->statePush = name;
    }

    void Game::PopState() {
        // Check if a state pop has already been queued, if so bail
        if(this->statePop) {
            Log_Warning("state pop requested but pop already queued");
            return;
        }

        // Check if a state push has already been queued, if so bail
        if(!this->statePush.empty()) {
            Log_Warning("state pop requested but state push to %s already queued", this->statePush.c_str());
            return;
        }

        // Flag state pop request (we will perform it later)
        this->statePop = true;
    }

    void Game::LoadGame(const std::string &name) {
        // Open deserializer on file
        util::Deserializer *deserializer = new util::Deserializer();
        deserializer->Init(name);

        // read header

        // Load the game state

    }

    void Game::SaveGame(const std::string &name) {
        // Open serializer on file
        util::Serializer *serializer = new util::Serializer();
        serializer->Init(name);

        // write header

        // Save the game state stack
        for(std::vector<State *>::iterator i = this->states.begin(); i != this->states.end(); i++) {
            serializer->Serialize(*i);
        }

        // close file
        serializer->Shutdown();
    }

    void Game::PerformPushState() {
        Log_Info(this->statePush.c_str());

        // Initialise the new state
        State *state = new State();
        state->Init(this->statePush, this);

        // Push then new state on to the top of the stack
        this->states.push_back(state);

        this->statePush.clear();
    }

    void Game::PerformPopState() {
        Log_Info("");

        // Pop the top state off the stack
        State *state = this->states.back();
        state->Shutdown();
        delete state;
        this->states.pop_back();

        this->statePop = false;
    }

    client::Client *Game::GetClient() {
        return this->client;
    }

}
