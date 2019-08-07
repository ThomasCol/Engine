#include "PlayerHandler.h"
#include "RenderWindow.h"
#include "Camera.h"
#include "Sprites.h"
#include "Sound.h"
#include "Mesh.h"
#include "Animation.h"
#include "Debug.h"
#include "RaycastHit.h"
#include "HUD.h"
#include "Canvas.hpp"
#include "Enemy.h"
#include "Scene.h"
#include "OrientedBox.h"
#include <SDL2/SDL.h>

using namespace Physics::Collision;
using namespace Core::Maths::Geometry;

PlayerHandler::PlayerHandler()
{
	for (int i = 0; i < NBWEAPON; i++)
		m_weapons[i] = new WeaponHandler();
	m_hp = PLAYER_LIFE_MAX;
	m_idx = 0;
	m_fire = false;
}

PlayerHandler::PlayerHandler(Core::DataStructure::GameObject* object):
	Component(object), Behaviour(object)
{
}

PlayerHandler::PlayerHandler(int hp)
{
	m_hp = hp;
	for (int i = 0; i < NBWEAPON; i++)
		m_weapons[i] = new WeaponHandler();
	m_idx = m_currentIdx = 0;
	m_fire = false;
	m_change = m_take = false;
}

PlayerHandler::PlayerHandler(const PlayerHandler& p) :
	Component(p.m_object), Behaviour(p.m_object)
{
	m_hp = p.m_hp;
	for (int i = 0; i < NBWEAPON; i++)
		m_weapons[i] = p.m_weapons[i];
	m_idx = p.m_idx;
	m_currentIdx = p.m_currentIdx;
	m_fire = p.m_fire;
	m_change = p.m_change;
	m_take = p.m_take;
	m_cam = p.m_cam;
}

PlayerHandler::~PlayerHandler()
{
	for (unsigned int i = 0; i < 5; i++)
		delete m_weapons[i];
	delete m_endPunch;
	delete m_hud;
}

const std::type_info&   PlayerHandler::GetType() const
{
	return typeid(*this);
}

Core::DataStructure::Component* PlayerHandler::Copy() const
{
	return new PlayerHandler(*this);
}

void		PlayerHandler::Start(Resources::ResourcesManager* rm)
{
	m_fire = m_lockFire = false;
	m_rm = rm;
	m_hp = PLAYER_LIFE_MAX;
	m_doFire = m_change = m_take = m_crouch = m_canCrouch = m_isJumping = false;
	m_canJump = true;

	m_object->Layer() = PLAYER_LAYER;

	m_crossHair->SetPos(Vec3(0, 0, 0));
	m_crossHair->SetScale(Vec3(0.05f,0.1f,0.1f));

	InitWeapon(rm);
	EndPunch(rm);

	m_headShotSound = rm->AddSound("headShot", HEADSHOT);
	m_coolSound = rm->AddSound("cool", COOLSOUND);
	m_dmgSound = rm->AddSound("dmg", DMGSOUND);

	InitHealthSprite(rm);

	m_idx = m_currentIdx = 0;
	m_elapsedTime = 0;
	InitCamera();

	InitHud(rm);
}

void		PlayerHandler::InitHud(Resources::ResourcesManager* rm)
{
	Core::DataStructure::GameObject* obj{ m_object->GetScene()->CreateGameObject("Canvas") };
	obj->AddComponent<UI::Canvas>();
	UI::Canvas* canvas = obj->GetComponent<UI::Canvas>();

	m_hud = new HUD(this);
	m_hud->Init(rm, canvas);
}

