#ifndef _ANIMATION_H_
#define _ANIMATION_H

#include "Define.h"
#include "Vec3.h"

namespace	Resources
{
	class		ResourcesManager;
	class		Texture;

	class		Animation
	{
		private:
			std::string		m_name;
			double			m_elapsedTime;
			float			m_speed;
			int				m_nbImg;
			int				m_frame;

		public:
			Animation();
			Animation(std::string name, int nbImg);
			~Animation();

			inline int	GetFrame() const { return m_frame; }
			inline bool	GetState() const { return (m_frame == m_nbImg) ? true : false; }

			void		SetSpeed(float speed);
			void		SetImgFrame(int frame);

			Texture*	SetFrameLoopFire(ResourcesManager* rm, double frameTime, bool& fire);
			Texture*	SetFrameLoop(ResourcesManager* rm, double frameTime, bool loop = true);
			Texture*	SetFrame(ResourcesManager* rm);
	};
}

#endif // !_ANIMATION_H_
