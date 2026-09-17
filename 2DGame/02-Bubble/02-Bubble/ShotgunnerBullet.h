#ifndef _ShotgunnerBULLET_INCLUDE
#define _ShotgunnerBULLET _INCLUDE

#pragma once
#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"

class ShotgunnerBullet {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setPlayer(Player* play);
    void setDirection(const glm::vec2& dir);
    bool isActive() const;
    void deactivate();

    void shoot(const glm::vec2& pos, const glm::vec2& dir);

private:
    glm::ivec2 tileMapDispl, posBullet;
    glm::vec2 direction;
    bool active;
    Texture spritesheet;
    Sprite* sprite;
    Player* player;
    TileMap* map;
};


#endif // _Shotgunner_INCLUDE