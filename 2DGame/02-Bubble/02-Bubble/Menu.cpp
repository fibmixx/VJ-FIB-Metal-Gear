#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

Menu::Menu()
{
    accion = 0;
    actionConfirmed = false;
    currentTime = 0.0f;
    cursorVisible = false;
    fondo = nullptr;
    cursor = nullptr;
}

Menu::~Menu()
{
    if (fondo != nullptr) delete fondo;
    if (cursor != nullptr) delete cursor;
}

void Menu::init()
{
    initShaders();
    currentTime = 0.0f;
    accion = 0;
    actionConfirmed = false;
    cursorVisible = true; 

 
    glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    fondo = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);


    imgFondo.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);

 
    cursor = nullptr;


    imgCursor.loadFromFile("images/cursor.png", TEXTURE_PIXEL_FORMAT_RGBA);

    projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

    updateCursorPosition();

    AudioManager::instance().init();
    AudioManager::instance().playMusic("audio/Start.mp3", false);
}

void Menu::update(int deltaTime)
{
    currentTime += deltaTime;

    if (Game::instance().getKey(GLFW_KEY_DOWN)) {
        Game::instance().keyReleased(GLFW_KEY_DOWN);
        accion = (accion + 1) % 3;
        cursorVisible = true; 
        updateCursorPosition();
    }
    else if (Game::instance().getKey(GLFW_KEY_UP)) {
        Game::instance().keyReleased(GLFW_KEY_UP);
        accion = (accion - 1 + 3) % 3;
        cursorVisible = true; 
        updateCursorPosition();
    }

 
}

void Menu::updateCursorPosition()
{
 
    if (cursorVisible && cursor == nullptr) {
        float cursorX, cursorY;
        switch (accion) {
        case START_GAME:
            cursorX = START_GAME_X;
            cursorY = START_GAME_Y;
            break;
        case INSTRUCTIONS:
            cursorX = INSTRUCTIONS_X;
            cursorY = INSTRUCTIONS_Y;
            break;
        case CREDITS:
            cursorX = CREDITS_X;
            cursorY = CREDITS_Y;
            break;
        }

        glm::vec2 geom2[2] = { glm::vec2(cursorX, cursorY),
                              glm::vec2(cursorX + CURSOR_SIZE, cursorY + CURSOR_SIZE) };
        glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
        cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
    }

    else if (cursorVisible && cursor != nullptr) {
        float cursorX, cursorY;
        switch (accion) {
        case START_GAME:
            cursorX = START_GAME_X;
            cursorY = START_GAME_Y;
            break;
        case INSTRUCTIONS:
            cursorX = INSTRUCTIONS_X;
            cursorY = INSTRUCTIONS_Y;
            break;
        case CREDITS:
            cursorX = CREDITS_X;
            cursorY = CREDITS_Y;
            break;
        }

        delete cursor;
        glm::vec2 geom2[2] = { glm::vec2(cursorX, cursorY),
                              glm::vec2(cursorX + CURSOR_SIZE, cursorY + CURSOR_SIZE) };
        glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
        cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
    }
  
    else if (!cursorVisible && cursor != nullptr) {
        delete cursor;
        cursor = nullptr;
    }
}

void Menu::render()
{
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);


    fondo->render(imgFondo);


    if (cursorVisible && cursor != nullptr) {
        cursor->render(imgCursor);
    }
}

void Menu::initShaders()
{
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled())
    {
        std::cout << "Vertex Shader Error" << std::endl;
        std::cout << "" << vShader.log() << std::endl << std::endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled())
    {
        std::cout << "Fragment Shader Error" << std::endl;
        std::cout << "" << fShader.log() << std::endl << std::endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked())
    {
        std::cout << "Shader Linking Error" << std::endl;
        std::cout << "" << texProgram.log() << std::endl << std::endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}