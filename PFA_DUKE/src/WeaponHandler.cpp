#include "WeaponHandler.h"
#include "Animation.h"
#include "RenderWindow.h"
#include "ResourcesManager.h"
#include <string>


WeaponHandler::WeaponHandler(Resources::ResourcesManager* rm, int id, Sprite* wSprite, Sprite* hSprite)
{
	if (id == 0)
		InitGun(rm, wSprite, hSprite);
	else if (id == 1)
		InitShotGun(rm, wSprite, hSprite);
	else if (id == 2)
		InitRifle(rm, wSprite, hSprite);
	else if (id == 3)
		InitRocket(rm, wSprite, hSprite);
	else if (id == 4)
		InitRpg(rm, wSprite, hSprite);
	m_ammo = m_ammoMax;
	m_elapsedTime = 0;
}

WeaponHandler::WeaponHandler(const WeaponHandler& w)
{
	m_sprite = w.m_sprite;
	m_ammoMax = w.m_ammoMax;
	m_ammo = w.m_ammo;
	m_elapsedTime = w.m_elapsedTime;
	m_fireSound = w.m_fireSound;
	m_changeSound = w.m_changeSound;
	m_currentPos = w.m_currentPos;
	m_pos = w.m_pos;
	m_anim = w.m_anim;
}

WeaponHandler::~WeaponHandler()
{
	delete m_anim;
}

