#ifndef _SCENE_H_
#define _SCENE_H_

#include "GameObject.hpp"
#include "Mesh.h"
#include "Behaviour.h"
#include "CollisionManager.h"
#include "Sprites.h"
#include "CollisionManager.h"
#include "Rigidbody.h"
#include "Level.h"

class CRenderWindow;
class Music;

namespace UI
{
	class Menu;
}

namespace Core::DataStructure
{
	class SceneManager;
}

namespace Resources
{
	class ResourcesManager;
	/* class for drawing some mesh */
	class Scene
	{
	private:
		std::list<Core::DataStructure::GameObject*>		m_objects;
		std::string										m_name;
		std::list<LowRenderer::Mesh*>					m_toRender;
		std::list<Physics::Collision::Collider*>		m_toCollide;
		std::list<Core::DataStructure::Behaviour*>		m_behaviours;
		Physics::Collision::CollisionManager			m_collisionManager;
		std::list<Physics::Rigidbody*>					m_rigidbodies;
		LowRenderer::Camera*							m_camera;
		CRenderWindow*									m_window;
		Resources::ResourcesManager*					m_manager;
		Music*											m_music;
		UI::Menu*										m_menu;
		Core::DataStructure::SceneManager*				m_sceneManager;
		int												m_isMenu{0};

		void											CreateHorizontalWall(unsigned int i, unsigned int j, int& nbw, std::string file, int nbH);
		void											CreateVerticalWall(unsigned int i, unsigned int j, int& nbw, std::string file, int nbV);
		void											CreateWall(unsigned int i, unsigned int j, int& nbw, std::string file);
		void											CreateFloor();
		void											CreateEnd(unsigned int i, unsigned int j, std::string file, std::string wFile);
		void    										CreateBorder(unsigned int i, std::string file);
		int												CountWallHorinzontal(int i, int j, Level::Case num, const Level& level);
		int												CountWallVertical(int i, int j, Level::Case num, const Level& level);
		void											CreateHeightWall(const Level& level, int& nbw);
		void											CreateWidthWall(const Level& level, int& nbw);
		void											CreateMap(const std::string map);
		void											CreatePlayer();
		void											CreateEnemy(int& id, const Vec3& pos, std::string fileSprite);
		void											CreateItem(std::string type, const int& id, const Vec3& pos, std::string fileSprite);
		void											GenerateEnemies(Level& level);
		void											GenerateItems(Level& level);
		void											SetScene(const std::string map);
		void											CreateMenu();

		void											SceneGame(const std::string map, const std::string music);
		void											SceneMainMenu();
		void											SceneWinMenu();

	public:
		Scene();
		Scene(const Scene& scene);
		Scene(Scene&& scene);
		Scene(std::string name);
		~Scene();

		void								Setup(const int& id);

		Scene&								operator=(const Scene& scene);

		inline std::string					GetName() const { return m_name; }
		inline void							SetSceneManager(Core::DataStructure::SceneManager* sm) { m_sceneManager = sm; }
		inline void							SetWindow(CRenderWindow* window) { m_window = window; }
		inline CRenderWindow*				GetWindow() { return m_window; }
		inline UI::Menu*					GetMenu() { return m_menu; }
		inline void							IsMenu(const int& b) { m_isMenu = b; }
		inline bool							IsMenu() { return m_isMenu > 0; }
		inline ResourcesManager*			GetResourcesManager() { return m_manager; }
		inline void							SetResourcesManager(ResourcesManager* manager) { m_manager = manager; }

		Core::DataStructure::GameObject*	CreateGameObject(std::string name);
		void								Draw();
		void								DrawGUI();

		void								DestroyObject(Core::DataStructure::GameObject* object);

		void								UpdatePhysics(double frameTime);
		void								UpdateScene(CRenderWindow* win, double frameTime);
		void								CleanScene();

		void								AddMeshToRender(LowRenderer::Mesh* mesh);
		void								AddObjectToCollide(Physics::Collision::Collider* collider);
		void								AddRigidbody(Physics::Rigidbody* rigidbody);
		void								AddBehaviour(Core::DataStructure::Behaviour* behaviour);
		
		void								RemoveMeshToRender(LowRenderer::Mesh* mesh);
		void								RemoveObjectToCollide(Physics::Collision::Collider* collider);
		void								RemoveRigidbody(Physics::Rigidbody* rigidbody);
		void								RemoveBehaviour(Core::DataStructure::Behaviour* behaviour);

		void								StartScene(Resources::ResourcesManager* rm);

		Core::DataStructure::GameObject*	GetGameObject(std::string name);

		void								AddChildToGameObject(Core::DataStructure::GameObject* object, std::string name);

		void								SetActive();

		bool								SecretMode();
	};
}

#endif //_SCENE_H_
