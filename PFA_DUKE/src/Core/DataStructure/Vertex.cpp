#include "Vertex.h"

namespace Core::DataStructure
{
	Vertex::Vertex(Maths::Vec3 position, Maths::Vec2 text, Maths::Vec3 norm):
		pos {position},
		uv {text},
		normal {norm}
	{}

	Vertex::Vertex(const Vertex& ver):
		pos {ver.pos},
		uv {ver.uv},
		normal {ver.normal}
	{}

	Vertex&	Vertex::operator=(const Vertex& ver)
	{
		pos = ver.pos;
		uv = ver.uv;
		normal = ver.normal;

		return *this;
	}

	std::string	Vertex::ToString()
	{
		std::string	str = pos.ToString() + " " + uv.ToString() + " " +
																normal.ToString();

		return str;
	}
}
