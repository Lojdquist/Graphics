#include "ZombieGame.h"

#include<random>
#include<ctime>
#include<algorithm>
#include <glm/glm.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include <iostream>


const float HUMAN_SPEED = 1.3f;
const float ZOMBIE_SPEED = 1.0f;
const float PLAYER_SPEED = 5.0f;


ZombieGame::ZombieGame(){
}


ZombieGame::~ZombieGame(){

	for (int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}

	for (int i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}

	for (int i = 0; i < m_zombie.size(); i++) {
		delete m_zombie[i];
	}
}

void ZombieGame::run(){
	initSystems();
	initLevel();

	std::cout << m_humans.size() << std::endl;
	gameLoop();
}

void ZombieGame::updateAgents(float deltaTime){

	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),
							m_humans,
							m_zombie,
							deltaTime);
	}

	for (int i = 0; i < m_zombie.size(); i++) {
		m_zombie[i]->update(m_levels[m_currentLevel]->getLevelData(),
							m_humans,
							m_zombie,
							deltaTime);
	}

	//Update Zombie collisions
	for (int i = 0; i < m_zombie.size(); i++) {
		
		//Collide with other zombies
		for (int j = i + 1; j < m_zombie.size(); j++) {
			m_zombie[i]->collideWithAgent(m_zombie[j]);
		}

		//Collide with human
		for (int j = 1; j < m_humans.size(); j++) {

			if (m_zombie[i]->collideWithAgent(m_humans[j])) {
				//Add new zombie
				m_zombie.push_back(new Zombie);
				m_zombie.back()->init(ZOMBIE_SPEED, m_humans[j]->getPostion());

				//Delete the human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}

			//TODO: Zombie collide with player code here
		}
	}


	//Collide with player
	for (int i = 0; i < m_humans.size(); i++) {
		for (int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}

void ZombieGame::updateBullets(float deltaTime){
	//Update and collide with world
	for (int i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;

	for (int i = 0; i < m_bullets.size(); i++) {
		wasBulletRemoved = false;

		//Loop through zombies
		for (int j = 0; j < m_zombie.size();) {

			//Check collision
			if (m_bullets[i].collideWithAgent(m_zombie[j])) {
				//Add blood
				addBlood(m_bullets[i].getPosition(), 5);


				//Damage zombie and kill it if out of health
				if (m_zombie[j]->applyDamage(m_bullets[i].getDamage())) {
					//If zombie died, remove
					delete m_zombie[j];
					m_zombie[j] = m_zombie.back();
					m_zombie.pop_back();

				}
				else {
					j++;
				}

				//Remove the bullet
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				wasBulletRemoved = true;
				i--; // Makes sure we don't skip a bullet
				//Since the bullet died, no need to loop more
				break;
			}
			else {
				j++;
			}
		}

		//Loop through Humans
		if (!wasBulletRemoved) {
			for (int j = 1; j < m_humans.size();) {

				//Check collision
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					//Add blood
					addBlood(m_bullets[i].getPosition(), 5);

					//Damage Human, and kill it if its out of health
					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
						//If the human died, remove him
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
					}
					else {
						j++;
					}

					//Remove the bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();

					//Make sure we don't skip a bullet
					i--;
					break;
				}else {
					j++;
				}
			}
		}
	}
}

void ZombieGame::initSystems(){
	Engine::init();

	//Initialize sound, must happen after init
	m_audioEngine.init();

	m_window.create("Zombie Game",m_screenWidth, m_screenHeight, 0);

	//Set the background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();
	m_fpsLimiter.init(m_maxFPS);

	//init spriteBatch
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	//init spriteFont
	m_spriteFont = new Engine::SpriteFont("Fonts/ostrich-regular.ttf", 128);

	//Set up the camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth,m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth/2, m_screenHeight/2));

	// Initialize particles
	m_bloodParticleBatch = new Engine::ParticleBatch2D();
	m_bloodParticleBatch->init(1000, 0.05f, 
								Engine::ResourceManager::getTexture("Textures/particle.png"), 
								[](Engine::Particle2D& p, float deltaTime) {
		p.position += p.velocity * deltaTime;
		p.color.a = (GLubyte)(p.life * 255.0f);
	});
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);
}

