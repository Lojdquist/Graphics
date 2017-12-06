#pragma once

#include <glm/glm.hpp>
#include<unordered_map>

namespace Engine {

	class InputManager{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyID);

		void releaseKey(unsigned int keyID);

		void setMouseCoords(float x, float y);

		bool isKeyDown(unsigned int keyID);

		bool isKeyPressed(unsigned int keyID);

		//getters
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _prevKeyMap;
		glm::vec2 _mouseCoords;

		bool wasKeyDown(unsigned int keyID);


	};
}

