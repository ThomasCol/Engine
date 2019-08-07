#include "GameManager.h"
#include "Scene.h"
#include "EndLevel.h"
#include "PlayerHandler.h"
#include "Animation.h"
#include "RenderWindow.h"
#include "Menu.h"
#include "Define.h"


GameManager::GameManager()
{
	m_player = nullptr;
	m_level = nullptr;
	m_gameOver = false;
	m_win = false;
}

GameManager::GameManager(const GameManager & gm)
{
	m_player = gm.m_player;
	m_level = gm.m_level;
	m_gameOver = gm.m_gameOver;
	m_win = gm.m_win;
}

GameManager::GameManager(GameManager && gm)
{
	m_player = std::move(gm.m_player);
	m_level = std::move(gm.m_level);
	m_gameOver = std::move(gm.m_gameOver);
	m_win = std::move(gm.m_win);
}

GameManager::GameManager(Core::DataStructure::GameObject* object) :
	Component(object), Behaviour(object)
{
	m_gameOver = false;
	m_win = false;
}

GameManager::~GameManager()
{
}

const std::type_info&   GameManager::GetType() const
{
	return typeid(*this);
}

Core::DataStructure::Component* GameManager::Copy() const
{
	return new GameManager(*this);
}

void GameManager::Start(Resources::ResourcesManager * rm)
{
	m_player = m_object->GetScene()->GetGameObject("Player")->GetComponent<PlayerHandler>();
	m_level = m_object->GetScene()->GetGameObject("w1")->GetComponent<EndLevel>();
	m_gameOverSound = rm->AddSound("GameOver", DEATHSOUND);
}

void GameManager::Update(CRenderWindow * render, double frameTime)
{
	if (m_gameOver || m_win)
		return;
	if (m_player->GetHealth() <= 0)
	{
		m_gameOver = true;
		m_gameOverSound->Play(frameTime, false);
		render->GetSimulation().GetScene()->GetMenu()->OpenGameOverMenu();
	}
	if (m_level->GetAnimation()->GetState())
	{
		m_win = true;
		render->GetSimulation().GetSceneManager()->NextLevel();
	}
}
