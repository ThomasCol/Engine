#include "Enemy.h"
#include "GameObject.hpp"
#include "Collider.h"
#include "PlayerHandler.h"
#include "CollisionManager.h"
#include "Animation.h"
#include "Define.h"
#include "Transform.h"
#include "Scene.h"
#include "Mesh.h"

using namespace Core::DataStructure;
using namespace Physics::Collision;

namespace Game
{
	Enemy::Enemy():
		Component(),
		Behaviour()
	{}

	Enemy::Enemy(GameObject* obj):
		Component(obj),
		Behaviour(obj)
	{

	}

	Enemy::Enemy(const Enemy& enemy):
		Component(enemy.m_object),
		Behaviour(enemy.m_object),
		m_path {enemy.m_path}
	{}

	Enemy::Enemy(Enemy&& enemy):
		Component(std::move(enemy.m_object)),
		Behaviour(std::move(enemy.m_object)),
		m_path {std::move(enemy.m_path)}
	{}

	Enemy::~Enemy()
	{
		delete m_path;
		delete m_animDead;
		delete m_animWalkBackWard;
		delete m_animWalkForward;
	}

	const std::type_info&   Enemy::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component* Enemy::Copy() const
	{
		return new Enemy(*this);
	}

	void	Enemy::Start(Resources::ResourcesManager* rm)
	{
		m_rm = rm;
		m_path = new AI::Path(AI::PathFinder::Instance()->FindPath(m_object->GetLocalPos(), Vec3(m_object->GetLocalPos() + Vec3(1,0,1))));
		m_nodeIt = m_path->nodes.begin();
		m_target = (*m_nodeIt)->pos;

		m_dir = Vec3(m_object->GetLocalPos(), (*m_nodeIt)->pos).Normalize();

		m_goBack = false;
		m_attack = false;

		m_object->Layer() = ENEMY_LAYER;

		m_sprite = m_object->GetComponent<Sprite>();

		if (m_object->GetScene()->SecretMode())
		{
			m_animWalkForward = new Resources::Animation(std::string("resources/media/Sprites/SpriteEnemy/Stroopers"), 3);
			m_animDead = new Resources::Animation(std::string("resources/media/Sprites/SpriteEnemy/StroopersDeath"), 4);
		}
		else
		{
			m_animDead = new Resources::Animation(std::string(DEADE), 4);
			m_animWalkForward = new Resources::Animation(std::string(FORWARDWALKE), 3);
		}
		m_animWalkForward->SetSpeed(0.3f);
		m_animWalkBackWard = new Resources::Animation(std::string(BACKWARDWALKE), 3);
		m_animWalkBackWard->SetSpeed(0.3f);
		m_animDead->SetSpeed(0.2f);
		m_triggerSound = rm->AddSound("TriggerEnemy", TRIGGERENEMY);
		m_deadSound = rm->AddSound("DieEnemy", DIEENEMY);
		m_takeDmgSound = rm->AddSound("TakeDmgEnemy", DMGENEMY);
		m_trigger = m_dead = false;
		m_life = ENEMY_LIFE;
		m_damage = ENEMY_DAMAGE;
		m_elapsedTime = 0.0;
		m_damageCD = DAMAGE_CD;

		m_playerCollider = m_player->GetComponent<Physics::Collision::SphereCollider>();
	}

	void	Enemy::CheckDir(double frameTime)
	{
		if (m_dir.Dot(m_player->GetLocalPos() - m_object->GetLocalPos()) > 0)
		{
			m_animWalkBackWard->SetImgFrame(0);
			m_sprite->Draw(m_object->GetTransform(), m_cam);
			m_sprite->SetTexture(m_animWalkForward->SetFrameLoop(m_rm, frameTime));
		}
		else
		{
			m_animWalkForward->SetImgFrame(0);
			m_sprite->Draw(m_object->GetTransform(), m_cam);
			m_sprite->SetTexture(m_animWalkBackWard->SetFrameLoop(m_rm, frameTime));
		}

	}

	void	Enemy::Update(CRenderWindow* win, double frameTime)
	{
		m_elapsedTime += frameTime;

		if (m_life > 0)
		{
			if (abs(m_target.x - m_object->GetWorldPos().x) < 0.1f && abs(m_target.z - m_object->GetWorldPos().z) < 0.1f)
			{
				if (!m_goBack)
				{
					m_nodeIt++;
					if (m_nodeIt == --m_path->nodes.end())
						m_goBack = true;
					m_target = (*m_nodeIt)->pos;
				}
				else
				{
					m_nodeIt--;
					if (m_nodeIt == m_path->nodes.begin())
						m_goBack = false;
					m_target = (*m_nodeIt)->pos;
				}
				
				m_dir = Vec3(m_object->GetLocalPos(), m_target).Normalize();
			}

			if (!m_attack)
				m_object->Translate(m_dir * (float)frameTime);

			CheckDir(frameTime);
		}
		else
		{
			m_sprite->Draw(m_object->GetTransform(), m_cam);
			m_sprite->SetTexture(m_animDead->SetFrameLoop(m_rm, frameTime, false));
			if (!m_dead)
			{
				std::list<Physics::Transform*> list = m_object->GetChildrenTransform();
				for (Physics::Transform* transform : list)
					transform->GetObject()->GetComponent<LowRenderer::Mesh>()->SetActive(false);
				m_object->GetComponent<Physics::Collision::BoxCollider>()->SetActive(false);
				m_dead = true;
				m_deadSound->Play(frameTime, false);
			}
		}

		FindOrientation();

		CanSeePlayer();

		m_attack = false;
	}

	void	Enemy::SetPlayer(Core::DataStructure::GameObject* player)
	{
		m_player = player;
	}

	void	Enemy::FindOrientation()
	{
		float	angle = Core::Maths::AngleBetweenVector(Vec3::forward, Vec3(m_object->GetLocalPos(), m_player->GetLocalPos()));
		m_object->SetRotation(Vec3(0, angle, 0));

	}

	void	Enemy::CanSeePlayer()
	{
		Vec3 enemyPlayer = Vec3(m_object->GetWorldPos(), m_playerCollider->Position());
		if (enemyPlayer.SqrLength() > ENEMY_VIEW)
			return;
		RaycastHit hit;
		if (CollisionManager::Raycast(m_object->GetWorldPos(), Vec3(m_object->GetWorldPos(), m_playerCollider->Position()), 10, hit))
		{
			if (hit.Collider()->GetObject()->Layer() == PLAYER_LAYER)
			{
				if (!m_trigger && m_life > 0)
				{
					m_triggerSound->Play(0.02, false);
					m_trigger = true;
				}
				m_target.x = m_player->GetWorldPos().x;
				m_target.z = m_player->GetWorldPos().z;
			}
		}
		else
		{
			if (m_trigger)
				m_trigger = false;
			m_target = (*m_nodeIt)->pos;
		}
		m_dir = Vec3(m_object->GetLocalPos(), m_target).Normalize();
	}

	void	Enemy::Hit(const int& damage)
	{
		m_life -= damage;
		if (m_life > 0)
			m_takeDmgSound->Play(0.02, false);
	}

	void	Enemy::OnCollision(Physics::Collision::Collider* collider)
	{
		if (collider->GetObject()->Layer() == PLAYER_LAYER)
		{
			PlayerHandler* player = m_player->GetComponent<PlayerHandler>();
			if (m_elapsedTime >= m_damageCD)
			{
				player->SetHealth(m_damage);
				m_elapsedTime = 0;
			}
			m_attack = true;
		}
	}
}