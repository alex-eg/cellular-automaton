#include "LApp.hpp"
#include <iostream>

inline void LApp::__Init__(void)
{
    graphics = Graphics(800,800);
    running = true;
    updating = false;
    updatingstep = false;
    nowDrawing = false;
    counter = 0;
    update_counter = 0;
    speed = 3;
    whatDraw = 1;
}

LApp::LApp()
{
    __Init__();
    Board.x = Board.y = 40;
}

LApp::LApp(unsigned int x, unsigned int y)
{
    __Init__();
    Board.x = x;
    Board.y = y;
}

int LApp::Execute()
{
    if(!Init()) return -1;
    SDL_Event event;
    
    while (running) {
	while (SDL_PollEvent(&event)) Event(&event);
	Loop();
	Render();
	SDL_Delay(5);
    }
    Clean();
    return 0;
}

bool LApp::Init()
{
    graphics.grid = Grid(Board.x, Board.y, 10);
    life = LLife(Board.x, Board.y);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) return false;
   
    return graphics.Init();
}

void LApp::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // CatchErrorOpengl(__LINE__);
    // glFramebufferTexture2D(GL_FRAMEBUFFER,
    //  			   GL_COLOR_ATTACHMENT0,
    //  			   GL_TEXTURE_2D,
    //  			   texture,
    //  			   0);
    //CatchErrorOpengl(__LINE__);
    graphics.grid.DrawWithMap(life.front);

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // CatchErrorOpengl(__LINE__);

    //glBindTexture(GL_TEXTURE_2D, texture);

    //Bloom.Begin();
 //    if(Bloom.Running) glUniform1i(uniform_tex0, 1);
    
     
    // glBegin(GL_QUADS);
    // glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
    // glTexCoord2d(1.0,0.0); glVertex2d(1.0,0.0);
    // glTexCoord2d(1.0,1.0); glVertex2d(1.0,1.0);
    // glTexCoord2d(0.0,1.0); glVertex2d(0.0,1.0);
    // glEnd();

    //Bloom.End();

    // glDeleteTextures(1, &texture);
    graphics.grid.DrawBorder();
    if (graphics.grid.cellsize > 22) graphics.grid.Draw();

    SDL_GL_SwapBuffers();
}

void LApp::Loop()
{
    if (updatingstep && (update_counter == 0)) life.Update();
    if (updatingstep && (update_counter < 20)) update_counter++;
    if (updatingstep && (update_counter == 20)) counter++;


    if (updating) counter++;
    if (counter > speed) {
	life.Update();
	counter = 0;
    }
}

void LApp::Clean()
{
    glDeleteFramebuffers(1, &graphics.fbo);
    SDL_FreeSurface(graphics.display);
    SDL_Quit();
    std::cout<<"Clean normal, exitting\n";
}

/* ---------------------------------------------  */
int main(int argc, char **argv)
{
    if (argc<3) {
	LApp Life(50, 50);
	return Life.Execute();
    } else {
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	LApp Life(x, y);
	return Life.Execute();
    }
}

/* ---------------------------------------------  */
inline void CatchErrorOpengl(int l)
{
    GLenum errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
	std::cout<<"Opengl error "<<errCode<<" on line "<<l - 1<<std::endl;
}
