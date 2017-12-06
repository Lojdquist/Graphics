#include "Player.h"

#include <Engine/ResourceManager.h>
#include<SDL/SDL.h>

Player::Player() : _currentGunIndex(-1)
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, Engine::InputManager * inputManager, Engine::Camera2D * camera, std::vector<Bullet>* bullets){
	_speed = speed;
	_position = pos;
	_inputManager = inputManager;
	_bullets = bullets;
	_camera = camera;
	_color = Engine::ColorRGBA8(255, 255, 255,255);
	_health = 150;
	_textureID = Engine::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::addGun(Gun * gun){
	// Add the gun to his inventory
	_guns.push_back(gun);

	if (_currentGunIndex == -1) {
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){
	
	if (_inputManager->isKeyDown(SDLK_w)) {
		_position.y += _speed * deltaTime;

	}else if (_inputManager->isKeyDown(SDLK_s)) {
		_position.y -= _speed * deltaTime;
	}

	if (_inputManager->isKeyDown(SDLK_a)) {
		_position.x -= _speed * deltaTime;

	}else if (_inputManager->isKeyDown(SDLK_d)) {
		_position.x += _speed * deltaTime;
	}

	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0) {
		_currentGunIndex = 0;
	}
	else if(_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1){
		_currentGunIndex = 1;
	}
	else if(_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2){
		_currentGunIndex = 2;
	}

	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);

	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);

	_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1) {
		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
										centerPosition,
										_direction,
										*_bullets,
										deltaTime);
	}

	collideWithLevel(levelData);
}
