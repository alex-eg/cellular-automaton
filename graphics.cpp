#include "graphics.hpp"
#include "automaton.hpp"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

namespace ca {
Grid::Grid(u32 _width, u32 _height, u32 _cellsize)
    : cellsize(_cellsize)
    , width(_width)
    , height(_height) {}

Grid & Grid::operator=(const Grid &right) {
    if (this == &right) {
        return *this;
    }
    width = right.width;
    height = right.height;
    cellsize = right.cellsize;
    return *this;
}

u32 Grid::get_height() const {
    return height;
}

u32 Grid::get_width() const {
    return width;
}

void Grid::draw_border() {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, static_cast<GLfloat>((cellsize + 1) * height + 1));
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * width), 0);
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * width),
               static_cast<GLfloat>((cellsize + 1) * height + 1));
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * width), 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, static_cast<GLfloat>((cellsize + 1) * width));
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * width),
               static_cast<GLfloat>((cellsize + 1) * width));
    glEnd();
}

void Grid::draw() {
    glColor3f(0.8f, 0.8f, 0.8f);
    for (u32 i = 1; i <= width; i++) {
        if (i == width / 2) {
            glColor3f(0.2f, 0.723f, 0.32f);
        }
        glBegin(GL_LINES);
        glVertex2f(static_cast<GLfloat>(cellsize * i + i), 0);
        glVertex2f(static_cast<GLfloat>(cellsize * i + i),
                   static_cast<GLfloat>((cellsize + 1) * height));
        glEnd();
        if (i == width / 2) {
            glColor3f(0.8f, 0.8f, 0.8f);
        }
    }
    for (u32 i = 1; i <= height; i++) {
        if (i == height / 2) {
            glColor3f(0.2f, 0.723f, 0.32f);
        }
        glBegin(GL_LINES);
        glVertex2f(0, static_cast<GLfloat>(cellsize * i + i));
        glVertex2f(static_cast<GLfloat>((cellsize + 1) * width),
                   static_cast<GLfloat>(cellsize * i + i));
        glEnd();
        if (i == width / 2) {
            glColor3f(0.8f, 0.8f, 0.8f);
        }
    }
}

void Grid::draw_with_map(const std::unique_ptr<Automaton> &life) {
    for (u32 i = 0; i < height; i++) {
        for (u32 j = 0; j < width; j++) {
            statecode cell_state = (*life)(i, j);
            fill_cell(i, j, life->state[cell_state].get_color());
        }
    }
}

inline void Grid::fill_cell(u32 x, u32 y, f64 *color) {
    glColor3f(static_cast<GLfloat>(color[0]), static_cast<GLfloat>(color[1]),
              static_cast<GLfloat>(color[2]));
    glBegin(GL_QUADS);
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * x + 1),
               static_cast<GLfloat>((cellsize + 1) * y + 1));
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * x + cellsize + 1),
               static_cast<GLfloat>((cellsize + 1) * y + 1));
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * x + cellsize + 1),
               static_cast<GLfloat>((cellsize + 1) * y + cellsize + 1));
    glVertex2f(static_cast<GLfloat>((cellsize + 1) * x + 1),
               static_cast<GLfloat>((cellsize + 1) * y + cellsize + 1));
    glEnd();
}

RelativePosition::RelativePosition(f64 _x, f64 _y)
    : x(_x)
    , y(_y) {}

Graphics::Graphics()
    : sdl_ogl(0.375, 0.375)
    , window(nullptr, [](SDL_Window *) {}) {}

Graphics::Graphics(u32 w, u32 h)
    : width(w)
    , height(h)
    , sdl_ogl(0.375, 0.375)
    , window(nullptr, [](SDL_Window *) {}) {}

bool Graphics::init() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

    window = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(
        SDL_CreateWindow("Cellular Automaton", 0, 0,
                         static_cast<i32>(width), static_cast<i32>(height),
                         SDL_WINDOW_OPENGL),
        SDL_DestroyWindow);
    if (window == nullptr) {
        return false;
    }

    /* Load and init GLEW */
