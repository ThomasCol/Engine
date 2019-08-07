#include "Sphere.h"

namespace Core::Maths::Geometry
{
	Sphere::Sphere(const Vec3& position, const float& radius):
		m_position(position),
		m_radius{radius}
	{}

	Sphere::Sphere(SphereCollider* sphere)
	{
		m_position = sphere->Position();
		//std::cout << m_position;
		m_radius = sphere->Radius();
	}

	Sphere::Sphere(const Sphere& sphere):
		m_position(sphere.m_position),
		m_radius{sphere.m_radius}
	{}

	Sphere::Sphere(Sphere&& sphere):
		m_position(std::move(sphere.m_position)),
		m_radius{std::move(sphere.m_radius)}
	{}

	Sphere& Sphere::operator=(const Sphere& sphere)
	{
		m_position = sphere.m_position;
		m_radius = sphere.m_radius;

		return *this;
	}
}
