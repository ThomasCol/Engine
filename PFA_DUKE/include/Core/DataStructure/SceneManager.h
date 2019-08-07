#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include <vector>

#include "Light.h"

class CRenderWindow;

namespace Resources
{
	class Scene;
	class ResourcesManager;
}

namespace Core::DataStructure
{
	class SceneManager
	{
	private:
		std::vector<LowRenderer::Lights::Light*>	m_lights;
		Resources::Scene*							m_actualScene;
		CRenderWindow*								m_window;
		Resources::ResourcesManager*				m_manager;
		unsigned									m_actualID;
		unsigned									m_newID;
		bool										m_isReload;

	public:
		SceneManager(CRenderWindow* window, Resources::ResourcesManager* manager);
		SceneManager(const SceneManager& smgr) = delete;
		SceneManager(SceneManager&& smgr) = delete;
		~SceneManager();

		void						ChangeScene(const unsigned& id);
		void						CheckScene();
		void						NextLevel();
		inline Resources::Scene*	GetActualScene() { return m_actualScene; };
		void						ReloadLevel();
		void						SetActualScene(const unsigned int& id);
		void						SecretScene(const unsigned& id);

		bool						secretMode{false};
	};
}


#endif
