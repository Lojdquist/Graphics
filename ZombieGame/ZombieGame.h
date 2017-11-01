#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Engine/Window.h>
#include <Engine/Engine.h>
#include <Engine/GLSLProgram.h>
#include <Engine/Timing.h>
#include <Engine/Camera2D.h>
#include <Engine/InputManager.h>

#include "Level.h"

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
	void drawGame();

	int _screenWidth;
	int _screenHeight;

	Engine::Window _window;
	Engine::GLSLProgram _colorProgram;
	Engine::FpsLimiter _fpsLimiter;
	Engine::Camera2D _camera;
	Engine::InputManager _inputManager;

	GameState _currentState;

	std::vector<Level*> _levels;
	int _currentLevel;

	float _maxFPS;
	float _currentFPS;
};

