#include "Music.h"
#include "Define.h"
#include "Debug.h"
#include <iostream>

Music::Music()
{
	m_mus = NULL;
	m_mus = Mix_LoadMUS(DEFAULTMUSIC);
	if (m_mus == NULL)
		LOG(ERROR, "Fail to load Music.");
	else
		LOG(INFO, "Load Music: " + DEFAULTMUSIC);
}

Music::Music(std::string file)
{
	m_mus = Mix_LoadMUS(file.c_str());
	if (m_mus == NULL)
		LOG(ERROR, "Fail to load Music.");
	else
		LOG(INFO, "Load Music: " + file);
}

Music::Music(const Music & Music) :
	m_mus(Music.m_mus)
{
}

Music::Music(const Music && Music)
{
	m_mus = std::move(Music.m_mus);
}

Music::~Music()
{
	/* Free Music */
	Mix_FreeMusic(m_mus);
}

void Music::Play()
{
	if (m_mus != NULL)
	{
		/* Play Music */
		Mix_PlayMusic(m_mus, -1);
	}
}

void Music::Stop()
{
	if (m_mus != NULL)
	{
		/* Stop the music */
		Mix_HaltMusic();
	}
}
