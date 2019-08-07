#include "RenderWindow.h"
#include "Behaviour.h"
#include "GameObject.hpp"

namespace Core::DataStructure
{
    Behaviour::Behaviour(GameObject* object):
		Component(object)
    {
        m_object->AddBehaviour(this);
    }

	void	Behaviour::Destroy()
	{
		m_object->RemoveBehaviour(this);
	}
}