//    glewExperimental = true;
//    GLenum err = glewInit();
//    if (err != GLEW_OK) {
//        std::cout << "glewInit failed, aborting." << std::endl;
//        exit(1);
//    }

    /* Load and compile shaders */
    shade = DefaultShader(width, height);
    if (!shade.load_fragment_shader("shd_frag.glsl")) {
        std::cout << "Failed loading fragment shader\n";
    }
    if (!shade.load_vertex_shader("shd_vert.glsl")) {
        std::cout << "Failed loading vertex shader\n";
    }
    if (!shade.compile()) {
        std::cout << "Failed compiling shader\n";
    }

    if (!bloom.load_fragment_shader("bloom_f.glsl")) {
        std::cout << "Failed loading bloom fragment shader\n";
    }
    if (!bloom.load_vertex_shader("bloom_v.glsl")) {
        std::cout << "Failed loading bloom vertex shader\n";
    }
    if (!bloom.compile()) {
        std::cout << "Failed compiling shader\n";
    }

    /* Set OpenGL parameters */
    glClearColor(0.07f, 0.03f, 0.05f, 0.0f);

    glBlendFunc(GL_ONE, GL_ONE);

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    dx = (width - grid.get_width() * grid.cellsize) / 2;
    dy = (height - grid.get_height() * grid.cellsize) / 2;

    glTranslatef(0.375f, 0.375f, 0);
    uniform_tex0 = glGetUniformLocation(bloom.shader_program, "tex0");

    return hud.init();
}

void Graphics::present() {
    SDL_GL_SwapWindow(window.get());
}


int Graphics::nextpoweroftwo(int x) {
    f64 logbase2 = log(x) / log(2);
    return static_cast<int>(round(pow(2, ceil(logbase2))));
}

Graphics::~Graphics() {
    glFinish();
    if (fbo != 0) {
        glDeleteFramebuffers(1, &fbo);
    }
}


/* Class HUD */

HUD::HUD()
    : font16(nullptr, [](TTF_Font *) {})
    , font24(nullptr, [](TTF_Font *) {}) {}

bool HUD::init(void) {
    if (TTF_Init() == -1) {
        std::cout << "TTF init failed\n";
        return false;
    }
    font24 = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>(
        TTF_OpenFont("DejaVuSansMono.ttf", 24),
        TTF_CloseFont);
    if (font24 == nullptr) {
        std::cout << "Loading font failed\n";
        return false;
    }

    font16 = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>(
        TTF_OpenFont("DejaVuSansMono.ttf", 16),
        TTF_CloseFont);
    if (font16 == nullptr) {
        std::cout << "Loading font failed\n";
        return false;
    }

    return true;
}

void HUD::render_text(const char *text, SDL_Rect *location, SDL_Color *color,
                      FontSize s) {
    SDL_Surface *initial;
    SDL_Surface *intermediary;
    int w, h;
    GLuint texture;

    if (s == FontSize::size24) {
        initial = TTF_RenderUTF8_Blended(font24.get(), text, *color);
    } else {
        initial = TTF_RenderUTF8_Blended(font16.get(), text, *color);
    }

    w = Graphics::nextpoweroftwo(initial->w);
    h = Graphics::nextpoweroftwo(initial->h);

    intermediary = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00,
                                        0x000000ff, 0xff000000);

    SDL_BlitSurface(initial, nullptr, intermediary, nullptr);

    /* Tell GL about our new texture */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                 intermediary->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Prepare to render our texture */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Draw a texture quad at location */
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(static_cast<GLfloat>(location->x),
               static_cast<GLfloat>(location->y));
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(static_cast<GLfloat>(location->x + w),
               static_cast<GLfloat>(location->y));
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(static_cast<GLfloat>(location->x + w),
               static_cast<GLfloat>(location->y + h));
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(static_cast<GLfloat>(location->x),
               static_cast<GLfloat>(location->y + h));
    glEnd();

    /* Bad things happen if we delete the texture before it finishes */
    glFinish();

    /* return the deltas in the unused w,h part of the rect */
    location->w = static_cast<unsigned short>(initial->w);
    location->h = static_cast<unsigned short>(initial->h);

    /* Clean up */
    SDL_FreeSurface(initial);
    SDL_FreeSurface(intermediary);
    glDeleteTextures(1, &texture);
}

void HUD::gl_enable2_d() {
    int v_port[4];

    glGetIntegerv(GL_VIEWPORT, v_port);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, v_port[2], 0, v_port[3], -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void HUD::gl_disable2_d() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

HUD::~HUD() {
    TTF_Quit();
}
} // namespace ca
