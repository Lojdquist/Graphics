#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace Engine {

	class SoundEffect {
	public:
		friend class AudioEngine;
		///Plays the effect file
		/// @param loops: If loops = -1, loop forever, 
		///otherwise play it loops +1 time
		void play(int loops = 0);
	private:
		Mix_Chunk* m_chunck = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;
		///Plays the audio file
		/// @param loops: If loops = -1, loop forever, 
		///otherwise play it loops time
		void play(int loops = 1);

		static void pause();
		static void stop();
		static void resume();

	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loudSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:
		bool m_isInitialized = false;

		std::map<std::string, Mix_Chunk*> m_effectMap;
		std::map<std::string, Mix_Music*> m_musicMap;

	};
}

