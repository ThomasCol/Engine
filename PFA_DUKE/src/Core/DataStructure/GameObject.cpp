#include "GameObject.hpp"
#include "Scene.h"
#include "Behaviour.h"
#include "Collider.h"

using namespace Core::Maths;
using namespace Core;

namespace Core::DataStructure
{
    GameObject::GameObject()
    {}

    GameObject::GameObject(const GameObject& object):
		m_name {object.m_name},
		m_scene {object.m_scene},
		m_transform {new Physics::Transform(*object.m_transform)},
		m_layer {object.m_layer},
		m_isStatic {object.m_isStatic}
    {
        for (size_t i = 0; i < object.m_components.size(); i++)
        {
            m_components.push_back(object.m_components[i]->Copy());
        }
    }

    GameObject::GameObject(GameObject&& object):
		m_name {std::move(object.m_name)},
		m_scene {std::move(object.m_scene)},
		m_transform {std::move(object.m_transform)},
		m_isStatic {std::move(object.m_isStatic)},
		m_layer{ std::move(object.m_layer) },
		m_components {std::move(object.m_components)}
    {}

    GameObject::GameObject(Resources::Scene* scene, std::string name, const bool& isStatic):
		m_name {name},
		m_scene {scene},
		m_transform {new Physics::Transform(this)},
		m_layer {0},
		m_isStatic {isStatic}
    {
        m_components.push_back(m_transform);
    }

    GameObject::~GameObject()
    {
        for (size_t i = 0; i < m_components.size(); i++)
        {
			if (m_components[i] != nullptr)
			{
				m_components[i]->Destroy();
				delete m_components[i];
			}
        }
    }

    void    GameObject::AddChild(GameObject* object)
    {
        m_transform->AddChild(object->m_transform);
    }

    Mat4    GameObject::GetTransform()
    {
        return m_transform->GetTransform();
    }

    void    GameObject::HaveToRender(LowRenderer::Mesh* mesh)
    {
        m_scene->AddMeshToRender(mesh);
    }
    
    void    GameObject::AddBehaviour(Behaviour* behaviour)
    {
		m_collisionEvent += std::bind(&Behaviour::OnCollision, behaviour, ARG1);
        m_scene->AddBehaviour(behaviour);
    }

    void    GameObject::HasRigidbody(Physics::Rigidbody* rigidbody)
    {
        m_scene->AddRigidbody(rigidbody);
    }

    void    GameObject::HasCollider(Physics::Collision::Collider* collider)
    {
        m_scene->AddObjectToCollide(collider);
    }
	
	void    GameObject::RemoveRenderer(LowRenderer::Mesh* mesh)
    {
        m_scene->RemoveMeshToRender(mesh);
    }
    
    void    GameObject::RemoveBehaviour(Behaviour* behaviour)
    {
		m_collisionEvent -= std::bind(&Behaviour::OnCollision, behaviour, ARG1);
        m_scene->RemoveBehaviour(behaviour);
    }

    void    GameObject::RemoveRigidbody(Physics::Rigidbody* rigidbody)
    {
        m_scene->RemoveRigidbody(rigidbody);
    }

    void    GameObject::RemoveCollider(Physics::Collision::Collider* collider)
    {
        m_scene->RemoveObjectToCollide(collider);
    }

    void    GameObject::SetTransform(const Vec3& pos, const Vec3& rot, const Vec3& scale)
    {
        m_transform->Init(pos, rot, scale);
    }

    Vec3    GameObject::GetWorldPos()
    {
        return m_transform->GetWorldPos();
    }
    
    Vec3    GameObject::GetLocalPos()
    {
        return m_transform->GetLocalPos();
    }

    void    GameObject::SetPos(const Vec3& pos)
    {
        m_transform->SetPos(pos);
    }

	void	GameObject::SetRotation(const Vec3& rot)
	{
		m_transform->SetRotation(rot);
	}

    void    GameObject::Translate(const Vec3& translation)
    {
        m_transform->Translate(translation);
    }

	void	GameObject::Rotate(const Vec3& rotation)
	{
		m_transform->Rotate(rotation);
	}

    Ref3    GameObject::GetRef()
    {
        return m_transform->GetRef();
    }

    Vec3    GameObject::GetWorldScale()
    {
        return m_transform->GetWorldScale();
    }

	void	GameObject::SetTransform(const Mat4& mat)
	{
		m_transform->SetTransform(mat);
	}

	void	GameObject::Collide(Physics::Collision::Collider* collider)
	{
		m_collisionEvent(collider);
	}
    
    void    GameObject::DrawGUI(SDL_Window* w) const
    {
	    for (auto it {m_components.begin()}; it != m_components.end(); ++it)
		    (*it)->OnDrawGUI(w);
    }

	void	GameObject::CleanDestroyedComponents()
	{
		for (unsigned i{ 0 }; i < m_components.size(); ++i)
			if (m_components[i]->Isdestroyed())
			{
				m_components[i]->Destroy();
				delete m_components[i];
				m_components.erase(m_components.begin() + i--);
			}
	}

	void	GameObject::Destroy()
	{
		m_scene->DestroyObject(this);
	}
}
