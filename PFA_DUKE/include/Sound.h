#ifndef _SOUND_H_
#define _SOUND_H_

#include <string>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>

class		Sound
{
	private:
		Mix_Chunk*		m_sound;
		int				m_channel;
		double			m_elapsedTime;
		float			m_speed;
	public:
		Sound();
		Sound(std::string file);
		Sound(const Sound& sound);
		Sound(const Sound&& sound);
		~Sound();

		void		Play(double frameTime, bool repeat);
		void		SetSpeed(float speed);
};

#endif // !_SOUND_H_
