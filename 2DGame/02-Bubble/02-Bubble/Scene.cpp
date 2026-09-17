#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_LEVEL 1				//1
#define INIT_PLAYER_X_TILES 14		//14	
#define INIT_PLAYER_Y_TILES 17		//17

enum Music {
	JUNGLE, INTERIOR, ALERT, BOSS
};

Scene::Scene()
{
	map = NULL;
	player = NULL;
	enemy1 = NULL;
	enemy2 = NULL;
	enemy3 = NULL;
	door = NULL;
	boss = NULL;
	pickup1 = NULL;  
	pickup2 = NULL;  
	pickup3 = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (enemy1 != NULL)
		delete enemy1;
	if (enemy2 != NULL)
		delete enemy2;
	if (enemy3 != NULL)
		delete enemy3;
	if (boss != NULL)
		delete boss;


	if (pickup1 != NULL) delete pickup1;
	if (pickup2 != NULL) delete pickup2;
	if (pickup3 != NULL) delete pickup3;

	if (door != NULL)
		delete door;

	AudioManager::instance().playMusic("audio / Start.mp3", false);

}


void Scene::init()
{
	level = INIT_LEVEL;
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, 1);
	map->setLevel(INIT_LEVEL);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	
	enemy1 = new Enemy();
	enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
	enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize())); //fora de pantalla, ja que es inactiu
	enemy1->setTileMap(map);
	enemy1->setPlayer(player);
	enemy1->prepareBullet(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	enemy2 = new Enemy();
	enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
	enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	enemy2->setTileMap(map);
	enemy2->setPlayer(player);
	enemy2->prepareBullet(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	enemy3 = new Enemy();
	enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
	enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	enemy3->setTileMap(map);
	enemy3->setPlayer(player);
	enemy3->prepareBullet(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	boss = new Shotgunner();
	boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	boss->toggleDeath(true);
	boss->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	boss->setTileMap(map);
	boss->setPlayer(player);
	boss->prepareBullet(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);



	pickup1 = new Pickup();
	pickup1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Pickup::HEALTH,
		glm::ivec2(33 * map->getTileSize(),25 * map->getTileSize()));
	pickup1->setTileMap(map);

	pickup2 = new Pickup();
	pickup2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Pickup::WEAPON,
		glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	pickup2->setTileMap(map);

	pickup3 = new Pickup();
	pickup3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Pickup::KEY,
		glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	pickup3->setTileMap(map);


	door = new Door();
	door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	door->toggleactive(false);
	door->setPosition(glm::vec2(12 * map->getTileSize(), 0 * map->getTileSize()));
	door->setTileMap(map);
	door->setPlayer(player);

	current_music = JUNGLE;
	AudioManager::instance().playMusic("audio/Jungle.mp3", true);


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	glm::ivec2 pos = player->returnPosPlayer();
	enemy1->update(deltaTime);
	enemy2->update(deltaTime);
	enemy3->update(deltaTime);
	if (boss != NULL) boss->update(deltaTime);

	if (pickup1 != NULL) pickup1->update(deltaTime);
	if (pickup2 != NULL) pickup2->update(deltaTime);
	if (pickup3 != NULL) pickup3->update(deltaTime);


	
	for (auto bullet : player->getBullets()) {
		if (!bullet->isActive() || !bullet->isFromPlayer()) continue;

		if (enemy1->isActive() && !enemy1->isDead()) {
			if (checkCollision(bullet->getPosition(), bullet->getSize(),
				enemy1->getPosition(), enemy1->getSize())) {
				enemy1->takeDamage();
				bullet->deactivate();
				break;
			}
		}

		if (enemy2->isActive() && !enemy2->isDead()) {
			if (checkCollision(bullet->getPosition(), bullet->getSize(),
				enemy2->getPosition(), enemy2->getSize())) {
				enemy2->takeDamage();
				bullet->deactivate();
				break;
			}
		}

		if (enemy3->isActive() && !enemy3->isDead()) {
			if (checkCollision(bullet->getPosition(), bullet->getSize(),
				enemy3->getPosition(), enemy3->getSize())) {
				enemy3->takeDamage();
				bullet->deactivate();
				break;
			}
		}

		if (boss != NULL && boss->isActive() && !boss->isDead()) {
			if (checkCollision(bullet->getPosition(), bullet->getSize(),
				boss->getPosition(), boss->getSize())) {
				boss->takeDamage();
				bullet->deactivate();
				break;
			}
		}
	}

	for (auto& pickup : pickups) {
		pickup.update(deltaTime);
	}

	if (door != NULL) door->update(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_K)) {
		level = 4; //para que funcione switch level
		switchLevel(6, pos);
	}
	if (Game::instance().getKey(GLFW_KEY_B)) {
		level = 11; //para que funcione switch level
		switchLevel(12, pos);
	}
	if (enemy1->isDog() && enemy1->isAlert() || enemy3->isDog() && enemy3->isAlert()) {
		//si un perro esta chasing todos lo estan
		enemy1->setEnemy(1, 2);
		enemy3->setEnemy(1, 2);
	}
	// MUSICA
	if (enemy1->isAlert() && !enemy1->isDead() || enemy2->isAlert() && !enemy2->isDead() || enemy3->isAlert() && !enemy3->isDead()) {
		if (current_music != ALERT) {
			AudioManager::instance().playMusic("audio/Alert.mp3", true);
			current_music = ALERT;
		}
	}
	else {
		if (level < 6) {
			if (current_music != JUNGLE) {
				AudioManager::instance().playMusic("audio/Jungle.mp3", true);
				current_music = JUNGLE;
			}
		}
		else if (level <= 11) {
			if (current_music != INTERIOR) {
				AudioManager::instance().playMusic("audio/Interior.mp3", true);
				current_music = INTERIOR;
			}
		}
		else if (level == 12) {
			if (current_music != BOSS) {
				AudioManager::instance().playMusic("audio/Boss.mp3", true);
				current_music = BOSS;
			}
		}
	}
	if (player->shoots()) {
		if (enemy1->isActive()) enemy1->setChasing();
		if (enemy2->isActive()) enemy2->setChasing();
		if (enemy3->isActive()) enemy3->setChasing();
	}
	if (player->shoots()) {
		enemy1->setChasing();
		enemy2->setChasing();
		enemy3->setChasing();
	}
	if (player->isDead()) AudioManager::instance().stopMusic();
	if (level == 12 && boss->isDead()) player->win();

	//canvi de nivell
	checkSwitch(pos);

	checkPickupCollisions();
	
}

