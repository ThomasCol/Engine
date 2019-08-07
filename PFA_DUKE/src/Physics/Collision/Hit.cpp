#include "Hit.h"

using namespace Core::Maths;

namespace Physics::Collision
{
	Hit::Hit():	m_position(0),
				m_normal(0, 1, 0),
				m_penetration(1)
	{}

	Hit::Hit(const Vec3& position, const Vec3& normal):
				m_position(position),
				m_normal(normal),
				m_penetration(1)
	{}

	Hit::Hit(const Hit& hit):	m_position(hit.m_position),
								m_normal(hit.m_normal),
								m_penetration(hit.m_penetration)
	{}

	Hit::Hit(Hit&& hit):	m_position(std::move(hit.m_position)),
							m_normal(std::move(hit.m_normal)),
							m_penetration(std::move(hit.m_penetration))
	{}

	Hit& Hit::operator=(const Hit& hit)
	{
		m_position = hit.m_position;
		m_normal = hit.m_normal;
		m_penetration = hit.m_penetration;

		return *this;
	}
}
