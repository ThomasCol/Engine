#include <cstring>
#include <iostream>
#include "RenderWindow.h"


int main(int ac, char** av)
{
		CRenderWindow* window = new CRenderWindow;

		window->Init(1024, 600);

		delete window;

		return 0;
}
