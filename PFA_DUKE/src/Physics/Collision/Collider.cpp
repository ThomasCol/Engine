#include "Collider.h"
#include "Transform.h"
//#include "Entity.h"

using namespace LowRenderer;
using namespace Core::DataStructure;
using namespace Core::Maths;

namespace Physics::Collision
{
	Collider::Collider(Core::DataStructure::GameObject* object):	Component(object),
																	m_position(0),
																	m_type{CollisionType::NONE},
																	m_isTrigger{false}
	{}

	Collider::Collider(const Collider& collider):	Component(collider),
													m_position(collider.m_position),
													m_type{collider.m_type},
													m_isTrigger{collider.m_isTrigger}
	{}

	Collider::Collider(Collider&& collider):	Component(std::move(collider)),
												m_position(std::move(collider.m_position)),
												m_type{std::move(collider.m_type)},
												m_isTrigger{std::move(collider.m_isTrigger)}
	{}

	const std::type_info&	Collider::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component*	Collider::Copy() const
	{
		return new Collider(*this);
	}

	Vec3 Collider::Position() const
	{
		return m_object->GetWorldPos() + m_position;
	}

	Core::Maths::Vec3 Collider::GetPositionStartFrame() const
	{
		return m_object->GetStartFramePosition() + m_position;
	}

	Mat4 Collider::GetTRS() const
	{
		if (!m_object)
			return Mat4::Identity();

		return m_object->GetTransform() * Mat4::CreateTranslationMatrix(m_position);
	}
}
