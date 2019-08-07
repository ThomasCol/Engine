#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glew.h>

#include "Vec3.h"
#include "Mat4.h"
#include "Component.h"

class		WeaponHandler;
class		CRenderWindow;
class		Mesh;

#define NBWEAPON	5
#define ANGLE		400
#define MOVE		1
#define STRAFE		5
#define CROUCHSPEED 2.5f
#define NORMALSPEED 5.0f
#define SENSIBILITY 0.05f
#define SNEAK		0.5f

using namespace	Core::Maths;
using namespace	Physics;

namespace	LowRenderer
{
	class		Camera: public virtual Core::DataStructure::Component
	{
		private:
			Vec3	m_front;
			Vec3	m_dir;
			Vec3	m_angle;
			Vec3	m_pos;
			Vec3	m_up;
			Mat4	m_matrix;
			Mat4	m_invMatrix;
			double	m_elapseTime;
			float	m_x;
			float	m_y;

		public:
			Camera();
			Camera(Vec3 pos, Vec3 up);
			Camera(Core::DataStructure::GameObject* object);
			Camera(const Camera& camera);
			Camera(Camera&& camera) = default;
			~Camera(){};

			void            				Init(Vec3 pos, Vec3 up);
			const std::type_info&			GetType() const override;
			Core::DataStructure::Component*	Copy() const override;

			inline void						Destroy() override {}

			inline Mat4						GetMatrix() const { return m_matrix; }
			inline Mat4						GetInvMatrix() const { return m_invMatrix; }
			void							SetMatrix(const Mat4& mat);
			void							SetInvMatrix();

			inline float					GetX() const { return m_x; }
			inline float					GetY() const { return m_y; }
			inline Core::Maths::Vec3		GetFront() const { return m_front; }
			inline Core::Maths::Vec3		GetView() const { return Core::Maths::Vec3((m_matrix[0][2] * -1), (m_matrix[1][2] * -1), (m_matrix[2][2] * -1)); }
			inline Core::Maths::Vec3		GetUp() const { return Core::Maths::Vec3(m_matrix[0][1], m_matrix[1][1], m_matrix[2][1]); }

			void							UpdateEye(const Vec3& pos);
			void							UpdateUp(Vec3& up);
			void							UpdateWaxis(Vec3& waxis);
			void							UpdateView(Vec3& view);
			void							UpdateCamera(const Vec3& pos);

			void							StartCamera(Vec3& pos);
			void            				Rotate(Vec3 rotate);
			void            				ChangeView(CRenderWindow* render, double frameTime);

			Camera&							operator=(const Camera& cam);
	};
}

#endif // !_CAMERA_H_
