#ifndef __OPERATINGSYSTEM_H__
#define __OPERATINGSYSTEM_H__

#include <unordered_map>
#include "Simulation.h"
#include <SDL2/SDL.h>
#include "InputManager.h"

class CRenderWindow
{
public:
	CRenderWindow();
	~CRenderWindow();

	void				Init(uint width, uint height);
	void				Reshape(uint width, uint height);
	
	void				GetMousePos( float& x, float& y );
	bool				GetMouseButton(int button);

	const CSimulation&	GetSimulation() const {return m_simulation;};
	SDL_Window*			GetWindow() { return m_window; }

	void				Quit();

	InputManager*							m_input;

private:
	SDL_Window*								m_window;
	CSimulation								m_simulation;

	bool									m_isRunning;
};

#endif