#ifndef __SPHERECOLLIDER_H__
#define __SPHERECOLLIDER_H__

#include "Collider.h"

namespace Physics::Collision
{
	class Entity;

	class SphereCollider : public Collider
	{
	private:
		float	m_radius;

	public:
		SphereCollider() = default;
		SphereCollider(const SphereCollider& sphereCollider);
		SphereCollider(SphereCollider&& sphereCollider);
		SphereCollider(Core::DataStructure::GameObject* object);
		~SphereCollider() = default;

		const std::type_info&			GetType() const override;
		Core::DataStructure::Component*	Copy() const override;
		void							Destroy() override;
		float							Radius() const;
		inline float&					Radius() {return m_radius;};
	};
}



#endif
