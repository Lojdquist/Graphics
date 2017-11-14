#include "Zombie.h"
#include <Engine/ResourceManager.h>

#include "Human.h"



Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos){
	_speed = speed;
	_position = pos;
	_health = 50;
	//Set color green
	_color.r = 0;
	_color.g = 255;
	_color.b = 0;
	_color.a = 255;

	_textureID = Engine::ResourceManager::getTexture("Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		_direction = glm::normalize(closestHuman->getPostion() - _position);
		_position += _direction * _speed * deltaTime;
	}

	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans){
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;

	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPostion() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}
