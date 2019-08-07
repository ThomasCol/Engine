#include "NavMeshNode.h"

namespace AI
{
	NavMeshNode::NavMeshNode(const Core::Maths::Vec3& p) : pos{ p }
	{
	}

	NavMeshNode::NavMeshNode(const NavMeshNode& n) : nodes{ n.nodes }, pos{ n.pos }
	{
	}
	/*
	NavMeshNode::NavMeshNode(NavMeshNode&& n) : nodes {std::move(n.nodes)}, pos {std::move(n.pos)}
	{
	}*/
}