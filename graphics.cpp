#include "graphics.hpp"

namespace ca {
void Grid::DrawBorder()
{
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, static_cast<GLfloat>((cellsize+1)*height+1));
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(static_cast<GLfloat>((cellsize+1)*width), 0);
    glVertex2f(static_cast<GLfloat>((cellsize+1)*width), static_cast<GLfloat>((cellsize+1)*height+1));
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(static_cast<GLfloat>((cellsize+1)*width), 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, static_cast<GLfloat>((cellsize+1)*width));
    glVertex2f(static_cast<GLfloat>((cellsize+1)*width), static_cast<GLfloat>((cellsize+1)*width));
    glEnd();
}

void Grid::Draw()
{
    glColor3f(0.8f, 0.8f, 0.8f);
    for(u32 i = 1; i<=width; i++) {
        if (i == width/2) glColor3f(0.2f, 0.723f, 0.32f);
        glBegin(GL_LINES);
        glVertex2f(static_cast<GLfloat>(cellsize*i+i), 0);
        glVertex2f(static_cast<GLfloat>(cellsize*i+i), static_cast<GLfloat>((cellsize+1)*height));
        glEnd();
        if (i == width/2) glColor3f(0.8f, 0.8f, 0.8f);
    }
    for(u32 i = 1; i<=height; i++) {
        if (i == height/2) glColor3f(0.2f, 0.723f, 0.32f);
        glBegin(GL_LINES);
        glVertex2f(0, static_cast<GLfloat>(cellsize*i+i));
        glVertex2f(static_cast<GLfloat>((cellsize+1)*width), static_cast<GLfloat>(cellsize*i+i));
        glEnd();
        if (i == width/2) glColor3f(0.8f, 0.8f, 0.8f);
    }
}

void Grid::DrawWithMap(Automaton &life)
{
    for (u32 i=0; i<height; i++) {
        for(u32 j=0; j<width; j++) {
            statecode cell_state = life(i,j);
            FillCell(i, j, life.State[cell_state].GetColor());
        }
    }
}

inline void Grid::FillCell(u32 x, u32 y, f64 *color)
{
    glColor3f(static_cast<GLfloat>(color[0]), static_cast<GLfloat>(color[1]), static_cast<GLfloat>(color[2]));
    glBegin(GL_QUADS);
    glVertex2f(static_cast<GLfloat>((cellsize+1)*x+1), static_cast<GLfloat>((cellsize+1)*y+1));
    glVertex2f(static_cast<GLfloat>((cellsize+1)*x+cellsize+1), static_cast<GLfloat>((cellsize+1)*y+1));
    glVertex2f(static_cast<GLfloat>((cellsize+1)*x+cellsize+1), static_cast<GLfloat>((cellsize+1)*y+cellsize+1));
    glVertex2f(static_cast<GLfloat>((cellsize+1)*x+1), static_cast<GLfloat>((cellsize+1)*y+cellsize+1));
    glEnd();
}

bool Graphics::Init()
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

    if ((display = SDL_SetVideoMode(static_cast<int>(width), static_cast<int>(height), 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL)) == nullptr)
    {
        return false;
    }

    /* Load and init GLEW */
    glewExperimental = true;
    GLenum err = glewInit();
    if(err!=GLEW_OK) {
        std::cout<<"glewInit failed, aborting."<<std::endl;
        exit(1);
    }

    /* Load and compile shaders */
    Shade = DefaultShader(width, height);
    if (!Shade.LoadFragmentShader("shd_frag.glsl")) std::cout<<"Failed loading fragment shader\n";
    if (!Shade.LoadVertexShader("shd_vert.glsl")) std::cout<<"Failed loading vertex shader\n";
    if (!Shade.Compile()) std::cout<<"Failed compiling shader\n";

    if (!Bloom.LoadFragmentShader("bloom_f.glsl")) std::cout<<"Failed loading bloom fragment shader\n";
    if (!Bloom.LoadVertexShader("bloom_v.glsl")) std::cout<<"Failed loading bloom vertex shader\n";
    if (!Bloom.Compile()) std::cout<<"Failed compiling shader\n";

    /* Set OpenGL parameters */
    glClearColor(0.07f, 0.03f, 0.05f, 0.0f);

    glBlendFunc(GL_ONE, GL_ONE);

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    dx = (width - grid.GetWidth() * grid.cellsize)/2;
    dy = (height - grid.GetHeight() * grid.cellsize)/2;

    glTranslatef(0.375f, 0.375f, 0);
    uniform_tex0 = glGetUniformLocation(Bloom.ShaderProgram,"tex0");

    return hud.Init();
}

int Graphics::nextpoweroftwo(int x)
{
    f64 logbase2 = log(x) / log(2);
    return static_cast<int>(round(pow(2,ceil(logbase2))));
}

/* Class HUD */

bool HUD::Init(void)
{
    if (TTF_Init() == -1) {
        std::cout<<"TTF init failed\n";
        return false;
    }
    font24 = TTF_OpenFont("DejaVuSansMono.ttf", 24);
    if (font24 == nullptr ) {
        std::cout<<"Loading font failed\n";
        return false;
    }

    font16 = TTF_OpenFont("DejaVuSansMono.ttf", 16);
    if (font16 == nullptr ) {
        std::cout<<"Loading font failed\n";
        return false;
    }

    return true;
}

void HUD::RenderText(const char *text, SDL_Rect *location, SDL_Color *color, enum FontSize s)
{
    SDL_Surface *initial;
    SDL_Surface *intermediary;
    int w,h;
    GLuint texture;

    if (s == size24)
    initial = TTF_RenderUTF8_Blended(font24, text, *color);
    else
    initial = TTF_RenderUTF8_Blended(font16, text, *color);

    w = Graphics::nextpoweroftwo(initial->w);
    h = Graphics::nextpoweroftwo(initial->h);

    intermediary = SDL_CreateRGBSurface(0, w, h, 32,
                    0x00ff0000,
                    0x0000ff00,
                    0x000000ff,
                    0xff000000);

    SDL_BlitSurface(initial, nullptr, intermediary, nullptr);

    /* Tell GL about our new texture */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA,
         GL_UNSIGNED_BYTE, intermediary->pixels );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Prepare to render our texture */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Draw a texture quad at location */
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(static_cast<GLfloat>(location->x), static_cast<GLfloat>(location->y));
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(static_cast<GLfloat>(location->x + w), static_cast<GLfloat>(location->y));
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(static_cast<GLfloat>(location->x + w), static_cast<GLfloat>(location->y + h));
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(static_cast<GLfloat>(location->x), static_cast<GLfloat>(location->y + h));
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

void HUD::glEnable2D()
{

    int vPort[4];

    glGetIntegerv(GL_VIEWPORT, vPort);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void HUD::glDisable2D() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
} // namespace ca
