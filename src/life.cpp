#include "life.hpp"
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
    speed = 2;
    whatDraw = 1;
    help = false;
}

LApp::LApp()
{
    __Init__();
    Board.x = Board.y = 40;
}

LApp::LApp(unsigned int x, unsigned int y, std::string r)
{
    __Init__();
    Board.x = x;
    Board.y = y;
    rule = r;
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
    size_t found;
    
    graphics.grid = Grid(Board.x, Board.y, 10);
    life = Automaton(Board.x, Board.y);
    
    AutomatonState dead = AutomatonState(0, 0.0, 0.0, 0.0, "dead");
    AutomatonState live = AutomatonState(1, 0.3, 0.8765, 0.3, "live");
    
    std::map <statecode, Set<int>> born_req;
    Set <int> born_req_count;

    found = rule.find("/");
    if (found == std::string::npos) return false;
    for (size_t i = found+1; i<rule.length(); i++)
	born_req_count.add((int)(rule[i]-48));
    born_req[live.code] = born_req_count;
    
    std::map <statecode, Set<int>> surv_req;
    Set <int> surv_req_count;

    for (size_t i = 0; i<found; i++)
	surv_req_count.add((int)(rule[i]-48));
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

    glEnable(GL_BLEND);

    position.x = 160;
    position.y = 700;
    graphics.hud.RenderText("Несложный симулятор \"Жизни\" Конвея", &position, &color, size24);
    position.x = 55;
    position.y = 600;
    position.y -= position.h;
    graphics.hud.RenderText("F1                                       показать/скрыть эту справку", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("Пробел                                   остановить/продолжить", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("S                                        один шаг", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("R                                        случайно заполнить поле", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("C                                        очистить поле", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("Правая кнопка мыши                       рисовать/стирать", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("Левая кнопка мыши                        перемещать обзор", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("Колесо мыши                              приблизить/отдалить", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("Num +                                    увеличить скорость", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("Num -                                    уменьшить скорость", &position, &color, size16);
    position.y -= position.h;
    graphics.hud.RenderText("ESC                                      выход", &position, &color, size16);

    glDisable(GL_BLEND);

    HUD::glDisable2D();

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
    TTF_CloseFont(graphics.hud.font16);
    TTF_CloseFont(graphics.hud.font24);
    TTF_Quit();
    SDL_FreeSurface(graphics.display);
    SDL_Quit();
    std::cout<<"Clean normal, exitting\n";
}

/* ---------------------------------------------  */
inline void CatchErrorOpengl(int l)
{
    GLenum errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
	std::cout<<"Opengl error "<<errCode<<" on line "<<l - 1<<std::endl;
}
