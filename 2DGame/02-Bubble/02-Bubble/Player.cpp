#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#include <chrono>
#include <thread>
#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN,
	PUNCH_LEFT, PUNCH_RIGHT, PUNCH_UP, PUNCH_DOWN, DEAD
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	punching = false;
	punchTimer = 0; punchDuration = 300;
	openTimer = 300; openDuration = 300;
	dirPunch = -1;
	maxHealth = 100;
	health =maxHealth;
	shooted = false;
	deathTimer = 0; deathDuration = 6000;
	wonTimer = 0; wonDuration = 500; won = false;
	godMode = false;	
	godModeEffectTimer = 0;
	healthBar.init(SCREEN_WIDTH, SCREEN_HEIGHT, shaderProgram);

	end.init(); deadScreen = false;

	inventory.clear();
	currentEquipped = NO_ITEM;

	
	shootCooldown = 0;
	this->shaderProgram = &shaderProgram;  
	tileMapDispl = tileMapPos;
	bullet1 = NULL;
	bullet2 = NULL;
	bullet3 = NULL;
	bullet4 = NULL;
	bullet5 = NULL;
	currentBullet = 0;

	bullet1 = new EnemyBullet();
	bullet1->init(tileMapDispl, shaderProgram);
	bullet1->setTileMap(map);
	bullet1->setSource(true);
	bullet1->setDamage(25);
	bullet1->setSpeed(10.0f);
	bullet1->deactivate();

	bullet2 = new EnemyBullet();
	bullet2->init(tileMapDispl, shaderProgram);
	bullet2->setTileMap(map);
	bullet2->setSource(true);
	bullet2->setDamage(25);
	bullet2->setSpeed(10.0f);
	bullet2->deactivate();

	bullet3 = new EnemyBullet();
	bullet3->init(tileMapDispl, shaderProgram);
	bullet3->setTileMap(map);
	bullet3->setSource(true);
	bullet3->setDamage(25);
	bullet3->setSpeed(10.0f);
	bullet3->deactivate();


	bullet4 = new EnemyBullet();
	bullet4->init(tileMapDispl, shaderProgram);
	bullet4->setTileMap(map);
	bullet4->setSource(true);
	bullet4->setDamage(25);
	bullet4->setSpeed(10.0f);
	bullet4->deactivate();

	bullet5 = new EnemyBullet();
	bullet5->init(tileMapDispl, shaderProgram);
	bullet5->setTileMap(map);
	bullet5->setSource(true);
	bullet5->setDamage(25);
	bullet5->setSpeed(10.0f);
	bullet5->deactivate();
	sizePlayer = glm::ivec2(18*4, 34*4);
	spritesheet.loadFromFile("images/snake_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//									MIDA PLAYER			MIDA SPRITES
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(1.f/15.f, 0.5), &spritesheet, &shaderProgram);
	// Numero de animaciones
	sprite->setNumberAnimations(13);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(STAND_UP, 8);
	sprite->addKeyframe(STAND_UP, glm::vec2(1.f / 15.f, 0.f));

	sprite->setAnimationSpeed(STAND_DOWN, 8);
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2.f / 15.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3.f / 15.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_UP, 8);
	sprite->addKeyframe(MOVE_UP, glm::vec2(3.f / 15.f, 0.f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(5.f / 15.f, 0.f));

	sprite->setAnimationSpeed(MOVE_DOWN, 8);
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(2.f / 15.f, 0.f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(4.f / 15.f, 0.f));

	sprite->setAnimationSpeed(PUNCH_LEFT, 8);
	sprite->addKeyframe(PUNCH_LEFT, glm::vec2(12.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_RIGHT, 8);
	sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(13.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_UP, 8);
	sprite->addKeyframe(PUNCH_UP, glm::vec2(13.f / 15.f, 0.f));

	sprite->setAnimationSpeed(PUNCH_DOWN, 8);
	sprite->addKeyframe(PUNCH_DOWN, glm::vec2(12.f / 15.f, 0.f));

	sprite->setAnimationSpeed(DEAD, 1);
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(1.f / 15.f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(1.f / 15.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(14.f / 15.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(14.f / 15.f, 0.5f));
	

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	
	shootCooldown = 0;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);


	if (shootCooldown > 0) {
		shootCooldown -= deltaTime;
		shooted = false;
	}
	if (openTimer > 0) {
		opening = false;
		openTimer -= deltaTime;
	}


	if (bullet1 != NULL) bullet1->update(deltaTime);
	if (bullet2 != NULL) bullet2->update(deltaTime);
	if (bullet3 != NULL) bullet3->update(deltaTime);
	if (bullet4 != NULL) bullet4->update(deltaTime);
	if (bullet5 != NULL) bullet5->update(deltaTime);

	// C per canviar
	if (Game::instance().getKey(GLFW_KEY_C)) {
		Game::instance().keyReleased(GLFW_KEY_C);
		equipNextItem();
	}

	//  X per usar
	if (Game::instance().getKey(GLFW_KEY_X)) {
		if (!inventory.empty()) {               
			Game::instance().keyReleased(GLFW_KEY_X);
			useCurrentItem();
		}
	}
	//  I afegir tots els items a l'inventari
	if (Game::instance().getKey(GLFW_KEY_I)) {
		Game::instance().keyReleased(GLFW_KEY_I);
		addAllItemsToInventory();
	}
	if (isDead()) {
			if (sprite->animation() != DEAD) {
				sprite->changeAnimation(DEAD);
				AudioManager::instance().playSound("audio/Snake Dead.wav", 0.5f);
			}
			deathTimer += deltaTime;
			if (deathTimer >= deathDuration)
			{
				end.init();
				deadScreen = true;
			}
	}
	if (won) {
			wonTimer += deltaTime;
			if (wonTimer >= wonDuration) {
				end.init();
				deadScreen = true;
			}

		}

		if (godMode) {
			godModeEffectTimer += deltaTime;
		}
		// G god mode
		if (Game::instance().getKey(GLFW_KEY_G) && !isDead()) {
			Game::instance().keyReleased(GLFW_KEY_G);
			toggleGodMode();
		}

		//F recupera tota vida
		if (Game::instance().getKey(GLFW_KEY_H) && !isDead()) {
			Game::instance().keyReleased(GLFW_KEY_H);
			fullHeal();
		}





		if (!punching && !isDead()) {


			// Cop de puny
			if (Game::instance().getKey(GLFW_KEY_Z))
			{
				punching = true;
				punchTimer = 0;
			}
			// Moviment, només 1 direcció a la vegada
			else if (Game::instance().getKey(GLFW_KEY_UP) || Game::instance().getKey(GLFW_KEY_W))
			{
				if (sprite->animation() != MOVE_UP)
					sprite->changeAnimation(MOVE_UP);
				posPlayer.y -= 4;
				if (map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
				{
					sprite->changeAnimation(STAND_UP);
					posPlayer.y += 4;
				}
			}
			else if (Game::instance().getKey(GLFW_KEY_DOWN) || Game::instance().getKey(GLFW_KEY_S))
			{
				if (sprite->animation() != MOVE_DOWN)
					sprite->changeAnimation(MOVE_DOWN);
				posPlayer.y += 4;
				if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
				{
					sprite->changeAnimation(STAND_DOWN);
					posPlayer.y -= 4;
				}
			}
			else if (Game::instance().getKey(GLFW_KEY_LEFT) || Game::instance().getKey(GLFW_KEY_A))
			{
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
				posPlayer.x -= 4;
				if (map->collisionMoveLeft(posPlayer, sizePlayer))
				{
					posPlayer.x += 4;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			else if (Game::instance().getKey(GLFW_KEY_RIGHT) || Game::instance().getKey(GLFW_KEY_D))
			{
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
				posPlayer.x += 4;
				if (map->collisionMoveRight(posPlayer, sizePlayer))
				{
					posPlayer.x -= 4;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else
			{
				if (sprite->animation() == MOVE_UP)
					sprite->changeAnimation(STAND_UP);
				else if (sprite->animation() == MOVE_DOWN)
					sprite->changeAnimation(STAND_DOWN);
				else if (sprite->animation() == MOVE_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == MOVE_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}

			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		}
		// PUNCHING
		else if (punching && !isDead()) {
			punchTimer += deltaTime;

			if (sprite->animation() == MOVE_UP || sprite->animation() == STAND_UP) {
				sprite->changeAnimation(PUNCH_UP);
				dirPunch = 3;
			}

			else if (sprite->animation() == MOVE_DOWN || sprite->animation() == STAND_DOWN) {
				sprite->changeAnimation(PUNCH_DOWN);
				dirPunch = 0;
			}

			else if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
				sprite->changeAnimation(PUNCH_LEFT);
				dirPunch = 1;
			}

			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(PUNCH_RIGHT);
				dirPunch = 2;
			}


			if (punchTimer >= punchDuration) {
				punching = false;
				if (sprite->animation() == PUNCH_UP)
					sprite->changeAnimation(STAND_UP);
				else if (sprite->animation() == PUNCH_DOWN)
					sprite->changeAnimation(STAND_DOWN);
				else if (sprite->animation() == PUNCH_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == PUNCH_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
				dirPunch = -1;
			}

		}
	
}

void Player::render()
{
	if (!isDead()) {

		healthBar.render(health, maxHealth);
		healthBar.renderObj(inventory, currentEquipped);
	}

	if (bullet1 != NULL && bullet1->isActive()) bullet1->render();
	if (bullet2 != NULL && bullet2->isActive()) bullet2->render();
	if (bullet3 != NULL && bullet3->isActive()) bullet3->render();
	if (bullet4 != NULL && bullet4->isActive()) bullet4->render();
	if (bullet5 != NULL && bullet5->isActive()) bullet5->render();

	if (godMode) {
		
		if ((godModeEffectTimer / 200) % 2 == 0) {
			sprite->render();
		}
	
	}
	else {
		sprite->render();
	}
	if(deadScreen) end.render();
	
}

void Player::setTileMap(TileMap* tileMap)
{
	
	map = tileMap;
	if (bullet1 != NULL) bullet1->setTileMap(map);
	if (bullet2 != NULL) bullet2->setTileMap(map);
	if (bullet3 != NULL) bullet3->setTileMap(map);
	if (bullet4 != NULL) bullet4->setTileMap(map);
	if (bullet5 != NULL) bullet5->setTileMap(map);
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::returnPosPlayer() {
	return posPlayer;
}

int Player::isPunching()
{
	return dirPunch;
}

bool Player::punchEnemy(const glm::ivec2& posEnemy, const glm::ivec2& sizeEnemy)
{
	if (!punching) return false; // per si de cas

	int punchRange = 20;  //distancia
	glm::ivec2 punchBoxPos = posPlayer;
	glm::ivec2 punchBoxSize = sizePlayer;

	switch (dirPunch)
	{
	case 0: // DOWN
		punchBoxPos.y += sizePlayer.y;  
		punchBoxSize.y = punchRange;    
		break;

	case 3: // UP
		punchBoxPos.y -= punchRange;    
		punchBoxSize.y = punchRange;
		break;

	case 1: // LEFT
		punchBoxPos.x -= punchRange;    
		punchBoxSize.x = punchRange;
		break;

	case 2: // RIGHT
		punchBoxPos.x += sizePlayer.x;  
		punchBoxSize.x = punchRange;
		break;

	default:
		return false;
	}

	// comprobar si hit o si no
	bool overlapX = (punchBoxPos.x < posEnemy.x + sizeEnemy.x) &&
		(punchBoxPos.x + punchBoxSize.x > posEnemy.x);
	bool overlapY = (punchBoxPos.y < posEnemy.y + sizeEnemy.y) &&
		(punchBoxPos.y + punchBoxSize.y > posEnemy.y);

	return overlapX && overlapY;
}

bool Player::openDoor(const glm::ivec2& posDoor, const glm::ivec2& sizeDoor)
{
	if (!opening) return false;
	//mismo codigo que punch pero con opening
	int punchRange = 20;  
	glm::ivec2 punchBoxPos = posPlayer;
	glm::ivec2 punchBoxSize = sizePlayer;

	switch (sprite->animation())
	{
	case STAND_DOWN: // DOWN
		punchBoxPos.y += sizePlayer.y;
		punchBoxSize.y = punchRange;
		break;

	case STAND_UP: // UP
		punchBoxPos.y -= punchRange;
		punchBoxSize.y = punchRange;
		break;

	case STAND_LEFT: // LEFT
		punchBoxPos.x -= punchRange;
		punchBoxSize.x = punchRange;
		break;

	case STAND_RIGHT: // RIGHT
		punchBoxPos.x += sizePlayer.x;
		punchBoxSize.x = punchRange;
		break;

	default:
		return false;
	}

	// comprobar si hit o si no
	bool overlapX = (punchBoxPos.x < posDoor.x + sizeDoor.x) &&
		(punchBoxPos.x + punchBoxSize.x > posDoor.x);
	bool overlapY = (punchBoxPos.y < posDoor.y + sizeDoor.y) &&
		(punchBoxPos.y + punchBoxSize.y > posDoor.y);

	return overlapX && overlapY;
}

bool Player::collisionEnemy(const glm::ivec2& posEnemy, const glm::ivec2& sizeEnemy) {
	bool overlapX = (posPlayer.x + 10 < posEnemy.x + sizeEnemy.x - 10) &&
		(posPlayer.x + sizePlayer.x - 10> posEnemy.x + 10);
	bool overlapY = (posPlayer.y + 50 < posEnemy.y + sizeEnemy.y - 10) &&
		(posPlayer.y + sizePlayer.y - 10 > posEnemy.y + 10);

	return overlapX && overlapY;
}


void Player::heal(int amount)
{
	health += amount;
	if (health > maxHealth) health = maxHealth;
	AudioManager::instance().playSound("audio/Heal.wav", 0.5f);

}

void Player::takeDamage(int damage)
{
	if (godMode|| isDead()) return;
	health -= damage;
	if (health > 0) {
		AudioManager::instance().playSound("audio/Snake damaged.wav", 0.5f);
	}
	if (health < 0) health = 0;
}

void Player::toggleGodMode()
{
	godMode = !godMode;
	godModeEffectTimer = 0;

}

void Player::fullHeal()
{
	health = maxHealth;
	AudioManager::instance().playSound("audio/Heal.wav", 0.5f);
}

void Player::collisionDoor(const glm::ivec2& posDoor, const glm::ivec2& sizeDoor)
{
	if (collisionEnemy(posDoor, sizeDoor))
		posPlayer.y += 4;
}

void Player::win()
{
	if (!won) {
		won = true;
		AudioManager::instance().stopMusic();
		AudioManager::instance().playSound("audio/Heal.wav", 0.5f);
	}
}

void Player::collectPickup(int pickupType)
{
	if (!hasItem(pickupType)) {
		addToInventory(pickupType);
	}
}

void Player::addToInventory(int pickupType)
{
	inventory.push_back(pickupType);

	if (currentEquipped == NO_ITEM) {
		currentEquipped = 0;
	}

}


void Player::equipNextItem()
{
	if (inventory.empty()) {
		currentEquipped = NO_ITEM;
		return;
	}

	currentEquipped = (currentEquipped + 1) % inventory.size();
	
}

void Player::useCurrentItem()
{

	int itemType = inventory[currentEquipped];

	switch (itemType) {
	case 0: //cura
		heal(25); 
		inventory.erase(inventory.begin() + currentEquipped);
		if (inventory.empty()) {
			currentEquipped = NO_ITEM;
		}
		else {
			currentEquipped = currentEquipped % inventory.size();
		}
		break;

	case 1: // arma
		if (shootCooldown <= 0) {
			shooted = true;
			shoot();
			shootCooldown = SHOOT_COOLDOWN_TIME;
		}
		break;
		

	case 2: //targeta
		
		if (openTimer <= 0) {
			opening = true;
			openTimer = openDuration;
		}
		break;
	}
}

void Player::addAllItemsToInventory()
{

	if (!hasItem(0)) {
		addToInventory(0);
	}

	
	if (!hasItem(1)) {
		addToInventory(1);
	}

	
	if (!hasItem(2)) {
		addToInventory(2);
	}

}

void Player::desactivatebullets()
{
	bullet1->deactivate();
	bullet2->deactivate();
	bullet3->deactivate();
	bullet4->deactivate();
	bullet5->deactivate();
}

bool Player::hasItem(int itemId) const
{
	for (int item : inventory) {
		if (item == itemId) {
			return true;
		}
	}
	return false;
}

void Player::shoot()
{
	if (!hasItem(1)) return;
	AudioManager::instance().playSound("audio/Shoot.wav", 0.5f);
	EnemyBullet* bulletToUse = NULL;

	if (currentBullet == 0) {
		bulletToUse = bullet1;
		currentBullet = 1;
	}
	else if (currentBullet == 1) {
		bulletToUse = bullet2;
		currentBullet = 2;
	}
	else if (currentBullet == 2) {
		bulletToUse = bullet3;
		currentBullet = 3;
	}
	else if (currentBullet == 3) {
		bulletToUse = bullet4;
		currentBullet = 4;
	}
	else {
		bulletToUse = bullet5;
		currentBullet = 0;
	}

	if (bulletToUse && bulletToUse->isActive()) {
		if (bullet1 && !bullet1->isActive()) bulletToUse = bullet1;
		else if (bullet2 && !bullet2->isActive()) bulletToUse = bullet2;
		else if (bullet3 && !bullet3->isActive()) bulletToUse = bullet3;
		else if (bullet4 && !bullet4->isActive()) bulletToUse = bullet4;
		else if (bullet5 && !bullet5->isActive()) bulletToUse = bullet5;
		else return; 
	}

	if (bulletToUse) {
		glm::vec2 shootPos = glm::vec2(posPlayer.x + sizePlayer.x / 2 - 8,posPlayer.y + sizePlayer.y / 2 - 8);
		glm::vec2 direction;

		int anim = sprite->animation();
		if (anim == STAND_UP || anim == MOVE_UP || anim == PUNCH_UP) direction = glm::vec2(0, -1);
		
		else if (anim == STAND_DOWN || anim == MOVE_DOWN || anim == PUNCH_DOWN) direction = glm::vec2(0, 1);
		
		else if (anim == STAND_LEFT || anim == MOVE_LEFT || anim == PUNCH_LEFT) direction = glm::vec2(-1, 0);
		
		else direction = glm::vec2(1, 0);
		

		bulletToUse->shoot(shootPos, direction);
	}
}

std::vector<EnemyBullet*> Player::getBullets()
{
	std::vector<EnemyBullet*> activeBullets;

	if (bullet1 && bullet1->isActive()) activeBullets.push_back(bullet1);
	if (bullet2 && bullet2->isActive()) activeBullets.push_back(bullet2);
	if (bullet3 && bullet3->isActive()) activeBullets.push_back(bullet3);
	if (bullet4 && bullet4->isActive()) activeBullets.push_back(bullet4);
	if (bullet5 && bullet5->isActive()) activeBullets.push_back(bullet5);

	return activeBullets;
}



