#ifndef _Door_INCLUDE
#define _Door_INCLUDE


#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"
#include "AudioManager.h"

class Door
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPlayer(Player* p);
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	void open();
	void toggleactive(bool a);

private:
	bool active;
	int openTimer; int openDuration;

	glm::ivec2 tileMapDispl, posDoor, sizeDoor, posPlayer, sizePlayer;

	Texture spritesheet;
	Player* player;
	Sprite* sprite;
	TileMap* map;


};

#endif // _Door_INCLUDE