void		PlayerHandler::EndPunch(Resources::ResourcesManager* rm)
{
	m_endLevel = m_object->GetScene()->GetGameObject("w1")->GetLocalPos();
	m_doEndAnim = false;
	m_object->AddComponent<Sprite>();
        std::vector<Sprite*>    sprite;

        sprite = m_object->GetComponents<Sprite>();
        m_endP = sprite.back();
        rm->CreateSprite(m_endP, "resources/obj/face.obj",
                        (std::string(ENDP) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
        m_endP->SetPos({0.f,-0.6f,0.f});
        m_endP->SetScale(Core::Maths::Vec3(0.5f,0.5f,0.5f));
        m_endPunch = new Resources::Animation(std::string(ENDP), 3);
        m_endPunch->SetSpeed(0.25f);
		m_endSound = m_rm->AddSound("SoundEndPOC", POC); 
		m_playPoc = true;
}

void		PlayerHandler::InitWeapon(Resources::ResourcesManager* rm)
{
	for (int i = 0; i < NBWEAPON; i++)
	{
		std::vector<Sprite*>    sprite;
		m_object->AddComponent<Sprite>();
		m_object->AddComponent<Sprite>();

		sprite = m_object->GetComponents<Sprite>();
		m_weapons[i] = new WeaponHandler(rm, i, sprite[i*2], sprite[(i*2) + 1]);
	}
}

void		PlayerHandler::InitCamera()
{
	m_cam = GetObject()->GetComponent<LowRenderer::Camera>();
	m_cam->Init({2,3,2}, Core::Maths::Vec3::up);
}

void PlayerHandler::SetHealth(int health)
{
	m_hp -= health;

	if (health > 0)
		m_dmgSound->Play(0, false);
	else
		m_coolSound->Play(0, false);

	if (m_hp < 0)
		m_hp = 0;

	if (m_hp > PLAYER_LIFE_MAX)
		m_hp = PLAYER_LIFE_MAX;
}

void		PlayerHandler::InitHealthSprite(Resources::ResourcesManager* rm)
{
	std::vector<Sprite*>    sprite;
	m_object->AddComponent<Sprite>();
	sprite = m_object->GetComponents<Sprite>();

	m_healthSprite = sprite.back();
	rm->CreateSprite(m_healthSprite, "resources/obj/face.obj",
			(std::string(HEALTH) + std::string(PNG)).c_str(),
			"resources/shader/vertexHUD.vsh",
			"resources/shader/fragmentHUD.fsh");
	m_healthSprite->SetPos(Core::Maths::Vec3(-0.85f,-0.9f,0.f));
	m_healthSprite->SetScale(Core::Maths::Vec3(0.15f,0.15f,0.f));
}

void		PlayerHandler::ChangeIdWeapon(int id)
{
	if (m_idx != id)
	{
		m_idx = id;
		m_change = true;
	}
}

void		PlayerHandler::ChangeWeapon(CRenderWindow* render)
{
	if (render->m_input->IsDoAction(Action::WEAPON1))
		ChangeIdWeapon(0);
	if (render->m_input->IsDoAction(Action::WEAPON2))
		ChangeIdWeapon(1);
	if (render->m_input->IsDoAction(Action::WEAPON3))
		ChangeIdWeapon(2);
	if (render->m_input->IsDoAction(Action::WEAPON4))
		ChangeIdWeapon(3);
	if (render->m_input->IsDoAction(Action::WEAPON5))
		ChangeIdWeapon(4);
	if (render->m_input->GetDownWheel())
	{
		m_idx += 1;
		if (m_idx > 4)
			m_idx = 0;
		render->m_input->SetDownWheel(false);
		m_change = true;
	}
	else if (render->m_input->GetUpWheel())
	{
		m_idx -= 1;
		if (m_idx < 0)
			m_idx = 4;
		render->m_input->SetUpWheel(false);
		m_change = true;
	}
}

void		PlayerHandler::Fire(CRenderWindow* render, double frameTime)
{
	if ((SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0 && (!m_change || !m_take)
		&& !m_object->GetScene()->IsMenu())
	{
		if (m_weapons[m_currentIdx]->GetAmmo() > 0)
		{
			if (!m_fire)
			{
				m_fire = true;
				if (!m_doFire)
				{
					m_doFire = true;
					RaycastHit	hit;

					if (CollisionManager::Raycast(m_object->GetLocalPos(), m_cam->GetView(), 1000.f, hit))
					{
						Core::DataStructure::GameObject* hitSprite{ m_object->GetScene()->CreateGameObject("bullet") };
						LowRenderer::Mesh* mesh;
						hitSprite->AddComponent<LowRenderer::Mesh>();
						mesh = hitSprite->GetComponent<LowRenderer::Mesh>();

						if (hit.Collider()->GetObject()->Layer() == ENEMY_LAYER)
						{
							Game::Enemy* enemy {hit.Collider()->GetObject()->GetComponent<Game::Enemy>()};
							
							if (hit.Position().y >= 2.8f)
							{
								enemy->Hit(GetWeapon()->GetDmg() * 2);
								m_headShotSound->Play(frameTime, false);
							}
							else
								enemy->Hit(GetWeapon()->GetDmg());

							OrientedBox	ob((BoxCollider*)hit.Collider());

							Vec3	local;
							local.x = (hit.Position() - hit.Collider()->Position()).Dot(ob.I());
							local.y = (hit.Position() - hit.Collider()->Position()).Dot(ob.J());
							local.z = (hit.Position() - hit.Collider()->Position()).Dot(ob.K());

							if (m_object->GetScene()->SecretMode())
							{
								m_object->GetScene()->GetResourcesManager()->CreateMesh(mesh, "resources/obj/Hit.obj",
									"resources/media/Sprites/Sbullet_enemy.png",
									"resources/shader/vertexEnemy.vsh",
									"resources/shader/fragmentHUD.fsh");
							}
							else
							{
								m_object->GetScene()->GetResourcesManager()->CreateMesh(mesh, "resources/obj/Hit.obj",
									"resources/media/Sprites/bullet_enemy.png",
									"resources/shader/vertexEnemy.vsh",
									"resources/shader/fragmentHUD.fsh");
							}

							hitSprite->SetTransform(local, Vec3(0.f), Vec3((float)m_weapons[m_currentIdx]->GetImpactSize()));
							m_object->GetScene()->AddChildToGameObject(hitSprite, hit.Collider()->GetName());
						}
						else
						{
							if (m_object->GetScene()->SecretMode())
							{
								m_object->GetScene()->GetResourcesManager()->CreateMesh(mesh, "resources/obj/Hit.obj",
									"resources/media/Sprites/Sbullet.png",
									"resources/shader/vertexEnemy.vsh",
									"resources/shader/fragmentHUD.fsh");
							}
							else
							{
								m_object->GetScene()->GetResourcesManager()->CreateMesh(mesh, "resources/obj/Hit.obj",
									"resources/media/Sprites/bullet.png",
									"resources/shader/vertexEnemy.vsh",
									"resources/shader/fragmentHUD.fsh");
							}

							m_object->GetScene()->AddChildToGameObject(hitSprite, "SkyBox");
							if (hit.Normal() == Vec3(0.f, 1.f, 0.f))
								hitSprite->SetTransform(hit.Position(), Vec3((float)(-M_PI * 0.5f), 0.f, 0.f), Vec3((float)m_weapons[m_currentIdx]->GetImpactSize()));
							else if (hit.Normal() == Vec3(0.f, -1.f, 0.f))
								hitSprite->SetTransform(hit.Position(), Vec3((float)(M_PI * 0.5f), 0.f, 0.f), Vec3((float)m_weapons[m_currentIdx]->GetImpactSize()));
							else if (hit.Normal() == Vec3(1.f, 0.f, 0.f))
								hitSprite->SetTransform(hit.Position(), Vec3(0.f, (float)(M_PI * 0.5f), 0.f), Vec3((float)m_weapons[m_currentIdx]->GetImpactSize()));
							else if (hit.Normal() == Vec3(-1.f, 0.f, 0.f))
								hitSprite->SetTransform(hit.Position(), Vec3(0.f, (float)(-M_PI * 0.5f), 0.f), Vec3((float)m_weapons[m_currentIdx]->GetImpactSize()));
							else if (hit.Normal() == Vec3(0.f, 0.f, -1.f))
								hitSprite->SetTransform(hit.Position(), Vec3(0.f, (float)M_PI, 0.f), Vec3((float)m_weapons[m_currentIdx]->GetImpactSize()));
							else
								hitSprite->SetTransform(hit.Position(), Vec3(0.f), Vec3((float)m_weapons[m_currentIdx]->GetImpactSize()));
						}
					}
				}
				m_weapons[m_currentIdx]->GiveAmmo(-m_weapons[m_currentIdx]->GetUseAmmo());
			}

		}
		else
			m_doFire = false;

	}
	else
		m_doFire = false;

	m_weapons[m_currentIdx]->ChangeWeapon(frameTime, m_change, m_take);
	if (m_take && !m_change)
		m_currentIdx = m_idx;
	m_weapons[m_currentIdx]->Fire(render->GetSimulation().GetManager(), frameTime, m_fire, m_doFire, false);
}

void		PlayerHandler::WeaponInput(CRenderWindow* render, double frameTime)
{
	if (!m_doEndAnim)
	{
		ChangeWeapon(render);
		Fire(render, frameTime);
		m_weapons[m_currentIdx]->Update();
	}
}

void            PlayerHandler::TranslatePos(float trans, float timer)
{
	float   camSpeed = 0.f;
	Vec3    pos = GetObject()->GetLocalPos();

	if (m_crouch)
		camSpeed = CROUCHSPEED * timer;
	else
		camSpeed = NORMALSPEED * timer;

	if (trans > 0)
		pos += m_cam->GetFront() * camSpeed;
	else
		pos -= m_cam->GetFront() * camSpeed;
	GetObject()->SetPos(pos);
}

void            PlayerHandler::Strafe(float strafe, double timer)
{
	float   camSpeed = 0.f;
	Vec3 front(m_cam->GetView());
	Vec3 tmp = front.Cross(m_cam->GetUp());
	Vec3 pos = GetObject()->GetLocalPos();

	if (m_crouch)
		camSpeed = CROUCHSPEED * (float)timer;
	else
		camSpeed = NORMALSPEED * (float)timer;

	tmp.Normalize();
	if (strafe > 0)
		pos += tmp * camSpeed;
	else
		pos -= tmp * camSpeed;
	GetObject()->SetPos(pos);
}

void		PlayerHandler::Move(CRenderWindow* render, double frameTime)
{
	float speed{ PLAYER_SPEED * (float)frameTime };
	Vec3 front = m_cam->GetFront();
	Vec3 right = front.Cross(m_cam->GetUp());
	Rigidbody* rb = m_object->GetComponent<Rigidbody>();
	m_cam->ChangeView(render, frameTime);
	if (render->m_input->IsDoAction(Action::FORWARD))
		rb->AddForce(front * speed);
	if (render->m_input->IsDoAction(Action::BACKWARD))
		rb->AddForce(-front * speed);
	if (render->m_input->IsDoAction(Action::LEFT))
		rb->AddForce(-right * speed);
	if (render->m_input->IsDoAction(Action::RIGHT))
		rb->AddForce(right * speed);

	Vec3 velocity = rb->Velocity();
	Vec2 vel2D(velocity.x, velocity.z);

	if (vel2D.SqrLength() > 25.f)
	{
		vel2D = vel2D.Normalize() * 5.f;
		velocity.x = vel2D.x;
		velocity.z = vel2D.y;
		rb->Velocity() = velocity;
	}

	if (m_canJump && render->m_input->IsDoAction(Action::JUMP))
	{
		m_canJump = false;
		rb->AddForce(Vec3(0, 15.f, 0));
	}
}

void		PlayerHandler::ChangePosPunch()
{
	if (m_endPunch->GetFrame() == 0)
	{
		m_endP->SetPos({0.f,-0.75f,0.f});
		m_endP->SetScale(Core::Maths::Vec3(0.5f,0.5f,0.5f));
	}
	else if (m_endPunch->GetFrame() == 1)
	{
		m_endP->SetPos({0.05f,-0.8f,0.f});
		m_endP->SetScale({0.45f,0.45f,0.45f});
	}
	else if (m_endPunch->GetFrame() == 2)
	{
		m_endP->SetPos({0.1f,-0.85f,0.f});
		m_endP->SetScale({0.4f,0.4f,0.4f});
	}
	else if (m_endPunch->GetFrame() == 3)
	{
		m_endP->SetPos({0.15f,-0.9f,0.f});
		m_endP->SetScale({0.35f,0.35f,0.35f});
	}
}

void		PlayerHandler::Action(CRenderWindow* render, double frameTime)
{
	if (render->m_input->IsDoAction(Action::USE))
	{
		if ((m_endLevel - m_object->GetLocalPos()).Length() < 2)
		{
			if (m_playPoc)
			{
				m_playPoc = false;
				m_endSound->Play(frameTime, false);
			}
			m_doEndAnim = true;
		}
	}
	if (m_doEndAnim)
	{
		ChangePosPunch();
		m_endP->Draw();
		m_endP->SetTexture(m_endPunch->SetFrameLoop(m_rm, frameTime, false));
	}
}

void		PlayerHandler::Update(CRenderWindow* render, double frameTime)
{
	Action(render, frameTime);
	WeaponInput(render, frameTime);
	Move(render, frameTime);
	m_crossHair->Draw();
	m_cam->UpdateCamera(m_object->GetWorldPos());
	m_hud->Update(render, this);
}

void		PlayerHandler::operator=(const PlayerHandler& p)
{
	m_hp = p.m_hp;
	for (int i = 0; i < NBWEAPON; i++)
		m_weapons[i] = p.m_weapons[i];
	m_idx = p.m_idx;
	m_currentIdx = p.m_currentIdx;
	m_fire = p.m_fire;
	m_change = p.m_change;
	m_take = p.m_take;
	m_cam = p.m_cam;
}

void	PlayerHandler::OnCollision(Physics::Collision::Collider* collider)
{
	if (collider->GetObject()->Layer() == GROUND_LAYER)
		m_canJump = true;
}

bool	PlayerHandler::CanPickAmmo()
{
	return GetWeapon()->GetAmmo() == GetWeapon()->GetMaxAmmo();
}

bool	PlayerHandler::CanPickLife()
{
	return m_hp == PLAYER_LIFE_MAX;
}
