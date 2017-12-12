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

	m_health = 20;

	m_color = Engine::ColorRGBA8(255, 255, 255, 255);

	m_speed = speed;
	m_position = pos;
	m_textureID = Engine::ResourceManager::getTexture("Textures/human.png").id;

	//Generate random start frame
	m_frames = ranInt(randomEngine);

	//Get random direction
	m_direction = glm::vec2(ranDir(randomEngine), ranDir(randomEngine));
	//make sure direction isn't zero
	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);
	m_direction = glm::normalize(m_direction);
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);

	m_position += m_direction * m_speed * deltaTime;

	if (m_frames == 60) {
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		m_frames = 0;
	}else {
		m_frames++;
	}

	if (collideWithLevel(levelData)) {
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	}

}
