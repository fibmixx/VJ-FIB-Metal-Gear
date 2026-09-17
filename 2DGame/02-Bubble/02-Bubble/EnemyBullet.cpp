#include "EnemyBullet.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"

void EnemyBullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
 

    active = false;
    tileMapDispl = tileMapPos;

    damage = 50;
    speed = 8.0f;
    fromPlayer = false;

    spritesheet.loadFromFile("images/bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);
}

void EnemyBullet::update(int deltaTime)
{
    if (!active) return;

    posBullet += glm::ivec2(direction * 8.f); //moverse + velocidad

    // Colisión con el mapa
    if (map->collisionMoveLeft(posBullet, glm::ivec2(8, 8)) ||
        map->collisionMoveRight(posBullet, glm::ivec2(8, 8)) ||
        map->collisionMoveUp(posBullet, glm::ivec2(8, 8), &posBullet.y) ||
        map->collisionMoveDown(posBullet, glm::ivec2(8, 8), &posBullet.y)) {
        active = false;
        return;
    }   
    if (!fromPlayer && player && player->collisionEnemy(posBullet, glm::vec2(16,16))) {
        player->takeDamage(50);
        active = false;
    }

    sprite->setPosition(glm::vec2(tileMapDispl.x + posBullet.x, tileMapDispl.y + posBullet.y));
}

void EnemyBullet::render()
{
   
    if (active) sprite->render();


    
}

void EnemyBullet::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void EnemyBullet::setPosition(const glm::vec2& pos)
{
    posBullet = pos;
    sprite->setPosition(glm::vec2(tileMapDispl.x + posBullet.x, tileMapDispl.y + posBullet.y));
}

void EnemyBullet::setPlayer(Player* play)
{
    player = play;
}

void EnemyBullet::setDirection(const glm::vec2& dir)
{
    direction = glm::normalize(dir);
}

bool EnemyBullet::isActive() const
{
    return active;
}

void EnemyBullet::deactivate()
{
    active = false;
}

void EnemyBullet::shoot(const glm::vec2& pos, const glm::vec2& dir) {
    setPosition(pos);
    setDirection(dir);
    active = true;
}

