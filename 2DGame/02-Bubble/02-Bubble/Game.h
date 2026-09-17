#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Menu.h"
#include "Credits.h"
#include "Instructions.h"
#include "End.h"



#define SCREEN_WIDTH 32*32
#define SCREEN_HEIGHT 24*32+100


enum GameState {
    MAIN_MENU,
    IN_GAME,
    INSTRUCTIONS_SCREEN,
    CREDITS_SCREEN
};

class Game
{
private:
    Game() {}

public:
    static Game& instance()
    {
        static Game G;
        return G;
    }

    void init();
    bool update(int deltaTime);
    void render();

    // Input callback methods
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMove(int x, int y);
    void mousePress(int button);
    void mouseRelease(int button);

    bool getKey(int key) const;


    GameState getGameState() const { return gameState; }
    void setGameState(GameState state) { gameState = state; }
    void handleMenuAction(int action);

private:
    bool bPlay;
    bool keys[GLFW_KEY_LAST + 1];
    Scene scene;
    Menu menu;
    Credits credits;
    Instructions instructions;
    GameState gameState;
    End end;
};

#endif // _GAME_INCLUDE