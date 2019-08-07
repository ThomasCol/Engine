#ifndef __MENU__
#define __MENU__

#include <SDL2/SDL.h>
#include <vector>

#include "Canvas.hpp"
#include "Component.h"
#include "ResourcesManager.h"
#include "Sprites.h"
#include "SceneManager.h"

namespace Resources
{
	class Scene;
}

namespace UI
{
	class Menu : virtual public Core::DataStructure::Component
	{
	private:
		Canvas*								m_canvas;
		Resources::ResourcesManager*		m_manager;
		InputManager*						m_input;
		Resources::Scene*					m_scene;
		Core::DataStructure::SceneManager*	m_sceneManager;
		bool								m_isActive;
		bool								m_isInMenu;
	protected:
	public:
		Menu();
		Menu(const Menu&);
		Menu(Core::DataStructure::GameObject*);
		virtual ~Menu() = default;

		Menu&	operator= (const Menu&);

		const std::type_info&		GetType() const override;
		Core::DataStructure::Component*	Copy() const override;

		void	Init(InputManager*, Resources::ResourcesManager*, Resources::Scene*, Core::DataStructure::SceneManager*);

		void	ClearMenu();
		void	CloseMenu();
		void	OpenMainMenu();
		void	OpenWinMenu();
		void	OpenGameOverMenu();
		void	OpenPauseMenu();
		void	OpenBindingMenu(const bool&);

		inline void	Enable() { m_isActive = true; m_canvas->Enable(); }
		inline void	Disable() { m_isActive = false; m_canvas->Disable(); }
	};
}


#endif
