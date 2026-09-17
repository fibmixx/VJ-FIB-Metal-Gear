#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"
#include "EnemyBullet.h"
#include "AudioManager.h"


class Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int eType, int eState);
	void update(int deltaTime);
	void render();

	void setPlayer(Player* p);

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void prepareBullet(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void pathfinding(int deltatime, glm::vec2 dir);
	void updateGuard(int deltaTime);
	void updateDog(int deltaTime);
	void updateScorpion(int deltaTime);
	void setEnemy(int eType, int eState);
	void seeEnemy();
	void takeDamage();
	bool isDead();
	bool isDog();
	bool isAlert();
	void disableBullet();
	void setChasing() { state = 2; }

	bool isActive() const { return !dead; }
	glm::ivec2 getPosition() const { return posEnemy; }
	glm::ivec2 getSize() const { return sizeEnemy; }
	std::vector<EnemyBullet>& getBullets(); 
	EnemyBullet* getBullet() { return bullet; }
	

private:
	int type;
	int state;
	int lookAt; 
	int idleTimer; int idleSwapDuration;
	int guardingTimer; int guardingSwapDuration;
	int scorpionDuration;

	bool evading = false;      // estamos en maniobra de evasión
	int evadeAxis = 0;         // 0 = none, 1 = evadir en X, 2 = evadir en Y (perpendicular al movimiento principal)
	int evadeSign = 0;         // +1 o -1, dirección de la evasión
	int evadeAttempts = 0;     // contador para abandonar si no consigue nada
	const int MAX_EVADE_ATTEMPTS = 6000; // frames/intentos antes de rendirse

	float directionTimer = 0.0f;
	float directionChangeDelay = 200.0f;
	glm::ivec2 currentDir = glm::ivec2(0,0);

	int health; bool dead; 
	bool damaged; int damagedTimer;
	bool dealtdamage; int dealtTimer;
	int speed;
	int shootTimer; int shootDuration;

	glm::ivec2 tileMapDispl, posEnemy, sizeEnemy, posPlayer, sizePlayer;

	Texture spritesheet;
	Player* player;
	Sprite* sprite;
	TileMap* map;
	EnemyBullet* bullet;
};

#endif // _ENEMY_INCLUDE


