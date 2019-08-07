#include "Menu.h"
#include "SceneManager.h"
#include "Canvas.hpp"
#include "RenderWindow.h"
#include "Define.h"
#include "Scene.h"

namespace UI
{
	Menu::Menu() : Component(), m_isActive {true}, m_isInMenu{ false }
	{
		m_canvas = m_object->GetComponent<Canvas>();
		if (m_canvas == nullptr)
		{
			m_object->AddComponent<Canvas>();
			m_canvas = m_object->GetComponent<Canvas>();
		}
	}

	Menu::Menu(const Menu& m) : Component(m), m_isActive{ true }, m_isInMenu{ false }
	{
		m_canvas = m.m_canvas;
		m_input = m.m_input;
		m_manager = m.m_manager;
	}

	Menu::Menu(Core::DataStructure::GameObject* go) : Component(go), m_isActive{ true }, m_isInMenu{ false }
	{
		m_canvas = m_object->GetComponent<Canvas>();
		if (m_canvas == nullptr)
		{
			m_object->AddComponent<Canvas>();
			m_canvas = m_object->GetComponent<Canvas>();
		}
	}

	Menu&	Menu::operator= (const Menu& m)
	{
		m_canvas = m.m_canvas;
		m_input = m.m_input;
		m_manager = m.m_manager;
		m_isActive = m.m_isActive;
		m_isInMenu = m.m_isInMenu;
		return *this;
	}

	const std::type_info&		Menu::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component*	Menu::Copy() const
	{
		return new Menu(*this);
	}

	void	Menu::ClearMenu()
	{
		m_canvas->Clear();
		m_scene->IsMenu(3);
		m_isInMenu = true;
		SDL_ShowCursor(SDL_ENABLE);
	}

	void	Menu::CloseMenu()
	{
		m_canvas->Clear();
		m_scene->IsMenu(2);
		SDL_ShowCursor(SDL_DISABLE);
		m_isInMenu = false;
	}

	void	Menu::OpenMainMenu()
	{
		m_sceneManager->secretMode = false;
		if (!m_isActive)
			return;
		ClearMenu();
		m_canvas->AddSprite(MAINBACKGROUND, Vec2(0, 0), Vec2(2, 2), m_manager);
		(*m_canvas->AddButton<unsigned>(std::string("New Game"), Vec2(-0.95f, 0.6f), Vec2(0.75f, 0.25f), 1, m_manager, 1, Vec3(1.f), Vec4(0.f, 0.f, 0.f, 0.4f)))
			+= std::bind(&Core::DataStructure::SceneManager::ChangeScene, m_sceneManager, 1);
		(*m_canvas->AddButton(std::string("Settings"), Vec2(-0.95f, 0.1f), Vec2(0.55f, 0.25f), m_manager, 1, Vec3(1.f), Vec4(0.f,0.f,0.f,0.4f)))
			+= std::bind(&Menu::OpenBindingMenu, this, true);
		(*m_canvas->AddButton("Quit", Vec2(-0.95f, -0.4f), Vec2(0.35f, 0.25f), m_manager, 1, Vec3(1.f), Vec4(0.f,0.f,0.f,0.4f)))
			+= std::bind(&CRenderWindow::Quit, m_scene->GetWindow());
		(*m_canvas->AddButton("", Vec2(-0.10f, 0.85f), Vec2(0.19f, 0.25f), m_manager, 1, Vec3(1.f), Vec4(0.f, 0.f, 0.f, 0.f)))
			+= std::bind(&Core::DataStructure::SceneManager::SecretScene, m_sceneManager, 1);
	}

	void	Menu::OpenWinMenu()
	{
		if (!m_isActive)
			return;
		ClearMenu();
		m_canvas->AddSprite(MAINBACKGROUND, Vec2(0.f, 0.f), Vec2(2.f, 2.f), m_manager);
		m_canvas->AddText("You    Win!", Vec2(-0.5f, 0.7f), Vec2(1.f, 0.5f), m_manager, 3.f);
		(*m_canvas->AddButton<unsigned>("Back to main menu", Vec2(-0.6f, -0.5f), Vec2(1.2f, 0.25f), 0, m_manager, 1, Vec3(1.f), Vec4(0.f,0.f,0.f,0.6f)))
			+= std::bind(&Core::DataStructure::SceneManager::ChangeScene, m_sceneManager, 0);
	}
	
