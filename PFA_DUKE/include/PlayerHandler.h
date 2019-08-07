#ifndef _PLAYERHANDLER_H_
#define _PLAYERHANDLER_H_

#include "WeaponHandler.h"
#include "ResourcesManager.h"
#include "Behaviour.h"
#include "GameObject.hpp"

#include <array>

#define NBWEAPON 5

using namespace Core::Maths;

class		CRenderWindow;
class		Sprite;
class		HUD;
class		Sound;

namespace       Resources
{
        class           Animation;
}

namespace LowRenderer
{
	class		Camera;
	class		Mesh;
}

class		PlayerHandler : public virtual Core::DataStructure::Behaviour
{
	private:
		std::array<WeaponHandler*, 5>		m_weapons;
		LowRenderer::Camera*				m_cam;
		Sprite*								m_healthSprite;
		Sprite*								m_endP;
		Sprite*								m_crossHair;
		Resources::Animation*				m_endPunch;
		Resources::ResourcesManager*		m_rm;
		Sound*								m_endSound;
		Sound*								m_headShotSound;
		Sound*								m_coolSound;
		Sound*								m_dmgSound;
		HUD*								m_hud;
		Vec3								m_endLevel;
		double								m_elapsedTime;
		int									m_idx;
		int									m_currentIdx;
		int									m_hp;
		bool								m_fire;
		bool								m_doFire;
		bool								m_lockFire;
		bool								m_change;
		bool								m_take;
		bool								m_doEndAnim;
		bool								m_playPoc;
		bool								m_crouch;
		bool								m_canCrouch;
		bool								m_isJumping;
		bool								m_canJump;
	public:
		PlayerHandler();
		PlayerHandler(int hp);
		PlayerHandler(Core::DataStructure::GameObject* object);
		PlayerHandler(const PlayerHandler& p);
		PlayerHandler(PlayerHandler&& p) = default;
		~PlayerHandler() override;

		const std::type_info&                   GetType() const override;
        Core::DataStructure::Component*         Copy() const override;
		void									Start(Resources::ResourcesManager* rm);
		void									Update(CRenderWindow* render, double frameTime) override;

		void									EndPunch(Resources::ResourcesManager* rm);
		void									InitHud(Resources::ResourcesManager* rm);
		void            						InitWeapon(Resources::ResourcesManager* rm);
		void            						InitHealthSprite(Resources::ResourcesManager* rm);
		void            						InitCamera();
		
		void									SetHealth(int health);
		inline void								SetCrossAir(Sprite* cross) { m_crossHair = cross; };
		inline bool								GetEnd() const { return m_doEndAnim; };
		inline int								GetHealth() const {return m_hp;};
		inline Sprite*							GetSpriteHealth() const {return m_healthSprite;};
		inline WeaponHandler*					GetWeapon() const {return m_weapons[m_currentIdx];};
		inline void								GiveAmmo(int ammo) const { m_coolSound->Play(0, false);  m_weapons[m_currentIdx]->GiveAmmo(ammo); }

		void            						Fire(CRenderWindow* render, double frameTime);
		void									ChangeIdWeapon(int id);
		void            						ChangeWeapon(CRenderWindow* render);
		void           							Action(CRenderWindow* render, double frameTime);
		void            						ChangePosPunch();
		void									TranslatePos(float trans, float timer);
		void									Strafe(float strafe, double timer);
		void									Move(CRenderWindow* render, double frameTime);
		void									WeaponInput(CRenderWindow* render, double frameTime);
		void									operator=(const PlayerHandler& p);
		void									OnCollision(Physics::Collision::Collider* collider) override;
		bool									CanPickAmmo();
		bool									CanPickLife();

};

#endif // !_PLAYER_H_
