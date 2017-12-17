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
#include <Engine/AudioEngine.h>

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

	int m_screenWidth = 1024;
	int m_screenHeight = 768;

	Engine::Window m_window;
	Engine::GLSLProgram m_colorProgram;
	Engine::FpsLimiter m_fpsLimiter;
	Engine::Camera2D m_camera;
	Engine::Camera2D m_hudCamera;

	Engine::InputManager m_inputManager;

	Engine::SpriteBatch m_agentSpriteBatch;
	Engine::SpriteBatch m_hudSpriteBatch;
	
	Engine::SpriteFont* m_spriteFont;

	Engine::AudioEngine m_audioEngine;

	GameState m_currentState = GameState::PLAY;

	std::vector<Level*> m_levels;
	int m_currentLevel = 1;

	float m_maxFPS = 60.0f;
	float m_currentFPS = 0.0f;
	int m_drawFPS = 0;

	Player* m_player;

	std::vector<Human*> m_humans;
	std::vector<Zombie*> m_zombie;
	std::vector<Bullet> m_bullets;
	
};

