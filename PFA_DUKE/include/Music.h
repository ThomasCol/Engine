#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <string>
#include "SDL2/SDL_audio.h"
#include "SDL2/SDL_mixer.h"

class Music
{
private:
	Mix_Music*	m_mus;

public:
	Music();
	Music(std::string file);
	Music(const Music& Music);
	Music(const Music&& Music);
	~Music();

	void		Play();
	void		Stop();
};

#endif // !_MUSIC_H_