#include "Rigidbody.h"

//#include "Entity.h"
#include "GameObject.hpp"
#include "Mat4.h"
#include "math.h"
#include "SphereCollider.h"

using namespace Core::Maths;
using namespace Physics::Collision;

namespace Physics
{
	constexpr float Rigidbody::gravityForce;
	
	Rigidbody::Rigidbody(const float& mass, bool applyGravity, bool isKinematic, const float& drag)
		: m_velocity {Vec3(0.0f)}, m_mass {mass}, m_drag {drag}, 
		m_applyGravity {applyGravity}, m_isKinematic {isKinematic}
	{}

	Rigidbody::Rigidbody(const float& mass, const float& drag)
		: m_velocity {Vec3(0.f)}, m_mass {mass}, m_drag {drag},
		m_applyGravity {true}, m_isKinematic {false}
	{}

	Rigidbody::Rigidbody(const Rigidbody& r)
		: Component(r), m_velocity {r.m_velocity}, m_mass {r.m_mass}, m_drag {r.m_drag}, 
		m_applyGravity {r.m_applyGravity}, m_isKinematic {r.m_isKinematic}
	{}

	Rigidbody::Rigidbody(Rigidbody&& r)
	{
		m_velocity = std::move(r.m_velocity);

		m_mass = std::move(r.m_mass);
		m_drag = std::move(r.m_drag);

		m_applyGravity = std::move(r.m_applyGravity);
		m_isKinematic = std::move(r.m_isKinematic);
	}

	Rigidbody::Rigidbody(Core::DataStructure::GameObject* object)
		: Component(object), m_velocity {Vec3(0.f)}, m_mass {2.f}, m_drag {15.f},
		m_applyGravity {true}, m_isKinematic {false}
	{
		m_object->HasRigidbody(this);
	}

	const std::type_info&	Rigidbody::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component*	Rigidbody::Copy() const
	{
		return new Rigidbody(*this);
	}

	void	Rigidbody::Destroy()
	{
		m_object->RemoveRigidbody(this);
	}

	void	Rigidbody::Update(const float& frameTime)
	{
		m_deltaTime = frameTime;

		if (m_isKinematic)
			return;

		if (m_velocity.SqrLength() > EPSILON)
			m_velocity += (m_velocity * -1.f).Normalize() * m_drag * frameTime;
		else if (!m_applyGravity)
			m_velocity = Vec3(0.f);

		if (m_applyGravity)
			m_velocity += Vec3(0.f, gravityForce, 0.f) * m_mass * frameTime;

		m_object->SetPos(m_object->GetWorldPos() + m_velocity * frameTime);

		m_applyGravity = true;
	}

	void	Rigidbody::AddForce(const Vec3& force)
	{
		Vec3	f = force * (1.f / m_mass);
		
		if (f.SqrLength() > EPSILON)
			m_velocity += f;
	}

	void	Rigidbody::CollisionHit(const Hit& hit)
	{
		Vec3 normal(hit.Normal());

		m_velocity += normal * m_velocity.Length() * hit.Penetration();

		if (normal == Vec3(0, 1, 0))
		{
			m_velocity.y = 0.f;
			m_applyGravity = false;
		}

		Vec3 offsetCollider = m_object->GetComponent<Physics::Collision::SphereCollider>()->LocalPosition();
		m_object->SetPos(hit.Position() - offsetCollider);

		hit.ColliderA()->GetObject()->Collide(hit.ColliderB());
		hit.ColliderB()->GetObject()->Collide(hit.ColliderA());
	}

	void Rigidbody::CollisionTrigger(Collider* collider)
	{
		if (!collider)
			return;

		SphereCollider* col = m_object->GetComponent<SphereCollider>();
		if (!col)
			return;

		col->GetObject()->Collide(collider);
		collider->GetObject()->Collide(col);
	}

	Rigidbody&	Rigidbody::operator=(const Rigidbody& r)
	{
		m_velocity = r.m_velocity;

		m_mass = r.m_mass;
		m_drag = r.m_drag;

		m_applyGravity = r.m_applyGravity;
		m_isKinematic = r.m_isKinematic;

		return *this;
	}
}