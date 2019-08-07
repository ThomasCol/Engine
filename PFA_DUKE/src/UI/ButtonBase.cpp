#include "ButtonBase.h"
#include "TextRenderer.h"

namespace UI
{
	ButtonBase::ButtonBase(const float& x, const float& y, const float& width, 
			const float& height, const std::string& message, Resources::ResourcesManager* m, float scale,
			const Vec3& textColor, const Vec4& backColor)
		: UIObject(x, y, width, height), m_message {message}, 
		m_tr {new LowRenderer::TextRenderer(m, "resources/Font/arial.ttf", scale, textColor, backColor)}
	{
	}	

	ButtonBase::ButtonBase(const ButtonBase& b) : UIObject {b}, m_message {b.GetMessage()}
	{

	}

	ButtonBase::~ButtonBase()
	{
		delete m_tr;
	}

	void	ButtonBase::Draw(SDL_Window* w) const
	{
		if (m_isTexture)
			m_tr->RenderButton(m_message, m_x, m_y, m_width, m_height, w, m_texture);
		else
			m_tr->RenderButton(m_message, m_x, m_y, m_width, m_height, w);
	}

	void	ButtonBase::Update(const float& mouseX, const float& mouseY)
	{
		IsPressed(mouseX, mouseY);
	}
}
