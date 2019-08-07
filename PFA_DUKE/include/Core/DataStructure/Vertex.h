#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <string>

#include "Vec3.h"
#include "Vec2.h"

namespace Core::DataStructure
{
	/* struct Vertex for creating index array */
	struct Vertex
	{
		/* class insrance */
		Maths::Vec3	pos;
		Maths::Vec2	uv;
		Maths::Vec3	normal;

		/* constructor & destructor */
		Vertex() = default;
		Vertex(const Vertex& ver);
		Vertex(Vertex&& ver) = default;
		Vertex(Maths::Vec3 position, Maths::Vec2 text, Maths::Vec3 norm);
		~Vertex() = default;

		/* operator */
		Vertex&			operator=(const Vertex& ver);

		/* Debug func */
		std::string		ToString();
	};
}

#endif //_VERTEX_H_
