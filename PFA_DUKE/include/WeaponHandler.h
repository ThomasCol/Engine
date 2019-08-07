#ifndef _WEAPONHANDLER_H_
#define _WEAPONHANDLER_H_

#include "Sprites.h"
#include "Define.h"

/* Ammo of Weapons */

class		CRenderWindow;

class		Sound;

namespace	Resources
{
	class		Animation;
	class		ResourcesManager;
}

class		WeaponHandler
{
	private:
		Resources::Animation*	m_anim;
		Sound*					m_fireSound;
		Sound*					m_changeSound;
		Sprite*					m_hudSprite;
		Sprite*					m_sprite;
		Core::Maths::Vec3		m_pos;
		Core::Maths::Vec3		m_currentPos;
		double					m_elapsedTime;
		int						m_dmg;
		int						m_useAmmo;
		int						m_ammoMax;
		int						m_ammo;
		int						m_impactSize;
	public:
		WeaponHandler() = default;
		WeaponHandler(Resources::ResourcesManager* rm, int id, Sprite* wSprite, Sprite* hSprite);
		WeaponHandler(const WeaponHandler& w);
		WeaponHandler(WeaponHandler&& w) = default;
		~WeaponHandler();
		
		void			InitGun(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite);
		void			InitShotGun(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite);
		void			InitRifle(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite);
		void			InitRocket(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite);
		void			InitRpg(Resources::ResourcesManager* rm, Sprite* wSprite, Sprite* hSprite);

		inline Sprite*	GetHudSprite() const {return m_hudSprite;};
		inline Sprite*	GetSprite() const {return m_sprite;};
		inline int		GetAmmo() { if (m_ammo > m_ammoMax) { m_ammo = m_ammoMax; } else if (m_ammo < 0) { m_ammo = 0; }return m_ammo; };
		inline int		GetMaxAmmo() { return m_ammoMax; }
		inline int		GetUseAmmo() const { return m_useAmmo; };
		inline int		GetDmg() const { return m_dmg; };
		inline int		GetImpactSize() const { return m_impactSize; }

		void			GiveAmmo(int ammo);
		void   	 		TakeW(bool& take, double frameTime);
		void    		RemoveW(bool& change, bool& take);
		void    		ChangeWeapon(double frameTime, bool& change, bool& take);
		void			Fire(Resources::ResourcesManager* rm, double frameTime, bool& fire, bool& doFire, bool repeat);
		void			Idle(Resources::ResourcesManager* rm);
		void			Update();
};

#endif // _WEAPONHANDLER_H_
