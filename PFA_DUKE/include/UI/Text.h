#ifndef __TEXT__
#define __TEXT__

#include <string>

#include "TextRenderer.h"
#include "UIObject.h"
#include "ResourcesManager.h"
#include "GameObject.hpp"

namespace UI
{
	class Text : public virtual UIObject
	{
		private:
		protected:
			LowRenderer::TextRenderer*	m_tr;
			std::string					m_message;
		public:
			Text(const float& x, const float& y, const float& width, 
				const float& height, const std::string& message, Resources::ResourcesManager*, float scale = 1, Core::Maths::Vec3 color = Core::Maths::Vec3(1.f,0.f,0.f));
			Text(const Text&);
			~Text();

			Text&								operator= (const Text& t);

			inline void							SetMessage(const std::string& str) { m_message = str; };
			inline std::string					GetMessage() const {return m_message;};
			inline LowRenderer::TextRenderer*	GetRenderer() const {return m_tr;};

			void								Draw(SDL_Window*) const override;
	};
}

#endif
