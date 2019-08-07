#include "Component.h"
#include "GameObject.hpp"

namespace Core::DataStructure
{
    Component::Component():
		m_object {nullptr},
		m_isDestroyed {false},
		m_isEnable(true)
    {}

    Component::Component(const Component& comp):
		m_object {comp.m_object},
		m_isDestroyed {false},
		m_isEnable(true)
    {}

    Component::Component(Component&& comp):
		m_object {std::move(comp.m_object)},
		m_isDestroyed {std::move(comp.m_isDestroyed)},
		m_isEnable {std::move(comp.m_isEnable)}
    {}

    Component::Component(GameObject* obj):
		m_object {obj}, 
		m_isDestroyed {false}, 
		m_isEnable {true}
    {}

    GameObject* Component::GetObject()
    {
        return m_object;
    }

	void		Component::EreaseEndOfFrame()
	{
		m_isDestroyed = true;
	}

	bool		Component::Isdestroyed() const
	{
		return m_isDestroyed;
	}
}