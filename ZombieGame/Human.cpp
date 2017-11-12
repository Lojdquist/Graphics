#include "Human.h"

#include <Engine/ResourceManager.h>



Human::Human()
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos){

	_health = 20;
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	_speed = speed;
	_position = pos;

	_textureID = Engine::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){

	_position += _direction * _speed * deltaTime;
}
