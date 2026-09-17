#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "End.h"
#include "HealthBar.h"
#include "EnemyBullet.h"
#include <vector>



#include "AudioManager.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class EnemyBullet;

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getSize() const { return sizePlayer; }
	glm::ivec2 returnPosPlayer();
	int isPunching();
	bool punchEnemy(const glm::ivec2& posEnemy, const glm::ivec2& sizeEnemy);
	bool openDoor(const glm::ivec2& posDoor, const glm::ivec2& sizeDoor);

	bool collisionEnemy(const glm::ivec2& posEnemy, const glm::ivec2& sizeEnemy);

	int getHealth() const { return health; }
	int getMaxHealth() const { return maxHealth; }
	void takeDamage(int damage);
	void heal(int amount);
	bool isDead() const { return health <= 0; }

	bool isGodMode() const { return godMode; }
	void toggleGodMode();
	void fullHeal();


	void shoot();
	std::vector<EnemyBullet*> getBullets();

	void equipNextItem();
	void useCurrentItem();
	void addToInventory(int pickupType);
	void collectPickup(int pickupType);
	
	
	int getCurrentItem() const { return currentEquipped; }
	bool hasItems() const { return !inventory.empty(); }
	void addAllItemsToInventory();
	bool shoots() const { return shooted; }
	void desactivatebullets();

	void collisionDoor(const glm::ivec2& posDoor, const glm::ivec2& sizeDoor);
	void win();
	
private:
	bool punching; bool opening;
	int punchTimer, punchDuration;
	int openTimer, openDuration;

	int health;
	int maxHealth;
	bool godMode;
	int godModeEffectTimer;
	int deathTimer, deathDuration; 
	bool deadScreen;

	HealthBar healthBar;
	End end;

	glm::ivec2 tileMapDispl, posPlayer, sizePlayer;
	
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	int dirPunch;

	
	
	EnemyBullet* bullet1;
	EnemyBullet* bullet2;
	EnemyBullet* bullet3;
	EnemyBullet* bullet4;
	EnemyBullet* bullet5;
	int currentBullet;

	int shootCooldown;
	static const int SHOOT_COOLDOWN_TIME = 200;

	ShaderProgram* shaderProgram;

	bool shooted;

	std::vector<int> inventory;        
	int currentEquipped;                  
	static const int NO_ITEM = -1;
	bool hasItem(int itemId) const;
	bool won = true;
	int wonDuration;
	int wonTimer;
};


#endif // _PLAYER_INCLUDE


