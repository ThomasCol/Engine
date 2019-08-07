#include "RaycastHit.h"

using namespace Core::Maths;

namespace Physics::Collision
{
	RaycastHit::RaycastHit() :
		m_collider{ nullptr },
		m_position{ Vec3(0.f) },
		m_normal{ Vec3(0.f) },
		m_distance{ FLT_MAX }
	{}

	RaycastHit::RaycastHit(const RaycastHit& ray) :
		m_collider{ nullptr },
		m_position{ ray.m_position },
		m_normal{ ray.m_normal },
		m_distance{ ray.m_distance }
	{}

	RaycastHit::RaycastHit(RaycastHit&& ray) :
		m_collider{ std::move(ray.m_collider) },
		m_position{ std::move(ray.m_position) },
		m_normal{ std::move(ray.m_normal) },
		m_distance{ std::move(ray.m_distance) }
	{}

	RaycastHit& RaycastHit::operator=(const RaycastHit& ray)
	{
		m_collider = ray.m_collider;
		m_position = ray.m_position;
		m_normal = ray.m_normal;
		m_distance = ray.m_distance;

		return *this;
	}
}