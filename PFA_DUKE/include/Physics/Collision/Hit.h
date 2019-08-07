#ifndef __HIT_H__
#define __HIT_H__

#include "Vec3.h"

namespace Physics::Collision
{
	class Collider;

	class Hit
	{
	private:
		Collider*			m_colliderA;
		Collider*			m_colliderB;
		Core::Maths::Vec3	m_position;
		Core::Maths::Vec3	m_normal;
		float				m_penetration;

	public:
		Hit();
		Hit(const Core::Maths::Vec3& position, const Core::Maths::Vec3& normal);
		Hit(const Hit& hit);
		Hit(Hit&& hit);
		~Hit() = default;

		inline Collider*&			ColliderA() { return m_colliderA; }
		inline Collider*&			ColliderB() { return m_colliderB; }
		inline Core::Maths::Vec3&	Position() {return m_position;}
		inline Core::Maths::Vec3&	Normal() {return m_normal;}
		inline float&				Penetration() {return m_penetration;}

		inline Collider*			ColliderA() const { return m_colliderA; }
		inline Collider*			ColliderB() const { return m_colliderB; }
		inline Core::Maths::Vec3	Position() const {return m_position;}
		inline Core::Maths::Vec3	Normal() const {return m_normal;}
		inline float				Penetration() const {return m_penetration;}

		Hit&						operator=(const Hit& hit);
	};
}



#endif
