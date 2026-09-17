#pragma once
#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

class Pickup
{
public:
    enum PickupType {
        HEALTH = 0,
        WEAPON = 1,
        KEY = 2
    };

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, PickupType type, const glm::ivec2& pos);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::ivec2& pos);
    glm::ivec2 getPosition() const { return posPickup; }
    glm::ivec2 getSize() const { return sizePickup; }
    PickupType getType() const { return type; }
    bool isPickedUp() const { return pickedUp; }
    void pickUp() { pickedUp = true; }

private:
    bool pickedUp;
    glm::ivec2 posPickup;
    glm::ivec2 sizePickup;
    glm::ivec2 tileMapDispl;
    TileMap* map;
    Texture spritesheet;
    Sprite* sprite;
    PickupType type;

    
    std::string getTexturePath(PickupType type);
};