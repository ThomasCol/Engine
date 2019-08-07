#include "HUD.h"

#include "PlayerHandler.h"
#include "RenderWindow.h"
#include "Canvas.hpp"
#include <SDL2/SDL.h>

HUD::HUD()
{
	m_showAmmo = 0;
	m_showHp = 0;
	m_spriteHealth = NULL;
	m_spriteWeapon = NULL;
	m_renderHp = NULL;
	m_renderAmmo = NULL;
}

HUD::HUD(const PlayerHandler* p)
{
	m_showAmmo = p->GetWeapon()->GetAmmo();
	m_showHp = p->GetHealth();
	m_spriteHealth = p->GetSpriteHealth();
	m_spriteWeapon = p->GetWeapon()->GetHudSprite();
	m_renderHp = NULL;
	m_renderAmmo = NULL;
}

HUD::~HUD()
{}

void		HUD::Init(Resources::ResourcesManager* rm, UI::Canvas* canvas)
{
	m_renderHp = canvas->AddText(std::to_string(m_showHp), Vec2(-0.65f, -1.f), Vec2(0.f), rm);
	m_renderAmmo = canvas->AddText(std::to_string(m_showAmmo), Vec2(0.82f, -1.f), Vec2(0.f), rm);
}

void		HUD::Change(const PlayerHandler* p)
{
	if (m_showAmmo != p->GetWeapon()->GetAmmo())
		m_showAmmo = p->GetWeapon()->GetAmmo();
	if (m_showHp != p->GetHealth())
		m_showHp = p->GetHealth();
	if (m_spriteHealth != p->GetSpriteHealth())
		m_spriteHealth = p->GetSpriteHealth();
	if (m_spriteWeapon != p->GetWeapon()->GetHudSprite())
		m_spriteWeapon = p->GetWeapon()->GetHudSprite();
}

void		HUD::Draw(CRenderWindow* win)
{
	int	width, height;

	SDL_GetWindowSize(win->GetWindow(), &width, &height);
	m_spriteHealth->Draw();
	m_renderHp->SetMessage(std::to_string(m_showHp));
	m_spriteWeapon->Draw();
	m_renderAmmo->SetMessage(std::to_string(m_showAmmo));
}

void		HUD::Update(CRenderWindow* win, const PlayerHandler* p)
{
	Change(p);
	Draw(win);
}
