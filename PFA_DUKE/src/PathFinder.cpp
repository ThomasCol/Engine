#include <algorithm>

#include "PathFinder.h"

namespace AI
{
	PathFinder* PathFinder::m_instance{ nullptr };

	PathFinder::~PathFinder()
	{
		delete m_nav;
	}

	PathFinder*	PathFinder::Instance()
	{
		if (m_instance == nullptr)
			m_instance = new PathFinder;
		return m_instance;
	}

	void	PathFinder::Init(const Level& level)
	{
		if (m_nav != nullptr)
			delete m_nav;
		m_nav = new NavMesh(level);
	}

	void	PathFinder::Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}


	Path	PathFinder::FindPath(const Core::Maths::Vec3& pos, const Core::Maths::Vec3& dest)
	{
		NavMeshNode*	posNode{ m_nav->GetClosestNode(pos) };
		NavMeshNode*	destNode{ m_nav->GetClosestNode(dest) };

		Path p;
		std::list<NavMeshNode*> l;
		FindPath(posNode, destNode, p, l);

		if (p.nodes.size() > 3)
			OptimizePath(p);

		return p;
	}

	/* removes unneeded nodes in path */
	void	PathFinder::OptimizePath(Path& p) const
	{
		std::list<NavMeshNode*>::iterator	it{ ++p.nodes.begin() }, prevIt{ p.nodes.begin() };
		std::list<NavMeshNode*>::iterator	it2, it3;
		Vec3	dir{ (*it)->pos - (*prevIt)->pos }, prevDir;
		++prevIt;
		for (++it; it != --(--p.nodes.end()); ++it)
		{
			for (it2 = it, ++(++it2); it2 != p.nodes.end(); ++it2)
			{
				for (it3 = (*it2)->nodes.begin(); it3 != (*it2)->nodes.end(); ++it3)
				{
					if (*it3 == *it)
					{
						p.nodes.erase((++it)--, it2);
						it = it2;
					}
				}
			}
		}
		it = ++p.nodes.begin();
		while (it != p.nodes.end())
		{
			prevDir = dir;
			dir = { (*it)->pos - (*prevIt)->pos };

			/* operator== of Vec3 compares length, making my own here */
			if ((prevDir.x == dir.x) && (prevDir.y == dir.y) && (prevDir.z == dir.z))
				p.nodes.erase(prevIt);
			prevIt = it++;
		}
	}

	bool	compareDist(const std::pair<float, NavMeshNode*>& a, const std::pair<float, NavMeshNode*>& b)
	{
		return a.first < b.first;
	}

	bool	PathFinder::FindPath(NavMeshNode* pos, NavMeshNode* dest, Path& p, std::list<NavMeshNode*>& usedNodes)
	{
		for (auto it2{ usedNodes.begin() }; it2 != usedNodes.end(); ++it2)
		{
			if (*it2 == pos)
				return false;
		}
		usedNodes.push_back(pos);
		if (!p.AddNode(pos))
			return false;

		std::list<std::pair<float, NavMeshNode*>>	childs;
		for (auto it{ pos->nodes.begin() }; it != pos->nodes.end(); ++it)
		{
			if (*it == pos)
				continue;
			for (auto it2{ usedNodes.begin() }; it2 != usedNodes.end(); ++it2)
				if (*it2 == *it)
					continue;
			childs.push_back(std::make_pair(((*it)->pos - dest->pos).SqrLength(), *it));
		}


		childs.sort(compareDist);
		for (auto it{ childs.begin() }; it != childs.end(); ++it)
		{
			if ((*it).second == dest && p.AddNode((*it).second))
				return true;
			if (FindPath((*it).second, dest, p, usedNodes))
				return true;
		}


		p.nodes.pop_back();
		return false;
	}
}