void Scene::render()
{

	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	
	door->render();
	enemy1->render();
	enemy2->render();
	enemy3->render();
	boss->render();
	player->render();

	if (pickup1 != NULL) pickup1->render();
	if (pickup2 != NULL) pickup2->render();
	if (pickup3 != NULL) pickup3->render();

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::checkSwitch(glm::vec2 pos) {
	if (level == 1 && pos.x == 32 * map->getTileSize() - 72) // Size of Screen - Size of Player
		switchLevel(2, pos);

	if (level == 2 && pos.x == 0 * map->getTileSize())
		switchLevel(1, pos);
	if (level == 2 && pos.y == 23 * map->getTileSize() - 34 * 4)
		switchLevel(3, pos);

	if (level == 3 && pos.y == 0 * map->getTileSize())
		switchLevel(2, pos);
	if (level == 3 && pos.y == 24 * map->getTileSize() - 34 * 4)
		switchLevel(4, pos);

	if (level == 4 && pos.y == 0 * map->getTileSize())
		switchLevel(3, pos);
	if (level == 4 && pos.x == 14 * map->getTileSize() && pos.y >= 11 * map->getTileSize() && pos.y <= 24 * map->getTileSize())
		switchLevel(5, pos);
	if (level == 4 && pos.y == 23 * map->getTileSize() - 34 * 4)
		switchLevel(6, pos);

	if (level == 5 && pos.x == 26 * map->getTileSize() - 72 && pos.y >= 9 * map->getTileSize())
		switchLevel(4, pos);

	if (level == 6 && pos.y == 4 * map->getTileSize() && pos.x >= 22 * map->getTileSize())
		switchLevel(4, pos);
	if (level == 6 && pos.y == 24 * map->getTileSize() - 34 * 4)
		switchLevel(7, pos);
	if (level == 6 && pos.x == 0 * map->getTileSize())
		switchLevel(8, pos);

	if (level == 7 && pos.y == 0 * map->getTileSize())
		switchLevel(6, pos);
	if (level == 7 && pos.x == 0 * map->getTileSize())
		switchLevel(9, pos);

	if (level == 8 && pos.x == 32 * map->getTileSize() - 72)
		switchLevel(6, pos);
	if (level == 8 && pos.y == 24 * map->getTileSize() - 34 * 4)
		switchLevel(9, pos);
	if (level == 8 && pos.y == 0 * map->getTileSize())
		switchLevel(10, pos);

	if (level == 9 && pos.x == 32 * map->getTileSize() - 72)
		switchLevel(7, pos);
	if (level == 9 && pos.y == 0 * map->getTileSize())
		switchLevel(8, pos);

	if (level == 10 && pos.y == 23 * map->getTileSize() - 34 * 4)
		switchLevel(8, pos);
	if (level == 10 && pos.y == 1 * map->getTileSize())
		switchLevel(11, pos);

	if (level == 11 && pos.y == 24 * map->getTileSize() - 34 * 4)
		switchLevel(10, pos);
	if (level == 11 && pos.y == 0 * map->getTileSize())
		switchLevel(12, pos);

}

void Scene::switchLevel(int newlevel, glm::vec2 pos) {
	enemy1->disableBullet();
	enemy2->disableBullet();
	enemy3->disableBullet();

	player->desactivatebullets();

	if (level == 1 && newlevel == 2) // Pasar del nivell 1 al nivell 2
	{
		level = 2;
		map->changeLevel("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(1 * map->getTileSize(), pos.y));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 0);
		enemy1->setPosition(glm::vec2(23 * map->getTileSize(), 15 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);


		//initPickups();//inicialitzar pickups de nou
		door->toggleactive(false);

	}
	if (level == 2 && newlevel == 1) // Tornar del nivell 2 al nivell 1 
	{
		level = 1;
		map->changeLevel("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(29 * map->getTileSize(), pos.y));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);

		door->toggleactive(false);

	}
	if (level == 2 && newlevel == 3) // Pasar del 2 al 3
	{
		level = 3;
		player->setPosition(glm::vec2(pos.x, 1 * map->getTileSize()));
		map->changeLevel("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, 0);
		enemy1->setPosition(glm::vec2(16 * map->getTileSize(), 12 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(9 * map->getTileSize(), 20 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, 0);
		enemy3->setPosition(glm::vec2(22 * map->getTileSize(), 18 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);

	

	}
	if (level == 3 && newlevel == 2) // Tornar del 3 al 2
	{
		level = 2;
		map->changeLevel("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 21 * map->getTileSize() - 72));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 0);
		enemy1->setPosition(glm::vec2(23 * map->getTileSize(), 15 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);

		door->toggleactive(false);

	}
	if (level == 3 && newlevel == 4) // Pasar del 3 al 4
	{
		level = 4;
		map->changeLevel("levels/level04.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 0 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 1);
		enemy1->setPosition(glm::vec2(23 * map->getTileSize(), 11 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);

		door->toggleactive(false);

	}
	if (level == 4 && newlevel == 3) // Tornar del 4 al 3
	{
		level = 3;
		map->changeLevel("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 21 * map->getTileSize() - 72));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, 0);
		enemy1->setPosition(glm::vec2(16 * map->getTileSize(), 12 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(9 * map->getTileSize(), 20 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, 0);
		enemy3->setPosition(glm::vec2(22 * map->getTileSize(), 18 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 4 && newlevel == 5) // Pasar del 4 al 5
	{
		level = 5;
		map->changeLevel("levels/level05.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(23 * map->getTileSize(), 11 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);

		pickup1->setPosition(glm::ivec2(9 * map->getTileSize(), 13 * map->getTileSize()));

		door->toggleactive(false);

	}
	if (level == 5 && newlevel == 4) // Tornar del 5 al 4
	{
		level = 4;
		map->changeLevel("levels/level04.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(15 * map->getTileSize(), 11 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 1);
		enemy1->setPosition(glm::vec2(23 * map->getTileSize(), 11 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);

		pickup1->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		pickup2->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		pickup3->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	}
	if (level == 4 && newlevel == 6) // Pasar del 4 al 6
	{
		level = 6;
		map->changeLevel("levels/level06.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(25 * map->getTileSize(), 6 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 6 && newlevel == 4) // Tornar del 6 al 4
	{
		level = 4;
		map->changeLevel("levels/level04.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(22 * map->getTileSize(), 21 * map->getTileSize() - 72));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 1);
		enemy1->setPosition(glm::vec2(23 * map->getTileSize(), 11 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 6 && newlevel == 7) // Anar del 6 al 7
	{
		level = 7;
		map->changeLevel("levels/level07.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 0 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 6 && newlevel == 8) // Anar del 6 al 8
	{
		level = 8;
		map->changeLevel("levels/level08.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(29 * map->getTileSize(), pos.y));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 1);
		enemy1->setPosition(glm::vec2(15 * map->getTileSize(), 5 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 7 && newlevel == 6) // Anar del 7 al 6
	{
		level = 6;
		map->changeLevel("levels/level06.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 22 * map->getTileSize() - 72));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 8 && newlevel == 6) // Anar del 8 al 6
	{
		level = 6;
		map->changeLevel("levels/level06.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(0 * map->getTileSize(), pos.y));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 9 && newlevel == 8) // Anar del 9 al 8
	{
		level = 8;
		map->changeLevel("levels/level08.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 22 * map->getTileSize() - 72));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 1);
		enemy1->setPosition(glm::vec2(15 * map->getTileSize(), 5 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 9 && newlevel == 7) // Anar del 9 al 7
	{
		level = 7;
		map->changeLevel("levels/level07.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(0 * map->getTileSize(), pos.y));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 8 && newlevel == 9) // Anar del 8 al 9
	{
		level = 9;
		map->changeLevel("levels/level09.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 0 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 7 && newlevel == 9) // Anar del 7 al 9
	{
		level = 9;
		map->changeLevel("levels/level09.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(29 * map->getTileSize(), pos.y));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(25 * map->getTileSize(), 33 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);
	}
	if (level == 8 && newlevel == 10) // Anar del 8 al 10
	{
		level = 10;
		map->changeLevel("levels/level10.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 21 * map->getTileSize() - 72));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);

		pickup2->setPosition(glm::ivec2(22 * map->getTileSize(), 2 * map->getTileSize()));
		pickup3->setPosition(glm::ivec2(23 * map->getTileSize(), 15 * map->getTileSize()));

		door->toggleactive(false);

	}
	if (level == 10 && newlevel == 8) // Anar del 10 al 8
	{
		level = 8;
		map->changeLevel("levels/level08.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 0 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 1);
		enemy1->setPosition(glm::vec2(15 * map->getTileSize(), 5 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);
		door->toggleactive(false);

		pickup1->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		pickup2->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		pickup3->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	}
	if (level == 10 && newlevel == 11) // Anar del 10 al 11
	{
		level = 11;
		map->changeLevel("levels/level11.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 21 * map->getTileSize() - 72));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, 1);
		enemy1->setPosition(glm::vec2(15 * map->getTileSize(), 3 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->toggleDeath(true);

		door->toggleactive(true);

		pickup1->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		pickup2->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		pickup3->setPosition(glm::ivec2(33 * map->getTileSize(), 25 * map->getTileSize()));
	}
	if (level == 11 && newlevel == 10) // Anar del 11 al 10
	{
		level = 10;
		map->changeLevel("levels/level10.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(pos.x, 1 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		boss->toggleDeath(true);
		boss->setPosition(glm::vec2(16 * map->getTileSize(), 3 * map->getTileSize()));

		pickup2->setPosition(glm::ivec2(22 * map->getTileSize(), 2 * map->getTileSize()));
		pickup3->setPosition(glm::ivec2(23 * map->getTileSize(), 15 * map->getTileSize()));

		door->toggleactive(false);

	}
	if (level == 11 && newlevel == 12) // Anar del 11 al 12
	{
		
		map->changeLevel("levels/level12.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(16 * map->getTileSize(), 17.7 * map->getTileSize()));
		map->setLevel(newlevel);
		enemy1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy1->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy2->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, 0);
		enemy3->setPosition(glm::vec2(33 * map->getTileSize(), 25 * map->getTileSize()));
		
		boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		boss->toggleDeath(false);
		boss->setPosition(glm::vec2(16 * map->getTileSize(), 3 * map->getTileSize()));

		door->toggleactive(false);
		level = 12;
		AudioManager::instance().playMusic("audio/Boss.mp3", true);

	}
}
int Scene::getPlayerHealth() const
{
	if (player != nullptr) {
		return player->getHealth();
	}
	return 0; 
}

int Scene::getPlayerMaxHealth() const
{
	if (player != nullptr) {
		return player->getMaxHealth();
	}
	return 100; 
}




//colisio per recollir
void Scene::checkPickupCollisions()
{

	if (pickup1 != NULL && !pickup1->isPickedUp()) {
		checkSinglePickup(pickup1);
	}
	if (pickup2 != NULL && !pickup2->isPickedUp()) {
		checkSinglePickup(pickup2);
	}
	if (pickup3 != NULL && !pickup3->isPickedUp()) {
		checkSinglePickup(pickup3);
	}
}

void Scene::checkSinglePickup(Pickup* pickup)
{
	if (pickup != nullptr && !pickup->isPickedUp()) {
		glm::ivec2 playerPos = player->returnPosPlayer();
		glm::ivec2 playerSize = player->getSize();
		glm::ivec2 pickupPos = pickup->getPosition();
		glm::ivec2 pickupSize = pickup->getSize();

		//colisio
		bool collisionX = (playerPos.x < pickupPos.x + pickupSize.x) &&
			(playerPos.x + playerSize.x > pickupPos.x);
		bool collisionY = (playerPos.y < pickupPos.y + pickupSize.y) &&
			(playerPos.y + playerSize.y > pickupPos.y);

		if (collisionX && collisionY) {
			player->collectPickup(static_cast<int>(pickup->getType()));
			pickup->pickUp();
		}
	}
}

bool Scene::checkCollision(const glm::ivec2& pos1, const glm::ivec2& size1,
	const glm::ivec2& pos2, const glm::ivec2& size2)
{
	return (pos1.x < pos2.x + size2.x &&
		pos1.x + size1.x > pos2.x &&
		pos1.y < pos2.y + size2.y &&
		pos1.y + size1.y > pos2.y);
}