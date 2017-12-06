#include "InputManager.h"

namespace Engine {

	InputManager::InputManager() :_mouseCoords(0.0f, 0.0f){

	}


	InputManager::~InputManager(){

	}

	void InputManager::update(){

		for (auto& it : _keyMap) {
			_prevKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID){
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID){
		_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y){
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID){
		auto it = _keyMap.find(keyID);

		if(it != _keyMap.end()){
			return it->second;
		}
		else{
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID){
		return isKeyDown(keyID) && !wasKeyDown(keyID);
	}

	bool InputManager::wasKeyDown(unsigned int keyID){
		auto it = _prevKeyMap.find(keyID);

		if (it != _prevKeyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}

	}
}