	void	Menu::OpenPauseMenu()
	{
		if (!m_isActive || m_isInMenu)
			return;
		ClearMenu();
		(*m_canvas->AddButton("Return To Game", Vec2(-0.5f, 0.5f), Vec2(1.f, 0.25f), m_manager))
			+= std::bind(&Menu::CloseMenu, this);
		(*m_canvas->AddButton(std::string("Settings"), Vec2(-0.5f, 0.f), Vec2(1.f, 0.25f), m_manager)) 
			+= std::bind(&Menu::OpenBindingMenu, this, false);
		(*m_canvas->AddButton("Quit", Vec2(-0.5f, -0.5f), Vec2(1.f, 0.25f), m_manager)) 
			+= std::bind(&CRenderWindow::Quit, m_scene->GetWindow());
	}

	void	Menu::OpenGameOverMenu()
	{
		if (!m_isActive)
			return;
		ClearMenu();
		m_canvas->AddText("GAME OVER!", Vec2(-0.5f, 0.7f), Vec2(1.f, 0.5f), m_manager, 2.5f);
		(*m_canvas->AddButton("Restart", Vec2(-0.2f, 0.f), Vec2(0.5f, 0.25f), m_manager, 1, Vec3(1.f), Vec4(0.f,0.f,0.f,0.4f)))
			+= std::bind(&Core::DataStructure::SceneManager::ReloadLevel, m_sceneManager);
		(*m_canvas->AddButton<unsigned>("Back to main menu", Vec2(-0.5f, -0.5f), Vec2(1.2f, 0.25f), 0, m_manager, 1, Vec3(1.f), Vec4(0.f,0.f,0.f,0.4f)))
			+= std::bind(&Core::DataStructure::SceneManager::ChangeScene, m_sceneManager, 0);
	}

	void	Menu::Init(InputManager* im, Resources::ResourcesManager* rm, Resources::Scene* s, Core::DataStructure::SceneManager* sm)
	{
		m_manager = rm;
		m_input = im;
		m_scene = s;
		m_sceneManager = sm;
		m_canvas->SetInput(im);
	}

	void	Menu::OpenBindingMenu(const bool& b)
	{
		if (!m_isActive)
			return;
		ClearMenu();
		if (b)
			m_canvas->AddSprite(MAINBACKGROUND, Vec2(0, 0), Vec2(2, 2), m_manager);
		m_canvas->AddButton("", Vec2(-1.f, 1.f), Vec2(2.f), m_manager, 1, Vec3(0.f), Vec4(0.f, 0.f, 0.f, 0.7f));
		float	x{ -0.9f }, y{ 0.9f };
		TemplateButton<Action>* button{ nullptr };
		for (Action i{ 0 }; i < Action::Count; i = (Action)((unsigned)i + 1), y -= 0.2f)
		{
			if (y < -0.8f)
			{
				y = 0.9f;
				x += 1.f;
			}
			std::string     s{ SDL_GetScancodeName((SDL_Scancode)m_input->GetKey((Action)i)) };
			button = m_canvas->AddButton<Action>(s, Vec2(x + 0.3f, y), Vec2(0.4f, 0.15f), i, m_manager, 0.5f, { 1.f,1.f,1.f }, {0.4f,0.4f,0.4f,1.f});
			(*button) += std::bind(static_cast<void (InputManager::*)(Action, TemplateButton<Action>*)>(
					&InputManager::CheckKeyToAssign), m_input, (Action)i, button);
			m_canvas->AddText(m_input->GetName((Action)i), Vec2(x, y), Vec2(0.15f, 0.15f), m_manager, 0.5f, {1.f,1.f,0.f});
		}
		if (b)
			(*m_canvas->AddButton("Done", Vec2(0.1f, -0.5f), Vec2(0.4f, 0.2f), m_manager, 0.5f, { 1.f,1.f,1.f }, { 0.4f,0.4f,0.4f,1.f }))
				+= std::bind(&Menu::OpenMainMenu, this);
		else
		{
			(*m_canvas->AddButton("Done", Vec2(0.1f, -0.5f), Vec2(0.4f, 0.2f), m_manager, 0.5f, { 1.f,1.f,1.f }, { 0.4f,0.4f,0.4f,1.f }))
				+= std::bind(&Menu::OpenPauseMenu, this);
			m_isInMenu = false;
		}

	}
}



