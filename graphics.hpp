#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_ttf.h>

#include "matrix.hpp"
#include "shader.hpp"

#include <iostream>
#include <string>
#include <math.h>

#include "automaton.hpp"
#include "base.hpp"

namespace ca {

enum FontSize {size24, size16};

class Grid {
private:
    u32 height, width;
    inline void FillCell(u32 x, u32 y, f64 *color);
public:
    Grid() {}
    u32 cellsize;
    Grid(u32 h, u32 w, u32 cllsz) : height(h), width(w), cellsize(cllsz) {}

    Grid &operator = (const Grid &right) {
        if (this == &right)
            return *this;
        width = right.width;
        height = right.height;
        cellsize = right.cellsize;
        return *this;
    }

    u32 GetHeight() {
        return height;
    }

    u32 GetWidth() {
        return width;
    }

    void Draw();
    void DrawWithMap(Automaton &M);
    void DrawBorder();
};

struct RelativePosition {
    f64 x;
    f64 y;
};

class HUD {
public:
    TTF_Font *font24 = nullptr;
    TTF_Font *font16 = nullptr;

    bool Init();
    void RenderText(const char *text, SDL_Rect *location, SDL_Color *color, enum FontSize s);
    static void glEnable2D();
    static void glDisable2D();

    HUD &operator = (const HUD &right) {
        if (this == &right)
            return *this;
        if (font24 != nullptr) TTF_CloseFont(font24);
        font16 = right.font16;

        if (font16 != nullptr) TTF_CloseFont(font16);
        font16 = right.font16;

        return *this;
    }
};

class Graphics {
public:
    u32 width, height; /* SDL window */
    int uniform_tex0;
    f64 dx, dy; /* Scaling displacement */
    GLuint fbo, bloom_texture;
    Grid grid;
    SDL_Surface *display;
    DefaultShader Shade;
    Shader Bloom;
    struct RelativePosition SDL_OGL;
    HUD hud;

    Graphics() {
        display = nullptr;
        SDL_OGL.x = SDL_OGL.y = 0.375;
        fbo = 0;
        bloom_texture = 0;
        width = height = 800;
    }

    Graphics(u32 w, u32 h) : width(w), height(h) {
        display = nullptr;
        SDL_OGL.x = 0.375;
        SDL_OGL.y = 0.375;
        fbo = 0;
        bloom_texture = 0;
    }

    Graphics &operator = (const Graphics &right) {
        if (this == &right)
            return *this;
        if (display != nullptr) SDL_FreeSurface(display);
        display = right.display;
        SDL_OGL.x = right.SDL_OGL.x;
        SDL_OGL.y = right.SDL_OGL.y;
        fbo = right.fbo;
        bloom_texture = right.bloom_texture;
        width = right.width;
        height = right.height;
        return *this;

    }
    ~Graphics() {}
    bool Init();
    static i32 nextpoweroftwo(i32 x);
};
} // namespace ca
