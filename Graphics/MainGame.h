#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Engine/Engine.h>
#include <Engine/GLSLProgram.h>
#include <Engine/Sprite.h>
#include <Engine/GLTexture.h>
#include <Engine/Window.h>
#include <Engine/Camera2D.h>
#include <Engine/SpriteBatch.h>
#include <Engine/InputManager.h>
#include <Engine/Timing.h>

#include <vector>

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();


private:
	void initSystems();
	void initShaders();
	void processInput();
	void gameLoop();
	void drawGame();

	Engine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	Engine::Camera2D _camera;
	Engine::GLSLProgram _colorProgram;

	Engine::SpriteBatch _spriteBatch;
	Engine::InputManager _inputManager;
	Engine::FpsLimiter _fpsLimiter;
	
	

	float _time;
	float _fps;
	float _maxFPS;

};