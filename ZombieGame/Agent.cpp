#include "Agent.h"
#include <Engine/ResourceManager.h>
#include <algorithm>
#include "Level.h"


Agent::Agent()
{
}


Agent::~Agent()
{
}

void Agent::draw(Engine::SpriteBatch & _spriteBatch){

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	
	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	_spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);

}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData){
	std::vector<glm::vec2> collideTilePositions;

	// Check the four corners
	// First corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x,
		m_position.y);
	// Second Corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x + AGENT_WIDTH,
		m_position.y);

	// Third Corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x,
		m_position.y + AGENT_WIDTH);

	// Third Corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x + AGENT_WIDTH,
		m_position.y + AGENT_WIDTH);

	// Check if there was no collision
	if (collideTilePositions.size() == 0) {
		return false;
	}

	// Do the collision
	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
	return true;
}

bool Agent::collideWithAgent(Agent * agent){

	//Minimun distance before there is a collison
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	//Center position of this agent
	glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);
	//Center Position of the parameter agent
	glm::vec2 centerPosB = agent->getPostion() + glm::vec2(AGENT_RADIUS);

	// Distance vector between the two agents
	glm::vec2 distVec = centerPosA - centerPosB;

	// Length of the distance vector
	float distance = glm::length(distVec);

	// Depth of the collision
	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {
		
		//Get the direction times the collision depth so we can push the away from each other
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

		// Push them in opposite directions
		m_position += collisionDepthVec / 2.0f;
		agent->m_position -= collisionDepthVec / 2.0f;
		return true;
	}

	return false;
}

bool Agent::applyDamage(float damage){

	m_health -= damage;
	// if we died return true
	if (m_health <= 0) {
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,
	std::vector<glm::vec2>& collideTilePositions, float x, float y) {

	glm::vec2 gridPos = glm::vec2(floor(x / (float)TILE_WIDTH),
								floor(y / (float)TILE_WIDTH));

	//if outside world
	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
		gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return;
	}

	// If this tile is not an air tile, we should collide with it
	if (levelData[gridPos.y][gridPos.x] != '.') {
		collideTilePositions.push_back(gridPos * (float) TILE_WIDTH + glm::vec2((float) TILE_WIDTH / 2.0f));
	}

}

// AABB (Axis aligned Bounding box) collision
void Agent::collideWithTile(glm::vec2 tilePos){
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;

	// The minimum distance before a collision occurs
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	// Center Postion of Agent
	glm::vec2 centerAgentPos = m_position + glm::vec2(AGENT_RADIUS);

	// Vector from the agent to the tile
	glm::vec2 distVec = centerAgentPos - tilePos;

	//Get the depth of the collision
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) {
		
		//check which collision depth is less
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			// X Collision depth is smaller so we push in x direction
			if (distVec.x < 0) {
				m_position.x -= xDepth;
			}else {
				m_position.x += xDepth;
			}
		
		}
		else {
			// Y collision depth is smaller so we push in y direction
			if (distVec.y < 0) {
				m_position.y -= yDepth;
			}else {
				m_position.y += yDepth;
			}
		}
	}
}
