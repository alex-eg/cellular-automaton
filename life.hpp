#include "automaton.hpp"
#include "base.hpp"

#include <SDL2/SDL.h>

#include <memory>

namespace ca {
class Graphics;
class Automaton;

struct BoardSize {
    BoardSize(u32 width, u32 height);
    u32 x;
    u32 y;
};

class LApp {
public:
    LApp(u32 width, u32 height, std::string rule);
    ~LApp();

    bool init();
    i32 execute();

    void process_event(SDL_Event *event);
    void process_event_field(SDL_Event *event);
    void process_event_help(SDL_Event *event);

    void loop();

    void render();
    void render_field();
    void render_help();

    void clean();

    void exit();
    void key_down(SDL_Keycode sym);
    void help_key_down(SDL_Keycode sym);
    void key_up(SDL_Keycode sym);
    void mouse_move(i32 m_x, i32 m_y, i32 rel_x, i32 rel_y, bool left, bool right,
                   bool middle);
    void mouse_button_down(u8 button, i32 x, i32 y);
    void mouse_wheel(const SDL_MouseWheelEvent& event);

private:
    bool running = false;
    bool updating = false;
    bool updatingstep = false;
    i32 update_counter = 0;
    i32 counter = 0;
    i32 speed = 2;
    BoardSize board;
    std::unique_ptr<Automaton> life = nullptr;
    bool now_drawing = false;
    statecode what_draw = 1;
    std::unique_ptr<Graphics> graphics;

    bool help;

    std::string rule = "";
};

inline void catch_error_opengl(i32 l);
} // namespace ca
