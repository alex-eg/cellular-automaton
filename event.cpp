#include "life.hpp"

namespace ca {
void LApp::process_event(SDL_Event *event) {
    if (help) {
        process_event_help(event);
    } else {
        process_event_field(event);
    }
}

void LApp::process_event_help(SDL_Event *event) {
    switch (event->type) {
    case SDL_KEYDOWN: {
        help_key_down(event->key.keysym.sym, event->key.keysym.mod,
                    event->key.keysym.unicode);
        break;
    }
    case SDL_QUIT: {
        running = false;
        break;
    }
    default:
        break;
    }
}

void LApp::help_key_down(SDLKey sym, SDLMod mod, Uint16 unicode) {
    std::ignore = mod;
    std::ignore = unicode;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"

    switch (sym) {
    case SDLK_F1:
    case SDLK_ESCAPE: {
        help = false;
        break;
    }
    default:
        break;
    }
#pragma clang diagnostic pop
}

void LApp::process_event_field(SDL_Event *event) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"

    switch (event->type) {
    case SDL_KEYDOWN: {
        key_down(event->key.keysym.sym, event->key.keysym.mod,
                event->key.keysym.unicode);
        break;
    }
    case SDL_KEYUP: {
        key_up(event->key.keysym.sym, event->key.keysym.mod,
              event->key.keysym.unicode);
        break;
    }
    case SDL_QUIT: {
        running = false;
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        mouse_button_down(event->button.button, event->button.x, event->button.y);
        break;
    }
    case SDL_MOUSEMOTION: {
        bool left, right, middle;
        left = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(1);
        middle = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(2);
        right = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(3);
        mouse_move(event->motion.x, event->motion.y, event->motion.xrel,
                  event->motion.yrel, left, right, middle);
        break;
    }
    default:
        break;
    }
#pragma clang diagnostic pop
}

void LApp::key_down(SDLKey sym, SDLMod mod, Uint16 unicode) {
    std::ignore = mod;
    std::ignore = unicode;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
    switch (sym) {
    case SDLK_SPACE: {
        if (updating) {
            updating = false;
        } else {
            updating = true;
        }
        break;
    }
    case SDLK_ESCAPE: {
        running = false;
        break;
    }
    case SDLK_s: {
        updatingstep = true;
        break;
    }
    case SDLK_r: {
        life.randomize();
        break;
    }
    case SDLK_c: {
        life.clear();
        break;
    }
    case SDLK_LCTRL: {
        //nowDrawing = true;
        break;
    }
    case SDLK_b: {
        graphics.bloom.Switch();
        break;
    }
    case SDLK_h: {
        graphics.shade.Switch();
        graphics.shade.invoke();
        break;
    }
    case SDLK_KP_PLUS: {
        if (speed > 0) {
            updating = updating || true;
            speed--;
            counter = 0;
        }
        break;
    }
    case SDLK_KP_MINUS: {
        if (speed < 30) {
            speed++;
        } else {
            updating = false;
        }
        counter = 0;
        break;
    }
    case SDLK_F1: {
        if (help) {
            help = false;
        } else {
            help = true;
            updating = false;
        }
        break;
    }
    default:
        break;
    }
#pragma clang diagnostic pop
}

void LApp::key_up(SDLKey sym, SDLMod mod, Uint16 unicode) {
    std::ignore = mod;
    std::ignore = unicode;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
    switch (sym) {
    case SDLK_LCTRL: {
        //nowDrawing = false;
        break;
    }
    case SDLK_s: {
        updatingstep = false;
        update_counter = 0;
        break;
    }
    default:
        break;
    }
#pragma clang diagnostic pop
}

void LApp::mouse_move(int m_x, int m_y, int rel_x, int rel_y, bool left, bool right,
                     bool middle) {
    std::ignore = middle;

    if (now_drawing && right && !updating) {
        int i, j;
        i = static_cast<int>((m_x - graphics.sdl_ogl.x - graphics.dx) /
                             (graphics.grid.cellsize + 1));
        j = static_cast<int>((m_y - graphics.sdl_ogl.y - graphics.dy) /
                             (graphics.grid.cellsize + 1));
        if (i >= 0 && j >= 0 && i < static_cast<int>(board.x) &&
            j < static_cast<int>(board.y)) {
            life.draw(i, j, what_draw);
        }
    }

    if (left) {
        glTranslatef(static_cast<GLfloat>(rel_x), static_cast<GLfloat>(rel_y), 0);
        graphics.sdl_ogl.x += rel_x;
        graphics.sdl_ogl.y += rel_y;
    }
}

void LApp::mouse_button_down(Uint8 button, int x, int y) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
    switch (button) {
    case SDL_BUTTON_WHEELUP: {
        if (graphics.grid.cellsize == 1) {
            graphics.grid.cellsize++;
        } else {
            graphics.grid.cellsize += 2;
        }
        if (graphics.grid.cellsize > 100) {
            graphics.grid.cellsize = 100;
        }

        graphics.dx = (graphics.width -
                       graphics.grid.get_width() * graphics.grid.cellsize) /
            2;
        graphics.dy = (graphics.height -
                       graphics.grid.get_height() * graphics.grid.cellsize) /
            2;
        break;
    }
    case SDL_BUTTON_WHEELDOWN: {
        graphics.grid.cellsize -= 2;
        if (graphics.grid.cellsize < 1) {
            graphics.grid.cellsize = 1;
        }

        graphics.dx = (graphics.width -
                       graphics.grid.get_width() * graphics.grid.cellsize) /
            2;
        graphics.dy = (graphics.height -
                       graphics.grid.get_height() * graphics.grid.cellsize) /
            2;
        break;
    }
    case SDL_BUTTON_RIGHT: {
        if (!updating) {
            now_drawing = true;
            int i, j;
            i = static_cast<int>((x - graphics.sdl_ogl.x - graphics.dx) /
                                 (graphics.grid.cellsize + 1));
            j = static_cast<int>((y - graphics.sdl_ogl.y - graphics.dy) /
                                 (graphics.grid.cellsize + 1));
            if (i >= 0 && j >= 0 && i < static_cast<int>(board.x) &&
                j < static_cast<int>(board.y)) {
                if ((*life.front)(i, j) == 0) {
                    what_draw = 1;
                } else {
                    what_draw = 0;
                }
                life.draw(i, j, what_draw);
            }
        }
        break;
    }
    default:
        break;
    }
#pragma clang diagnostic pop
}

void LApp::mouse_button_up(Uint8 button, int x, int y) {
    std::ignore = x = y;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
    switch (button) {
    case SDL_BUTTON_RIGHT: {
        now_drawing = false;
        what_draw = 1;
        break;
    }
    default:
        break;
    }
#pragma clang diagnostic pop
}
} // namespace ca
