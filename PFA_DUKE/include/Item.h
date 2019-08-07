#ifndef _ITEM_H_
#define _ITEM_H_

#include "Behaviour.h"
#include "PlayerHandler.h"

namespace Game
{
	enum class ItemType
	{
		HEAL = 0,
		AMMO,
		DEFAULT 
	};

	class Item : public Core::DataStructure::Behaviour
	{
	private:
		ItemType							m_type;
		LowRenderer::Camera*				m_cam;
		Core::DataStructure::GameObject*	m_playerObject;
		PlayerHandler*						m_player;
		Sprite*								m_sprite;
		Sound*								m_sound;
		int									m_value;

	public:
		Item() = default;
		Item(Core::DataStructure::GameObject* object);
		Item(const Item& item) = default;
		Item(Item&& item) = default;
		~Item();

		const std::type_info&                   GetType() const override;
		Core::DataStructure::Component*         Copy() const override;

		void									Start(Resources::ResourcesManager* rm) override;

		void									OnCollision(Physics::Collision::Collider* collider) override;

		void									CreateItem(const ItemType& type, const Core::Maths::Vec3& pos,
												PlayerHandler* player, const int& value);

		void									Update(CRenderWindow* render, double frameTime) override;
	};
}

#endif // !_ITEM_H_

