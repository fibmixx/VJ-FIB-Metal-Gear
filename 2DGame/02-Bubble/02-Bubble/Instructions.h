#pragma once
#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "TexturedQuad.h"

class Instructions
{
public:
    Instructions();
    ~Instructions();

    void init();
    void render();
    void update(int deltaTime);

    bool shouldReturnToMenu() const { return returnToMenu; }
    void resetReturnFlag() { returnToMenu = false; }

private:
    void initShaders();

    ShaderProgram texProgram;
    glm::mat4 projection;

    TexturedQuad* fondo;
    Texture imgFondo;

    bool returnToMenu;

    const int CAMERA_WIDTH = 640;
    const int CAMERA_HEIGHT = 480;
};

#endif // _INSTRUCTIONS_INCLUDE