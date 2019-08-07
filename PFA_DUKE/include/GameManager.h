#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "Behaviour.h"
#include "EndLevel.h"
#include "ResourcesManager.h"

class		CRenderWindow;


class	GameManager : public virtual Core::DataStructure::Behaviour
{
private:
	PlayerHandler*		m_player;
	EndLevel*			m_level;
	Sound*				m_gameOverSound;
	bool				m_gameOver;
	bool				m_win;
	
public:
	GameManager();
	GameManager(const GameManager& gm);
	GameManager(GameManager&& gm);
	GameManager(Core::DataStructure::GameObject* object);
	~GameManager();



	const std::type_info&                   GetType() const override;
	Core::DataStructure::Component*         Copy() const override;

	void									Start(Resources::ResourcesManager* rm);
	void									Update(CRenderWindow* render, double frameTime);

};

#endif // !_GAMEMANAGER_H
