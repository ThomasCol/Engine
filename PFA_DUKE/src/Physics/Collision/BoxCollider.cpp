#include "BoxCollider.h"

using namespace Core::Maths;

namespace Physics::Collision
{
	BoxCollider::BoxCollider(const BoxCollider& boxCollider):
						Collider(boxCollider),
						m_scale{boxCollider.m_scale}
	{
		m_type = CollisionType::BOX;
	}

	BoxCollider::BoxCollider(BoxCollider&& boxCollider):
						Collider(std::move(boxCollider)),
						m_scale{std::move(boxCollider.m_scale)}
	{
		m_type = CollisionType::BOX;
	}

	BoxCollider::BoxCollider(Core::DataStructure::GameObject* object):
						Collider(object),
						m_scale(1)
	{
		m_type = CollisionType::BOX;
		m_object->HasCollider(this);
	}

	const std::type_info&	BoxCollider::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component*	BoxCollider::Copy() const
	{
		return new BoxCollider(*this);
	}

	void	BoxCollider::Destroy()
	{

		m_object->RemoveCollider(this);
	}

	Mat4 BoxCollider::GetTRS() const
	{
		return Collider::GetTRS() * Mat4::CreateScaleMatrix(m_scale);
	}
}
