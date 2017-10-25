#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Engine/Engine.h>
#include <Engine/GLSLProgram.h>
#include <Engine/Sprite.h>
#include <Engine/GLTexture.h>
#include <Engine/Window.h>
#include <Engine/Camera2D.h>

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

	std::vector<Engine::Sprite *> _sprites;
	Engine::GLSLProgram _colorProgram;

	float _time;

};