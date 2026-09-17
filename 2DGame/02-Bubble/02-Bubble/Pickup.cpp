#include "Pickup.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include <iostream>
std::string Pickup::getTexturePath(PickupType type)
{
    switch (type) {
    case HEALTH:
        return "images/health.png";  
    case WEAPON:
        return "images/weapon.png";  
    case KEY:
        return "images/card.png";    
    default:
        return "images/health.png";  
    }
}

void Pickup::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, PickupType pickupType, const glm::ivec2& pos)
{
    pickedUp = false;
    type = pickupType;
    posPickup = pos;
    sizePickup = glm::ivec2(40, 40);


    std::string texturePath = getTexturePath(type);
    spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);


    sprite = Sprite::createSprite(sizePickup, glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);

    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
    sprite->changeAnimation(0);

    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPickup.x), float(tileMapDispl.y + posPickup.y)));
}

void Pickup::update(int deltaTime)
{
    if (!pickedUp) {
        sprite->update(deltaTime);
       
        float floatOffset = sin(glfwGetTime() * 3) * 2;
        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPickup.x),
            float(tileMapDispl.y + posPickup.y + floatOffset)));
    }
}

void Pickup::render()
{
    if (!pickedUp) {
        sprite->render();
    }
}

void Pickup::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Pickup::setPosition(const glm::ivec2& pos)
{
    posPickup = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPickup.x), float(tileMapDispl.y + posPickup.y)));
}
