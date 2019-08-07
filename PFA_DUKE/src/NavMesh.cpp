#include <queue>

#include "NavMesh.h"

namespace AI
{
	NavMesh::NavMesh(const Level& l)
	{
		std::list<NavMeshNode>::iterator	it{ m_nodes.begin() }, it2{ m_nodes.begin() };

		/* isPrevLink -> indicate if iterator has link with next one */
		bool	isPrevLink = false;
		/* isTopLink -> indicate if iterator has link with the same one on next row */
		std::queue<bool>				isTopLink;
		std::queue<std::list<NavMeshNode>::iterator>	topLinkIt;

		for (unsigned i{ 0 }, j{ 0 }; i < l.GetLen(); ++i)
			for (j = 0; j < l.GetLen(); ++j)
			{
				if (!InitNode(l, i, j, isTopLink, topLinkIt, isPrevLink))
					continue;
				if (m_nodes.size() > 1)
					++it;
				else
					it = m_nodes.begin();

				CheckLinks(it, isTopLink, topLinkIt, isPrevLink, i > 0);

				/* pushing data for next loop */
				isTopLink.push(true);
				topLinkIt.push(it);
				isPrevLink = true;
			}
	}

	bool		NavMesh::InitNode(const Level& l, const unsigned& i, const unsigned& j,
		std::queue<bool>& isTopLink,
		std::queue<std::list<NavMeshNode>::iterator>& topLinkIt,
		bool& isPrevLink)
	{
		if (l.m_map[i][j] != Level::Case::WALL && l.m_map[i][j] != Level::Case::WALL2
			&& l.m_map[i][j] != Level::Case::END && l.m_map[i][j] != Level::Case::END2)
			m_nodes.push_back(Core::Maths::Vec3((float)i, 2.40f, (float)j));
		/* if not, pushing values for next loop */
		else
		{
			BalanceQueue(isTopLink, topLinkIt, isPrevLink, i > 0);
			return false;
		}
		return true;
	}

	void		NavMesh::CheckLinks(std::list<NavMeshNode>::iterator& it,
		std::queue<bool>& isTopLink,
		std::queue<std::list<NavMeshNode>::iterator>& topLinkIt,
		bool& isPrevLink, const bool& b)
{
		std::list<NavMeshNode>::iterator	it2{ it };
		if (it2 != m_nodes.begin())
			--it2;

		/* if outside of first row make top link */
		if (b)
			CheckTopLink(isTopLink, topLinkIt, it);

		/* if previous tile is walkable, make link */
		CheckPrevLink(isPrevLink, it, it2);
	}

	void		NavMesh::BalanceQueue(std::queue<bool>& isTopLink,
		std::queue<std::list<NavMeshNode>::iterator>& topLinkIt,
		bool& isPrevLink, const bool& b)
	{
		topLinkIt.push(m_nodes.end());
		isTopLink.push(false);
		isPrevLink = false;

		/* if outside of first row, pop first element in list */
		if (b)
		{
			isTopLink.pop();
			topLinkIt.pop();
		}
	}

	void		NavMesh::CheckTopLink(std::queue<bool>& b, std::queue<std::list<NavMeshNode>::iterator>& itQueue,
		std::list<NavMeshNode>::iterator& it) const
	{
		/* if tile on previous row is linkable, do the link */
		if (b.front())
		{
			(*it).nodes.push_back(&(*itQueue.front()));
			(*itQueue.front()).nodes.push_back(&(*it));
		}

		/* popping link to let next loop have the corresponding one */
		b.pop();
		itQueue.pop();

	}
	void		NavMesh::CheckPrevLink(const bool& b, std::list<NavMeshNode>::iterator& it1,
		std::list<NavMeshNode>::iterator& it2) const
	{
		if (b)
		{
			(*it1).nodes.push_back(&(*it2));
			(*it2).nodes.push_back(&(*it1));
		}
	}

	NavMeshNode*	NavMesh::GetClosestNode(const Core::Maths::Vec3& pos)
	{
		auto	it{ m_nodes.begin() };
		auto	resultIt{ m_nodes.begin() };
		float	dist{ ((*it).pos - pos).SqrLength() };
		++it;
		for (float d{ 0 }; it != m_nodes.end(); ++it)
		{
			d = ((*it).pos - pos).SqrLength();
			if (d < dist)
			{
				dist = d;
				resultIt = it;
			}
		}
		return &(*resultIt);
	}
}