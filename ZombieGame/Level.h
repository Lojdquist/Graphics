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

	int getWidth() { return m_levelData[0].size(); }
	int getHeight() { return m_levelData.size(); }
	int getNumHumans() { return m_numHumans; }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
	glm::vec2 getStartpos() { return m_startPlayerPos; }
	const std::vector<glm::vec2> getZombieStartPos() const { return m_zombieStartPos; }


private:
	std::vector<std::string> m_levelData;
	int m_numHumans;
	Engine::SpriteBatch m_spriteBatch;


	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_zombieStartPos;
};

