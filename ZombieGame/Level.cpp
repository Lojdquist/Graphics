#include "Level.h"

#include <fstream>
#include <iostream>
#include <Engine/Errors.h>
#include <Engine/ResourceManager.h>


Level::Level(const std::string& filename){

	std::ifstream file;
	file.open(filename);

	if (file.fail()) {
		Engine::fatalError("Failed to open " + filename);
	}

	std::string tmp;

	file >> tmp >> _numHumans;
	std::getline(file, tmp); //Throw away rest of first line.

	while (std::getline(file, tmp)) {
		_levelData.emplace_back(tmp);
	}
	
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	Engine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	//Render all tiles
	for (int y = 0; y < _levelData.size(); y++){
		for (int x = 0; x < _levelData[0].size(); x++){
			//Get tile
			char tile = _levelData[y][x];

			std::cout << tile;


			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile){
			case 'B':
				_spriteBatch.draw(destRect,
					uvRect,
					Engine::ResourceManager::getTexture("Textures/red_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case 'R':
				_spriteBatch.draw(destRect,
					uvRect,
					Engine::ResourceManager::getTexture("Textures/red_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect,
					uvRect,
					Engine::ResourceManager::getTexture("Textures/glass.png").id,
					0.0f,
					whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect,
					uvRect,
					Engine::ResourceManager::getTexture("Textures/light_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.'; // So we dont collide with a @
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.'; // So we dont collide with a Z
				_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
		std::cout << std::endl;
	}

	_spriteBatch.end();
}


Level::~Level(){

}

void Level::draw(){
	_spriteBatch.renderBatch();
}
