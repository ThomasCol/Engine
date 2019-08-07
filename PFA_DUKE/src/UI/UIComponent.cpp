#include <functional>
#include <iostream>

#include "UIComponent.h"

namespace UI
{
	UIComponent::UIComponent() : Component(), m_object {nullptr}
	{
	}

	UIComponent::UIComponent(Core::DataStructure::GameObject* go) : Component(go), m_object{ nullptr }
	{
	}

	UIComponent::UIComponent(const UIComponent& o) : Component(o), m_object{ nullptr }
	{
	}

	UIComponent::UIComponent(UIComponent&& o)
	{
		m_object = std::move(o.m_object);
	}

	UIComponent::~UIComponent()
	{
		if (m_object != nullptr)
			delete m_object;
	}

	UIComponent&	UIComponent::operator= (const UIComponent& c)
	{
		m_object = new UIObject(*c.m_object);
		return *this;
	}

	void		UIComponent::SetUIObject(UIObject* o)
	{
		m_object = o;
	}

	void		UIComponent::Draw(SDL_Window* w) const
	{
		if (m_object != nullptr)
			m_object->Draw(w);
	}

	void		UIComponent::OnClick(const float& x, const float& y)
	{
		if (m_object != nullptr)
			m_object->Update(x, y);
	}

	bool		UIComponent::Press()
	{
		if (m_object != nullptr)
			m_object->Press();
	}
	
	Core::DataStructure::Component*	UIComponent::Copy() const
	{
		return new UIComponent(*this);
	}

	const std::type_info&		UIComponent::GetType() const
	{
		return typeid(*this);
	}
}
