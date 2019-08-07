#ifndef __NAVMESHNODE__
#define __NAVMESHNODE__

#include <list>
#include <functional>

#include "Vec3.h"

namespace AI
{
	struct NavMeshNode
	{
		/* stores other nodes next to this one and the weight to go to them */
		std::list<NavMeshNode*>		nodes;
		Core::Maths::Vec3			pos;

		NavMeshNode() = delete;
		NavMeshNode(const Core::Maths::Vec3&);
		NavMeshNode(const NavMeshNode&);
		NavMeshNode(NavMeshNode&&) = default;

		NavMeshNode&				operator= (const NavMeshNode&) = default;
	};
}
#endif
