#include "Scene.h"
#include "Debug.h"
#include "Mat4.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "RenderWindow.h"
#include "PlayerHandler.h"
#include "Enemy.h"
#include "Item.h"
#include "Music.h"
#include "Menu.h"
#include "Level.h"
#include "SceneManager.h"

using namespace Core::DataStructure;

namespace Resources
{
	Scene::Scene()
	{}

	Scene::Scene(const Scene& scene) :
		m_objects {scene.m_objects},
		m_name {scene.m_name}
	{}

	Scene::Scene(Scene&& scene) :
		m_objects {std::move(scene.m_objects)},
		m_name {std::move(scene.m_name)}
	{}

	Scene::Scene(std::string name):
		m_name {name}
	{}

	Scene::~Scene()
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); it++)
		{
			if ((*it) != nullptr)
				delete *it;
		}

		delete m_music;
	}


	//Private Methodes
	void	Scene::CreateVerticalWall(unsigned int i, unsigned int j, int& nbw, std::string file, int nbV)
	{
		/* create full scene */
		Core::DataStructure::GameObject* obj{ CreateGameObject("Wall" + std::to_string(nbw)) };
		LowRenderer::Mesh* mesh;
		obj->AddComponent<LowRenderer::Mesh>();
		mesh = obj->GetComponent<LowRenderer::Mesh>();

		m_manager->CreateMesh(mesh, "resources/obj/cube.obj",
			file.c_str(),
			"resources/shader/vertexShader.vsh",
			"resources/shader/fragmentShader.fsh");

		obj->AddComponent<Physics::Collision::BoxCollider>();
		obj->IsStatic() = true;

		mesh->SetTile(Vec2(nbV / 2.f, 3.f));
		obj->SetTransform({ (float)(j + (nbV / 2.f) - 0.5f), 3.f, (float)i }, Vec3::zero, Vec3((nbV / 2.f), 1.5f, 0.5f));
		nbw++;
		AddChildToGameObject(obj, "SkyBox");
	}

	void	Scene::CreateHorizontalWall(unsigned int i, unsigned int j, int& nbw, std::string file, int nbH)
	{
		/* create full scene */
		Core::DataStructure::GameObject* obj{ CreateGameObject("Wall" + std::to_string(nbw)) };
		LowRenderer::Mesh* mesh;
		obj->AddComponent<LowRenderer::Mesh>();
		mesh = obj->GetComponent<LowRenderer::Mesh>();

		m_manager->CreateMesh(mesh, "resources/obj/cube.obj",
			file.c_str(),
			"resources/shader/vertexShader.vsh",
			"resources/shader/fragmentShader.fsh");

		obj->AddComponent<Physics::Collision::BoxCollider>();
		obj->IsStatic() = true;

		mesh->SetTile(Vec2(nbH / 2.f, 3.f));
		obj->SetTransform({ (float)j, 3.f, (float)(i + (nbH / 2.f) - 0.5f) }, Vec3::zero, Vec3(0.5f, 1.5f, (nbH / 2.f)));
		nbw++;
		AddChildToGameObject(obj, "SkyBox");
	}

	void	Scene::CreateWall(unsigned int i, unsigned int j, int& nbw, std::string file)
	{
		/* create full scene */
		Core::DataStructure::GameObject* obj{ CreateGameObject("Wall" + std::to_string(nbw)) };
		LowRenderer::Mesh* mesh;
		obj->AddComponent<LowRenderer::Mesh>();
		mesh = obj->GetComponent<LowRenderer::Mesh>();

		m_manager->CreateMesh(mesh, "resources/obj/cube.obj",
			file.c_str(),
			"resources/shader/vertexShader.vsh",
			"resources/shader/fragmentShader.fsh");

		obj->AddComponent<Physics::Collision::BoxCollider>();
		obj->IsStatic() = true;

		mesh->SetTile(Vec2(1.f, 3.f));
		obj->SetTransform({ (float)i, 3.f, (float)j }, Vec3::zero, Vec3(0.5f, 1.5f, 0.5f));
		nbw++;
		AddChildToGameObject(obj, "SkyBox");
	}

	void    Scene::CreateFloor()
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject("Floor") };
		for (int h = 0; h < 2; h++)
		{
			Core::DataStructure::GameObject* pobj{ CreateGameObject("SP" + std::to_string(h)) };
			LowRenderer::Mesh* mesh;
			pobj->AddComponent<LowRenderer::Mesh>();
			mesh = pobj->GetComponent<LowRenderer::Mesh>();

			pobj->AddComponent<Physics::Collision::BoxCollider>();
			pobj->IsStatic() = true;

			if (h == 0)
				pobj->Layer() = GROUND_LAYER;

			m_manager->CreateMesh(mesh, "resources/obj/cube.obj",
				"resources/media/Level/floor.png",
				"resources/shader/vertexShader.vsh",
				"resources/shader/fragmentShader.fsh");
			mesh->SetTile(Vec2(24, 24));
			pobj->SetTransform({ 20, (float)(h * 4) + 1.f, 20 }, Vec3::zero, Vec3(20, 0.5, 20));
			AddChildToGameObject(pobj, "Floor");
		}
		obj->SetTransform({ 0,0,0 }, Vec3::zero, Vec3(1, 1, 1));
		AddChildToGameObject(obj, "SkyBox");
	}

	void    Scene::CreateEnd(unsigned int i, unsigned int j, std::string file, std::string wFile)
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject("End") };
		for (int h = 0; h < 3; h++)
		{
			Core::DataStructure::GameObject* pobj{ CreateGameObject("w" + std::to_string(h)) };
			LowRenderer::Mesh* mesh;
			pobj->AddComponent<LowRenderer::Mesh>();
			mesh = pobj->GetComponent<LowRenderer::Mesh>();
			pobj->AddComponent<Physics::Collision::BoxCollider>();
			pobj->IsStatic() = true;

			if (h == 1)
			{
				m_manager->CreateMesh(mesh, "resources/obj/cube.obj",
					file.c_str(),
					"resources/shader/vertexShader.vsh",
					"resources/shader/fragmentShader.fsh");
				pobj->AddComponent<EndLevel>();
			}
			else
				m_manager->CreateMesh(mesh, "resources/obj/cube.obj",
					wFile.c_str(),
					"resources/shader/vertexShader.vsh",
					"resources/shader/fragmentShader.fsh");

			pobj->SetTransform({ (float)i, (float)h + 2, (float)j }, { 0,0,0 }, Vec3(0.5f));
			AddChildToGameObject(pobj, "End");
		}

		obj->SetTransform({ 0,0,0 }, Vec3::zero, Vec3(1.f));
		AddChildToGameObject(obj, "SkyBox");
	}

	void	Scene::CreateBorder(unsigned int i, std::string file)
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject("Border" + std::to_string(i)) };
		LowRenderer::Mesh* mesh;
		obj->AddComponent<LowRenderer::Mesh>();
		mesh = obj->GetComponent<LowRenderer::Mesh>();
		obj->AddComponent<Physics::Collision::BoxCollider>();
		obj->IsStatic() = true;

		m_manager->CreateMesh(mesh, "resources/obj/cube.obj",
			"resources/media/Level/Wall1.png",
			"resources/shader/vertexShader.vsh",
			"resources/shader/fragmentShader.fsh");
		mesh->SetTile(Vec2(32, 3));

		if (i == 0)
			obj->SetTransform({ 0, 3, 20 }, Vec3::zero, Vec3(0.5, 1.5, 20));
		else if (i == 1)
			obj->SetTransform({ 20, 3, 0 }, Vec3::zero, Vec3(20, 1.5, 0.5));
		else if (i == 2)
			obj->SetTransform({ 39, 3, 20 }, Vec3::zero, Vec3(0.5, 1.5, 20));
		else if (i == 3)
			obj->SetTransform({ 20, 3, 39 }, Vec3::zero, Vec3(20, 1.5, 0.5));

		AddChildToGameObject(obj, "SkyBox");
	}

	int		Scene::CountWallHorinzontal(int i, int j, Level::Case num, const Level& level)
	{
		int nb = 0;
		for (; (unsigned)j < level.GetLen(); j++)
		{
			if (level.m_map[i][j] != num)
				return nb;
			nb += 1;
		}
		return nb;
	}

	int		Scene::CountWallVertical(int i, int j, Level::Case num, const Level& level)
	{
		int nb = 0;
		for (; (unsigned)i < level.GetLen(); i++)
		{
			if (level.m_map[i][j] != num)
				return nb;
			nb += 1;
		}
		return nb;
	}

	void	Scene::CreateWidthWall(const Level& level, int& nbw)
	{
		int nbH = 1;
		int cpt = 1;
		for (unsigned int i = 1; i < level.GetLen() - 1; i++)
		{
			while ((unsigned)cpt < level.GetLen() - 1)
			{
				if (level.m_map[i][cpt] == Level::Case::WALL)
				{
					if (CountWallHorinzontal(i, cpt, Level::Case::WALL, level) > 1)
					{
						nbH = CountWallHorinzontal(i, cpt, Level::Case::WALL, level);
						CreateHorizontalWall(cpt, i, nbw, WALLS, nbH);
						cpt += nbH;
					}
					else
						cpt += 1;
				}
				else if (level.m_map[i][cpt] == Level::Case::WALL2)
				{
					if (CountWallHorinzontal(i, cpt, Level::Case::WALL2, level) > 1)
					{
						nbH = CountWallHorinzontal(i, cpt, Level::Case::WALL2, level);
						CreateHorizontalWall(cpt, i, nbw, WALLS2, nbH);
						cpt += nbH;
					}
					else
						cpt += 1;
				}
				else
					cpt += 1;
			}
			cpt = 1;
		}
	}

	void	Scene::CreateHeightWall(const Level& level, int& nbw)
	{
		int nbV = 1;
		int cpt = 1;
		for (unsigned int i = 1; i < level.GetLen() - 1; i++)
		{
			while ((unsigned)cpt < level.GetLen() - 1)
			{
				if (level.m_map[cpt][i] == Level::Case::WALL)
				{
					if (CountWallVertical(cpt, i, Level::Case::WALL, level) > 1)
					{
						nbV = CountWallVertical(cpt, i, Level::Case::WALL, level);
						CreateVerticalWall(i, cpt, nbw, WALLS, nbV);
						cpt += nbV;
					}
					else
						cpt++;
				}
				else if (level.m_map[cpt][i] == Level::Case::WALL2)
				{
					if (CountWallVertical(cpt, i, Level::Case::WALL2, level) > 1)
					{
						nbV = CountWallVertical(cpt, i, Level::Case::WALL2, level);
						CreateVerticalWall(i, cpt, nbw, WALLS2, nbV);
						cpt += nbV;
					}
					else
						cpt++;
				}
				else
					cpt += 1;
			}
			cpt = 1;
		}
	}

	void	Scene::CreateMap(const std::string map)
	{
		Level level;
		level.SetFile(map);
		level.SetMap();

		AI::PathFinder::Instance()->Init(level);

		int nbw = 1;
		for (unsigned int i = 1; i < level.GetLen() - 1; i++)
		{
			for (unsigned int j = 1; j < level.GetLen() - 1; j++)
			{
				if (level.m_map[i][j] == Level::Case::END)
				{
					if (m_sceneManager->secretMode)
						CreateEnd(i, j, "resources/media/Level/FEndTW.png", WALLS);
					else
						CreateEnd(i, j, std::string(ENDL) + PNG, WALLS);
				}
				else if (level.m_map[i][j] == Level::Case::END2)
				{
					if (m_sceneManager->secretMode)
						CreateEnd(i, j, "resources/media/Level/SEndW.png", WALLS2);
					else
						CreateEnd(i, j, std::string(ENDL2) + PNG, WALLS2);
				}
				else if (level.m_map[i][j] == Level::Case::WALLTAP1)
					CreateWall(i, j, nbw, WALLTAPS1);
				else if (level.m_map[i][j] == Level::Case::WALLTAP2)
					CreateWall(i, j, nbw, WALLTAPS2);
			}
		}

		CreatePlayer();

		CreateWidthWall(level, nbw);
		CreateHeightWall(level, nbw);

		for (unsigned int i = 0; i < 4; i++)
		{
			if (level.m_map[0][0] == Level::Case::WALL)
				CreateBorder(i, WALLS);
			else if (level.m_map[0][0] == Level::Case::WALL2)
				CreateBorder(i, WALLS2);
		}
		CreateFloor();

		GenerateEnemies(level);
		GenerateItems(level);
	}

	void	Scene::SetScene(const std::string map)
	{
		/* create full scene */
		Core::DataStructure::GameObject* obj{ CreateGameObject("SkyBox") };
		obj->IsStatic() = true;

		obj->SetTransform({ 0, 0, 0 }, Vec3::zero, Vec3(1, 1, 1));
		CreateMap(map);
	}

	void	Scene::CreatePlayer()
	{
		GameObject* player = CreateGameObject("Player");


		player->AddComponent<LowRenderer::Camera>();
		m_camera = player->GetComponent<LowRenderer::Camera>();

		player->SetTransform({ 2, 3, 2 }, { 0, 0, 0 }, { 1, 1, 1 });

		player->AddComponent<PlayerHandler>();
		PlayerHandler* playerHandler = player->GetComponent<PlayerHandler>();

		player->AddComponent<Physics::Collision::SphereCollider>();

		SphereCollider* col = player->GetComponent<Physics::Collision::SphereCollider>();

		col->Radius() = 0.3f;
		col->LocalPosition() = Vec3(0, -1, 0);

		player->AddComponent<Physics::Rigidbody>();

		EndLevel* end = GetGameObject("w1")->GetComponent<EndLevel>();
		end->SetPlayer(playerHandler);
		AddChildToGameObject(player, "SkyBox");

		Core::DataStructure::GameObject* crossHair{ CreateGameObject("crossHair") };
		Sprite* sprite;
		crossHair->AddComponent<Sprite>();
		sprite = crossHair->GetComponent<Sprite>();
		m_manager->CreateSprite(sprite, "resources/obj/face.obj",
			"resources/media/Sprites/CrossHair.png",
			"resources/shader/vertexHUD.vsh",
			"resources/shader/fragmentHUD.fsh");
		playerHandler->SetCrossAir(sprite);
	}

	void	Scene::CreateEnemy(int& id, const Vec3& pos, std::string fileSprite)
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject("enemy" + std::to_string(id)) };
		obj->IsStatic() = true;

		obj->SetTransform(pos, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		obj->AddComponent<Game::Enemy>();
		Game::Enemy* enemy = obj->GetComponent<Game::Enemy>();
		enemy->SetPlayer(GetGameObject("Player"));
		enemy->SetCam(m_camera);

		Sprite* sprite;
		obj->AddComponent<Sprite>();

		sprite = obj->GetComponent<Sprite>();
		obj->AddComponent<Physics::Collision::BoxCollider>();
		Physics::Collision::BoxCollider* col = obj->GetComponent<Physics::Collision::BoxCollider>();
		col->SetScaleCollider(Vec3(0.5f, 0.7f, 0.1f));

		m_manager->CreateSprite(sprite, "resources/obj/Enemy.obj",
			fileSprite.c_str(),
			"resources/shader/vertexEnemy.vsh",
			"resources/shader/fragmentHUD.fsh");

		AddChildToGameObject(obj, "SkyBox");
	}

	void Scene::CreateItem(std::string type, const int& id, const Vec3 & pos, std::string fileSprite)
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject(type + std::to_string(id)) };
		obj->IsStatic() = true;

		obj->SetTransform(pos, { 0.f, 0.f, 0.f }, { 1.f,1.f,0.2f });

		Sprite* sprite;
		obj->AddComponent<Sprite>();

		sprite = obj->GetComponent<Sprite>();
		obj->AddComponent<Physics::Collision::BoxCollider>();
		Physics::Collision::BoxCollider* collider = obj->GetComponent<Physics::Collision::BoxCollider>();
		collider->isTrigger() = true;

		m_manager->CreateSprite(sprite, "resources/obj/face.obj",
			fileSprite.c_str(),
			"resources/shader/vertexEnemy.vsh",
			"resources/shader/fragmentHUD.fsh");

		obj->AddComponent<Game::Item>();
		Game::Item*	item = obj->GetComponent<Game::Item>();
		if (type == "Ammo")
			item->CreateItem(Game::ItemType::AMMO, pos, GetGameObject("Player")->GetComponent<PlayerHandler>(), AMMO_VALUE);
		else if (type == "Heal")
			item->CreateItem(Game::ItemType::HEAL, pos, GetGameObject("Player")->GetComponent<PlayerHandler>(), HEAL_VALUE);

		AddChildToGameObject(obj, "SkyBox");
	}

	void Scene::GenerateEnemies(Level& level)
	{
		int		idEnemy = 1;
		for (unsigned int i = 1; i < level.GetLen() - 1; i++)
		{
			for (unsigned int j = 1; j < level.GetLen() - 1; j++)
			{
				if (level.m_map[i][j] == Level::Case::ENEMY)
				{
					if (m_sceneManager->secretMode)
						CreateEnemy(idEnemy, Vec3((float)i, 2.2f, (float)j), "resources/media/Sprites/SpriteEnemy/Stroopers.png");
					else
						CreateEnemy(idEnemy, Vec3((float)i, 2.2f, (float)j), "resources/media/Sprites/SpriteEnemy/troopers.png");
					idEnemy++;
				}
			}
		}
	}

	void Scene::GenerateItems(Level& level)
	{
		int		idItem = 1;
		for (unsigned int i = 1; i < level.GetLen() - 1; i++)
		{
			for (unsigned int j = 1; j < level.GetLen() - 1; j++)
			{
				if (level.m_map[i][j] == Level::Case::HEAL)
				{
					if (m_sceneManager->secretMode)
						CreateItem("Heal", idItem, Vec3((float)i, 2.2f, (float)j), "resources/media/Sprites/Sheal.png");
					else
						CreateItem("Heal", idItem, Vec3((float)i, 2.2f, (float)j), "resources/media/Sprites/heal.png");
					idItem++;
				}
				else if (level.m_map[i][j] == Level::Case::AMMO)
				{
					if (m_sceneManager->secretMode)
						CreateItem("Ammo", idItem, Vec3((float)i, 2.2f, (float)j), "resources/media/Sprites/Sammo.png");
					else
						CreateItem("Ammo", idItem, Vec3((float)i, 2.2f, (float)j), "resources/media/Sprites/ammo.png");
					idItem++;
				}
			}
		}
	}

	void	Scene::CreateMenu()
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject("Menu") };
		obj->AddComponent<UI::Menu>();
		m_menu = obj->GetComponent<UI::Menu>();
		m_menu->Init(m_window->m_input, m_manager, this, m_sceneManager);
		m_menu->CloseMenu();
	}

	void Scene::SceneGame(const std::string map, const std::string music)
	{
		SetScene(map);
		CreateMenu();

		Core::DataStructure::GameObject* obj = CreateGameObject("GameManager");
		obj->AddComponent<GameManager>();

		m_music = new Music(music);
		m_music->Play();
	}

	void	Scene::SceneMainMenu()
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject("Menu") };
		obj->AddComponent<UI::Menu>();
		m_menu = obj->GetComponent<UI::Menu>();
		m_menu->Init(m_window->m_input, m_manager, this, m_sceneManager);
		m_menu->OpenMainMenu();

		m_music = new Music(DEFAULTMUSIC);
		m_music->Play();
	}

	void	Scene::SceneWinMenu()
	{
		Core::DataStructure::GameObject* obj{ CreateGameObject("Menu") };
		obj->AddComponent<UI::Menu>();
		m_menu = obj->GetComponent<UI::Menu>();
		m_menu->Init(m_window->m_input, m_manager, this, m_sceneManager);
		m_menu->OpenWinMenu();

		m_music = new Music(DEFAULTMUSIC);
		m_music->Play();
	}

	//Private Methodes


	void Scene::Setup(const int& id)
	{
		switch (id)
		{
		case 0:
			SceneMainMenu();
			break;
		case 1:
			SceneGame(LEVEL1, MUSIC1);
			break;
		case 2:
			SceneGame(LEVEL2, MUSIC2);
			break;
		default:
			SceneWinMenu();
			break;
		}
	}

	Scene&	Scene::operator=(const Scene& scene)
	{
		m_name = scene.m_name;
		m_objects = scene.m_objects;

		return *this;
	}

	GameObject*	Scene::CreateGameObject(std::string name)
	{
		GameObject*	object = new GameObject{this, name};

		m_objects.push_back(object);

		return object;
	}

	void	Scene::StartScene(Resources::ResourcesManager* rm)
	{
		m_behaviours.reverse();
		for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
		{
			(*it)->Start(rm);
		}
	}

	void	Scene::AddMeshToRender(LowRenderer::Mesh* mesh)
	{
		m_toRender.push_back(mesh);
	}

	void	Scene::AddObjectToCollide(Physics::Collision::Collider* collider)
	{
		m_collisionManager.AddCollider(collider);
	}

	void	Scene::AddRigidbody(Physics::Rigidbody* rigidbody)
	{
		m_rigidbodies.push_back(rigidbody);
	}

	void	Scene::AddBehaviour(Core::DataStructure::Behaviour* behaviour)
	{
		m_behaviours.push_back(behaviour);
	}

	void	Scene::DestroyObject(Core::DataStructure::GameObject* object)
	{
		m_objects.remove(object);
		delete object;
	}
	
	void	Scene::RemoveMeshToRender(LowRenderer::Mesh* mesh)
	{
		m_toRender.remove(mesh);
	}

	void	Scene::RemoveObjectToCollide(Physics::Collision::Collider* collider)
	{
		m_collisionManager.RemoveCollider(collider);
	}

	void	Scene::RemoveRigidbody(Physics::Rigidbody* rigidbody)
	{
		m_rigidbodies.remove(rigidbody);
	}

	void	Scene::RemoveBehaviour(Core::DataStructure::Behaviour* behaviour)
	{
		m_behaviours.remove(behaviour);
	}

	void	Scene::Draw()
	{
		for (auto it = m_toRender.begin(); it != m_toRender.end(); it++)
		{
			if ((*it)->IsEnable())
				(*it)->Draw(m_camera);
		}
	}

	void	Scene::DrawGUI()
	{
		for (auto it{ m_objects.begin() }; it != m_objects.end(); ++it)
			(*it)->DrawGUI(m_window->GetWindow());
	}

	void	Scene::UpdatePhysics(double frameTime)
	{
		for (Rigidbody* r : m_rigidbodies)
			r->Update((const float)frameTime);

		m_collisionManager.Update((const float)frameTime);
	}

	void	Scene::UpdateScene(CRenderWindow* win, double frameTime)
	{
		for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
			(*it)->Update(win, frameTime);

		if (m_isMenu < 3 && m_isMenu > 0)
			--m_isMenu;

	}

	void	Scene::CleanScene()
	{
		for (auto it{ m_objects.begin() }; it != m_objects.end(); ++it)
			(*it)->CleanDestroyedComponents();
	}

	GameObject*	Scene::GetGameObject(std::string name)
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); it++)
		{
			if ((*it)->GetName() == name)
				return (*it);
		}

		return nullptr;
	}

	void	Scene::AddChildToGameObject(GameObject* obj, std::string name)
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); it++)
		{
			if ((*it)->GetName() == name)
			{
				(*it)->AddChild(obj);
				return;
			}
		}
	}

	void Scene::SetActive()
	{
		m_collisionManager.SetActive();
	}

	bool	Scene::SecretMode()
	{
		return m_sceneManager->secretMode;
	}
}
