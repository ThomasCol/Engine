#ifndef __SIMULATION_H__
#define __SIMULATION_H__

typedef unsigned int uint;

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "Scene.h"
#include "ResourcesManager.h"
#include "Level.h"
#include "Event.hpp"
#include "HUD.h"
#include "EndLevel.h"
#include "Music.h"
#include "GameManager.h"
#include "SceneManager.h"

namespace UI
{
	class Menu;
}
class CRenderWindow;
class CSimulation
{
public:
	CSimulation();
	~CSimulation();


	void										SetRenderWindow(CRenderWindow* pRenderWindow);

	void										Init(uint width, uint height);
	void										Update(double frameTime, SDL_Window* window);
	void										Resize(uint width, uint height);

	inline CRenderWindow*						GetRenderWindow() { return m_pRenderWindow; };
	inline Resources::ResourcesManager*			GetManager() const {return m_manager;}
	inline Core::DataStructure::SceneManager*	GetSceneManager() const {return m_sceneManager;}
	inline Resources::Scene*					GetScene() const { return m_scene; };

private:
	CRenderWindow*								m_pRenderWindow;
	Resources::Scene*							m_scene;
	Core::DataStructure::SceneManager*			m_sceneManager;
	Resources::ResourcesManager*				m_manager;
	double										m_lag;	
	uint										m_width;
	uint										m_height;
};

#endif
