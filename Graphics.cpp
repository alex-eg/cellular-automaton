#include "Graphics.hpp"

void Grid::DrawBorder()
{
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
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.375, 0.375, 0);
   

    uniform_tex0 = glGetUniformLocation(Bloom.ShaderProgram,"tex0");
  
    // glActiveTexture(GL_TEXTURE1);

    glGenFramebuffers(1, &fbo);
    return true;
}
