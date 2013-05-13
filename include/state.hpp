#ifndef LSTATE
#define LSTATE

#include <SDL/SDL.h>

class LState {
public:
    virtual void Event(SDL_Event *event) = 0;
    virtual void Render() = 0;
    virtual void SwitchState() = 0;
}

class StateField : public LState {
public:
    void Event(SDL_event *event) = 0;
    void Render();
    void SwitchState();

public:
    
}

#endif
