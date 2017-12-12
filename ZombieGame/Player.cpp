#include "Player.h"

#include <Engine/ResourceManager.h>
#include<SDL/SDL.h>

Player::Player()
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, Engine::InputManager * inputManager, Engine::Camera2D * camera, std::vector<Bullet>* bullets){
	m_speed = speed;
	m_position = pos;
	m_inputManager = inputManager;
	m_bullets = bullets;
	m_camera = camera;
	m_color = Engine::ColorRGBA8(255, 255, 255,255);
	m_health = 150;
	m_textureID = Engine::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::addGun(Gun * gun){
	// Add the gun to his inventory
	m_guns.push_back(gun);

	if (_currentGunIndex == -1) {
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){
	
	if (m_inputManager->isKeyDown(SDLK_w)) {
		m_position.y += m_speed * deltaTime;

	}else if (m_inputManager->isKeyDown(SDLK_s)) {
		m_position.y -= m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_a)) {
		m_position.x -= m_speed * deltaTime;

	}else if (m_inputManager->isKeyDown(SDLK_d)) {
		m_position.x += m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0) {
		_currentGunIndex = 0;
	}
	else if(m_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 1){
		_currentGunIndex = 1;
	}
	else if(m_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 2){
		_currentGunIndex = 2;
	}

	glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
	mouseCoords = m_camera->convertScreenToWorld(mouseCoords);

	glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);

	m_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1) {
		m_guns[_currentGunIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
										centerPosition,
										m_direction,
										*m_bullets,
										deltaTime);
	}

	collideWithLevel(levelData);
}