void ZombieGame::initShaders(){
	// Compile our color shader
	m_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	m_colorProgram.addAttribute("vertexPosition");
	m_colorProgram.addAttribute("vertexColor");
	m_colorProgram.addAttribute("vertexUV");
	m_colorProgram.linkShaders();
}

void ZombieGame::initLevel(){
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartpos(), &m_inputManager, &m_camera, &m_bullets);

	m_humans.push_back(m_player);

	//Set up the players guns
	const float BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 0.0f, 30.0f, BULLET_SPEED, m_audioEngine.loudSoundEffect("Sound/pistol.ogg")));
	m_player->addGun(new Gun("Shotgun", 30, 12, 0.5f, 4.0f, BULLET_SPEED, m_audioEngine.loudSoundEffect("Sound/shotgun.ogg")));
	m_player->addGun(new Gun("MP5", 5, 1, 0.2f, 20.0f, BULLET_SPEED, m_audioEngine.loudSoundEffect("Sound/minigun.ogg")));

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() -2);

	//Add random humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
		m_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		m_humans.back()->init(HUMAN_SPEED,pos);
	}

	//Add the zombies
	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		m_zombie.push_back(new Zombie);
		m_zombie.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

}

void ZombieGame::processInput(){
	SDL_Event evnt;

	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {

		case SDL_QUIT:
			m_currentState = GameState::QUIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;

		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;

		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		default:
			break;
		}
	}
}

void ZombieGame::gameLoop(){
	const float DESIRED_FPS = 60;
	const int MAX_PHYSICS_STEPS = 6;
	const float CAMERA_SCALE = 1.0f / 3.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float previousTicks = SDL_GetTicks();

	m_camera.setPosition(m_levels[m_currentLevel]->getStartpos());
	m_camera.setScale(CAMERA_SCALE);

	while (m_currentState != GameState::QUIT) {
		m_fpsLimiter.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = SDL_GetTicks() - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS){
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateBullets(deltaTime);
			updateAgents(deltaTime);
			m_particleEngine.update(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}

		m_camera.setPosition(m_player->getPostion());
		m_camera.update();

		m_hudCamera.update();

		drawGame();
		m_currentFPS = m_fpsLimiter.end();

		static int frameCounter = 0;
		if (frameCounter == 25) {
			frameCounter = 0;
			m_drawFPS = m_currentFPS;
		}
		frameCounter++;

	}
}

void ZombieGame::drawGame(){

	//Set te base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Enable Shader
	m_colorProgram.use();
	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Get the uniformlocation
	GLint textureLocation = m_colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is texture unit 0
	glUniform1i(textureLocation, 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_colorProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_levels[m_currentLevel]->draw();

	m_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	//Draw all humans
	for (int i = 0; i < m_humans.size(); i++) {
		if (m_camera.isBoxInView(m_humans[i]->getPostion(), agentDims)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}

	//Draw all Zombies
	for (int i = 0; i < m_zombie.size(); i++) {
		if (m_camera.isBoxInView(m_zombie[i]->getPostion(), agentDims)) {
			m_zombie[i]->draw(m_agentSpriteBatch);
		}
	}

	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();

	//Render the particles
	m_particleEngine.draw(&m_agentSpriteBatch);

	//Renders the heads up display
	drawHud();

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//Disable the shader
	m_colorProgram.unuse();
	//Swap our buffer and draw everything to the screen
	m_window.swapBuffer();
}

void ZombieGame::drawHud(){
	char buffer[256];

	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_colorProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	sprintf_s(buffer, "num Humans %d", m_humans.size());

	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0,0), 
						glm::vec2(0.5), 0.0f, Engine::ColorRGBA8(255,255,255,255));

	sprintf_s(buffer, "num Zombies %d", m_zombie.size());

	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 48),
		glm::vec2(0.5), 0.0f, Engine::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "FPS %d", m_drawFPS);

	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 700),
		glm::vec2(0.5), 0.0f, Engine::ColorRGBA8(255, 255, 255, 255));

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}

void ZombieGame::addBlood(const glm::vec2 & position, int numParticles){

	static std::mt19937 randEng(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 2.0f * M_PI);

	glm::vec2 vel(2.0f, 0.0f);
	Engine::ColorRGBA8 col(255, 0 ,0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEng)), col, 30.0f);
	}
}
