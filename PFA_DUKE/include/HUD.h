#ifndef _HUD_H_
#define _HUD_H_

#include "Sprites.h"
#include "TextRenderer.h"
#include "ResourcesManager.h"

namespace UI
{
	class Canvas;
	class Text;
}

/* FONT HUD */
#define FONT "resources/Font/arial.ttf"

class		PlayerHandler;

class		HUD
{
	private:
		UI::Text*		m_renderHp;
		UI::Text*		m_renderAmmo;
		Sprite*			m_spriteHealth;
		Sprite*			m_spriteWeapon;
		int				m_showHp;
		int				m_showAmmo;
	public:
		HUD();
		HUD(const PlayerHandler* p);
		~HUD();

		void		Init(Resources::ResourcesManager* rm, UI::Canvas* canvas);
		void		Draw(CRenderWindow* win);
		void		Change(const PlayerHandler* p);
		void		Update(CRenderWindow* win, const PlayerHandler* p);
};

#endif // !_HUD_H_
