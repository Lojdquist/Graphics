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
	Engine::InputManager* _inputManager;
	Engine::Camera2D* _camera;
	std::vector<Bullet>* _bullets;

	std::vector<Gun*> _guns;
	int _currentGunIndex = -1;
};

