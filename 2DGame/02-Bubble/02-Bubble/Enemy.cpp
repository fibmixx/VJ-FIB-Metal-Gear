#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enemy.h"
#include "Game.h"

enum EnemyType
{
	GUARD, DOG, SCORPION
};

enum GuardAnims {
	STAND_DOWN, STAND_RIGHT, STAND_UP, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN
};

enum States {
	IDLE, GUARDING, CHASING, DEAD
};

enum Dir {
	DOWN, LEFT, RIGHT, UP
};

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int eType, int eState)
{
	type = eType;
	state = eState; lookAt = 0;
	
	idleTimer = 0; guardingTimer = 0;
	idleSwapDuration = 2000; guardingSwapDuration = 1500;
	damagedTimer = 0; dealtTimer = 0;
	shootTimer = 2000; shootDuration = 2500; //empieza casi a punto de disparar

	damaged = false; dealtdamage = false;
	scorpionDuration = 500;
	dead = false;
	if (type == GUARD) {
		health = 75; 
		speed = 2;

		sizeEnemy = glm::ivec2(18 * 4, 34 * 4);
		spritesheet.loadFromFile("images/guard_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		//								MIDA Enemy			MIDA SPRITES
		sprite = Sprite::createSprite(sizeEnemy, glm::vec2(1.f / 4.f, 0.5), &spritesheet, &shaderProgram);
		// Numero de animaciones
		sprite->setNumberAnimations(8);

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(1.f / 4.f, 0.5f));

		sprite->setAnimationSpeed(STAND_UP, 8);
		sprite->addKeyframe(STAND_UP, glm::vec2(1.f / 4.f, 0.f));

		sprite->setAnimationSpeed(STAND_DOWN, 8);
		sprite->addKeyframe(STAND_DOWN, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2.f / 4.f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0, 0.5f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1.f / 4.f, 0.5f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3.f / 4.f, 0.5f));

		sprite->setAnimationSpeed(MOVE_UP, 8);
		sprite->addKeyframe(MOVE_UP, glm::vec2(3.f / 4.f, 0.f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(1.f / 4.f, 0.f));


		sprite->setAnimationSpeed(MOVE_DOWN, 8);
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(2.f / 4.f, 0.f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.f));


	}
	if (type == DOG) {
		health = 50; 
		speed = 3;
		sizeEnemy = glm::ivec2(34 * 3, 34 * 3);
		spritesheet.loadFromFile("images/dog_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		//								MIDA Enemy			MIDA SPRITES
		sprite = Sprite::createSprite(sizeEnemy, glm::vec2(1.f / 10.f,1.f), &spritesheet, &shaderProgram);
		// Numero de animaciones
		sprite->setNumberAnimations(8);

		sprite->setAnimationSpeed(STAND_DOWN, 1);
		sprite->addKeyframe(STAND_DOWN, glm::vec2(0.f / 10.f, 0.f));
		sprite->addKeyframe(STAND_DOWN, glm::vec2(1.f / 10.f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(6.f / 10.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(7.f / 10.f, 0.f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8.f / 10.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9.f / 10.f, 0.f));

		sprite->setAnimationSpeed(MOVE_UP, 8);
		sprite->addKeyframe(MOVE_UP, glm::vec2(4.f / 10.f, 0.f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(5.f / 10.f, 0.f));


		sprite->setAnimationSpeed(MOVE_DOWN, 8);
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(2.f / 10.f, 0.f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(3.f / 10.f, 0.f));
	}
	if (type == SCORPION) {
		health = 25;
		speed = 1;
		sizeEnemy = glm::ivec2(18 * 4, 18 * 4);
		spritesheet.loadFromFile("images/scorpion_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		//								MIDA Enemy			MIDA SPRITES
		sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
		// Numero de animaciones
		sprite->setNumberAnimations(8);

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(STAND_UP, 8);
		sprite->addKeyframe(STAND_UP, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(STAND_DOWN, 8);
		sprite->addKeyframe(STAND_DOWN, glm::vec2(0.5f, 0.f));
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::update(int deltaTime)
{
	if (bullet != NULL)
		bullet->update(deltaTime);
	sprite->update(deltaTime);
	if (dead) return; 
	if (player->isDead()) return;
	if (player->punchEnemy(posEnemy, sizeEnemy) && !damaged && !dead) {
		takeDamage();
		state = CHASING;
		damaged = true;
	}
	if (player->collisionEnemy(posEnemy, sizeEnemy) && !damaged && !dealtdamage && !dead) {
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
	if (type == GUARD && !damaged && !dead)
		updateGuard(deltaTime);
	if (type == DOG && !damaged && !dead)
		updateDog(deltaTime);
	if (type == SCORPION && !damaged && !dead)
		updateScorpion(deltaTime);
}

void Enemy::updateGuard(int deltatime) {
	posPlayer = player->returnPosPlayer();
	glm::vec2 dir(posPlayer - posEnemy);
	if (state == IDLE) {
		idleTimer += deltatime;

		if (idleTimer >= idleSwapDuration) {
			idleTimer = 0;
			if (lookAt == DOWN) { // MIRANT A BAIX
				lookAt = LEFT;
				sprite->changeAnimation(STAND_LEFT);
			}
			else if (lookAt == LEFT) { // MIRANT ESQUERRA
				lookAt = RIGHT;
				sprite->changeAnimation(STAND_RIGHT);
			}
			else if (lookAt == RIGHT) { // MIRANT DRETA
				lookAt = UP;
				sprite->changeAnimation(STAND_UP);
			}
			else if (lookAt == UP) { // MIRANT A DALT
				lookAt = DOWN;
				sprite->changeAnimation(STAND_DOWN);
			}
		}
		seeEnemy();
	}
	else if (state == GUARDING) {
		guardingTimer += deltatime;
		if (sprite->animation() == MOVE_LEFT) {
			posEnemy.x -= speed;
			if (map->collisionMoveLeft(posEnemy, sizeEnemy)) {
				posEnemy.x += speed;
				lookAt = RIGHT;
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else if (sprite->animation() == MOVE_RIGHT) {
			posEnemy.x += speed;
			if (map->collisionMoveRight(posEnemy, sizeEnemy)) {
				posEnemy.x -= speed;
				lookAt = LEFT;
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
		
		if (guardingTimer >= guardingSwapDuration) {
			guardingTimer = 0;
			switch (lookAt) {
			case DOWN: 
				lookAt = LEFT;
				sprite->changeAnimation(MOVE_LEFT);
				break;
			case LEFT: 
				lookAt = UP;
				sprite->changeAnimation(STAND_UP);
				break;
			case UP: 
				lookAt = RIGHT;
				sprite->changeAnimation(MOVE_RIGHT);
				break;
			case RIGHT: 
				lookAt = DOWN;
				sprite->changeAnimation(STAND_DOWN);
				break;
			}
		}
		seeEnemy();
	}
	else if (state == CHASING) {
		pathfinding(deltatime, dir);
		shootTimer += deltatime;
		if (shootTimer >= shootDuration) {
			shootTimer = 0;
			bullet->shoot(glm::vec2(posEnemy + sizeEnemy/2), dir); //sale desde el centro
			AudioManager::instance().playSound("audio/Shoot.wav");
		}
			
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::updateDog(int deltatime) {
	posPlayer = player->returnPosPlayer();
	glm::vec2 dir(posPlayer - posEnemy);
	if (state == GUARDING) state = IDLE; //evitar que salga en guarding ya que no deberia
	if (state == IDLE) {
		if (abs(dir.x) < 200 && abs(dir.y) < 200) state = CHASING;

	}
	if (state == CHASING)
		pathfinding(deltatime, dir);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::updateScorpion(int deltatime) {
	posPlayer = player->returnPosPlayer();
	glm::vec2 dir(posPlayer - posEnemy); // no necesario pero por si acaso

	if (state == GUARDING || state == CHASING) state = IDLE; // evitar que salga en guarding/chasing ya que no deberia
	if (state == IDLE) {
		idleTimer += deltatime;
		if (sprite->animation() == STAND_DOWN) { posEnemy.x += speed; posEnemy.y += speed; }
		else if (sprite->animation() == STAND_UP) { posEnemy.x -= speed; posEnemy.y -= speed; }
		else if (sprite->animation() == STAND_RIGHT) { posEnemy.x += speed; posEnemy.y -= speed; }
		else if (sprite->animation() == STAND_LEFT) { posEnemy.x -= speed; posEnemy.y += speed; }

		if (idleTimer >= scorpionDuration) {
			idleTimer = 0;
			if (sprite->animation() == STAND_DOWN) { // MIRANT A BAIX
				sprite->changeAnimation(STAND_RIGHT);
			}
			else if (sprite->animation() == STAND_LEFT) { // MIRANT ESQUERRA
				sprite->changeAnimation(STAND_DOWN);
			}
			else if (sprite->animation() == STAND_RIGHT) { // MIRANT DRETA
				sprite->changeAnimation(STAND_UP);
			}
			else if (sprite->animation() == STAND_UP) { // MIRANT A DALT
				sprite->changeAnimation(STAND_LEFT);
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::seeEnemy()
{
	const float tolerance = 16.f;
	const float viewDistance = 400.f;

	glm::vec2 dir = posPlayer - posEnemy;
	float dist = glm::length(dir);

	bool canSee = false;

	// Si está muy cerca, lo detecta automáticamente
	if (dist < 100.f) {
		canSee = true;
	}
	else {
		// Calcular la "caja" del campo de visión según la dirección
		float left = posEnemy.x;
		float right = posEnemy.x + sizeEnemy.x;
		float top = posEnemy.y;
		float bottom = posEnemy.y + sizeEnemy.y;

		float viewLeft, viewRight, viewTop, viewBottom;

		switch (lookAt) {
		case LEFT:
			viewLeft = left - viewDistance;
			viewRight = left;
			viewTop = top - tolerance;
			viewBottom = bottom + tolerance;
			break;
		case RIGHT:
			viewLeft = right;
			viewRight = right + viewDistance;
			viewTop = top - tolerance;
			viewBottom = bottom + tolerance;
			break;
		case UP:
			viewTop = top - viewDistance;
			viewBottom = top;
			viewLeft = left - tolerance;
			viewRight = right + tolerance;
			break;
		case DOWN:
			viewTop = bottom;
			viewBottom = bottom + viewDistance;
			viewLeft = left - tolerance;
			viewRight = right + tolerance;
			break;
		}

		// Verifica si el jugador está dentro del rectángulo de visión
		if (posPlayer.x + sizePlayer.x > viewLeft && posPlayer.x < viewRight &&
			posPlayer.y + sizePlayer.y > viewTop && posPlayer.y < viewBottom)
		{
			// (opcional) comprobar línea de visión
			if (!map->hasObstacleBetween(posEnemy, posPlayer))
				canSee = true;
		}
	}

	// Cambiar de estado si lo ve
	if (canSee)
		state = CHASING;
}

void Enemy::takeDamage()
{
	health -= 25;
	std::cout << health << std::endl;
	if (health > 0) AudioManager::instance().playSound("audio/Enemy Damaged.wav");
	if (health <= 0) {
		dead = true;
		state = IDLE;
		AudioManager::instance().playSound("audio/Enemy Dead.wav");
		posEnemy = glm::ivec2(-10000, -10000);
		sprite->setPosition(glm::vec2(-10000.f, -10000.f));
		state = DEAD;
	}
}

bool Enemy::isDead()
{
	return dead;
}

void Enemy::render()
{
	if (!dead) sprite->render();
	if (bullet != nullptr)
		bullet->render();
}

void Enemy::setPlayer(Player* p)
{
	player = p;
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Enemy::setEnemy(int eType, int eState) {
	state = eState;
}

bool Enemy::isDog() {
	return type == DOG;
}

bool Enemy::isAlert() {
	return state == CHASING;
}

void Enemy::disableBullet()
{
	bullet->deactivate();
}

void Enemy::pathfinding(int deltatime, glm::vec2 dir) {
	bool moved = false;

	directionTimer += deltatime;


	// Solo actualizar dirección cada cierto tiempo
	if (directionTimer >= directionChangeDelay) {
		// Calcular la dirección bruta hacia el jugador
		int rawX = (dir.x > 0) ? 1 : (dir.x < 0 ? -1 : 0);
		int rawY = (dir.y > 0) ? 1 : (dir.y < 0 ? -1 : 0);

		// Determinar qué eje tiene más distancia y priorizarlo
		if (abs(dir.x) > abs(dir.y)) {
			currentDir = { rawX, 0 }; // movimiento horizontal
		}
		else {
			currentDir = { 0, rawY }; // movimiento vertical
		}

		directionTimer = 0.0f;
	}


	if (abs(currentDir.x) > abs(currentDir.y))
	{
		if (currentDir.x > 0) {

			posEnemy.x += speed;
			if (map->collisionMoveRight(posEnemy, sizeEnemy))
				posEnemy.x -= speed;
			else {
				moved = true;
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else {

			posEnemy.x -= speed;
			if (map->collisionMoveLeft(posEnemy, sizeEnemy))
				posEnemy.x += speed;
			else {
				moved = true;
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
			}
		}
	}
	else
	{
		if (currentDir.y > 0) {

			posEnemy.y += speed;
			if (map->collisionMoveDown(posEnemy, sizeEnemy, &posEnemy.y))
				posEnemy.y -= speed;
			else {
				moved = true;
				if (sprite->animation() != MOVE_DOWN)
					sprite->changeAnimation(MOVE_DOWN);
			}
		}
		else {

			posEnemy.y -= speed;
			if (map->collisionMoveUp(posEnemy, sizeEnemy, &posEnemy.y))
				posEnemy.y += speed;
			else {
				moved = true;
				if (sprite->animation() != MOVE_UP)
					sprite->changeAnimation(MOVE_UP);
			}
		}
	}
	if (!moved)
	{
		auto tryMoveX = [&](int dx)->bool {
			posEnemy.x += dx;
			bool coll = map->collisionMoveLeft(posEnemy, sizeEnemy) || map->collisionMoveRight(posEnemy, sizeEnemy);
			if (coll) posEnemy.x -= dx;
			return !coll;
			};
		auto tryMoveY = [&](int dy)->bool {
			posEnemy.y += dy;
			bool coll = map->collisionMoveUp(posEnemy, sizeEnemy, &posEnemy.y) || map->collisionMoveDown(posEnemy, sizeEnemy, &posEnemy.y);
			if (coll) posEnemy.y -= dy;
			return !coll;
			};
		auto clearLineToPlayer = [&]()->bool {
			glm::vec2 playerPos = player->returnPosPlayer(); // sustituye playerRef por tu referencia al player
			// Si el movimiento principal era horizontal (dir.x más grande)
			if (abs(currentDir.x) > abs(currentDir.y)) {
				// comprobar si hay obstáculos entre posEnemy.x y playerPos.x avanzando en pasos pequeños
				glm::ivec2 tmpPos = posEnemy;
				int step = (currentDir.x > 0 ? 2 : -2);
				while ((step > 0 && tmpPos.x < playerPos.x) || (step < 0 && tmpPos.x > playerPos.x)) {
					tmpPos.x += step;
					if (map->collisionMoveRight(tmpPos, sizeEnemy) || map->collisionMoveLeft(tmpPos, sizeEnemy))
						return false;
				}
				return true;
			}
			else {
				// movimiento vertical
				glm::ivec2 tmpPos = posEnemy;
				int step = (currentDir.y > 0 ? 2 : -2);
				while ((step > 0 && tmpPos.y < playerPos.y) || (step < 0 && tmpPos.y > playerPos.y)) {
					tmpPos.y += step;
					if (map->collisionMoveDown(tmpPos, sizeEnemy, &tmpPos.y) || map->collisionMoveUp(tmpPos, sizeEnemy, &tmpPos.y))
						return false;
				}
				return true;
			}
			};

		// Si ya estamos en evasión, intentar continuar con la misma dirección preferida
		if (evading)
		{
			evadeAttempts++;
			bool success = false;
			if (evadeAxis == 2) { // evadiendo en Y (nos movemos arriba/abajo)
				success = tryMoveY(evadeSign * 2);
				if (success) {
					if (evadeSign > 0 && sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
					if (evadeSign < 0 && sprite->animation() != MOVE_UP)   sprite->changeAnimation(MOVE_UP);
				}
				else {
					// si falla, intenta la otra dirección perpendicular una vez
					success = tryMoveY(-evadeSign * 2);
					if (success) {
						evadeSign = -evadeSign; // cambiar de lado si ha funcionado
						if (evadeSign > 0 && sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
						if (evadeSign < 0 && sprite->animation() != MOVE_UP)   sprite->changeAnimation(MOVE_UP);
					}
				}
			}
			else if (evadeAxis == 1) { // evadiendo en X (nos movemos izq/dcha)
				success = tryMoveX(evadeSign * 2);
				if (success) {
					if (evadeSign > 0 && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
					if (evadeSign < 0 && sprite->animation() != MOVE_LEFT)  sprite->changeAnimation(MOVE_LEFT);
				}
				else {
					success = tryMoveX(-evadeSign * 2);
					if (success) {
						evadeSign = -evadeSign;
						if (evadeSign > 0 && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
						if (evadeSign < 0 && sprite->animation() != MOVE_LEFT)  sprite->changeAnimation(MOVE_LEFT);
					}
				}
			}

			// Si la línea al jugador queda libre -> acabar evasión
			if (clearLineToPlayer() || evadeAttempts > MAX_EVADE_ATTEMPTS) {
				evading = false;
				evadeAttempts = 0;
			}

			// si pudo moverse, marca moved = true para que no reentre en la selección
			if (success) moved = true;
		}
		else
		{
			// No está evadiendo: elegir el lado perpendicular y "comprometerse" si es posible
			if (abs(currentDir.x) > abs(currentDir.y)) {
				// movimiento principal horizontal -> evadir moviendo en Y (arriba/abajo)
				int prefer = (currentDir.y > 0 ? +1 : -1); // preferir hacia el jugador
				if (tryMoveY(prefer * 2)) {
					evading = true; evadeAxis = 2; evadeSign = prefer; evadeAttempts = 0;
					if (prefer > 0 && sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
					if (prefer < 0 && sprite->animation() != MOVE_UP)   sprite->changeAnimation(MOVE_UP);
					moved = true;
				}
				else if (tryMoveY(-prefer * 2)) {
					evading = true; evadeAxis = 2; evadeSign = -prefer; evadeAttempts = 0;
					if (evadeSign > 0 && sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
					if (evadeSign < 0 && sprite->animation() != MOVE_UP)   sprite->changeAnimation(MOVE_UP);
					moved = true;
				}
				else {
					// ambos lados bloqueados -> intenta moverse ligeramente en X (retroceso / cambio)
					// esto quizá permita "desatascar" si el obstáculo es pequeño
					if (tryMoveX((currentDir.x > 0 ? -1 : 1) * 2)) {
						if (currentDir.x > 0 && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
						if (currentDir.x < 0 && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
						moved = true;
					}
				}
			}
			else {
				// movimiento principal vertical -> evadir moviendo en X (izq/dcha)
				int prefer = (currentDir.x > 0 ? +1 : -1);
				if (tryMoveX(prefer * 2)) {
					evading = true; evadeAxis = 1; evadeSign = prefer; evadeAttempts = 0;
					if (prefer > 0 && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
					if (prefer < 0 && sprite->animation() != MOVE_LEFT)  sprite->changeAnimation(MOVE_LEFT);
					moved = true;
				}
				else if (tryMoveX(-prefer * 2)) {
					evading = true; evadeAxis = 1; evadeSign = -prefer; evadeAttempts = 0;
					if (evadeSign > 0 && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
					if (evadeSign < 0 && sprite->animation() != MOVE_LEFT)  sprite->changeAnimation(MOVE_LEFT);
					moved = true;
				}
				else {
					// ambos lados bloqueados -> intentar un pequeño movimiento vertical de retroceso
					if (tryMoveY((currentDir.y > 0 ? -1 : 1) * 2)) {
						if (currentDir.y > 0 && sprite->animation() != MOVE_UP) sprite->changeAnimation(MOVE_UP);
						if (currentDir.y < 0 && sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
						moved = true;
					}
				}
			}
		}
	}
}

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::prepareBullet(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	bullet = new EnemyBullet();
	bullet->init(tileMapPos, shaderProgram);
	bullet->setTileMap(map);
	bullet->setPlayer(player);
	bullet->deactivate(); // asegurar que empieza inactiva
}

std::vector<EnemyBullet>& Enemy::getBullets()
{
	
	static std::vector<EnemyBullet> bullets; 
	if (bullet && bullet->isActive()) {
		bullets.clear();
		bullets.push_back(*bullet);
	}
	return bullets;
}