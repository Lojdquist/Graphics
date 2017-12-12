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
	m_speed = speed;
	m_position = pos;
	m_health = 50;
	//Set color green
	m_color = Engine::ColorRGBA8(0, 255, 0 ,255);

	m_textureID = Engine::ResourceManager::getTexture("Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		m_direction = glm::normalize(closestHuman->getPostion() - m_position);
		m_position += m_direction * m_speed * deltaTime;
	}

	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans){
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;

	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPostion() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}
