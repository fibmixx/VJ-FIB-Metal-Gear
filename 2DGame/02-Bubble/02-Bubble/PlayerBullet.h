#pragma once
#pragma once
#include "Sprite.h"
#include "TileMap.h"

class PlayerBullet {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setDirection(const glm::vec2& dir);

    void shoot(const glm::vec2& pos, const glm::vec2& dir);
    bool isActive() const;
    void deactivate();

private:
    bool active;
    glm::ivec2 tileMapDispl;
    glm::vec2 posBullet;
    glm::vec2 direction;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
};
