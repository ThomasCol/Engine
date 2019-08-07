#include "SphereCollider.h"

using namespace Core::Maths;
using namespace Core::DataStructure;

namespace Physics::Collision
{
	SphereCollider::SphereCollider(const SphereCollider& sphereCollider):
							Collider(sphereCollider),
							m_radius{sphereCollider.m_radius}
	{
		m_type = CollisionType::SPHERE;
	}

	SphereCollider::SphereCollider(SphereCollider&& sphereCollider):
							Collider(std::move(sphereCollider)),
							m_radius{std::move(sphereCollider.m_radius)}
	{
		m_type = CollisionType::SPHERE;
	}

	SphereCollider::SphereCollider(Core::DataStructure::GameObject* object)
		: Collider(object), m_radius{1}
	{
		m_type = CollisionType::SPHERE;
		m_object->HasCollider(this);
	}



	const std::type_info&	SphereCollider::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component*	SphereCollider::Copy() const
	{
		return new SphereCollider(*this);
	}

	void	SphereCollider::Destroy()
	{
		m_object->RemoveCollider(this);
	}

	float	SphereCollider::Radius() const
	{
		return m_radius * m_object->GetWorldScale().x;
	}
}
