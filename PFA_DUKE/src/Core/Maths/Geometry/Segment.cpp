#include "Segment.h"

namespace Core::Maths::Geometry
{
	Segment::Segment(const Vec3& a, const Vec3& b):
		m_a(a),
		m_b(b)
	{}

	Segment::Segment(const Segment& seg):
		m_a(seg.m_a),
		m_b(seg.m_b)
	{}

	Segment::Segment(Segment&& seg):
		m_a(std::move(seg.m_a)),
		m_b(std::move(seg.m_b))
	{}

	Segment& Segment::operator=(const Segment& seg)
	{
		m_a = seg.m_a;
		m_b = seg.m_b;

		return *this;
	}
}
