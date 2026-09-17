#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Door.h"
#include "Game.h"


enum DoorAnims {
	CLOSED, OPENING
};

void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	openTimer = 0;
	openDuration = 500;
	active = false;

	sizeDoor = glm::ivec2(32 * 4, 32 * 4);
	spritesheet.loadFromFile("images/door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//								MIDA Door			MIDA SPRITES
	sprite = Sprite::createSprite(sizeDoor, glm::vec2(1.f / 2.f, 1.f), &spritesheet, &shaderProgram);
	// Numero de animaciones
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(CLOSED, 8);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(OPENING, 8);
	sprite->addKeyframe(OPENING, glm::vec2(1.f / 2.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}

void Door::update(int deltaTime)
{
	if (!active) return;
	
	if (player->openDoor(posDoor, sizeDoor))
		open();

	if (sprite->animation() == OPENING) {
		openTimer += deltaTime;
		if (openTimer >= openDuration)
			active = false;
	}
	
	glm::vec2 posPlayer = player->returnPosPlayer();
	player->collisionDoor(posDoor, sizeDoor);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}


void Door::render()
{
	if (active) sprite->render();

}

void Door::setPlayer(Player* p)
{
	player = p;
}

void Door::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}


void Door::setPosition(const glm::vec2& pos)
{
	posDoor = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}

void Door::toggleactive(bool a)
{
	active = a;
}

void Door::open() {
	sprite->changeAnimation(OPENING);
	AudioManager::instance().playSound("audio/Door open.wav", 0.5f);
}