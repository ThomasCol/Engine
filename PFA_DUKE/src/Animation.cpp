#include "Animation.h"
#include "ResourcesManager.h"

namespace	Resources
{
	Animation::Animation() : m_name(""), m_elapsedTime(0), m_speed(0.f), m_nbImg(0), m_frame(0)
	{
	}

	Animation::Animation(std::string name, int nbImg) : m_name(name), m_elapsedTime(0), m_speed(0.f), m_nbImg(nbImg), m_frame(0)
	{
	}

	Animation::~Animation()
	{
	}

	void		Animation::SetSpeed(float speed)
	{
		m_speed = speed;
	}

	void		Animation::SetImgFrame(int frame)
	{
		m_frame = frame;
	}

	Texture*	Animation::SetFrame(ResourcesManager* rm)
	{
		return rm->AddTexture(((m_name + std::string(PNG)).c_str()));
	}

	Texture*	Animation::SetFrameLoop(ResourcesManager* rm, double frameTime, bool loop)
	{
		m_elapsedTime += frameTime;

		if (m_elapsedTime > m_speed)
		{
			m_elapsedTime = 0;
			if (m_frame + 1 > m_nbImg && loop)
				m_frame = 0;
			else if (m_frame + 1 > m_nbImg && !loop)
				m_frame = m_frame;
			else
				m_frame += 1;
		}

		if (m_frame != 0)
			return rm->AddTexture(((m_name + std::to_string(m_frame) + std::string(PNG)).c_str()));
		else
			return rm->AddTexture(((m_name + std::string(PNG)).c_str()));
	}

	Texture*	Animation::SetFrameLoopFire(ResourcesManager* rm, double frameTime, bool& fire)
	{
		if (fire)
		{
			m_elapsedTime += frameTime;
			if (m_elapsedTime > m_speed)
			{
				m_elapsedTime = 0;
				if (m_frame + 1 > m_nbImg)
				{
					fire = false;
					m_frame = 0;
				}
				else
					m_frame += 1;
			}
		}

		if (m_frame != 0)
			return rm->AddTexture(((m_name + std::to_string(m_frame) + std::string(PNG)).c_str()));
		else
			return rm->AddTexture(((m_name + std::string(PNG)).c_str()));
	}
}
