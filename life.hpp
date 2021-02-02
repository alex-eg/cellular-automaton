#include "automaton.hpp"
#include "graphics.hpp"
#include "base.hpp"

#include <SDL/SDL.h>

namespace ca {
struct BoardSize {
    u32 x;
    u32 y;
};

class LApp {
private:
    bool running;
    bool updating, updatingstep;
    int update_counter;
    int counter, speed;
    Automaton life;
    bool nowDrawing;
    statecode whatDraw;
    BoardSize Board;
    Graphics graphics;

    bool help;

    inline void __Init__(void);
    std::string rule;

public:
    LApp();
    LApp(u32 x, u32 y, std::string r);

    bool Init();
    i32 Execute();

    void Event(SDL_Event *event);
    void Event_Field(SDL_Event *event);
    void Event_Help(SDL_Event *event);

    void Loop();

    void Render();
    void Render_Field();
    void Render_Help();

    void Clean();

    void Exit();
    void KeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void HelpKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void KeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
    void MouseMove(i32 mX, i32 mY, i32 relX, i32 relY, bool Left, bool Right,
                   bool Middle);
    void MouseButtonDown(Uint8 button, i32 x, i32 y);
    void MouseButtonUp(Uint8 button, i32 x, i32 y);
};

inline void CatchErrorOpengl(i32 l);
} // namespace ca
