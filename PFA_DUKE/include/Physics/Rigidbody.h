#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include "Component.h"
#include "Vec3.h"
#include "Hit.h"

#define EPSILON	0.001

namespace Physics
{
	class Entity;

	class Rigidbody: public Core::DataStructure::Component
	{
	private:
		Core::Maths::Vec3	m_velocity;

		float					m_mass;
		float					m_drag;

		float					m_deltaTime;
		bool					m_applyGravity;
		bool					m_isKinematic;

	public:
		static constexpr float	gravityForce = -9.81f;

		Rigidbody(const float& mass = 1.0f, bool applyGravity = true, bool isKinematic = false,
			const float& drag = 0.05f);
		Rigidbody(const float& mass, const float& drag);
		Rigidbody(const Rigidbody& r);
		Rigidbody(Rigidbody&& r);
		Rigidbody(Core::DataStructure::GameObject* object);

		~Rigidbody() = default;

		const std::type_info&			GetType() const override;
		Core::DataStructure::Component*	Copy() const override;
		void							Destroy() override;

		void							Update(const float& frameTime);

		void							AddForce(const Core::Maths::Vec3& f);
		void							CollisionHit(const Physics::Collision::Hit& hit);
		void							CollisionTrigger(Collision::Collider* collider);

		inline Core::Maths::Vec3		Velocity() const	{ return m_velocity; }
		inline Core::Maths::Vec3&		Velocity()			{ return m_velocity; }

		inline float					Mass() const		{ return m_mass; }
		inline float&					Mass()				{ return m_mass; }

		inline float					Drag() const		{ return m_drag; }
		inline float&					Drag()				{ return m_drag; }

		inline bool						IsKinematic() const	{ return m_isKinematic; }
		inline bool&					IsKinematic()		{ return m_isKinematic; }

		inline bool						IsGravity() const { return m_applyGravity; }

		Rigidbody&						operator=(const Rigidbody& r);
	};
}

#endif
