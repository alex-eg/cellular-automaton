#include "state.hpp"

void StateField::Event(SDL_event *event)
{

}

void StateField::Render()
{
    glClear(GL_COLOR_BUFFER_BIT); 

    glPushMatrix();
    glTranslatef(graphics.dx, graphics.dy, 0.0);

    graphics.grid.DrawWithMap(life);
    graphics.grid.DrawBorder();
    
    if (graphics.grid.cellsize > 22) graphics.grid.Draw();

    glPopMatrix();
    
    if (!updating) {
	HUD::glEnable2D();
	glEnable(GL_BLEND);

	SDL_Rect position;
	SDL_Color color = {255, 255, 255};
	position.x = 10;
	position.y = 760;
	graphics.hud.RenderText("Пауза", &position, &color, size24);

	glDisable(GL_BLEND);
	HUD::glDisable2D();	
    }

    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(  0, 800);
    glVertex2f(  0, 760);
    glVertex2f(800, 760);
    glVertex2f(800, 800);
    glEnd();
    glPopMatrix();

    HUD::glEnable2D();

    SDL_Rect position;
    SDL_Color color = {255, 255, 255};

    char buff[64];
   
    position.x = 10;
    position.y = 0;
    graphics.hud.RenderText(("Правило: "+rule).c_str(), &position, &color, size16);
    position.x = 300;
    sprintf(buff, "Активных: %d", life.StateCount[1]);
    graphics.hud.RenderText(buff, &position, &color, size16);
    position.x = 500;
    sprintf(buff, "Неактивных: %d", life.StateCount[0]);
    graphics.hud.RenderText( buff, &position, &color, size16);
    
    HUD::glDisable2D();

    SDL_GL_SwapBuffers();
}
    
void StateField::Event_Field(SDL_Event *Event)
{
    switch (Event->type) {
    case SDL_KEYDOWN: {
        KeyDown(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
        break;
    }
    case SDL_KEYUP: {
        KeyUp(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
        break;
    }
    case SDL_QUIT: {
        running = false;
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
	MouseButtonDown(Event->button.button, Event->button.x, Event->button.y);
	break;
    }
    case SDL_MOUSEMOTION: {
	bool Left, Right, Middle;
	Left = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
	Middle = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
	Right = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);
	MouseMove(Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel, Left, Right, Middle);
	break;
    }
    default:
        break;
    }
}

