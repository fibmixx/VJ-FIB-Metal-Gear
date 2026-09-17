#pragma once
#ifndef _HEALTHBAR_INCLUDE
#define _HEALTHBAR_INCLUDE

#include <vector>
#include <string>
#include "Texture.h"
#include "ShaderProgram.h"

class HealthBar {
public:
    void init(int screenWidth, int screenHeight, ShaderProgram& shaderProgram);
    void render(int currentHealth, int maxHealth);
    void renderObj(const std::vector<int>& inventory, int currentEquipped);

private:
    int SCREEN_WIDTH, SCREEN_HEIGHT;

    Texture healthTexture;
    Texture weaponTexture;
    Texture keyTexture;
    Texture emptyTexture;
    Texture lifeLabelTexture;
    ShaderProgram* shaderProg;

    void loadTextures();
    void renderTexture(Texture& texture, int x, int y, int size);
    void renderTextureWithSize(Texture& texture, int x, int y, int width, int height);
};

#endif