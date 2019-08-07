#ifndef __COLLISIONMANAGER_H__
#define __COLLISIONMANAGER_H__

#include <vector>
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Hit.h"
#include "RaycastHit.h"
#include "Segment.h"
#include "Sphere.h"
#include "Box.h"
#include "OrientedBox.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Quad.h"

namespace Physics::Collision
{
	class CollisionManager
	{
	private:
		std::vector<Collider*>	m_colliders;
		float					m_deltaTime;

		void				ColliderBoxWith(BoxCollider* box, const GLuint& startIndex);
		void				ColliderSphereWith(SphereCollider* sphere, const GLuint& startIndex);

		bool				ColliderBoxSphere(BoxCollider* box, SphereCollider* sphere);
		bool				ColliderBoxBox(BoxCollider* boxA, BoxCollider* boxB);

		bool				OrientedBoxSphere(const Core::Maths::Geometry::OrientedBox& box, const Core::Maths::Geometry::Sphere& sphere);
		bool				OrientedBoxOrientedBox(const Core::Maths::Geometry::OrientedBox& boxA, const Core::Maths::Geometry::OrientedBox& boxB);
		bool				BoxBox(const Core::Maths::Geometry::Box& boxA, const Core::Maths::Geometry::Box& boxB);
		bool				SatOrientedBoxOrientedBox(Core::Maths::Vec3* pointsA, Core::Maths::Vec3* pointsB, const Core::Maths::Vec3& normal);

		bool				MovingSphereStaticOrientedBox(const Core::Maths::Geometry::OrientedBox& orientedBox, const Core::Maths::Geometry::Sphere& sphere,
										const Core::Maths::Vec3& vectSphereSpeed, const float& deltaTime, Hit& hit);

		bool				SegmentPlane(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Plane& plane, Hit& hit);
		bool				SegmentQuad(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Quad& quad, Hit& hit);
		bool				SegmentSphere(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Sphere& sphere, Hit& hit);
		bool				SegmentSphereNonCollision(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Sphere& sphere);
		bool				SegmentInfiniteCylinder(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Cylinder& cylinder, Hit& hit);
		bool				SegmentInfiniteCylinderNonCollision(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Cylinder& cylinder);
		bool				SegmentFiniteCylinder(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Cylinder& cylinder, Hit& hit);
		bool				SegmentInfiniteCylinderCollisionPoint(const Core::Maths::Geometry::Segment& seg,
										const Core::Maths::Geometry::Cylinder& cylinder, Hit& hit);
		bool				SegmentFiniteCylinderNonCollision(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Cylinder& cylinder);
		bool				SegmentCapsule(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Capsule& capsule, Hit& hit);
		bool				SegmentAABB(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::Box& box, Hit& hit);
		bool				SegmentOrientedBox(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Geometry::OrientedBox& orientedBox, Hit& hit);

		float				QuadraticSolver(const float& a, const float& b, const float& c);
		float				SqrtDistanceSegmentPoint(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Vec3 point);
		Core::Maths::Vec3	NearestPointOnLine(const Core::Maths::Geometry::Segment& seg, const Core::Maths::Vec3& point);

		static	CollisionManager* m_instance;

	public:
		CollisionManager();
		~CollisionManager() = default;

		void				AddCollider(Collider* collider);
		void				RemoveCollider(Collider* c);
		void				Update(const float& deltaTime);

		void				ClearColliders();

		inline void			SetActive() { m_instance = this; }
		static bool			Raycast(const Vec3& startPos, const Vec3& direction, const float& distance, Physics::Collision::RaycastHit& hit);
	};
}

#endif
