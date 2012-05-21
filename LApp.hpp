#include <SDL/SDL.h>
#include "Life.hpp"
#include "Graphics.hpp"

struct BoardSize {
    unsigned int x;
    unsigned int y;
};

struct Settings {
    void v;
};

class LApp {
 private:
    bool running;
    bool updating, updatingstep;
    int update_counter;
    int counter, speed;
    LLife life;
    bool nowDrawing;
    unsigned char whatDraw;
    struct BoardSize Board;
    Graphics graphics;
    inline void __Init__(void);
 public:
    LApp();
    LApp(unsigned int x, unsigned int y);
 public:
    bool Init();
    int Execute();
    void Event(SDL_Event *event);
    void Loop();
    void Render();
    void Clean();
 public:
    void Exit();
    void KeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void KeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
    void MouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    void MouseButtonDown(Uint8 button, int x, int y);
    void MouseButtonUp(Uint8 button, int x, int y);
};

inline void CatchErrorOpengl(int l);
