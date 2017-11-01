#pragma once

#include <string>
#include <vector>

#include <Engine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	Level(const std::string& filename);
	~Level();

	void draw();

	int getWidth() { return _levelData[0].size(); }
	int getHeight() { return _levelData.size(); }
	int getNumHumans() { return _numHumans; }
	const std::vector<std::string>& getLevelData() const { return _levelData; }
	glm::vec2 getStartpos() { return _startPlayerPos; }
	const std::vector<glm::vec2> getZombieStartPos() const { return _zombieStartPos; }


private:
	std::vector<std::string> _levelData;
	int _numHumans;
	Engine::SpriteBatch _spriteBatch;


	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _zombieStartPos;
};

