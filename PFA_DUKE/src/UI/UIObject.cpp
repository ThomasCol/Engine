#include <functional>
#include <iostream>

#include "UIObject.h"

namespace UI
{
	UIObject::UIObject() : m_x {0}, m_y {0}, m_width {0}, m_height {0}
	{
	}
	
	UIObject::UIObject(const float& x, const float& y, const float& width, const float& height) 
		: m_x {x}, m_y {y}, m_width(width), m_height(height)
	{
	}

	UIObject::UIObject(const UIObject& o) : m_x {o.m_x}, m_y {o.m_y}, m_width {o.m_width}, m_height {o.m_height}
	{
	}

	UIObject::UIObject(UIObject&& o)
	{
		m_x = std::move(o.GetX());
		m_y = std::move(o.GetY());
		m_width = std::move(o.m_width);
		m_height = std::move(o.m_height);
	}

	UIObject&	UIObject::operator= (const UIObject& o)
	{
		m_x = o.GetX();
		m_y = o.GetY();
		m_width = o.m_width;
		m_height = o.m_height;
		return *this;
	}
}
