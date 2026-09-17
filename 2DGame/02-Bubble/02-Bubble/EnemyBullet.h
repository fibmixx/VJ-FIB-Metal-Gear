#ifndef _ENEMYBULLET_INCLUDE
#define _ENEMYBULLET_INCLUDE

#pragma once
#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"
class Player;
class EnemyBullet {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setPlayer(Player* play);
    void setDirection(const glm::vec2& dir);
    void setDamage(int dmg) { damage = dmg; }
    void setSpeed(float spd) { speed = spd; }
    void setSource(bool isPlayerBullet) { fromPlayer = isPlayerBullet; }

    bool isActive() const;
    void deactivate();

    void shoot(const glm::vec2& pos, const glm::vec2& dir);

    glm::ivec2 getPosition() const { return posBullet; }
    glm::ivec2 getSize() const { return glm::ivec2(16, 16); }
    bool isFromPlayer() const { return fromPlayer; }

private:
    glm::ivec2 tileMapDispl, posBullet;
    glm::vec2 direction;
    bool active;
    bool fromPlayer; 
    int damage;
    float speed;
    Texture spritesheet;
    Sprite* sprite;
    Player* player;
    TileMap* map;
};


#endif // _ENEMYBULLET_INCLUDE