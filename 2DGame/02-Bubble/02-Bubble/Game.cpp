#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	menu.init();
	credits.init();
	instructions.init();
	gameState = MAIN_MENU;
	
}

bool Game::update(int deltaTime)
{

	switch (gameState) {
	case MAIN_MENU:
		menu.update(deltaTime);
		if (menu.isActionConfirmed()) {
			handleMenuAction(menu.getCurrentAction());
			menu.resetAction();
		}
		break;
	case IN_GAME:
		scene.update(deltaTime);
		break;
	case CREDITS_SCREEN:
		credits.update(deltaTime);
		if (credits.shouldReturnToMenu()) {
			credits.resetReturnFlag();
			gameState = MAIN_MENU;
		}
		break;
	case INSTRUCTIONS_SCREEN:
		
		instructions.update(deltaTime); 
		if (instructions.shouldReturnToMenu()) {
			instructions.resetReturnFlag();
			gameState = MAIN_MENU;
		}
		break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (gameState) {
	case MAIN_MENU:
		menu.render();
		break;
	case IN_GAME:
		scene.render();	
		break;
	case INSTRUCTIONS_SCREEN:
		instructions.render();
		break;
	case CREDITS_SCREEN:
		credits.render();
		break;
	}
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



void Game::handleMenuAction(int action)
{
	switch (action) {
	case Menu::START_GAME:
		gameState = IN_GAME;
		scene.init();
		break;
	case Menu::INSTRUCTIONS:
		gameState = INSTRUCTIONS_SCREEN;
		break;
	case Menu::CREDITS:
		gameState = CREDITS_SCREEN;
		break;
	}
}

void Game::keyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE) {
		if (gameState == IN_GAME || gameState == INSTRUCTIONS_SCREEN || gameState == CREDITS_SCREEN) {
			gameState = MAIN_MENU;
		}
		else {
		
			bPlay = false;
		}
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{

	if (gameState == MAIN_MENU && (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER)) {
		handleMenuAction(menu.getCurrentAction());
	}

	else if ((gameState == INSTRUCTIONS_SCREEN || gameState == CREDITS_SCREEN) &&
		(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)) {
		gameState = MAIN_MENU;
	}

	keys[key] = false;
}
