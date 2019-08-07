#include <functional>

#include "Canvas.hpp"
#include "RenderWindow.h"

namespace UI
{
	Canvas::Canvas() : Component(), m_im {nullptr}, m_isActive {true}
	{
	}

	Canvas::Canvas(const Canvas& c) : Component(c), m_im {nullptr}, m_isActive {true}
	{
	}

	Canvas::Canvas(Canvas&& c) : Component(std::move(c))
	{

		m_im = std::move(c.m_im);
		m_isActive = std::move(c.m_isActive);
	}

	Canvas::Canvas(Core::DataStructure::GameObject* g) : Component(g), m_im {nullptr}, m_isActive {true}
	{
	}

	Canvas::~Canvas()
	{
		if (m_im != nullptr)
		{
			m_im->RemoveOnClick(std::bind(&UI::Canvas::OnClick, this, ARG3));
			m_im->RemoveOnDelete(std::bind(&UI::Canvas::DeleteIM, this));
		}
	}

	void		Canvas::DeleteIM()
	{
		m_im->RemoveOnClick(std::bind(&UI::Canvas::OnClick, this, ARG3));
		m_im->RemoveOnDelete(std::bind(&UI::Canvas::DeleteIM, this));
		m_im = nullptr;
	}

	Canvas&		Canvas::operator= (const Canvas& c)
	{
		SetInput(c.m_im);
		m_isActive = c.m_isActive;
		return *this;
	}

	Text*	Canvas::AddText(const std::string& message, const Core::Maths::Vec2& pos, 
				const Core::Maths::Vec2& size, Resources::ResourcesManager* m, float scale, Core::Maths::Vec3 color)
	{
		m_object->AddComponent<UIComponent>();
		std::vector<UIComponent*>	objects {m_object->GetComponents<UIComponent>()};

		objects.back()->SetUIObject(new Text(pos.x, pos.y, size.x, size.y, message, m, scale, color));
		return dynamic_cast<Text*>(objects.back()->GetUIObject());
	}

	UISprite*	Canvas::AddSprite(const std::string& texture, const Core::Maths::Vec2& center,
		const Core::Maths::Vec2& size, Resources::ResourcesManager* m)
	{
		m_object->AddComponent<UIComponent>();
		std::vector<UIComponent*>	objects{ m_object->GetComponents<UIComponent>() };

		objects.back()->SetUIObject(new UISprite(center.x, center.y, size.x, size.y, m->AddTexture(texture), m));
		return dynamic_cast<UISprite*>(objects.back()->GetUIObject());
	}

	void		Canvas::OnClick(const int& xPix, const int& yPix, SDL_Window* w)
	{
		if (!m_isActive)
			return;
		int	windowW, windowH;
		SDL_GetWindowSize(w, &windowW, &windowH);
		
		CheckClick(2 * (float)xPix / (float)windowW - 1, -(2 * (float)yPix / (float)windowH - 1));
	}

	void		Canvas::CheckClick(const float& x, const float& y)
	{
		if (!m_isActive)
			return;
		std::vector<UIComponent*>	objects {m_object->GetComponents<UIComponent>()};
		for (unsigned i {0}; i < objects.size(); ++i)
		{
			objects[i]->OnClick(x, y);
		}
	}

	void		Canvas::OnDrawGUI(SDL_Window* w) const
	{
		if (!m_isActive)
			return;
		std::vector<UIComponent*>	objects {m_object->GetComponents<UIComponent>()};
		for (unsigned i {0}; i < objects.size(); ++i)
			objects[i]->Draw(w);
	}

	void		Canvas::Clear()
	{
		m_object->RemoveComponents<UIComponent>();
	}

	const std::type_info&		Canvas::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component*	Canvas::Copy() const
	{
		return new Canvas(*this);
	}

	void				Canvas::SetInput(InputManager* im)
	{
		if (m_im != nullptr)
		{
			m_im->RemoveOnClick(std::bind(&UI::Canvas::OnClick, this, ARG3));
			m_im->RemoveOnDelete(std::bind(&UI::Canvas::DeleteIM, this));
		}

		im->AddOnClick(std::bind(&UI::Canvas::OnClick, this, ARG3));
		im->AddOnDelete(std::bind(&UI::Canvas::DeleteIM, this));
		m_im = im;
	}
}
