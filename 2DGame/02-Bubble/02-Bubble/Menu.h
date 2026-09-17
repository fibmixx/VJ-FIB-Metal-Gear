#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include "AudioManager.h"

class Menu
{
public:
    Menu();
    ~Menu();

    void init();
    void update(int deltaTime);
    void render();

    enum MenuAction {
        START_GAME = 0,
        INSTRUCTIONS = 1,
        CREDITS = 2
    };

    int getCurrentAction() const { return accion; }
    bool isActionConfirmed() const { return actionConfirmed; }
    void resetAction() { actionConfirmed = false; }

private:
    void initShaders();
    void updateCursorPosition();

    ShaderProgram texProgram;
    glm::mat4 projection;

    TexturedQuad* fondo;
    TexturedQuad* cursor;
    Texture imgFondo;
    Texture imgCursor;

    float currentTime;
    int accion; // 0: START GAME, 1: INSTRUCTIONS, 2: CREDITS
    bool actionConfirmed;

    bool cursorVisible;


  

    const float CURSOR_SIZE = 32.f;

    const float START_GAME_X = 220.f;
    const float START_GAME_Y = 270.f;
    const float INSTRUCTIONS_X = 150.f;
    const float INSTRUCTIONS_Y = 320.f;
    const float CREDITS_X = 210.f;
    const float CREDITS_Y = 370.f;


};

#endif // _MENU_INCLUDE