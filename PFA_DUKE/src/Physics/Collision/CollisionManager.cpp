#include "CollisionManager.h"
#include "GameObject.hpp"
#include "Maths.hpp"
#include "Rigidbody.h"

#include <algorithm>

using namespace Core::Maths;
using namespace Core::Maths::Geometry;

namespace Physics::Collision
{
	CollisionManager*	CollisionManager::m_instance = nullptr;

	float min(const float& f1, const float& f2) { return f1 < f2 ? f1 : f2;}
	float max(const float& f1, const float& f2) { return f1 > f2 ? f1 : f2;}

	CollisionManager::CollisionManager()
	{
		m_instance = this;
	}

	void CollisionManager::AddCollider(Collider* collider)
	{
		if (collider)
			m_colliders.push_back(collider);
	}

	void CollisionManager::RemoveCollider(Collider* c)
	{
		m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), c));
	}

	void CollisionManager::ClearColliders()
	{
		m_colliders.clear();
	}

	void CollisionManager::Update(const float& deltaTime)
	{
		m_deltaTime = deltaTime;
		int count{(int)m_colliders.size()};

		for (int i = 0; i < count - 1; i++)
		{
			if (m_colliders[i]->IsEnable())
			{
				switch (m_colliders[i]->Type())
				{
				case CollisionType::BOX: ColliderBoxWith((BoxCollider*)(m_colliders[i]), i + 1); break;
				case CollisionType::SPHERE: ColliderSphereWith((SphereCollider*)(m_colliders[i]), i + 1); break;
				default: break;
				}
			}
		}
	}


	//private methodes
	void CollisionManager::ColliderBoxWith(BoxCollider* box, const GLuint& startIndex)
	{
		if (!box)
			return;

		Collider* collider = nullptr;
		GLuint count{(GLuint)m_colliders.size()};
		for (GLuint i = startIndex; i < count; i++)
		{
			collider = m_colliders[i];

			if (collider->IsEnable())
			{
				if (box->GetObject()->IsStatic() && collider->GetObject()->IsStatic())
					continue;

				switch (collider->Type())
				{
				case CollisionType::BOX: ColliderBoxBox(box, (BoxCollider*)(collider)); break;
				case CollisionType::SPHERE: ColliderBoxSphere(box, (SphereCollider*)(collider)); break;
				default: break;
				}
			}
		}
	}

	void CollisionManager::ColliderSphereWith(SphereCollider* sphere, const GLuint& startIndex)
	{
		if (!sphere)
			return;

		Collider* collider = nullptr;
		GLuint count{(GLuint)m_colliders.size()};
		for (GLuint i = startIndex; i < count; i++)
		{
			collider = m_colliders[i];

			if (collider->IsEnable())
			{
				if (sphere->GetObject()->IsStatic() && collider->GetObject()->IsStatic())
					continue;

				switch (collider->Type())
				{
				case CollisionType::BOX: ColliderBoxSphere(static_cast<BoxCollider*>(collider), sphere); break;
				default: break;
				}
			}
		}
	}

	bool CollisionManager::ColliderBoxSphere(BoxCollider* box, SphereCollider* sphere)
	{
		Hit hit;

		hit.ColliderA() = sphere;
		hit.ColliderB() = box;

		if (!box->isTrigger())
		{
			Rigidbody*	r = sphere->GetObject()->GetComponent<Rigidbody>();
			if (!r)
				return false;

			if (MovingSphereStaticOrientedBox(OrientedBox(box), Sphere(sphere),
							r->Velocity(), m_deltaTime, hit))
			{
				r->CollisionHit(hit);
				return true;
			}
		}
		else
		{
			Rigidbody*	r = sphere->GetObject()->GetComponent<Rigidbody>();
			if (!r)
				return false;
			if (OrientedBoxSphere(OrientedBox(box), Sphere(sphere)))
			{
				r->CollisionTrigger((Collider*)box);
				return true;
			}
			return false;
		}

		return false;
	}

	bool CollisionManager::ColliderBoxBox(BoxCollider* boxA, BoxCollider* boxB)
	{
		return (OrientedBoxOrientedBox(OrientedBox(boxA), OrientedBox(boxB)));
	}

	bool CollisionManager::OrientedBoxSphere(const OrientedBox& box, const Sphere& sphere)
	{
		float radius{sphere.Radius()};
		Vec3 localSpherePos;
		localSpherePos.x = (sphere.Position() - box.position()).Dot(box.I());
		localSpherePos.y = (sphere.Position() - box.position()).Dot(box.J());
		localSpherePos.z = (sphere.Position() - box.position()).Dot(box.K());

		if (abs(localSpherePos.x) > box.X() + radius ||
			abs(localSpherePos.y) > box.Y() + radius ||
			abs(localSpherePos.z) > box.Z() + radius)
				return false;

		Vec3 p(Clamp(-box.X(), box.X(), (const float)localSpherePos.x),
				Clamp(-box.Y(), box.Y(), (const float)localSpherePos.y),
				Clamp(-box.Z(), box.Z(), (const float)localSpherePos.z));

		return (p - localSpherePos).SqrLength() < radius * radius;
	}

	bool CollisionManager::OrientedBoxOrientedBox(const OrientedBox& boxA, const OrientedBox& boxB)
	{
		if (!BoxBox(boxA.AABB(), boxB.AABB()))
			return false;

		Vec3 iA(boxA.I());
		Vec3 jA(boxA.J());
		Vec3 kA(boxA.K());
		Vec3 iB(boxB.I());
		Vec3 jB(boxB.J());
		Vec3 kB(boxB.K());

		// Get the 8 points of each box
		Vec3* pointsA{boxA.Points()};
		Vec3* pointsB{boxB.Points()};

		if (SatOrientedBoxOrientedBox(pointsA, pointsB, iA))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, jA))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, kA))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, iB))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, jB))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, kB))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, iA.Cross(iB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, iA.Cross(jB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, iA.Cross(kB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, jA.Cross(iB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, jA.Cross(jB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, jA.Cross(kB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, kA.Cross(iB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, kA.Cross(jB)))
			return false;
		if (SatOrientedBoxOrientedBox(pointsA, pointsB, kA.Cross(kB)))
			return false;


		delete[] pointsA;
		delete[] pointsB;

		return true;
	}

	bool CollisionManager::BoxBox(const Box& boxA, const Box& boxB)
	{
		Vec3 posA(boxA.Position());
		Vec3 posB(boxB.Position());

		return !(posA.x + boxA.X() < posB.x - boxB.X() || posB.x + boxB.X() < posA.x - boxA.X() ||
				 posA.y + boxA.Y() < posB.y - boxB.Y() || posB.y + boxB.Y() < posA.y - boxA.Y() ||
				 posA.z + boxA.Z() < posB.z - boxB.Z() || posB.z + boxB.Z() < posA.z - boxA.Z());
	}

	bool CollisionManager::SatOrientedBoxOrientedBox(Vec3* pointsA, Vec3* pointsB, const Vec3& normal)
	{
		if (!pointsA || !pointsB)
			return false;

		float minA{ FLT_MAX };
		float maxA{ -FLT_MAX };
		float minB{ FLT_MAX };
		float maxB{ -FLT_MAX };
		float valA{0};
		float valB{0};

		for (int i = 0; i < 8; i++)
		{
			valA = normal.Dot(pointsA[i]);
			minA = min(minA, valA);
			maxA = max(maxA, valA);

			valB = normal.Dot(pointsB[i]);
			minB = min(minB, valB);
			maxB = max(maxB, valB);
		}

		return (minA > maxB || minB > maxA);
	}

	bool CollisionManager::MovingSphereStaticOrientedBox(const OrientedBox& orientedBox, const Sphere& sphere,
				const Vec3& vectSphereSpeed, const float& deltaTime, Hit& hit)
	{
		Segment AB(sphere.Position(), sphere.Position() + vectSphereSpeed * deltaTime);
		OrientedBox boxMinkowski(orientedBox, sphere.Radius());
		
		if (!SegmentOrientedBox(AB, boxMinkowski, hit))
		{
			Vec3 localA;
			localA.x = (AB.A() - orientedBox.position()).Dot(orientedBox.I());
			localA.y = (AB.A() - orientedBox.position()).Dot(orientedBox.J());
			localA.z = (AB.A() - orientedBox.position()).Dot(orientedBox.K());

			if (abs(localA.x) < abs(orientedBox.box().X()) &&
				abs(localA.y) < abs(orientedBox.box().Y()) &&
				abs(localA.z) < abs(orientedBox.box().Z()))
					return false;

			if (OrientedBoxSphere(orientedBox, Sphere(AB.B(), sphere.Radius())))
			{
				Vec3 point(orientedBox.GetNearestPoint(AB.A()));
				Segment* array = orientedBox.GetSegmentsWithThisPoint(point);
				Hit tmpHit;
				bool alreadyHit{false};

				if (SegmentSphere(AB, Sphere(point, sphere.Radius()), tmpHit))
				{
					hit = tmpHit;
					alreadyHit = true;
				}

				for (int i = 0; i < 3; i++)
				{
					if (SegmentFiniteCylinder(AB, Cylinder(array[i], sphere.Radius()), tmpHit))
					{
						if (alreadyHit)
						{
							if (AB.A().Nearest(tmpHit.Position(), hit.Position()))
								hit = tmpHit;
						}
						else
						{
							hit = tmpHit;
							alreadyHit = true;
						}
					}
				}

				delete[] array;
				
				
				if (!alreadyHit)
				{
					if (OrientedBoxSphere(orientedBox, sphere))
					{

						Vec3	d = sphere.Position() - orientedBox.position();
						float	dx = d.Dot(orientedBox.I());
						if (dx > orientedBox.X())	dx = orientedBox.X();
						if (dx < -orientedBox.X()) dx = -orientedBox.X();

						float	dy = d.Dot(orientedBox.J());
						if (dy > orientedBox.Y())	dy = orientedBox.Y();
						if (dy < -orientedBox.Y()) dy = -orientedBox.Y();
						
						float	dz = d.Dot(orientedBox.K());
						if (dz > orientedBox.Z())	dz = orientedBox.Z();
						if (dz < -orientedBox.Z()) dz = -orientedBox.Z();

						Vec3	p = orientedBox.position() + orientedBox.I() * dx + orientedBox.J() * dy + orientedBox.K() * dz;
						hit.Normal() = (sphere.Position() - p).Normalize();
						hit.Penetration() = 1 + sphere.Radius() - (sphere.Position() - p).Length();
						hit.Position() = p + hit.Normal() * sphere.Radius();
						
						return true;
					}
				}

				return alreadyHit;
			}
			return false;
		}

		int counter{orientedBox.GetNumberPlaneGoodSide(hit.Position())};

		if (counter == 1)
			return true;
		else if (counter == 2)
		{
			Cylinder cylinder(orientedBox.GetNearestSegment(hit.Position()), sphere.Radius());
			if (SegmentFiniteCylinder(AB, cylinder, hit))
				return true;
		}
		else // counter == 3
		{
			Sphere sphere(orientedBox.GetNearestPoint(hit.Position()), sphere.Radius());
			if (SegmentSphere(AB, sphere, hit))
				return true;
		}

		return false;
	}

	bool CollisionManager::SegmentPlane(const Segment& seg, const Plane& plane, Hit& hit)
	{
		// Calcul of the progression, from A to B, of the intersection point between the plane and the staight ligne of the segment
		if (plane.Normal().Dot(seg.AB()) == 0)
			return false;

		float	t{ (plane.D() - plane.Normal().Dot(seg.A())) / plane.Normal().Dot(seg.AB()) };

		if (t >= 0 && t <= 1)
		{
			hit.Position() = seg.GetPoint(t);
			hit.Normal() = (seg.AB().Dot(plane.Normal()) > 0) ?
											plane.Normal() * -1 : plane.Normal();
			return true;
		}

		return false;
	}

	bool CollisionManager::SegmentQuad(const Segment& seg, const Quad& quad, Hit& hit)
	{
		Plane plane(quad.K(), quad.Position());

		if (SegmentPlane(seg, plane, hit))
		{
			// Convertion Global to Local
			Vec3 localHit;
			localHit.x = (hit.Position() - quad.Position()).Dot(quad.I());
			localHit.y = (hit.Position() - quad.Position()).Dot(quad.J());
			localHit.z = (hit.Position() - quad.Position()).Dot(quad.K());

			if (abs(localHit.x) < abs(quad.X()) &&
					abs(localHit.y) < abs(quad.Y()))
				return true;
		}

		return false;
	}

	bool CollisionManager::SegmentSphere(const Segment& seg, const Sphere& sphere, Hit& hit)
	{
		// Check if non collision
		if (SegmentSphereNonCollision(seg, sphere))
			return false;

		Vec3 AB(seg.AB());
		Vec3 wO(sphere.Position() * -1);
		float a{ AB.SqrLength() };
		float b{ 2 * (AB.Dot(seg.A() - sphere.Position())) };
		float c{ -(sphere.Radius() * sphere.Radius()) +
					(seg.A() - sphere.Position()).SqrLength() };
		float t{ QuadraticSolver(a, b, c) };

		if (t >= 0 && t <= 1)
		{
			hit.Position() = seg.GetPoint(t);
			hit.Normal() = (hit.Position() - sphere.Position()).Normalize();
			return true;
		}

		return false;
	}

	bool CollisionManager::SegmentSphereNonCollision(const Segment& seg, const Sphere& sphere)
	{
		float sqRradius{ sphere.Radius() * sphere.Radius() };

		// A is inside the sphere ?
		if ((seg.A() - sphere.Position()).SqrLength() < sqRradius)
			return true;

		// Nearest segment's point to sphere out of the sphere ?
		if (SqrtDistanceSegmentPoint(seg, sphere.Position()) > sqRradius)
			return true;

		return false;
	}

	bool CollisionManager::SegmentInfiniteCylinder(const Segment& seg, const Cylinder& cylinder, Hit& hit)
	{
		// Check if non collision
		if (SegmentInfiniteCylinderNonCollision(seg, cylinder))
			return false;

		// Get collision point
		return SegmentInfiniteCylinderCollisionPoint(seg, cylinder, hit);
	}

	bool CollisionManager::SegmentInfiniteCylinderCollisionPoint(const Segment& seg,
						const Cylinder& cylinder, Hit& hit)
	{
		Vec3 ABcrossPQ(seg.AB().Cross(cylinder.GetSegment().AB().Normalize()));
		Vec3 PAcrossPQ((seg.A() - cylinder.GetSegment().B()).Cross(cylinder.GetSegment().AB().Normalize()));

		float a{ ABcrossPQ.SqrLength() };
		float b{ 2 * (PAcrossPQ.Dot(ABcrossPQ)) };
		float c{ PAcrossPQ.SqrLength() - cylinder.SqrtRadius() };
		float t{ QuadraticSolver(a, b, c) };

		if (t >= 0 && t <= 1)
		{
			hit.Position() = seg.GetPoint(t);
			hit.Normal() = (hit.Position() - NearestPointOnLine(cylinder.GetSegment(), hit.Position())).Normalize();
			return true;
		}

		return false;
	}

	bool CollisionManager::SegmentInfiniteCylinderNonCollision(const Segment& seg, const Cylinder& cylinder)
	{
		Vec3 PQ(cylinder.GetSegment().AB());
		Vec3 PA(seg.A() - cylinder.GetSegment().A());

		// A is inside the infinite cylinder ?
		if (PA.Cross(PQ.Normalize()).SqrLength() < cylinder.Radius())
			return true;

		Vec3 AB(seg.AB());

		if (PQ.Collinear(AB))
			return true;

		return false;
	}

	bool CollisionManager::SegmentFiniteCylinder(const Segment& seg, const Cylinder& cylinder, Hit& hit)
	{
		// Check if non collision
		if (SegmentFiniteCylinderNonCollision(seg, cylinder))
			return false;

		// Get the infinite cylinder collision point
		if (SegmentInfiniteCylinder(seg, cylinder, hit))
			// Check if intersection point is between P and Q
			if ((hit.Position() - cylinder.GetSegment().A()).Dot(cylinder.GetSegment().AB()) > 0)
				return true;

		// Get the nearest point(P or Q) for A
		bool isP{ (seg.A() - cylinder.GetSegment().A()).SqrLength() > (seg.A() - cylinder.GetSegment().B()).SqrLength() };
		Vec3 point(isP ? cylinder.GetSegment().A() : cylinder.GetSegment().B());
		Plane plane(cylinder.GetSegment().AB(), point);

		if (SegmentPlane(seg, plane, hit))
			if ((hit.Position() - point.SqrLength() < cylinder.SqrtRadius()))
				return true;

		return false;
	}

	bool CollisionManager::SegmentFiniteCylinderNonCollision(const Segment& seg, const Cylinder& cylinder)
	{
		// Check if A is outside the capsule
		if (SqrtDistanceSegmentPoint(cylinder.GetSegment(), seg.A()) > cylinder.SqrtRadius())
			return false;


		Vec3 PQ(cylinder.GetSegment().AB());
		Vec3 PA(seg.A() - cylinder.GetSegment().A());
		Vec3 QA(seg.A() - cylinder.GetSegment().B());
		
		// A is inside the finite cylinder ?
		if (PA.Dot(PQ) > 0 || QA.Dot(PQ) < 0)
			return true;

		return false;
	}

	bool CollisionManager::SegmentCapsule(const Segment& seg, const Capsule& capsule, Hit& hit)
	{
		// Check if A is inside the capsule
		if (SqrtDistanceSegmentPoint(capsule.GetSegment(), seg.A()) < capsule.SqrtRadius())
			return false;

		bool cylinderCollide{SegmentFiniteCylinder(seg, capsule, hit)};
		Hit tmpHit;
		bool alreadyHit{false};

		// Check with top sphere
		if (!cylinderCollide || (hit.Position() - capsule.GetSegment().A()).SqrLength() < capsule.SqrtRadius())
			if (SegmentSphere(seg, Sphere(capsule.GetSegment().A(), capsule.Radius()), tmpHit))
			{
				alreadyHit = true;
				hit = tmpHit;
			}

		// Check with bottom sphere
		if (!cylinderCollide || (hit.Position() - capsule.GetSegment().B()).SqrLength() < capsule.SqrtRadius())
			if (SegmentSphere(seg, Sphere(capsule.GetSegment().B(), capsule.Radius()), tmpHit))
			{
				if (alreadyHit)
				{
					if ((tmpHit.Position() - seg.A()).SqrLength() < (hit.Position() - seg.A()).SqrLength())
						hit = tmpHit;
				}
				else
					hit = tmpHit;
			}

		return cylinderCollide;
	}

	bool CollisionManager::SegmentAABB(const Segment& seg, const Box& box, Hit& hit)
	{
		if (abs(seg.A().x) < abs(box.X()) &&
			abs(seg.A().y) < abs(box.Y()) &&
			abs(seg.A().z) < abs(box.Z()))
			return false;

		Vec3 i(1, 0, 0);
		Vec3 j(0, 1, 0);
		Vec3 k(0, 0, 1);
		Vec3 x(box.X(), 0, 0);
		Vec3 y(0, box.Y(), 0);
		Vec3 z(0, 0, box.Z());

		Quad* array = new Quad[6];

		array[0] = Quad(x, j, k, i, box.Y(), box.Z());
		array[1] = Quad(-x, j, k, -i, box.Y(), box.Z());
		array[2] = Quad(y, k, i, j, box.Z(), box.X());
		array[3] = Quad(-y, k, i, -j, box.Z(), box.X());
		array[4] = Quad(z, i, j, k, box.X(), box.Y());
		array[5] = Quad(-z, i, j, -k, box.X(), box.Y());

		Hit tmpHit;
		bool alreadyHit{false};

		for (int i = 0; i < 6; i++)
		{
			if (SegmentQuad(seg, array[i], tmpHit))
			{
				if (alreadyHit)
				{
					if ((tmpHit.Position() - seg.A()).SqrLength() < (hit.Position() - seg.A()).SqrLength())
						hit = tmpHit;
				}
				else
				{
					alreadyHit = true;
					hit = tmpHit;
				}
			}
		}

		delete[] array;

		return alreadyHit;
	}

	bool CollisionManager::SegmentOrientedBox(const Segment& seg, const OrientedBox& orientedBox, Hit& hit)
	{
		Vec3 localA;
		localA.x = (seg.A() - orientedBox.position()).Dot(orientedBox.I());
		localA.y = (seg.A() - orientedBox.position()).Dot(orientedBox.J());
		localA.z = (seg.A() - orientedBox.position()).Dot(orientedBox.K());
		Vec3 localB;
		localB.x = (seg.B() - orientedBox.position()).Dot(orientedBox.I());
		localB.y = (seg.B() - orientedBox.position()).Dot(orientedBox.J());
		localB.z = (seg.B() - orientedBox.position()).Dot(orientedBox.K());

		if (!SegmentAABB(Segment(localA, localB), orientedBox.box(), hit))
			return false;


		// Convertion Local to Global
		hit.Position() = orientedBox.position() + orientedBox.I() * hit.Position().x +
													orientedBox.J() * hit.Position().y +
													orientedBox.K() * hit.Position().z;

		hit.Normal() = orientedBox.I() * hit.Normal().x +
						orientedBox.J() * hit.Normal().y +
						orientedBox.K() * hit.Normal().z;

		hit.Normal().Normalize();

		return true;
	}

	float CollisionManager::QuadraticSolver(const float& a, const float& b, const float& c)
	{
		float delta{ b*b - 4*a*c };
		float t{ -1 };

		if (delta == 0)
			t = -b / (2 * a);
		else if (delta > 0)
		{
			float sqrtDelta{ sqrt(delta) };
			float t1{ (-b - sqrtDelta) / (2 * a) };
			float t2{ (-b + sqrtDelta) / (2 * a) };

			if (t1 < 0 || t2 < 0)
				t = (t1 < 0) ? t2 : t1;
			else
				t = (t1 > t2) ? t2 : t1;
		}

		return t;
	}

	float CollisionManager::SqrtDistanceSegmentPoint(const Segment& seg, const Vec3 point)
	{
		Vec3 AB(seg.AB());
		float k{ (point - seg.A()).Dot(AB) / AB.Dot(AB) };
		Vec3 I;

		if (k <= 0)
			I = seg.A();
		else if (k >= 1)
			I = seg.B();
		else
			I = seg.A() + k * AB;

		return (I - point).SqrLength();
	}

	Vec3 CollisionManager::NearestPointOnLine(const Segment& seg, const Vec3& point)
	{
		Vec3 AP(point - seg.A());
		Vec3 AB(seg.AB());

		return seg.GetPoint(AP.Dot(AB) / AB.SqrLength());
	}

	bool	CollisionManager::Raycast(const Vec3& startPos, const Vec3& direction, const float& distance, RaycastHit& hit)
	{
		if (!m_instance)
			return false;

		hit.Distance() = FLT_MAX;

		Segment			ray(startPos, startPos + direction.Normalize() * distance);
		Hit				tmpHit;
		Collider*		collider{ nullptr };
		float			dist{ 0.f };
		unsigned int	size{ (unsigned int)m_instance->m_colliders.size() };
		bool			collision{ false };


		for (unsigned int i = 0; i < size; i++)
		{
			collision = false;
			collider = m_instance->m_colliders[i];

			if (collider->IsEnable() && !collider->isTrigger())
			{
				switch (collider->Type())
				{
				case CollisionType::BOX:	collision = m_instance->SegmentOrientedBox(ray, (BoxCollider*)(collider), tmpHit);
					break;
				case CollisionType::SPHERE:	collision = m_instance->SegmentSphere(ray, (SphereCollider*)(collider), tmpHit);
					break;
				default: break;
				}

				if (collision && (dist = (tmpHit.Position() - startPos).Length()) < hit.Distance())
				{
					hit.Collider() = collider;
					hit.Position() = tmpHit.Position();
					hit.Normal() = tmpHit.Normal();
					hit.Distance() = dist;
				}
			}
		}

		return hit.Distance() != FLT_MAX;
	}
}
