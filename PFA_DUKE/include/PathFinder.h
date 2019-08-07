#ifndef __PATHFINDER__
#define __PATHFINDER__

#include "NavMesh.h"
#include "Path.h"

namespace AI
{
	//Singleton
	class PathFinder
	{
	private:
		NavMesh*			m_nav {nullptr};
		static PathFinder*	m_instance;

		void				OptimizePath(Path& p) const;

	public:
		PathFinder() = default;
		PathFinder(const PathFinder& path) = delete;
		PathFinder(PathFinder&&) = delete;
		~PathFinder();

		PathFinder&			operator= (const PathFinder&) = delete;

		static PathFinder* Instance();

		void				Init(const Level& level);
		static void			Destroy();

		Path				FindPath(const Core::Maths::Vec3&, const Core::Maths::Vec3&);
		bool				FindPath(NavMeshNode*, NavMeshNode*, Path&, std::list<NavMeshNode*>&);
	};
}
#endif
