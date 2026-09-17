#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Shotgunner.h"
#include "Game.h"

enum ShotAnims {
	STAND_DOWN, ROLL_LEFT, ROLL_RIGHT
};

void Shotgunner::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
		
	health = 300;
	damagedTimer = 0; dealtTimer = 0;
	shootTimer = 350; shootDuration = 700; //empieza casi a punto de disparar
	rollTimer = 0; rollDuration = 300;
	rollTimer2 = 0; rollDuration2 = 2000;
	isRolling = false;
	damaged = false; dealtdamage = false;
	dead = false;
	shoot1 = false;
	
	sizeShotgunner = glm::ivec2(18 * 4, 34 * 4);
	spritesheet.loadFromFile("images/shotgunner_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//								MIDA Shotgunner			MIDA SPRITES
	sprite = Sprite::createSprite(sizeShotgunner, glm::vec2(1.f / 4.f, 1.f), &spritesheet, &shaderProgram);
	// Numero de animaciones
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(STAND_DOWN, 8);
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(ROLL_LEFT, 8);
	sprite->addKeyframe(ROLL_LEFT, glm::vec2(1.f / 4.f, 0.f));
	sprite->addKeyframe(ROLL_LEFT, glm::vec2(2.f / 4.f, 0.f));
	sprite->addKeyframe(ROLL_LEFT, glm::vec2(3.f / 4.f, 0.f));
	sprite->addKeyframe(ROLL_LEFT, glm::vec2(1.f / 4.f, 0.f));
	

	sprite->setAnimationSpeed(ROLL_RIGHT, 8);
	sprite->addKeyframe(ROLL_RIGHT, glm::vec2(1.f / 4.f, 0.f));
	sprite->addKeyframe(ROLL_RIGHT, glm::vec2(3.f / 4.f, 0.f));
	sprite->addKeyframe(ROLL_RIGHT, glm::vec2(2.f / 4.f, 0.f));
	sprite->addKeyframe(ROLL_RIGHT, glm::vec2(1.f / 4.f, 0.f));
	

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShotgunner.x), float(tileMapDispl.y + posShotgunner.y)));
}

void Shotgunner::update(int deltaTime)
{
	if (bullet1 != NULL)
		bullet1->update(deltaTime);
	if (bullet2 != NULL)
		bullet2->update(deltaTime);
	sprite->update(deltaTime);
	if (player->isDead()) return;
	if (dead) return;
	if (player->punchEnemy(posShotgunner, sizeShotgunner) && !damaged && !dead) {
		takeDamage();
		damaged = true;
	}
	if (player->collisionEnemy(posShotgunner, sizeShotgunner) && !damaged && !dealtdamage && !dead) {
		player->takeDamage(25);
		dealtdamage = true;
	}
	if (damaged) { // No es mou quan es golpejat
		damagedTimer += deltaTime;
		if (damagedTimer >= 300) {
			damagedTimer = 0;
			damaged = false;
		}
	}
	if (dealtdamage) {
		dealtTimer += deltaTime;
		if (dealtTimer >= 1000) {
			dealtTimer = 0;
			dealtdamage = false;
		}
	}
	glm::vec2 posPlayer = player->returnPosPlayer();
	
	rollTimer2 += deltaTime;
	shootTimer += deltaTime;
	if (!isRolling && shootTimer >= shootDuration) {
		shootTimer = 0;
		glm::vec2 dir;
		if (abs(posPlayer.x - posShotgunner.x) > 100 && posPlayer.x > posShotgunner.x) dir = glm::vec2(1, 1);
		else if (abs(posPlayer.x - posShotgunner.x) > 100 && posPlayer.x < posShotgunner.x) dir = glm::vec2(-1, 1);
		else if (abs(posPlayer.x - posShotgunner.x) > 50 && posPlayer.x > posShotgunner.x) dir = glm::vec2(0.25, 1);
		else if (abs(posPlayer.x - posShotgunner.x) > 50 && posPlayer.x < posShotgunner.x) dir = glm::vec2(-0.25, 1);
		else dir = glm::vec2(0, 1);

		if (!shoot1) {
			bullet1->shoot(glm::vec2(posShotgunner + sizeShotgunner / 2), dir); //sale desde el centro
			shoot1 = !shoot1;
		}
		else {
			bullet2->shoot(glm::vec2(posShotgunner + sizeShotgunner / 2), dir); //sale desde el centro
			shoot1 = !shoot1;
		}
		AudioManager::instance().playSound("audio/Shoot.wav");
	}
	if (!isRolling && rollTimer2 >= rollDuration2) {

		// Decide hacia dónde rodar según la posición del jugador
		if (abs(posPlayer.x - posShotgunner.x) > 100 && posPlayer.x > posShotgunner.x) {
			sprite->changeAnimation(ROLL_RIGHT);
			rollDir = 1;
			isRolling = true;
		}
		else if (abs(posPlayer.x - posShotgunner.x) > 100 && posPlayer.x < posShotgunner.x) {
			sprite->changeAnimation(ROLL_LEFT);
			rollDir = -1;
			isRolling = true;
		}
		else {
			sprite->changeAnimation(STAND_DOWN);
			rollDir = 0;
			return;
		}

		rollTimer = 0;
		rollTimer2 = 0;
	}
	if (isRolling) {
		rollTimer += deltaTime;

		if (rollDir < 0) { 
			posShotgunner.x -= 8;
			if (map->collisionMoveLeft(posShotgunner, sizeShotgunner)) {
				posShotgunner.x += 8;
				isRolling = false;
				sprite->changeAnimation(STAND_DOWN);
			}
		}
		else if (rollDir > 0) { 
			posShotgunner.x += 8;
			if (map->collisionMoveRight(posShotgunner, sizeShotgunner)) {
				posShotgunner.x -= 8;
				isRolling = false;
				sprite->changeAnimation(STAND_DOWN);
			}
		}
		if (rollTimer >= rollDuration) {
			isRolling = false;
			sprite->changeAnimation(STAND_DOWN);
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShotgunner.x), float(tileMapDispl.y + posShotgunner.y)));
}



