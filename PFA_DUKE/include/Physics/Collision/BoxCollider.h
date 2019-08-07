#ifndef __BOXCOLLIDER_H__
#define __BOXCOLLIDER_H__

#include "Collider.h"

namespace Physics::Collision
{
	class BoxCollider : public Collider
	{
	private:
		Core::Maths::Vec3	m_scale;

	public:
		BoxCollider() = default;
		BoxCollider(const BoxCollider& boxCollider);
		BoxCollider(BoxCollider&& boxCollider);
		BoxCollider(Core::DataStructure::GameObject* object);
		~BoxCollider() = default;

		const std::type_info&			GetType() const override;
		Core::DataStructure::Component*	Copy() const override;
		void							Destroy() override;

		Core::Maths::Mat4				GetTRS() const override;
		inline void						SetScaleCollider(const Vec3& scale) { m_scale = scale; };
	};
}



#endif
