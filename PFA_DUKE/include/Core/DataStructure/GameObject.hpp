#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <vector>
#include <typeinfo>

#include "Transform.h"
#include "Debug.h"
#include "Event.hpp"

class Sprite;

namespace Resources
{
	class Scene;
}

namespace LowRenderer
{
	class Mesh;
}

namespace Physics
{
	class Rigidbody;
	class Transform;
	namespace Collision
	{
		class Collider;
	}
}

namespace Core::DataStructure
{

	class Behaviour;

	class GameObject
	{
	protected:
		std::vector<Component*>						m_components;
		Core::Event<Physics::Collision::Collider*>	m_collisionEvent;
		Resources::Scene*							m_scene;
		Physics::Transform*							m_transform;
		std::string									m_name;
		int											m_layer;
		bool										m_isStatic;

	public:
		GameObject();
		GameObject(const GameObject& object);
		GameObject(GameObject&& object);
		GameObject(Resources::Scene* scene, std::string name, const bool& isStatic = false);
		~GameObject();

		template<typename T>
		inline void    AddComponent()
		{
			T*  comp = new T(this);
			m_components.push_back(comp);
		}

		template<typename T>
		inline T*      GetComponent()
		{
			const std::type_info& type = typeid(T&);
			for (Component* comp : m_components)
			{
				if (comp->GetType() == type)
					return dynamic_cast<T*>(comp);
			}

			return nullptr;
		}

		template<typename T>
		inline std::vector<T*>      GetComponents()
		{
			std::vector<T*>	vecT;
			const std::type_info& type = typeid(T&);

			for (Component* comp : m_components)
			{
				if (comp->GetType() == type)
					vecT.push_back(dynamic_cast<T*>(comp));
			}

			return vecT;
		}

		template<typename T>
		inline void				RemoveComponents()
		{
			const std::type_info&	type{ typeid(T&) };
			for (unsigned i{ 0 }; i < m_components.size(); ++i)
			{
				if (m_components[i]->GetType() == type)
				{
					m_components[i]->EreaseEndOfFrame();
				}
			}
		}

		template<typename T>
		inline void				RemoveComponent(T* component)
		{
			for (auto it{ m_components.begin() }; it != m_components.end(); ++it)
			{
				if (*it == component)
				{
					(*it)->EreaseEndOfFrame();
					return;
				}
			}
		}

		void											AddBehaviour(Behaviour* Behaviour);
		void											AddChild(GameObject* object);
		void											AddChild(const GameObject* object);
		void											CleanDestroyedComponents();
		void											Collide(Physics::Collision::Collider* collider);
		void											Destroy();
		void											DrawGUI(SDL_Window*) const;
		inline std::list<Physics::Transform*>			GetChildrenTransform() { return m_transform->GetChilds(); }
		Core::Maths::Vec3								GetLocalPos();
		inline std::string								GetName() const { return m_name; }
		inline Core::Maths::Ref3						GetParentRef() { return m_transform->GetParentRef(); }
		Core::Maths::Ref3								GetRef();
		inline Resources::Scene*						GetScene() const { return m_scene; }
		inline Core::Maths::Vec3						GetStartFramePosition() const
															{ return m_transform->GetStartFramePosition(); }
		Core::Maths::Mat4								GetTransform();
		Core::Maths::Vec3								GetWorldPos();
		Core::Maths::Vec3								GetWorldScale();
		void											HasRigidbody(Physics::Rigidbody* rigidbody);
		void											HasCollider(Physics::Collision::Collider* collider);
		void											HaveToRender(LowRenderer::Mesh* mesh);
		inline bool										IsStatic() const { return m_isStatic; }
		inline bool&									IsStatic() { return m_isStatic; }
		inline int										Layer() const { return m_layer; }
		inline int&										Layer() { return m_layer; }
		void											RemoveBehaviour(Behaviour* Behaviour);
		void											RemoveCollider(Physics::Collision::Collider* collider);
		void											RemoveRenderer(LowRenderer::Mesh* mesh);
		void											RemoveRigidbody(Physics::Rigidbody* rigidbody);
		void											Rotate(const Core::Maths::Vec3& rot);
		void											SetPos(const Core::Maths::Vec3& pos);
		void											SetRotation(const Core::Maths::Vec3& rot);
		void											SetTransform(const Core::Maths::Mat4& mat);
		void											SetTransform(const Core::Maths::Vec3& pos,
																	const Core::Maths::Vec3& rot,
																	const Core::Maths::Vec3& scale);
		void											Translate(const Core::Maths::Vec3& pos);
	};
}


#endif //_GAMEOBJECT_H_
