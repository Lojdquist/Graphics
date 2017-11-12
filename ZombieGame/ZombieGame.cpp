#include "ZombieGame.h"

const float HUMAN_SPEED = 5.0f;



ZombieGame::ZombieGame()  : _screenWidth(800), 
							_screenHeight(600),
							_currentState(GameState::PLAY),
							_maxFPS(60.f),
							_currentFPS(0.0f){

	_camera.init(_screenWidth, _screenHeight);
}


ZombieGame::~ZombieGame(){

	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}

	for (int i = 0; i < _humans.size(); i++) {
		delete _humans[i];
	}

	for (int i = 0; i < _zombie.size(); i++) {
		delete _zombie[i];
	}
}

void ZombieGame::run(){
	initSystems();
	initLevel();
	gameLoop();
}

void ZombieGame::updateAgents(float deltaTime){

	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
							_humans,
							_zombie,
							deltaTime);
	}
}

void ZombieGame::updateBullets(float deltaTime){
	//Update and collide with world
	for (int i = 0; i < _bullets.size();) {
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}
}

void ZombieGame::initSystems(){
	Engine::init();

	_window.create("Zombie Game",_screenWidth, _screenHeight, 0);

	//Set the background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();
	_fpsLimiter.init(_maxFPS);

	_camera.init(_screenWidth,_screenHeight);

	//init spriteBatch
	_agentSpriteBatch.init();
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

	_player = new Player();
	_player->init(HUMAN_SPEED, _levels[_currentLevel]->getStartpos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);


	//Set up the players guns
	const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 0.0f, 30.0f, BULLET_SPEED));
	_player->addGun(new Gun("Shotgun", 30, 12, 0.5f, 4.0f, BULLET_SPEED));
	_player->addGun(new Gun("MP5", 2, 1, 0.2f, 20.0f, BULLET_SPEED));
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
}

void ZombieGame::gameLoop(){

	_camera.setPosition(_levels[_currentLevel]->getStartpos());

	float deltaTime = 1.0f;

	while (_currentState != GameState::QUIT) {
		_fpsLimiter.begin();

		processInput();


		

		_currentFPS = _fpsLimiter.end();
		static int frameCounter = 0;
		if (frameCounter == 100) {
			frameCounter = 0;
		}
		frameCounter++;

		updateBullets(deltaTime);


		_camera.setPosition(_player->getPostion());
		updateAgents(deltaTime);
		_camera.update();

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

	_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	for (int i = 0; i < _humans.size(); i++) {
		if (_camera.isBoxInView(_humans[i]->getPostion(), agentDims)) {
			_humans[i]->draw(_agentSpriteBatch);
		}
	}

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//Disable the shader
	_colorProgram.unuse();
	//Swap our buffer and draw everything to the screen
	_window.swapBuffer();
}
