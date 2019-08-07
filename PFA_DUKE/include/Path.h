#ifndef __PATH__
#define __PATH__

#include "NavMesh.h"
#include <list>

namespace AI
{
	struct	Path
	{
		std::list<NavMeshNode*>	nodes;

		bool	AddNode(NavMeshNode*);
	};
}

#endif
