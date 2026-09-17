#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Shotgunner.h"
#include "Pickup.h"

#include "Door.h"
#include "AudioManager.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	int getPlayerHealth() const;
	int getPlayerMaxHealth() const;

private:
	void initShaders();
	void checkSwitch(glm::vec2 pos);
	void switchLevel(int newlevel, glm::vec2 pos);

private:
	TileMap *map;
	Player *player;
	Enemy* enemy1;
	Enemy* enemy2; 
	Enemy* enemy3;
	Shotgunner* boss;
	Pickup* pickup1;
	Pickup* pickup2;
	Pickup* pickup3;
	Door* door;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int level;

	std::vector<Pickup> pickups;
	void checkPickupCollisions();
	bool checkCollision(const glm::ivec2& pos1, const glm::ivec2& size1,
		const glm::ivec2& pos2, const glm::ivec2& size2);
	void checkSinglePickup(Pickup* pickup);
	int current_music;
};


#endif // _SCENE_INCLUDE

