#include "Sound.h"
#include "Debug.h"
#include <iostream>

static int channel = 1;

Sound::Sound() : m_sound(NULL), m_elapsedTime(0), m_speed(0)
{
}

Sound::Sound(std::string file)
{
	m_sound = NULL;
	m_sound = Mix_LoadWAV(file.c_str());

	if (m_sound == NULL)
		LOG(ERROR, "Fail to load Sound: " + file + ".");
	else
		LOG(INFO, "Sound: " + file + " loaded.");

	m_elapsedTime = 0;
	m_channel = channel;
	Mix_Volume(m_channel, MIX_MAX_VOLUME / 5);
	channel++;
	m_speed = 0;
}

Sound::Sound(const Sound& sound):
	m_sound(sound.m_sound),
	m_elapsedTime(sound.m_elapsedTime),
	m_speed(sound.m_speed)
{
}

Sound::Sound(const Sound&& sound):
	m_sound { std::move(sound.m_sound) },
	m_elapsedTime { std::move(sound.m_elapsedTime) },
	m_speed { std::move(sound.m_speed) }
{}

Sound::~Sound()
{
	/* Free Sound */
	Mix_FreeChunk(m_sound);
}

void		Sound::Play(double frameTime, bool repeat)
{
	if (m_sound != NULL)
	{
		if (repeat)
			Mix_PlayChannelTimed(m_channel, m_sound, 0, (int)(m_speed * frameTime));
		else
			Mix_PlayChannel(m_channel, m_sound, 0);
	}
}

void		Sound::SetSpeed(float speed)
{
	m_speed = speed;
}
