#ifndef _Shotgunner_INCLUDE
#define _Shotgunner_INCLUDE


#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"
#include "ShotgunnerBullet.h"
#include "AudioManager.h"

class Shotgunner
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPlayer(Player* p);

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void prepareBullet(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);


	void takeDamage();
	bool isDead();

	void toggleDeath(bool d);

	bool isActive() const { return !dead; }
	glm::ivec2 getPosition() const { return posShotgunner; }
	glm::ivec2 getSize() const { return sizeShotgunner; }

	
	ShotgunnerBullet* getBullet1() { return bullet1; }
	ShotgunnerBullet* getBullet2() { return bullet2; }

	
	std::vector<ShotgunnerBullet*> getBullets();

	void disableBullet();


private:
	int rollTimer, rollDuration; //cuanto dura el roll
	int rollTimer2, rollDuration2; //cada cuanto hace el roll
	bool isRolling; int rollDir;
	int health; bool dead;
	bool damaged; int damagedTimer;
	bool dealtdamage; int dealtTimer;
	int speed;
	int shootTimer; int shootDuration;
	bool shoot1;

	glm::ivec2 tileMapDispl, posShotgunner, sizeShotgunner, posPlayer, sizePlayer;

	Texture spritesheet;
	Player* player;
	Sprite* sprite;
	TileMap* map;
	ShotgunnerBullet* bullet1;
	ShotgunnerBullet* bullet2; //2 balas simultaneamente deberia ser suficiente
};

#endif // _Shotgunner_INCLUDE


