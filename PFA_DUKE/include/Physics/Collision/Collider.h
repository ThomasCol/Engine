#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <GL/glew.h>
#include "Vec3.h"
#include "Mesh.h"
#include "Component.h"
#include "Mat4.h"
#include "GameObject.hpp"


namespace Physics::Collision
{
	enum class CollisionType
	{
		NONE,
		BOX,
		SPHERE
	};

	class Collider : public Core::DataStructure::Component
	{
	protected:
		Core::Maths::Vec3	m_position;
		CollisionType		m_type;
		bool				m_isTrigger;

	public:
		static GLuint colliderShader;

		Collider() = default;
		Collider(const Collider& collider);
		Collider(Collider&& collider);
		Collider(Core::DataStructure::GameObject* object);
		virtual ~Collider() = default;

		virtual const std::type_info&			GetType() const override;
		virtual Core::DataStructure::Component*	Copy() const override;
		inline virtual	void					Destroy() override {};

		inline CollisionType					Type() const {return m_type;}
		Core::Maths::Vec3						Position() const;
		inline Core::Maths::Vec3				LocalPosition() const {return m_position;}
		inline Core::Maths::Vec3&				LocalPosition() {return m_position;}
		Core::Maths::Vec3						GetPositionStartFrame() const;
		virtual Core::Maths::Mat4				GetTRS() const;
		inline std::string						GetName() { return m_object->GetName(); }

		inline bool								isTrigger()	const	{ return m_isTrigger; }
		inline bool&							isTrigger()			{ return m_isTrigger; }
	};
}



#endif
