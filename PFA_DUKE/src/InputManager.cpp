#include "InputManager.h"
#include <iostream>
#include "RenderWindow.h"
#include "TemplateButton.hpp"
#include "Menu.h"

InputManager::InputManager() : m_assign {false}
{
	m_actionMap[SDL_SCANCODE_W] = Action::FORWARD;
	m_actionMap[SDL_SCANCODE_S] = Action::BACKWARD;
	m_actionMap[SDL_SCANCODE_D] = Action::RIGHT;
	m_actionMap[SDL_SCANCODE_A] = Action::LEFT;
	m_actionMap[SDL_SCANCODE_SPACE] = Action::JUMP;
	m_actionMap[SDL_SCANCODE_RETURN] = Action::SUBMIT;
	m_actionMap[SDL_SCANCODE_1] = Action::WEAPON1;
	m_actionMap[SDL_SCANCODE_2] = Action::WEAPON2;
	m_actionMap[SDL_SCANCODE_3] = Action::WEAPON3;
	m_actionMap[SDL_SCANCODE_4] = Action::WEAPON4;
	m_actionMap[SDL_SCANCODE_5] = Action::WEAPON5;
	m_actionMap[SDL_SCANCODE_E] = Action::USE;
	m_actionMap[SDL_SCANCODE_SPACE] = Action::JUMP;

	m_keyMap[Action::FORWARD] = SDL_SCANCODE_W;
	m_keyMap[Action::BACKWARD] = SDL_SCANCODE_S;
	m_keyMap[Action::RIGHT] = SDL_SCANCODE_D;
	m_keyMap[Action::LEFT] = SDL_SCANCODE_A;
	m_keyMap[Action::JUMP] = SDL_SCANCODE_SPACE;
	m_keyMap[Action::SUBMIT] = SDL_SCANCODE_RETURN;
	m_keyMap[Action::WEAPON1] = SDL_SCANCODE_1;
	m_keyMap[Action::WEAPON2] = SDL_SCANCODE_2;
	m_keyMap[Action::WEAPON3] = SDL_SCANCODE_3;
	m_keyMap[Action::WEAPON4] = SDL_SCANCODE_4;
	m_keyMap[Action::WEAPON5] = SDL_SCANCODE_5;
	m_keyMap[Action::USE] = SDL_SCANCODE_E;
	m_keyMap[Action::JUMP] = SDL_SCANCODE_SPACE;
	m_wheelUp = false;
	m_wheelDown = false;

	ResetKeys();
}

InputManager::~InputManager()
{
	m_onDelete();
}

bool	InputManager::ProcessEvents(CSimulation& simulation)
{
	//increase all frames in state by one
    for (unsigned i {0}; i < (unsigned)Action::Count; ++i)
		++m_framesInState[i];

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return false;

			case SDL_WINDOWEVENT:
				{
					switch (event.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							Reshape(simulation, event.window.data1, event.window.data2);
							break;
					}
				}
				break;

			case SDL_KEYDOWN:
				{
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						if (simulation.GetScene()->GetMenu())
							simulation.GetScene()->GetMenu()->OpenPauseMenu();
						continue;
					}

					auto itSdlKey = m_actionMap.find(event.key.keysym.scancode);
					if (itSdlKey != m_actionMap.end())
					{
						m_isAction[(size_t)itSdlKey->second] = true;
						m_framesInState[(size_t)itSdlKey->second] = 0;
					}
				}
				break;

			case SDL_KEYUP:
				{
					auto itSdlKey = m_actionMap.find(event.key.keysym.scancode);
					if (itSdlKey != m_actionMap.end())
					{
						m_isAction[(size_t)itSdlKey->second] = false;
						m_framesInState[(size_t)itSdlKey->second] = 0;
					}
				}
				break;
			case SDL_MOUSEWHEEL:
			{
				if (event.wheel.y > 0)
					m_wheelUp = true;
				else if (event.wheel.y < 0)
					m_wheelDown = true;
			}
			break;
			case SDL_MOUSEBUTTONUP:
				{
					m_onClick(event.button.x, event.button.y, simulation.GetRenderWindow()->GetWindow());
				}
				break;
		}
	}
	return true;
}

void	InputManager::ResetKeys()
{
	for (size_t i = 0; i < (size_t)Action::Count; ++i)
	{
		m_framesInState[i] = false;
		m_isAction[i] = false;
	}
}

bool    InputManager::IsDoAction(Action key)
{
	return m_isAction[(size_t)key];
}

int    InputManager::FramesInState(Action key)
{
        return m_framesInState[(size_t)key];
}

void    InputManager::CheckKeyToAssign(CSimulation* simulation)
{
	if (m_assign)
		AssignKey(m_toChange, simulation);
}

void InputManager::SetUpWheel(bool up)
{
	m_wheelUp = up;
}

void InputManager::SetDownWheel(bool down)
{
	m_wheelDown = down;
}

void    InputManager::CheckKeyToAssign(Action action, UI::TemplateButton<Action>* b)
{
	if (!m_assign)
	{
		m_assign = true;
		m_toChange = action;
		m_changeButton = b;
		b->SetMessage("Press any key");
	}
}

void	InputManager::AssignNewKey(Action action, SDL_Event& event)
{
	auto itAction = m_keyMap.find(action);

	m_actionMap.erase(itAction->second);
	itAction->second = event.key.keysym.scancode;

	m_actionMap[event.key.keysym.scancode] = action;
}

void	InputManager::AssignKey(Action action, CSimulation* simulation)
{
	(void)simulation;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				{
					AssignNewKey(action, event);
					m_changeButton->SetMessage(SDL_GetScancodeName((SDL_Scancode)GetKey(action)));
					LOG(INFO, "Action " + GetName(action) + " now has key "
						+ std::string(SDL_GetScancodeName((SDL_Scancode)m_keyMap[action])));
					m_assign = false;
				}
				break;
		}
	}
}

unsigned        InputManager::GetKey(Action action)
{
	return m_keyMap[action];
}

void    InputManager::Reshape(CSimulation& simulation, uint width, uint height)
{
	simulation.Resize(width, height);
}

std::string     InputManager::GetName(Action action)
{
	switch (action)
	{
		case (Action::FORWARD): return "Forward";
		case (Action::BACKWARD): return "Backward";
		case (Action::LEFT): return "Left";
		case (Action::RIGHT): return "Right";
		case (Action::JUMP): return "Jump";
		case (Action::SUBMIT): return "Submit";
		case (Action::WEAPON1): return "Pistol";
		case (Action::WEAPON2): return "Shotgun";
		case (Action::WEAPON3): return "Rifle";
		case (Action::WEAPON4): return "Rocket";
		case (Action::WEAPON5): return "RPG";
		case (Action::USE): return "Use";
		default: return "No text bound to action";
	}
}

void	InputManager::AddOnClick(std::function<void(const int&, const int&, SDL_Window*)> f)
{
	m_onClick += f;
}

void	InputManager::RemoveOnClick(std::function<void(const int&, const int&, SDL_Window*)> f)
{
	m_onClick -= f;
}

void	InputManager::AddOnDelete(std::function<void()> f)
{
	m_onDelete += f;
}

void	InputManager::RemoveOnDelete(std::function<void()> f)
{
	m_onDelete -= f;
}

