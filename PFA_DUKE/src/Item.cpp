#include "Item.h"
#include "CollisionManager.h"
#include "Define.h"

using namespace Core::DataStructure;

namespace Game
{
	Item::Item(GameObject* object):
		Component(object),
		Behaviour(object)
	{}

	Item::~Item()
	{}

	const std::type_info&   Item::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component* Item::Copy() const
	{
		return new Item(*this);
	}

	void	Item::Start(Resources::ResourcesManager* rm)
	{
		m_playerObject = m_player->GetObject();
		m_cam = m_playerObject->GetComponent<LowRenderer::Camera>();
		m_sprite = m_object->GetComponent<Sprite>();
		m_sound = rm->AddSound("SoundGetItem", GETITEM);
	}

	void	Item::CreateItem(const ItemType& type, const Core::Maths::Vec3& pos,
							PlayerHandler* player, const int& value)
	{
		m_type = type;
		m_object->SetPos(pos);
		m_value = value;
		m_player = player;
	}

	void	Item::OnCollision(Physics::Collision::Collider* collider)
	{
		if (collider->GetObject()->Layer() == PLAYER_LAYER)
		{
			switch (m_type)
			{
			case ItemType::AMMO:
				if (m_player->CanPickAmmo())
					break;
				m_sound->Play(0.01, false);
				m_player->GiveAmmo(m_value);
				m_object->Destroy();
				break;
			case ItemType::HEAL:
				if (m_player->CanPickLife())
					break;
				m_sound->Play(0.01, false);
				m_player->SetHealth(-m_value);
				m_object->Destroy();
				break;
			default:
				break;
			}
		}
	}

	void	Item::Update(CRenderWindow* render, double frameTime)
	{
		float	angle = Core::Maths::AngleBetweenVector(Vec3::forward, Vec3(m_object->GetLocalPos(),
																				m_playerObject->GetLocalPos()));
		m_object->SetRotation(Vec3(0, angle, 0));
		m_sprite->Draw(m_object->GetTransform(), m_cam);
	}
}