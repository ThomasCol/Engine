#ifndef __UISPRITE__
#define __UISPRITE__

#include "UIObject.h"
#include "ResourcesManager.h"

namespace UI
{
	class UISprite : virtual public UIObject
	{
	private:
	protected:
		Resources::Texture*			m_text;
		Resources::Shader*			m_shader;
		Resources::Model*			m_model;
	public:
		UISprite(const float& x, const float& y, const float& width,
			const float& height, Resources::Texture*, Resources::ResourcesManager*);
		UISprite(const UISprite&);

		UISprite&		operator=(const UISprite& t);

		void			Draw(SDL_Window*) const override;
	};
}

#endif // !__UISPRITE__

