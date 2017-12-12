#pragma once

#include "Human.h"
#include "Bullet.h"
#include "Gun.h"

#include <Engine/InputManager.h>
#include <Engine/Camera2D.h>

class Gun;

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Engine::InputManager* inputManager, Engine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
				std::vector<Human*>& humans,
				std::vector<Zombie*>& zombies,
				float deltaTime) override;

private:
	Engine::InputManager* m_inputManager;
	Engine::Camera2D* m_camera;
	std::vector<Bullet>* m_bullets;

	std::vector<Gun*> m_guns;
	int _currentGunIndex = -1;
};

