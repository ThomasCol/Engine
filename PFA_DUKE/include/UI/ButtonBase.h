#ifndef __BUTTON_BASE__
#define __BUTTON_BASE__

#include <string>
#include <functional>

#include "ResourcesManager.h"
#include "TextRenderer.h"
#include "UIObject.h"

namespace UI
{
	class ButtonBase : public virtual UIObject
	{
		protected:
			std::string					m_message;
			bool						m_isTexture {false};
			GLuint						m_texture;
			LowRenderer::TextRenderer*	m_tr;
		public:
			ButtonBase(const float& x, const float& y, const float& width, 
				const float& height, const std::string& message, Resources::ResourcesManager*, float scale = 1,
				const Vec3& textColor = Vec3(1), const Vec4& backColor = Vec4(0, 0, 0, 1));
			ButtonBase(const ButtonBase&);
			virtual ~ButtonBase();

			virtual bool				IsPressed(const float&, const float&) = 0;
			
			void						SetBackTexture(GLuint t) {m_isTexture = true; m_texture = t;};

			inline std::string			GetMessage() const {return m_message;};
			inline void					SetMessage(const std::string& m) { m_message = m; };

			void						Draw(SDL_Window*) const override;

			void						Update(const float& mouseX, const float& mouseY) override;
	};
}


#endif
