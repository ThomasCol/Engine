#include <iostream>
#include <fstream>
#include <math.h>

#include "Debug.h"
#include "Define.h"

#include "Simulation.h"
#include "RenderWindow.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "Camera.h"
#include "PlayerHandler.h"
#include "Enemy.h"
#include "Sprites.h"
#include "Menu.h"

#include "Canvas.hpp"

using namespace Core::Maths;

CSimulation::CSimulation()
	: m_pRenderWindow(nullptr)
{
	ClearLOG();

	m_lag = 0.0;
}

CSimulation::~CSimulation()
{
	delete m_sceneManager;
	delete m_manager;
	AI::PathFinder::Destroy();
}

void	CSimulation::SetRenderWindow(CRenderWindow* pRenderWindow)
{
	m_pRenderWindow = pRenderWindow;
}

void	CSimulation::Init(uint width, uint height)
{
	glEnable(GL_DEPTH_TEST);
	
	m_width = width;
	m_height = height;

	m_manager = new Resources::ResourcesManager;

	m_sceneManager = new Core::DataStructure::SceneManager(m_pRenderWindow, m_manager);

	m_sceneManager->SetActualScene(0);
}

void	CSimulation::Update(double frameTime, SDL_Window* window)
{
	m_sceneManager->CheckScene();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	(void)window;
	m_lag += frameTime;

	m_scene = m_sceneManager->GetActualScene();
	if (m_scene->IsMenu())
		frameTime = 0;

	m_scene->UpdatePhysics(frameTime);
	m_lag -= FIXED_FRAMETIME;

	m_scene->UpdateScene(m_pRenderWindow, frameTime);
	m_scene->Draw();
	m_scene->DrawGUI();

	if (!m_scene->IsMenu())
		SDL_WarpMouseInWindow(m_pRenderWindow->GetWindow(), m_width / 2, m_height / 2);
	m_scene->CleanScene();
}

void	CSimulation::Resize(uint width, uint height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
}
