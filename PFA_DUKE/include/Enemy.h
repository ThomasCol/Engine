#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Behaviour.h"
#include "PathFinder.h"


namespace	Resources
{
	class		Animation;
	class		ResourcesManager;
}

namespace Game
{
	class Enemy : public virtual Core::DataStructure::Behaviour
	{
	private:
		AI::Path*								m_path;
		std::list<AI::NavMeshNode*>::iterator	m_nodeIt;

		Core::DataStructure::GameObject*		m_player;
		Physics::Collision::Collider*			m_playerCollider;

		Resources::Animation*					m_animWalkForward;
		Resources::Animation*					m_animWalkBackWard;
		Resources::Animation*					m_animDead;

		Sound*									m_triggerSound;
		Sound*									m_deadSound;
		Sound*									m_takeDmgSound;

		Resources::ResourcesManager*			m_rm;

		LowRenderer::Camera*					m_cam;

		std::vector<Core::Maths::Vec3>			m_hitPoint;

		Core::Maths::Vec3						m_target;

		Core::Maths::Vec3						m_dir;

		Sprite*									m_sprite;

		double									m_elapsedTime;
		double									m_damageCD;

		int										m_life;
		int										m_damage;

		bool									m_dead;
		bool									m_trigger;
		bool									m_attack;
		bool									m_goBack;

	public:
		Enemy();
		Enemy(Core::DataStructure::GameObject* obj);
		Enemy(const Enemy& enemy);
		Enemy(Enemy&& enemy);
		~Enemy();

		const std::type_info&                   GetType() const override;
		Core::DataStructure::Component*         Copy() const override;

		void									Start(Resources::ResourcesManager* rm) override;
		void									Update(CRenderWindow* win, double frameTime) override;

		void									CheckDir(double frameTime);

		void									SetPlayer(Core::DataStructure::GameObject* player);
		void									SetCam(LowRenderer::Camera* cam) { m_cam = cam; }

		inline int								GetLife() { return m_life; }

		void									FindOrientation();
		void									CanSeePlayer();

		void									Hit(const int& damage);

		void									OnCollision(Physics::Collision::Collider* collider) override;
	};
}

#endif // !_ENEMY_H_