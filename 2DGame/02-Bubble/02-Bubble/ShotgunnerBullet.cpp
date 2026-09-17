#include "ShotgunnerBullet.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"

void ShotgunnerBullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    active = false;
    tileMapDispl = tileMapPos;

    spritesheet.loadFromFile("images/shotgunner_bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);
}

void ShotgunnerBullet::update(int deltaTime)
{
    if (!active) return;

    posBullet += glm::ivec2(direction * 10.f); //moverse + velocidad

    // Colisión con el mapa
    if (map->collisionMoveLeft(posBullet, glm::ivec2(8, 8)) ||
        map->collisionMoveRight(posBullet, glm::ivec2(8, 8)) ||
        map->collisionMoveUp(posBullet, glm::ivec2(8, 8), &posBullet.y) ||
        map->collisionMoveDown(posBullet, glm::ivec2(8, 8), &posBullet.y)) {
        active = false;
        return;
    }
    if (player->collisionEnemy(posBullet, glm::vec2(16, 16))) {
        player->takeDamage(50);
        active = false;
    }

    sprite->setPosition(glm::vec2(tileMapDispl.x + posBullet.x, tileMapDispl.y + posBullet.y));
}

void ShotgunnerBullet::render()
{
    if (active) sprite->render();
}

void ShotgunnerBullet::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void ShotgunnerBullet::setPosition(const glm::vec2& pos)
{
    posBullet = pos;
    sprite->setPosition(glm::vec2(tileMapDispl.x + posBullet.x, tileMapDispl.y + posBullet.y));
}

void ShotgunnerBullet::setPlayer(Player* play)
{
    player = play;
}

void ShotgunnerBullet::setDirection(const glm::vec2& dir)
{
    direction = glm::normalize(dir);
}

bool ShotgunnerBullet::isActive() const
{
    return active;
}

void ShotgunnerBullet::deactivate()
{
    active = false;
}

void ShotgunnerBullet::shoot(const glm::vec2& pos, const glm::vec2& dir) {
    setPosition(pos);
    setDirection(dir);
    active = true;
}