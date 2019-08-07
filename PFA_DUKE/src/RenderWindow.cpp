#include "RenderWindow.h"

#include <GL/glew.h>

CRenderWindow::CRenderWindow():
	m_isRunning {true}
{
	m_input = new InputManager();
	m_simulation.SetRenderWindow(this);
}

CRenderWindow::~CRenderWindow()
{
	if (m_input != nullptr)
		delete m_input;
}

void	CRenderWindow::Init(uint width, uint height)
{
	SDL_GLContext	context;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		return;
	}

	/* Turn on double buffering with a 24bit Z buffer.
	* You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)width, (int)height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (m_window == nullptr)
	{
		return;
	}

	context = SDL_GL_CreateContext(m_window);
	SDL_GL_SetSwapInterval(1);
	SDL_ShowCursor(SDL_DISABLE);

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
        	return;
	}

	Mix_AllocateChannels(40);

	if (GLEW_OK != glewInit())
	{
		return;
	}
	m_simulation.Init(width, height);


	Uint64 currentTime = SDL_GetPerformanceCounter();
	Uint64 lastTime = 0;
	double frameTime = 0.0;

	SDL_WarpMouseInWindow(m_window, width / 2, height / 2);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_isRunning = true;
	while (m_isRunning)
	{
		if (m_input->IsAssigning())
			m_input->CheckKeyToAssign(&m_simulation);
		else if (!m_input->ProcessEvents(m_simulation))
			m_isRunning = false;

		lastTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();

		Uint64 deltaTime = (currentTime - lastTime);
		frameTime = (double)deltaTime / SDL_GetPerformanceFrequency();
		
		m_simulation.Update(frameTime, m_window);
		SDL_GL_SwapWindow(m_window);
	}


	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void	CRenderWindow::Reshape(uint width, uint height)
{
	m_simulation.Resize(width, height);
}

void	CRenderWindow::GetMousePos(float& x, float& y)
{
	int _x, _y;
	SDL_GetMouseState(&_x, &_y);

	x = (float)_x;
	y = (float)_y;
}

bool	CRenderWindow::GetMouseButton(int button)
{
	int flag = 0;
	switch (button)
	{
	case 0:	flag = SDL_BUTTON_LEFT; break;
	case 1: flag = SDL_BUTTON_MIDDLE; break;
	case 2: flag = SDL_BUTTON_RIGHT; break;
	}

	return ((SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(flag)) != 0);
}

void	CRenderWindow::Quit()
{
	m_isRunning = false;
}
