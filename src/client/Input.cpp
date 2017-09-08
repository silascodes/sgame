#include <SDL2/SDL.h>
#include "Log.h"
#include "Input.h"

namespace client {

    void Input::Init() {
        Log_Info("");

        for(int i = 0; i < 256; i++) {
            this->keyMap[i] = 0;
        }

        this->listeners.clear();
    }

    void Input::Shutdown() {
        Log_Info("");

        this->listeners.clear();
    }

    void Input::AddListener(IInputListener *listener) {
        if(!listener) {
            Log_Error("attempted to add null input listener");
            return;
        }

        this->listeners.push_back(listener);
    }

    void Input::RemoveListener(IInputListener *listener) {
        if(!listener) {
            Log_Error("attempted to remove null input listener");
            return;
        }

        this->listeners.erase(std::remove(this->listeners.begin(), this->listeners.end(), listener), this->listeners.end());
    }

    void Input::HandleEvent(SDL_Event *event) {
        switch(event->type) {
            // Mouse input events
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
                this->HandleMouseEvent(event);
                break;

            // Keyboard input events
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                this->HandleKeyboardEvent(event);
                break;

            //case SDL_TEXTEDITING:
            //case SDL_TEXTINPUT:
            //    break;
        }
    }

    MouseButton Input::TranslateSDLMouseButton(unsigned int button) {
        // TODO: default case probably would be nice (and safer)
        switch(button) {
            case SDL_BUTTON_LEFT:
                return MouseButton::Mouse1;

            case SDL_BUTTON_RIGHT:
                return MouseButton::Mouse2;

            case SDL_BUTTON_MIDDLE:
                return MouseButton::Mouse3;

            case SDL_BUTTON_X1:
                return MouseButton::Mouse4;

            case SDL_BUTTON_X2:
                return MouseButton::Mouse5;
        }
    }

    void Input::HandleMouseEvent(SDL_Event *event) {
        MouseEvent *mevent = NULL;
        int x, y;
        int wheelX, wheelY;
        MouseButton button;
        unsigned int buttonState = SDL_GetMouseState(&x, &y);   // Grab current mouse state for info some events don't have

        switch(event->type) {
            // Mouse moved
            case SDL_MOUSEMOTION:
                mevent = new MouseEvent(MouseEventType::Move, event->motion.x, event->motion.y);
                break;

            // Mouse wheel moved (horizontally or vertically)
            case SDL_MOUSEWHEEL:
                wheelX = event->wheel.x;
                wheelY = event->wheel.y;

                // According to SDL docs, if direction is such we have to flip the values to maintain sanity
                if(event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                    wheelX *= -1;
                    wheelY *= -1;
                }

                mevent = new MouseEvent(MouseEventType::WheelMove, x, y, wheelX, wheelY);
                break;

            // Mouse button pressed down
            case SDL_MOUSEBUTTONDOWN:
                mevent = new MouseEvent(MouseEventType::ButtonDown, event->button.x, event->button.y, this->TranslateSDLMouseButton(event->button.button));
                break;

            // Mouse button released
            case SDL_MOUSEBUTTONUP:
                mevent = new MouseEvent(MouseEventType::ButtonUp, event->button.x, event->button.y, this->TranslateSDLMouseButton(event->button.button));
                break;

            default:
                return;
        }

        for(std::vector<IInputListener*>::iterator i = this->listeners.begin(); i != this->listeners.end(); i++) {
            // If something handles the event and doesn't want to propagate it, early out
            if(!(*i)->OnMouseEvent(*mevent)) {
                return;
            }
        }

        delete mevent;
    }

    void Input::HandleKeyboardEvent(SDL_Event *event) {
        KeyboardEvent *kevent = NULL;

        switch(event->type) {
            case SDL_KEYDOWN:
                kevent = new KeyboardEvent(KeyboardEventType::KeyDown);
                break;

            case SDL_KEYUP:
                kevent = new KeyboardEvent(KeyboardEventType::KeyUp);
                break;

            default:
                return;
        }

        for(std::vector<IInputListener*>::iterator i = this->listeners.begin(); i != this->listeners.end(); i++) {
            // If something handles the event and doesn't want to propagate it, early out
            if(!(*i)->OnKeyboardEvent(*kevent)) {
                return;
            }
        }

        delete kevent;
    }

}
