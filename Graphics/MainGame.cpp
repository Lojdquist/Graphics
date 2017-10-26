#include "MainGame.h"
#include <Engine/Errors.h>
#include <Engine/ResourceManager.h>

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(800),
	_screenHeight(600),
	_gameState(GameState::PLAY),
	_time(0.0f),
	_maxFPS(60.0f){

	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame()
{
}


void MainGame::run() {
	initSystems();


	gameLoop();
}

void MainGame::initSystems() {

	Engine::init();
	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);

}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();

}

void MainGame::gameLoop() {

	while (_gameState != GameState::EXIT) {

		_fpsLimiter.begin();

		processInput();
		_time += 0.01;

		_camera.update();

		drawGame();

		_fps = _fpsLimiter.end();
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {

		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			break;
			
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;

		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;

		default:
			break;
		}
	}


	if (_inputManager.isKeyPressed(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
}

void MainGame::drawGame() {
	//Set te base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Enable Shader
	_colorProgram.use();
	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Get the uniformlocation
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is texture unit 0
	glUniform1i(textureLocation, 0);

	//Set the constantly chaging time variable
	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	//set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Engine::GLTexture texture = Engine::ResourceManager::getTexture("Textures/PNG/CharacterLeft_Jump.png");
	Engine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
	_spriteBatch.draw(pos + glm::vec4(50.0f, 0.0f, 0.0f, 0.0f), uv, texture.id, 0.0f, color);

	_spriteBatch.end();
	_spriteBatch.renderBatch();


	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//Disable the shader
	_colorProgram.unuse();
	//Swap our buffer and draw everything to the screen
	_window.swapBuffer();

}