#include "AudioEngine.h"
#include "EngineErrors.h"

namespace Engine {

	void SoundEffect::play(int loops){
		if (Mix_PlayChannel(-1, m_chunck, loops) == -1) {
			if (Mix_PlayChannel(0, m_chunck, loops) == -1) {
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}

	}
	void Music::play(int loops){
		if (Mix_PlayMusic(m_music, loops) == -1) {
			fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
		}
	}

	void Music::pause(){
		Mix_PauseMusic();
	}

	void Music::stop(){
		Mix_HaltMusic();
	}

	void Music::resume(){
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine(){
		//Empty
	}


	AudioEngine::~AudioEngine(){
		destroy();
	}

	void Engine::AudioEngine::init(){

		if (m_isInitialized) {
			fatalError("Tried to initialize twice");
		}

		// parameter can be a bitwise combination of MIX_INIT_FAC
		// MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
			fatalError("Mix_init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1){
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}

	void Engine::AudioEngine::destroy(){
		if (m_isInitialized) {
			m_isInitialized = false;

			for (auto& it : m_effectMap) {
				Mix_FreeChunk(it.second);
			}

			for (auto& it : m_musicMap) {
				Mix_FreeMusic(it.second);
			}

			m_effectMap.clear();
			m_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect Engine::AudioEngine::loudSoundEffect(const std::string & filePath){
		// Try to find the audio in the cache
		auto it = m_effectMap.find(filePath);

		SoundEffect effect;

		if (it == m_effectMap.end()) {
			//Failed to find it, must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

			//Check for errors
			if (chunk == nullptr) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			m_effectMap[filePath] = chunk;
			effect.m_chunck = chunk;
		}
		else {
			//It is already cached
			effect.m_chunck = it->second;
		}

		return effect;
	}

	Music Engine::AudioEngine::loadMusic(const std::string & filePath){
		// Try to find the audio in the cache
		auto it = m_musicMap.find(filePath);

		Music music;

		if (it == m_musicMap.end()) {
			//Failed to find it, must load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());

			//Check for errors
			if (mixMusic == nullptr) {
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}

			m_musicMap[filePath] = mixMusic;
			music.m_music = mixMusic;
		}
		else {
			//It is already cached
			music.m_music = it->second;
		}

		return music;
	}
}
