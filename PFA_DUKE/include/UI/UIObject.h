#ifndef __UIOBJECT__
#define __UIOBJECT__

#include <SDL2/SDL.h>
#include <iostream>

#include "Component.h"

namespace UI
{
	class UIObject
	{
		private:
		protected:
			float	m_x;
			float	m_y;
			float	m_width;
			float	m_height;
		public:
			UIObject();
			UIObject(const float& x, const float& y, const float& width, const float& height);
			UIObject(const UIObject&);
			UIObject(UIObject&&);

			virtual ~UIObject() = default;

			UIObject&	operator= (const UIObject&);

			inline float		GetX() const {return m_x;}
			inline float		GetY() const {return m_y;}
			inline float		GetWidth() const {return m_width;};
			inline float		GetHeight() const {return m_height;};
			
			inline virtual bool	Press() { return false; };

			inline virtual void	Update(const float&, const float&) {};
			virtual void		Draw(SDL_Window*) const {};
	};
}

#endif
