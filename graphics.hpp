#pragma once

#include "base.hpp"
#include "matrix.hpp"
#include "shader.hpp"

#include <iostream>
#include <string>
#include <math.h>
#include <memory>


typedef struct _TTF_Font TTF_Font;

struct SDL_Rect;
struct SDL_Color;
struct SDL_Window;

namespace ca {
class Automaton;

enum class FontSize {
    size24,
    size16
};

class Grid {
public:
    Grid() = default;
    Grid(u32 h, u32 w, u32 cellsize);

    Grid &operator=(const Grid &right);
    u32 get_height() const;
    u32 get_width() const;

    void draw();
    void draw_with_map(const std::unique_ptr<Automaton> &life);
    void draw_border();

    u32 cellsize = 0;

private:
    u32 width = 0;
    u32 height = 0;
    void fill_cell(u32 x, u32 y, f64 *color);
};

struct RelativePosition {
    RelativePosition(f64 x, f64 y);

    f64 x;
    f64 y;
};

class HUD {
public:
    HUD();
    bool init();
    void render_text(const char *text, SDL_Rect *location, SDL_Color *color, enum FontSize s);
    static void gl_enable2_d();
    static void gl_disable2_d();

    ~HUD();

private:
    std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> font24;
    std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> font16;
};

class Graphics {
public:
    Graphics();
    Graphics(u32 w, u32 h);

    ~Graphics();

    void present();

    u32 width = 800;
    u32 height = 800;
    int uniform_tex0;
    f64 dx;
    f64 dy;
    GLuint fbo = 0;
    GLuint bloom_texture = 0;
    Grid grid;
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;

    DefaultShader shade;
    Shader bloom;
    RelativePosition sdl_ogl;
    HUD hud;

    bool init();
    static i32 nextpoweroftwo(i32 x);
};
} // namespace ca
