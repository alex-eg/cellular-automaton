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
    help = false;
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
	SDL_Delay(1);
    }
    Clean();
    return 0;
}

bool LApp::Init()
{
    graphics.grid = Grid(Board.x, Board.y, 10);
    life = Automaton(Board.x, Board.y);
    
    AutomatonState dead = AutomatonState(0, 0.0, 0.0, 0.0, "dead");
    AutomatonState live = AutomatonState(1, 0.3, 0.8765, 0.3, "live");
    
    std::map <statecode, Set<int>> born_req;
	Set <int> born_req_count;
    // born_req_count.add(3); Game Of Life
	// born_req_count.add(3); AMOEBA
	// born_req_count.add(5);
	// born_req_count.add(7);
	born_req_count.add(3);
	born_req_count.add(5);
	born_req_count.add(6);
	born_req_count.add(7);
	born_req_count.add(8);
	born_req[live.code] = born_req_count;
    
    std::map <statecode, Set<int>> surv_req;
    Set <int> surv_req_count;
    // surv_req_count.add(3); Game Of life
    // surv_req_count.add(2);
	// surv_req_count.add(1); AMOEBA
	// surv_req_count.add(3);
	// surv_req_count.add(5);
	// surv_req_count.add(8);
	surv_req_count.add(5);
	surv_req_count.add(6);
	surv_req_count.add(7);
	surv_req_count.add(8);
    surv_req[live.code] = surv_req_count;

    AutomatonTransition born = AutomatonTransition(live.code, dead.code, born_req, "born");
    AutomatonTransition survive = AutomatonTransition(live.code, dead.code, surv_req, "survive");

    life.AddState(dead, born);
    life.AddState(live, survive);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) return false;
    return graphics.Init();
}

void LApp::Render()
{
    if (help) Render_Help();
    else Render_Field();
}

void LApp::Render_Help()
{
    glClear(GL_COLOR_BUFFER_BIT);
    HUD::glEnable2D();

    SDL_Rect position;
    SDL_Color color = { 255, 255, 255 };
 
    position.x = 160;
    position.y = 700;
    graphics.hud.RenderText("Несложный симулятор \"Жизни\" Конвея", &position, &color);
    position.x = 30;
    position.y = 600;
    position.y -= position.h;
    graphics.hud.RenderText("F1                           показать/скрыть эту справку", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("Пробел                       остановить/продолжить", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("S                            один шаг", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("R                            случайно заполнить поле", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("C                            очистить поле", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("Правая кнопка мыши           рисовать/стирать", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("Левая кнопка мыши            перемещать обзор", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("Колесо мыши                  приблизить/отдалить", &position, &color);
    position.y -= position.h;
    graphics.hud.RenderText("ESC                          выход", &position, &color);

    HUD::glDisable2D();

    SDL_GL_SwapBuffers();
}

void LApp::Render_Field()
{

    glPushMatrix();
    glTranslatef(graphics.dx, graphics.dy, 0.0);
 
    glClear(GL_COLOR_BUFFER_BIT);

    graphics.grid.DrawWithMap(life);
    graphics.grid.DrawBorder();
    
    if (graphics.grid.cellsize > 22) graphics.grid.Draw();

    glPopMatrix();
    
    if (!updating) {
	HUD::glEnable2D();
	
	SDL_Rect position;
	SDL_Color color = {0, 128, 255};
	position.x = 10;
	position.y = 760;
	graphics.hud.RenderText("Пауза", &position, &color);

	HUD::glDisable2D();
    }
    
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
    glFinish();
    if (graphics.fbo != 0)
	glDeleteFramebuffers(1, &graphics.fbo);
    TTF_CloseFont(graphics.hud.font);
    TTF_Quit();
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
