#ifndef __NAVMESH__
#define __NAVMESH__

#include <list>
#include <queue>

#include "NavMeshNode.h"
#include "Level.h"
#include "Vec2.h"

namespace AI
{
	class NavMesh
	{
	private:
		std::list<NavMeshNode>	m_nodes;

		void					CheckLinks(std::list<NavMeshNode>::iterator&, std::queue<bool>&,
									std::queue<std::list<NavMeshNode>::iterator>&,
									bool&, const bool&);

		void					CheckPrevLink(const bool&, std::list<NavMeshNode>::iterator&,
									std::list<NavMeshNode>::iterator&) const;
		void					CheckTopLink(std::queue<bool>&, std::queue<std::list<NavMeshNode>::iterator>&,
									std::list<NavMeshNode>::iterator&) const;
		void					BalanceQueue(std::queue<bool>&, std::queue<std::list<NavMeshNode>::iterator>&,
									bool&, const bool&);

		bool					InitNode(const Level& l, const unsigned&, const unsigned&, std::queue<bool>&,
								std::queue<std::list<NavMeshNode>::iterator>&, bool&);

	public:
		NavMesh() = default;
		NavMesh(const Level&);
		NavMesh(const NavMesh&) = delete;
		NavMesh(NavMesh&&) = delete;

		NavMesh&				operator= (const NavMesh&) = delete;

		NavMeshNode*			GetClosestNode(const Core::Maths::Vec3& pos);
	};
}
#endif
