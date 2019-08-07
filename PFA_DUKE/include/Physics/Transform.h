#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Vec3.h"
#include "Mat4.h"
#include <list>

#include "Component.h"


namespace Physics
{
	/* Vertex transformation */
	class Transform : public virtual Core::DataStructure::Component
	{
	private:
		/* instance */
		Core::Maths::Vec3		m_localPosition;
		Core::Maths::Vec3		m_localRotation;
		Core::Maths::Vec3		m_localScale;

		Core::Maths::Vec3		m_worldPosition;
		Core::Maths::Vec3		m_worldRotation;
		Core::Maths::Vec3		m_worldScale;

		Core::Maths::Vec3		m_startFramePosition;

		Core::Maths::Ref3		m_ref;

		Core::Maths::Mat4		m_transform;
		Core::Maths::Mat4		m_rotation;

		Transform*				m_parent;

		std::list<Transform*>	m_childs;

	public:
		/* constructor & destructor */
		Transform();
		Transform(const Transform& transform);
		Transform(Transform&& transform);
		Transform(Core::DataStructure::GameObject* object);
		~Transform();

		const std::type_info&			GetType() const override;
		Component*						Copy() const override;
		inline void						Destroy() override {};
		
		void							AddChild(Transform* transform);

		void							Update();
		void							UpdateStartFramePosition();

		void							Translate(const Core::Maths::Vec3& translation);
		void							Rotate(const Core::Maths::Vec3& rotation);
		void							Scale(const Core::Maths::Vec3& scale);

		void							SetPos(const Core::Maths::Vec3& pos);

		void							SetWorldPos(const Core::Maths::Vec3& pos);
		void							SetRotation(const Core::Maths::Vec3& rot);

		void 							Init(const Core::Maths::Vec3& trans, const Core::Maths::Vec3& rot,
											const Core::Maths::Vec3& scale);

		Core::Maths::Mat4				GetTransform();

		Core::Maths::Vec3				GetWorldPos();
		Core::Maths::Vec3				GetLocalPos();
		Core::Maths::Vec3				GetWorldScale();
		Core::Maths::Ref3				GetRef();
		inline std::list<Transform*>	GetChilds() { return m_childs; }
		inline Core::Maths::Ref3		GetParentRef() {return m_parent->m_ref; }
		Core::Maths::Vec3				GetStartFramePosition() const;

		void							SetTransform(const Core::Maths::Mat4& mat);
	};
}

#endif //_TRANSFORM_H_
