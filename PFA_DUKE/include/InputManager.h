#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <unordered_map>
#include <SDL2/SDL.h>
#include "Simulation.h"
#include "Event.hpp"
#include "TemplateButton.hpp"

enum class Action : unsigned int
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP,
	SUBMIT,
	WEAPON1,
	WEAPON2,
	WEAPON3,
	WEAPON4,
	WEAPON5,
	USE,

	Count,
};

class		InputManager
{
	private:
		std::unordered_map<unsigned int, Action>		m_actionMap;
		std::unordered_map<Action, unsigned int>		m_keyMap;
		int												m_framesInState[(size_t)Action::Count];
		Action											m_toChange {Action::FORWARD};
		UI::TemplateButton<Action>*						m_changeButton{nullptr};
		bool											m_assign;
		bool											m_isAction[(size_t)Action::Count];
		Core::Event<const int, const int, SDL_Window*>	m_onClick;
		Core::Event<>									m_onDelete;
			bool										m_wheelUp;
			bool										m_wheelDown;
	public:
		InputManager();
		~InputManager();

		bool            ProcessEvents(CSimulation& simulation);
		void            ResetKeys();
		void            Reshape(CSimulation& simulation, uint width, uint height);
		void            CheckKeyToAssign(Action action, UI::TemplateButton<Action>*);
		void            CheckKeyToAssign(CSimulation*);
		bool            IsDoAction(Action key);
		unsigned        GetKey(Action action);
		int             FramesInState(Action key);
		void            AssignNewKey(Action action, SDL_Event& event);
		void            AssignKey(Action action, CSimulation*);

		inline bool		IsAssigning() const {return m_assign;};

		std::string     GetName(Action action);

		void			AddOnClick(std::function<void(const int&, const int&, SDL_Window*)>);
		void			RemoveOnClick(std::function<void(const int&, const int&, SDL_Window*)>);
		void			AddOnDelete(std::function<void()>);
		void			RemoveOnDelete(std::function<void()>);

		void			SetUpWheel(bool up);
		void			SetDownWheel(bool down);
		inline bool		GetUpWheel() const { return m_wheelUp; };
		inline bool		GetDownWheel() const { return m_wheelDown; };
};

#endif // !_INPUT_MANAGER_H_
