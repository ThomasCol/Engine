#include "Transform.h"
#include "GameObject.hpp"

#include "Debug.h"

using namespace Core::Maths;
using namespace Core::DataStructure;

namespace Physics
{
	Transform::Transform():
				m_ref {Vec3::zero}
	{}

	Transform::Transform(const Transform& transform):
				Component(transform.m_object),
				m_localPosition {transform.m_localPosition},
				m_localRotation {transform.m_localRotation},
				m_localScale {transform.m_localScale},
				m_worldPosition {transform.m_worldPosition},
				m_worldRotation {transform.m_worldRotation},
				m_worldScale {transform.m_worldScale},
				m_ref {transform.m_ref},
				m_transform {transform.m_transform},
				m_rotation {transform.m_rotation},
				m_parent {transform.m_parent},
				m_childs {transform.m_childs}
	{}

	Transform::Transform(Transform&& transform) :
				Component(std::move(transform.m_object)),
				m_localPosition{ std::move(transform.m_localPosition) },
				m_localRotation{ std::move(transform.m_localRotation) },
				m_localScale{ std::move(transform.m_localScale) },
				m_worldPosition{ std::move(transform.m_worldPosition) },
				m_worldRotation{ std::move(transform.m_worldRotation) },
				m_worldScale{ std::move(transform.m_worldScale) },
				m_ref{ std::move(transform.m_ref) },
				m_transform{ std::move(transform.m_transform) },
				m_rotation{ std::move(transform.m_rotation) },
				m_parent{ std::move(transform.m_parent) },
				m_childs{ std::move(transform.m_childs) }
	{}

	Transform::Transform(GameObject* object):
				Component(object),
				m_ref {Vec3::zero},
				m_rotation(Mat4::Identity()),
				m_parent{ nullptr }
	{}

	Transform::~Transform()
	{
		for (auto it = m_childs.begin(); it != m_childs.end(); it++)
			(*it)->m_object->Destroy();

		if (m_parent != nullptr && m_parent->m_childs.size() != 0)
			m_parent->m_childs.remove(this);
	}

	const std::type_info& Transform::GetType() const
	{
		return typeid(*this);
	}

	Component*	Transform::Copy() const
	{
		return new Transform(*this);
	}

	void	Transform::AddChild(Transform* transform)
	{
		m_childs.push_back(transform);
		transform->m_parent = this;
		transform->m_localPosition = transform->m_worldPosition;
		transform->m_localRotation = transform->m_worldRotation;
		transform->m_localScale = transform->m_worldScale;
		transform->m_rotation = m_rotation;
		transform->Update();
	}

	void	Transform::Update()
	{
		if (m_parent == nullptr)
		{
			m_transform = Mat4::CreateTRSMatrix(m_worldPosition, m_worldRotation, m_worldScale);
			m_ref = {m_worldPosition, m_rotation * m_ref.i, m_rotation * m_ref.j, m_rotation * m_ref.k};
		}
		else
		{
			m_transform = Mat4::CreateTRSMatrix(m_localPosition, m_localRotation, m_localScale);
			m_transform = m_parent->m_transform * m_transform;
			m_worldPosition = Vec3(m_transform[0][3], m_transform[1][3], m_transform[2][3]);
			m_ref = {m_worldPosition, m_rotation * m_ref.i, m_rotation * m_ref.j, m_rotation * m_ref.k};
		}

		for (auto it = m_childs.begin(); it != m_childs.end(); it++)
			(*it)->Update();
	}

	void	Transform::UpdateStartFramePosition()
	{
		m_startFramePosition = m_worldPosition;
	}

	void	Transform::Translate(const Vec3& translation)
	{
		if (m_parent == nullptr)
			m_worldPosition += translation;
		else
			m_localPosition += translation;

		Update();
	}

	void	Transform::Rotate(const Vec3& rotation)
	{
		if (m_parent == nullptr)
		{
			m_worldRotation += rotation;
			m_rotation = Mat4::CreateFixedAngleEulerRotationMatrix(m_worldRotation);
		}
		else
		{
			m_localRotation += rotation;
			m_rotation = Mat4::CreateFixedAngleEulerRotationMatrix(m_localRotation);
		}

		Update();
	}

	void	Transform::Scale(const Vec3& scale)
	{
		if (m_parent == nullptr)
			m_worldScale += scale;
		else
			m_localScale += scale;
		Update();
	}

	void 	Transform::Init(const Core::Maths::Vec3& trans, const Core::Maths::Vec3& rot,
															const Core::Maths::Vec3& scale)
	{
		/* set first transform matrix */
		if (!m_parent)
		{
			m_worldPosition = trans;
			m_worldRotation = rot;
			m_worldScale = scale;
		}
		else
		{
			m_localPosition = trans;
			m_localRotation = rot;
			m_localScale = scale;
		}

		Update();
		m_startFramePosition = m_worldPosition;
	}

	Mat4	Transform::GetTransform()
	{
		return m_transform;
	}

	Vec3	Transform::GetWorldPos()
	{
		return m_worldPosition;
	}

	Vec3	Transform::GetLocalPos()
	{
		return m_localPosition;
	}

	void	Transform::SetPos(const Vec3& pos)
	{
		if (m_parent == nullptr)
			m_worldPosition = pos;
		else
			m_localPosition = pos;

		Update();
	}

	void	Transform::SetRotation(const Vec3& rot)
	{
		if (m_parent == nullptr)
			m_worldRotation = rot;
		else
			m_localRotation = rot;

		Update();
	}

	void	Transform::SetWorldPos(const Vec3& pos)
	{
		m_worldPosition = pos;

		if (m_parent != nullptr)
			m_ref.o = m_worldPosition;
	}

	Vec3	Transform::GetWorldScale()
	{
		return m_worldScale;
	}

	Ref3	Transform::GetRef()
	{
		return m_ref;
	}

	Vec3	Transform::GetStartFramePosition() const
	{
		return m_startFramePosition;
	}

	void	Transform::SetTransform(const Mat4& mat)
	{
		m_transform = mat;
	}
}
