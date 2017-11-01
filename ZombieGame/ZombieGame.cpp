#include "ZombieGame.h"



ZombieGame::ZombieGame()  : _screenWidth(800), 
							_screenHeight(600),
							_currentState(GameState::PLAY),
							_maxFPS(60.f),
							_currentFPS(0.0f){

	_camera.init(_screenWidth, _screenHeight);
}


ZombieGame::~ZombieGame(){

}

void ZombieGame::run(){
	initSystems();
	initLevel();
	gameLoop();
}

void ZombieGame::initSystems(){
	Engine::init();

	_window.create("Zombie Game",_screenWidth, _screenHeight, 0);

	//Set the background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();
	_fpsLimiter.init(_maxFPS);

	_camera.init(_screenWidth,_screenHeight);
}

void ZombieGame::initShaders(){
	// Compile our color shader
	_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void ZombieGame::initLevel(){
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;
}

void ZombieGame::processInput(){
	SDL_Event evnt;

	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {

		case SDL_QUIT:
			_currentState = GameState::QUIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;

		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;

		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;

		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
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

}

void ZombieGame::gameLoop(){

	_camera.setPosition(_levels[_currentLevel]->getStartpos());

	while (_currentState != GameState::QUIT) {
		_fpsLimiter.begin();

		processInput();


		_camera.update();

		_currentFPS = _fpsLimiter.end();
		static int frameCounter = 0;
		if (frameCounter == 100) {
			frameCounter = 0;
		}
		frameCounter++;

		drawGame();

	}
}

void ZombieGame::drawGame(){

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

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _colorProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_levels[_currentLevel]->draw();

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//Disable the shader
	_colorProgram.unuse();
	//Swap our buffer and draw everything to the screen
	_window.swapBuffer();
}
