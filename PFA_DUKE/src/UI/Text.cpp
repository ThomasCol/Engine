#include "Text.h"

namespace UI
{
	Text::Text(const float& x, const float& y, const float& width, const float& height, 
			const std::string& message, Resources::ResourcesManager* m, float scale, Core::Maths::Vec3 color)
		: UIObject(x, y, width, height), m_message {message},
		m_tr {new LowRenderer::TextRenderer(m, "resources/Font/arial.ttf", scale, color, {1, 0, 1, 1})}
	{
	}	

	Text::Text(const Text& b) : UIObject(b), m_message {b.GetMessage()}, m_tr {b.m_tr}
	{

	}

	Text::~Text()
	{
		delete m_tr;
	}

	Text&	Text::operator= (const Text& t)
	{
		m_tr = t.GetRenderer();
		m_x = t.GetX(); m_y = t.GetY();
		m_width = t.GetWidth(); m_height = t.GetHeight();
		return *this;
	}

	void	Text::Draw(SDL_Window* w) const
	{
		m_tr->RenderTextCentered(m_message, m_x, m_y, m_width, m_height, w);
	}
}
