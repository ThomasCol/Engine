#include <SDL2/SDL.h>
#include "SceneManager.h"
#include "Scene.h"
#include "ResourcesManager.h"
#include "DirectionalLight.h"
#include "Define.h"
#include "RenderWindow.h"
#include "PathFinder.h"

using namespace Resources;

Core::DataStructure::SceneManager::SceneManager(CRenderWindow* window, Resources::ResourcesManager* manager):
	m_actualScene{ nullptr },
	m_window{ window },
	m_manager{ manager },
	m_isReload {false}
{
	LowRenderer::Lights::DirectionalLight* light1 = new LowRenderer::Lights::DirectionalLight(
		{ 0, -1, 0 }, { 0.9f, 0.9f, 0.9f }, { 0.1f, 0.1f, 0.1f }, { 0.1f, 0.1f, 0.1f });

	LowRenderer::Lights::DirectionalLight* light2 = new LowRenderer::Lights::DirectionalLight(
		{ 0, 1, 0 }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f });

	m_lights.push_back(light1);
	m_lights.push_back(light2);
}

Core::DataStructure::SceneManager::~SceneManager()
{
	delete m_actualScene;
	for (unsigned i{ 0 }; i < m_lights.size(); ++i)
		delete m_lights[i];
}

void	Core::DataStructure::SceneManager::ChangeScene(const unsigned& id)
{
	if (id != m_newID)
		m_newID = id;
}

void	Core::DataStructure::SceneManager::SecretScene(const unsigned& id)
{
	secretMode = true;
	if (id != m_newID)
		m_newID = id;
}

void	Core::DataStructure::SceneManager::CheckScene()
{
	if (m_actualID != m_newID)
		SetActualScene(m_newID);
	else if (m_isReload)
		SetActualScene(m_actualID);
}

void	Core::DataStructure::SceneManager::NextLevel()
{
	++m_newID;
}

void	Core::DataStructure::SceneManager::ReloadLevel()
{
	m_isReload = true;
}

void	Core::DataStructure::SceneManager::SetActualScene(const unsigned int& id)
{
	if (m_actualScene != nullptr)
		delete m_actualScene;

	m_newID = m_actualID = id;

	m_actualScene = new Resources::Scene("Scene");
	m_actualScene->SetSceneManager(this);
	m_actualScene->SetWindow(m_window);
	m_actualScene->SetResourcesManager(m_manager);
	m_actualScene->Setup(id);
	m_actualScene->StartScene(m_manager);

	if (!m_isReload)
	{
		m_manager->InitLightsShader(m_lights);

		int	width, height;
		SDL_GetWindowSize(m_window->GetWindow(), &width, &height);
		m_manager->SetProjectionMatrix(Mat4::CreateProjectionMatrix(width, height, (float)NEAR,
			(float)FAR, (float)FOV));
	}
	m_isReload = false;
}
