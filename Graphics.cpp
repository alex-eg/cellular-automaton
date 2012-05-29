#include "Graphics.hpp"

void Grid::DrawBorder()
{
    glDisable(GL_BLEND);
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, (cellsize+1)*height+1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f((cellsize+1)*width, 0);
    glVertex2f((cellsize+1)*width, (cellsize+1)*height+1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f((cellsize+1)*width, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, (cellsize+1)*width);
    glVertex2f((cellsize+1)*width, (cellsize+1)*width);
    glEnd();
    glEnable(GL_BLEND);
}

void Grid::Draw()
{
    for(int i = 1; i<=width; i++) {
	glBegin(GL_LINES);
	glVertex2f(cellsize*i+i, 0);
	glVertex2f(cellsize*i+i, (cellsize+1)*height);
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2f(0, cellsize*i+i);
	glVertex2f((cellsize+1)*width, cellsize*i+i);
	glEnd();
    }
}

void Grid::DrawWithMap(LMatrix <unsigned char> *M)
{
    for (int i=0; i<height; i++)
	for(int j=0; j<width; j++)
	    if ((*M)(i,j) != 0) FillCell(i,j);
}

inline void Grid::FillCell(int x, int y)
{
    glColor3f(0.3, 0.8765, 0.3);
    glBegin(GL_QUADS);
    glVertex2f((cellsize+1)*x+1, (cellsize+1)*y+1);
    glVertex2f((cellsize+1)*x+cellsize+1, (cellsize+1)*y+1);
    glVertex2f((cellsize+1)*x+cellsize+1, (cellsize+1)*y+cellsize+1);
    glVertex2f((cellsize+1)*x+1, (cellsize+1)*y+cellsize+1);
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
    
    if ((display = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL)) == NULL) return false;
    
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
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0, width, height, 0, -1, 1); //!!!!//
    glMatrixMode(GL_MODELVIEW);

    dx = (width - grid.GetWidth() * grid.cellsize)/2;
    dy = (height - grid.GetHeight() * grid.cellsize)/2;

    glTranslatef(0.375, 0.375, 0);
    uniform_tex0 = glGetUniformLocation(Bloom.ShaderProgram,"tex0");
  
    return hud.Init();
}

int Graphics::nextpoweroftwo(int x)
{
    double logbase2 = log(x) / log(2);
    return round(pow(2,ceil(logbase2)));
}

/* Class HUD */

bool HUD::Init(void)
{
    if (TTF_Init() == -1) {
	std::cout<<"TTF init failed\n";
	return false;
    }
    font = TTF_OpenFont("DejaVuSansMono.ttf", 24);
    if (font == NULL ) {
	std::cout<<"Loading font failed\n";
	return false;
    }
    return true;
}

void HUD::RenderText(const char *text, SDL_Rect *location, SDL_Color *color)
{
    SDL_Surface *initial;
    SDL_Surface *intermediary;
    int w,h;
    GLuint texture;

    initial = TTF_RenderUTF8_Blended(font, text, *color);
	
    w = Graphics::nextpoweroftwo(initial->w);
    h = Graphics::nextpoweroftwo(initial->h);
	
    intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
					0x00ff0000, 
					0x0000ff00, 
					0x000000ff, 
					0xff000000);
    
    SDL_BlitSurface(initial, 0, intermediary, 0);
	
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
    glVertex2f(location->x    , location->y);
    glTexCoord2f(1.0f, 1.0f); 
    glVertex2f(location->x + w, location->y);
    glTexCoord2f(1.0f, 0.0f); 
    glVertex2f(location->x + w, location->y + h);
    glTexCoord2f(0.0f, 0.0f); 
    glVertex2f(location->x    , location->y + h);
    glEnd();
	
    /* Bad things happen if we delete the texture before it finishes */
    glFinish();
	
    /* return the deltas in the unused w,h part of the rect */
    location->w = initial->w;
    location->h = initial->h;
	
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

void HUD::glDisable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
