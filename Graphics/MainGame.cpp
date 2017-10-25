#include "MainGame.h"
#include <Engine/Errors.h>
#include <Engine/Timing.h>

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(800),
	_screenHeight(600),
	_gameState(GameState::PLAY),
	_time(0.0f) {

	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame()
{
}


void MainGame::run() {
	initSystems();

	_sprites.push_back(new Engine::Sprite());
	_sprites.back()->init(-_screenWidth / 2,  -_screenHeight/2, _screenWidth/2, _screenWidth / 2, "Textures/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new Engine::Sprite());
	_sprites.back()->init( 0.0f, -_screenHeight/2, _screenWidth/2, _screenWidth/2, "Textures/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystems() {

	Engine::init();
	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();

}

void MainGame::gameLoop() {

	Engine::FpsLimiter limiter;
	limiter.setMaxFPS(60.0f);

	while (_gameState != GameState::EXIT) {

		limiter.begin();

		processInput();
		_time += 0.01;

		_camera.update();

		drawGame();
		int test = limiter.end();
		
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {

		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
			
		case SDL_KEYDOWN:

			switch (evnt.key.keysym.sym)
			{
			case SDLK_w:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
				break;

			case SDLK_s:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
				break;

			case SDLK_a:
				_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
				break;

			case SDLK_d:
				_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
				break;

			case SDLK_q:
				_camera.setScale(_camera.getScale() + SCALE_SPEED);
				break;

			case SDLK_e:
				_camera.setScale(_camera.getScale() - SCALE_SPEED);
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}

void MainGame::drawGame() {

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	for (int i = 0; i < _sprites.size(); i++) {
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	_window.swapBuffer();

}