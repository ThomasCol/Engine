#ifndef __RAYCASTHIT_H__
#define __RAYCASTHIT_H__

#include "Vec3.h"
#include "Collider.h"

namespace Physics::Collision
{
	class RaycastHit
	{
	private:
		Physics::Collision::Collider*	m_collider;
		Core::Maths::Vec3				m_position;
		Core::Maths::Vec3				m_normal;
		float							m_distance;


	public:
		RaycastHit();
		RaycastHit(const RaycastHit& hit);
		RaycastHit(RaycastHit&& hit);
		~RaycastHit() = default;

		inline Physics::Collision::Collider*	Collider() const	{ return m_collider; }
		inline Physics::Collision::Collider*&	Collider()			{ return m_collider; }

		inline Core::Maths::Vec3&				Position()			{ return m_position; }
		inline Core::Maths::Vec3				Position()			const { return m_position; }

		inline Core::Maths::Vec3&				Normal()			{ return m_normal; }
		inline Core::Maths::Vec3				Normal() const		{ return m_normal; }

		inline float&							Distance()			{ return m_distance; }
		inline float							Distance() const	{ return m_distance; }

		RaycastHit&								operator=(const RaycastHit& hit);
	};
}

#endif