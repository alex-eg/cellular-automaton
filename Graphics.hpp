#ifndef _L_GRAPHICS
#define _L_GRAPHICS

#include <SDL/SDL.h>
#include <GL/glew.h>
#include "LMatrix.hpp"
#include "Shader.hpp"
#include <iostream>

class Grid {
private:
    int height, width;
    inline void FillCell(int x, int y);
public:
    Grid(){}
    int cellsize;
    Grid(int h, int w, int cllsz) : height(h), width(w), cellsize(cllsz) {}

    Grid &operator = (const Grid &right)
    {
	if (this == &right)
	    return *this;
	width = right.width;
	height = right.height;
	cellsize = right.cellsize;
	return *this;
    }

    void Draw();
    void DrawWithMap(LMatrix <unsigned char> *M);
    void DrawBorder();
};

struct RelativePosition {
    double x;
    double y;
};

class Graphics {
private:
    int uniform_tex0;
    int width, height; /* SDL window */
public:
    GLuint fbo, texture;
    Grid grid;
    SDL_Surface *display;
    DefaultShader Shade;
    Shader Bloom;
    struct RelativePosition SDL_OGL;

    Graphics()
    {
	display = NULL;
	SDL_OGL.x = SDL_OGL.y = 0.375;
	fbo = 0;
	texture = 0;
	width = height = 800;
    }

    Graphics(unsigned int w, unsigned int h) : width(w), height(h)
    {
	display = NULL;
	SDL_OGL.x = 0.375;
	SDL_OGL.y = 0.375;
	fbo = 0;
	texture = 0;
    }

    Graphics &operator = (const Graphics &right) 
    {
	if (this == &right)
	    return *this;
	if (display != NULL) SDL_FreeSurface(display);
	display = right.display;
	SDL_OGL.x = right.SDL_OGL.x;
	SDL_OGL.y = right.SDL_OGL.y;
	fbo = right.fbo;
	texture = right.texture;
	width = right.width;
	height = right.height;
	return *this;
    }

    ~Graphics() {}
    bool Init();
};

#endif