void		WeaponHandler::InitGun(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite)
{
	m_ammoMax = AMMOGUN;
	m_useAmmo = GUN_AMMO_USED;
	m_dmg = GUN_DMG;
	m_impactSize = GUN_IMPACT_SIZE;
	m_anim = new Resources::Animation(std::string(GUN), 2);
	m_anim->SetSpeed(0.1f);
	m_sprite = wSprite;
	rm->CreateSprite(m_sprite, "resources/obj/face.obj",
                        (std::string(GUN) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");

	m_hudSprite = hSprite;
	rm->CreateSprite(m_hudSprite, "resources/obj/face.obj",
                        (std::string(HUDGUN) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite->SetPos(Core::Maths::Vec3(0.8f,-0.7f,0.f));
	m_hudSprite->SetScale(Core::Maths::Vec3(0.2f, 0.2f, 0.f));

	m_pos = Core::Maths::Vec3(0.2f, -0.85f, 0.f);
	m_currentPos = m_pos;
	m_sprite->SetPos(m_currentPos);
	m_sprite->SetScale(Core::Maths::Vec3(0.3f, 0.3f, 0.3f));
	m_fireSound = rm->AddSound("SoundGun", SOUNDGUN);
	m_fireSound->SetSpeed(10.f); 
	m_changeSound = NULL;
}

void		WeaponHandler::InitShotGun(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite)
{
	m_ammoMax = AMMOSHOTGUN;
	m_useAmmo = SHOTGUN_AMMO_USED;
	m_dmg = SHOTGUN_DMG;
	m_impactSize = SHOTGUN_IMPACT_SIZE;
	m_anim = new Resources::Animation(std::string(SHOTGUN), 4);
	m_anim->SetSpeed(0.2f);
	m_sprite = wSprite;
	rm->CreateSprite(m_sprite, "resources/obj/face.obj",
                        (std::string(SHOTGUN) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite = hSprite;
	rm->CreateSprite(m_hudSprite, "resources/obj/face.obj",
                        (std::string(HUDSHOTGUN) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite->SetPos(Core::Maths::Vec3(0.8f, -0.7f, 0.f));
	m_hudSprite->SetScale(Core::Maths::Vec3(0.2f, 0.2f, 0.f));
	m_pos = Core::Maths::Vec3(0.2f, -0.85f, 0.f);
	m_currentPos = m_pos;
	m_currentPos.y = m_pos.y - 1.f;
	m_sprite->SetPos(m_currentPos);
	m_sprite->SetScale(Core::Maths::Vec3(0.4f, 0.4f, 0.4f));
	m_fireSound = rm->AddSound("SoundShotGun", SOUNDSHOTGUN);
	m_fireSound->SetSpeed(0.8f); 
	m_changeSound = rm->AddSound("ChangeShotGun", CHANGESHOTGUN);
}

void		WeaponHandler::InitRifle(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite)
{
	m_ammoMax = AMMORIFLE;
	m_useAmmo = RIFLE_AMMO_USED;
	m_dmg = RIFLE_DMG;
	m_impactSize = RIFLE_IMPACT_SIZE;
	m_anim = new Resources::Animation(std::string(RIFLE), 4);
	m_anim->SetSpeed(0.05f);
	m_sprite = wSprite;
	rm->CreateSprite(m_sprite, "resources/obj/face.obj",
                        (std::string(RIFLE) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite = hSprite;
	rm->CreateSprite(m_hudSprite, "resources/obj/face.obj",
                        (std::string(HUDRIFLE) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite->SetPos(Core::Maths::Vec3(0.8f, -0.7f, 0.f));
	m_hudSprite->SetScale(Core::Maths::Vec3(0.2f, 0.2f, 0.f));
	m_pos = Core::Maths::Vec3(0.2f, -0.65f, 0.f);
	m_currentPos = m_pos;
	m_currentPos.y = m_pos.y - 1.f;
	m_sprite->SetPos(m_currentPos);
	m_sprite->SetScale(Core::Maths::Vec3(0.8f, 0.8f, 0.8f));
	m_fireSound = rm->AddSound("SoundRifle", SOUNDRIFLE);
	m_fireSound->SetSpeed(1.f); 
	m_changeSound = rm->AddSound("ChangeRifle", CHANGERIFLE);
}

void		WeaponHandler::InitRocket(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite)
{
	m_ammoMax = AMMOROCKET;
	m_useAmmo = ROCKET_AMMO_USED;
	m_dmg = ROCKET_DMG;
	m_impactSize = ROCKET_IMPACT_SIZE;
	m_anim = new Resources::Animation(std::string(ROCKET), 2);
	m_anim->SetSpeed(0.15f);
	m_sprite = wSprite;
	rm->CreateSprite(m_sprite, "resources/obj/face.obj",
                        (std::string(ROCKET) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite = hSprite;
	rm->CreateSprite(m_hudSprite, "resources/obj/face.obj",
                        (std::string(HUDROCKET) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite->SetPos(Core::Maths::Vec3(0.8f, -0.7f, 0.f));
	m_hudSprite->SetScale(Core::Maths::Vec3(0.2f, 0.2f, 0.f));
	m_pos = Core::Maths::Vec3(0.f, -0.75f, 0.f);
	m_currentPos = m_pos;
	m_currentPos.y = m_pos.y - 1.f;
	m_sprite->SetPos(m_currentPos);
	m_sprite->SetScale(Core::Maths::Vec3(0.8f, 0.5f, 0.5f));
	m_fireSound = rm->AddSound("SoundRocket", SOUNDROCKET);
	m_fireSound->SetSpeed(1.f); 
	m_changeSound = rm->AddSound("ChangeRifle", CHANGEROCKET);
}

void		WeaponHandler::InitRpg(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite)
{
	m_ammoMax = AMMORPG;
	m_useAmmo = RPG_AMMO_USED;
	m_dmg = RPG_DMG;
	m_impactSize = RPG_IMPACT_SIZE;
	m_anim = new Resources::Animation(std::string(RPG), 2);
	m_anim->SetSpeed(0.2f);
	m_sprite = wSprite;
	rm->CreateSprite(m_sprite, "resources/obj/face.obj",
                        (std::string(RPG) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite = hSprite;
	rm->CreateSprite(m_hudSprite, "resources/obj/face.obj",
                        (std::string(HUDRPG) + std::string(PNG)).c_str(),
                        "resources/shader/vertexHUD.vsh",
                        "resources/shader/fragmentHUD.fsh");
	m_hudSprite->SetPos(Core::Maths::Vec3(0.8f, -0.7f, 0.f));
	m_hudSprite->SetScale(Core::Maths::Vec3(0.2f, 0.2f, 0.f));
	m_pos = Core::Maths::Vec3(0.2f, -0.75f, 0.f);
	m_currentPos = m_pos;
	m_currentPos.y = m_pos.y - 1.f;
	m_sprite->SetPos(m_currentPos);
	m_sprite->SetScale(Core::Maths::Vec3(0.8f, 0.5f, 0.5f));
	m_fireSound = rm->AddSound("SoundRocket", SOUNDRPG);
	m_fireSound->SetSpeed(0.5f); 
	m_changeSound = rm->AddSound("ChangeRifle", CHANGERPG);
}

void		WeaponHandler::GiveAmmo(int ammo)
{
	m_ammo += ammo;
	if (m_ammo > m_ammoMax)
		m_ammo = m_ammoMax;
}

void		WeaponHandler::RemoveW(bool& change, bool& take)
{
	if (m_currentPos.y < m_pos.y - 1)
	{
		m_currentPos.y = m_pos.y - 1;
		change = false;
		take = true;
	}
	else
		m_currentPos.y -= 0.1f;
}

void		WeaponHandler::TakeW(bool& take, double frameTime)
{
	if (m_currentPos.y > m_pos.y)
        {
                m_currentPos.y = m_pos.y;
				if (m_changeSound != NULL && take)
					m_changeSound->Play(frameTime, false);
                take = false;
        }
        else
		m_currentPos.y += 0.1f;
}

void		WeaponHandler::ChangeWeapon(double frameTime, bool& change, bool& take)
{
	m_elapsedTime += frameTime;
	if (m_elapsedTime > 0.04f)
	{
		if (change)
		{
			RemoveW(change, take);
		}
		else if (take)
		{
			TakeW(take, frameTime);
		}
		m_sprite->SetPos(m_currentPos);
		m_elapsedTime = 0;
	}
}

void		WeaponHandler::Fire(Resources::ResourcesManager* rm, double frameTime, bool& fire, bool& doFire, bool repeat)
{
	m_sprite->SetTexture(m_anim->SetFrameLoopFire(rm, frameTime, fire));
	if (m_fireSound != NULL && doFire && repeat)
	{
		m_fireSound->Play(frameTime, repeat);
		//doFire = false;
	}
	else if (m_fireSound != NULL && doFire && !repeat)
	{
		m_fireSound->Play(frameTime, repeat);
		doFire = false;
	}
}

void            WeaponHandler::Idle(Resources::ResourcesManager* rm)
{
	m_sprite->SetTexture(m_anim->SetFrame(rm));
}

void		WeaponHandler::Update()
{
	m_sprite->Draw();
}
