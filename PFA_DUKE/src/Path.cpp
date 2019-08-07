#include "Path.h"

namespace AI
{
	bool	Path::AddNode(NavMeshNode* node)
	{
		for (auto it{ nodes.begin() }; it != nodes.end(); ++it)
		{
			if (*it == node)
			{
				return false;
			}
		}

		nodes.push_back(node);
		return true;
	}
}

