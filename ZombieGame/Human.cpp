#include "Human.h"

#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <Engine/ResourceManager.h>




Human::Human()
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos){
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> ranDir(-1.0f, 1.0f);
	static std::uniform_int_distribution<int> ranInt(1, 40);

	_health = 20;

	_color = Engine::ColorRGBA8(255, 255, 255, 255);

	_speed = speed;
	_position = pos;
	_textureID = Engine::ResourceManager::getTexture("Textures/human.png").id;

	//Generate random start frame
	_frames = ranInt(randomEngine);

	//Get random direction
	_direction = glm::vec2(ranDir(randomEngine), ranDir(randomEngine));
	//make sure direction isn't zero
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);
	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);

	_position += _direction * _speed * deltaTime;

	if (_frames == 60) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}else {
		_frames++;
	}

	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}

}
