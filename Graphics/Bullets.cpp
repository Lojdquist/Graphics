#include "Bullets.h"
#include <Engine/ResourceManager.h>
#include <Engine/GLTexture.h>


Bullets::Bullets(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime){
	_position = pos;
	_direction = dir;
	_speed = speed;
	_lifeTime = lifeTime;
}


Bullets::~Bullets()
{
}

void Bullets::draw(Engine::SpriteBatch & spriteBatch){

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Engine::GLTexture texture = Engine::ResourceManager::getTexture("Textures/PNG/CharacterLeft_Jump.png");

	Engine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30.0f, 30.0f);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullets::update(){
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0) {
		return true;
	}
	return false;
}