void Shotgunner::takeDamage()
{
	health -= 25;
	std::cout << health << std::endl;
	if (health > 0) AudioManager::instance().playSound("audio/Enemy Damaged.wav");
	if (health <= 0) {
		dead = true;
		AudioManager::instance().playSound("audio/Enemy Dead.wav");
		posShotgunner = glm::ivec2(-10000, -10000);
		sprite->setPosition(glm::vec2(-10000.f, -10000.f));
	}
}

bool Shotgunner::isDead()
{
	return dead;
}

void Shotgunner::toggleDeath(bool d)
{
	dead = d;
}

void Shotgunner::render()
{
	if (!dead) sprite->render();
	if (bullet1 != nullptr)
		bullet1->render();
	if (bullet2 != nullptr)
		bullet2->render();
}

void Shotgunner::setPlayer(Player* p)
{
	player = p;
}

void Shotgunner::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}


void Shotgunner::setPosition(const glm::vec2& pos)
{
	posShotgunner = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posShotgunner.x), float(tileMapDispl.y + posShotgunner.y)));
}

void Shotgunner::prepareBullet(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	bullet1 = new ShotgunnerBullet();
	bullet1->init(tileMapPos, shaderProgram);
	bullet1->setTileMap(map);
	bullet1->setPlayer(player);
	bullet1->deactivate(); // asegurar que empieza inactiva
	bullet2 = new ShotgunnerBullet();
	bullet2->init(tileMapPos, shaderProgram);
	bullet2->setTileMap(map);
	bullet2->setPlayer(player);
	bullet2->deactivate(); // asegurar que empieza inactiva
}

std::vector<ShotgunnerBullet*> Shotgunner::getBullets()
{
	std::vector<ShotgunnerBullet*> activeBullets;

	if (bullet1 && bullet1->isActive()) {
		activeBullets.push_back(bullet1);
	}
	if (bullet2 && bullet2->isActive()) {
		activeBullets.push_back(bullet2);
	}

	return activeBullets;
}

void Shotgunner::disableBullet()
{
	if (bullet1) {
		bullet1->deactivate();
	}
	if (bullet2) {
		bullet2->deactivate();
	}
}