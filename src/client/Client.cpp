#include <SDL2/SDL.h>
#include "Log.h"
#include "game/Game.h"
#include "Input.h"
#include "ui/UI.h"
#include "Client.h"

namespace client {

/*
static int client_console;
static char client_console_lines[1024][1024];
static int client_console_nextLine;
static int client_console_logListenerId;

static void Client_Console_PushLine(const char* str) {
    memset(client_console_lines[client_console_nextLine], 0, 1024);

    strcpy(client_console_lines[client_console_nextLine], str);

    client_console_nextLine++;
}

static void Client_Console_Log_Listener(int type, const char* msg) {
    Client_Console_PushLine(msg);
}

static void Client_Console_Init() {
    memset(client_console_lines, 0, 1024 * 1024);

    client_console_nextLine = 0;

    client_console_logListenerId = Log_AddListener(LOG_ANY, Client_Console_Log_Listener);
}

static void Client_Console_Shutdown() {
    Log_RemoveListener(client_console_logListenerId);
}

static void Client_Console_Draw() {
    if(!client_console) {
        return;
    }

    Renderer_SetDrawColor(0x33, 0x33, 0x33, 0xff);
    Renderer_DrawFilledRect(0, 0, 1280, 300);

    Renderer_SetDrawColor(0xaa, 0xaa, 0xaa, 0xff);
    Renderer_DrawLine(0, 300, 1280, 300);

    Renderer_SetDrawColor(0xdd, 0xdd, 0xdd, 0xff);
    for(int i = 0, line = client_console_nextLine - 1, y = 260; i < 1024; i++, line--, y -= 20) {
        if(line < 0) {
            line = 1023;
        }

        Renderer_DrawText(4, y, client_console_lines[line]);
    }
}

static void Client_Input_KeyDown_Listener(input_event_t* event) {
    client_console = !client_console;
}
*/

    void Client::Init() {
        Log_Info("");

        this->quitRequested = false;
        //client_console = 0;

        if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {
            Log_Error("failed to init SDL");
            return;
        }

        this->window = SDL_CreateWindow("sgame", 100, 100, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS/* | SDL_WINDOW_FULLSCREEN*/);
        if(!this->window) {
            Log_Error("failed to create window");
            SDL_Quit();
            return;
        }

        this->renderer = new graphics::Renderer();
        this->renderer->Init(this->window);

        this->input = new Input();
        this->input->Init();
        //Input_RegisterListener(INPUT_EVENT_KEY_DOWN, Client_Input_KeyDown_Listener);

        this->ui = new ui::Screen();
        this->ui->Init();

        this->input->AddListener(this->ui);

        //Client_Console_Init();
    }

    void Client::Shutdown() {
        Log_Info("");

        //Client_Console_Shutdown();

        this->input->RemoveListener(this->ui);

        this->ui->Shutdown();
        delete this->ui;

        this->input->Shutdown();
        delete this->input;
        this->renderer->Shutdown();
        delete this->renderer;

        SDL_DestroyWindow(this->window);

        SDL_Quit();
    }

    void Client::PumpEvents() {
        SDL_Event e;

        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                // Quit requested (window closed, etc.)
                case SDL_QUIT:
                    this->quitRequested = true;
                    break;

                // Mouse input events
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEWHEEL:

                // Keyboard input events
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    //case SDL_TEXTEDITING:
                    //case SDL_TEXTINPUT:
                    this->input->HandleEvent(&e);
                    break;
            }
        }
    }

    bool Client::BeginFrame() {
        this->PumpEvents();

        // Check if quit requested
        if(this->quitRequested) {
            return false;
        }

        this->renderer->BeginFrame();

        return true;
    }

    void Client::EndFrame() {
        //Client_Console_Draw();

        this->ui->Render(renderer);

        this->renderer->EndFrame();
    }

    int64_t Client::GetTicks() {
        return SDL_GetPerformanceCounter();
    }

    int64_t Client::GetFrequency() {
        return SDL_GetPerformanceFrequency();
    }

    graphics::Renderer *Client::GetRenderer() {
        return this->renderer;
    }

    client::Input *Client::GetInput() {
        return this->input;
    }

    void Client::Quit() {
        this->quitRequested = true;
    }

}
