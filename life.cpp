#include "life.hpp"
#include "graphics.hpp"

#include <iostream>

namespace ca {
BoardSize::BoardSize(u32 width, u32 height)
    : x(width)
    , y(height) {}

LApp::LApp(u32 x, u32 y, std::string _rule)
    : board(x, y)
    , life(new Automaton)
    , graphics(new Graphics)
    , rule(_rule) {}

int LApp::execute() {
    if (!init()) {
        return -1;
    }
    running = true;

    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            process_event(&event);
        }
        loop();
        render();
        SDL_Delay(10);
    }
    clean();
    return 0;
}

bool LApp::init() {
    size_t found;

    graphics->grid = Grid(board.x, board.y, 10);
    life = std::make_unique<Automaton>(board.x, board.y);

    AutomatonState dead = AutomatonState(0, 0.0, 0.0, 0.0, "dead");
    AutomatonState live = AutomatonState(1, 0.3, 0.8765, 0.3, "live");

    std::map<statecode, Set<int>> born_req;
    Set<int> born_req_count;

    found = rule.find("/");
    if (found == std::string::npos) {
        return false;
    }
    for (size_t i = found + 1; i < rule.length(); i++) {
        born_req_count.add(static_cast<int>(rule[i] - 48));
    }
    born_req[live.code] = born_req_count;

    std::map<statecode, Set<int>> surv_req;
    Set<int> surv_req_count;

    for (size_t i = 0; i < found; i++) {
        surv_req_count.add(static_cast<int>(rule[i] - 48));
    }
    surv_req[live.code] = surv_req_count;

    AutomatonTransition born =
        AutomatonTransition(live.code, dead.code, born_req, "born");
    AutomatonTransition survive =
        AutomatonTransition(live.code, dead.code, surv_req, "survive");

    life->add_state(dead, born);
    life->add_state(live, survive);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        return false;
    }
    return graphics->init();
}

void LApp::render() {
    if (help) {
        render_help();
    } else {
        render_field();
    }
}

void LApp::render_help() {
    glClear(GL_COLOR_BUFFER_BIT);
    HUD::gl_enable2_d();

    SDL_Rect position;
    SDL_Color color = { 255, 255, 255, 0 };

    glEnable(GL_BLEND);

    position.x = 160;
    position.y = 700;
    graphics->hud.render_text("Несложный симулятор \"Жизни\" Конвея", &position,
                            &color, FontSize::size24);
    position.x = 55;
    position.y = 600;
    position.y -= position.h;
    graphics->hud.render_text(
        "F1                                       показать/скрыть эту справку",
        &position, &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "Пробел                                   остановить/продолжить",
        &position, &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text("S                                        один шаг",
                            &position, &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "R                                        случайно заполнить поле",
        &position, &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "C                                        очистить поле", &position,
        &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "Правая кнопка мыши                       рисовать/стирать", &position,
        &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "Левая кнопка мыши                        перемещать обзор", &position,
        &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "Колесо мыши                              приблизить/отдалить",
        &position, &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "Num +                                    увеличить скорость",
        &position, &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text(
        "Num -                                    уменьшить скорость",
        &position, &color, FontSize::size16);
    position.y -= position.h;
    graphics->hud.render_text("ESC                                      выход",
                            &position, &color, FontSize::size16);

    glDisable(GL_BLEND);

    HUD::gl_disable2_d();
    graphics->present();
}

void LApp::render_field() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(static_cast<GLfloat>(graphics->dx),
                 static_cast<GLfloat>(graphics->dy), 0.0);

    graphics->grid.draw_with_map(life);
    graphics->grid.draw_border();

    if (graphics->grid.cellsize > 22) {
        graphics->grid.draw();
    }

    glPopMatrix();

    if (!updating) {
        HUD::gl_enable2_d();
        glEnable(GL_BLEND);

        SDL_Rect position;
        SDL_Color color = { 255, 255, 255, 0 };
        position.x = 10;
        position.y = 760;
        graphics->hud.render_text("Пауза", &position, &color, FontSize::size24);

        glDisable(GL_BLEND);
        HUD::gl_disable2_d();
    }

    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(0, 800);
    glVertex2f(0, 760);
    glVertex2f(800, 760);
    glVertex2f(800, 800);
    glEnd();
    glPopMatrix();

    HUD::gl_enable2_d();

    SDL_Rect position;
    SDL_Color color = { 255, 255, 255, 0 };

    char buff[64];

    position.x = 10;
    position.y = 0;
    graphics->hud.render_text(("Правило: " + rule).c_str(), &position, &color,
                              FontSize::size16);
    position.x = 300;
    sprintf(buff, "Активных: %d", life->state_count[1]);
    graphics->hud.render_text(buff, &position, &color, FontSize::size16);
    position.x = 500;
    sprintf(buff, "Неактивных: %d", life->state_count[0]);
    graphics->hud.render_text(buff, &position, &color, FontSize::size16);

    HUD::gl_disable2_d();

    graphics->present();
}

void LApp::loop() {
    if (updatingstep && (update_counter == 0)) {
        life->update();
    }
    if (updatingstep && (update_counter < 20)) {
        update_counter++;
    }
    if (updatingstep && (update_counter == 20)) {
        counter++;
    }

    if (updating) {
        counter++;
    }
    if (counter > speed) {
        life->update();
        counter = 0;
    }
}

LApp::~LApp() {
    SDL_Quit();
    std::cout << "Clean normal, exitting\n";
}

/* --------------------------------------------- */
inline void catch_error_opengl(int l) {
    GLenum err_code;
    if ((err_code = glGetError()) != GL_NO_ERROR) {
        std::cout << "Opengl error " << err_code << " on line " << l - 1
                  << std::endl;
    }
}
} // namespace ca
