#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

int contador = 0;

Menu::Menu()
{
    accion = 0;
    actionConfirmed = false;
    currentTime = 0.0f;
    contador = 0;
    cur1 = true;
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

    // 创建背景四边形
    glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    fondo = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

    // 加载背景图片 - 使用PNG格式
    imgFondo.loadFromFile("images/menu_background.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // 创建光标四边形 - 初始位置在START GAME
    glm::vec2 geom2[2] = { glm::vec2(START_GAME_X, START_GAME_Y),
                          glm::vec2(START_GAME_X + CURSOR_SIZE, START_GAME_Y + CURSOR_SIZE) };
    glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);

    // 加载光标图片
    imgCursor.loadFromFile("images/cursor1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    cur1 = true;

    projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
}

void Menu::update(int deltaTime)
{
    currentTime += deltaTime;

    // 光标闪烁动画
    ++contador;
    if (contador > 30) {
        if (cur1) {
            imgCursor.loadFromFile("images/cursor2.png", TEXTURE_PIXEL_FORMAT_RGBA);
            cur1 = false;
        }
        else {
            imgCursor.loadFromFile("images/cursor1.png", TEXTURE_PIXEL_FORMAT_RGBA);
            cur1 = true;
        }
        contador = 0;
    }

    // 处理键盘输入
    if (Game::instance().getKey(GLFW_KEY_DOWN)) {
        Game::instance().keyReleased(GLFW_KEY_DOWN); // 重置按键状态
        accion = (accion + 1) % 3;
    }
    else if (Game::instance().getKey(GLFW_KEY_UP)) {
        Game::instance().keyReleased(GLFW_KEY_UP); // 重置按键状态
        accion = (accion - 1 + 3) % 3; // 确保不会变成负数
    }

    // 更新光标位置
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

    // 重新创建光标四边形到新位置
    delete cursor;
    glm::vec2 geom2[2] = { glm::vec2(cursorX, cursorY),
                          glm::vec2(cursorX + CURSOR_SIZE, cursorY + CURSOR_SIZE) };
    glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);

    // 处理确认选择 (Enter键)
    if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE)) {
        Game::instance().keyReleased(GLFW_KEY_ENTER);
        Game::instance().keyReleased(GLFW_KEY_SPACE);
        actionConfirmed = true;
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

    // 渲染背景
    fondo->render(imgFondo);

    // 渲染光标
    cursor->render(imgCursor);
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