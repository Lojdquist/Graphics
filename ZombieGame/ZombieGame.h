#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Engine/Window.h>
#include <Engine/Engine.h>
#include <Engine/GLSLProgram.h>
#include <Engine/Timing.h>
#include <Engine/Camera2D.h>
#include <Engine/InputManager.h>
#include <Engine/SpriteBatch.h>
#include <Engine/SpriteFont.h>

#include "Level.h"
#include "Human.h"
#include "Zombie.h"
#include "Player.h"


enum class GameState{PLAY,PAUSE,QUIT};

class ZombieGame
{
public:
	ZombieGame();
	~ZombieGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void initLevel();
	void processInput();
	void gameLoop();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void drawGame();
	void drawHud();

	int _screenWidth;
	int _screenHeight;

	Engine::Window _window;
	Engine::GLSLProgram _colorProgram;
	Engine::FpsLimiter _fpsLimiter;
	Engine::Camera2D _camera;
	Engine::Camera2D _hudCamera;

	Engine::InputManager _inputManager;
	Engine::SpriteBatch _agentSpriteBatch;
	Engine::SpriteBatch _hudSpriteBatch;
	

	Engine::SpriteFont* _spriteFont;

	GameState _currentState;

	std::vector<Level*> _levels;
	int _currentLevel;

	float _maxFPS;
	float _currentFPS;

	Player* _player;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombie;
	std::vector<Bullet> _bullets;
	
};

