#include "RenderWindow.h"
#include "Camera.h"
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

#include "Debug.h"

using namespace	Resources;
namespace LowRenderer
{
	Camera::Camera(): m_front{0.f, 0.f, 1.f}, m_dir{0.f, 0.f, 1.f}, 
		m_angle{0,0,0}, m_pos{0.f, 0.f, 0.f}, 
		m_x(0.f), m_y(0.f)
	{
	}

	Camera::Camera(Core::DataStructure::GameObject* object):
		Component(object)
	{
	}

	Camera::Camera(Vec3 pos, Vec3 up) :
		m_front{0,0,1},
		m_dir{0,0,1}, 
		m_angle{0,0,0},
		m_pos{0,0,0}, 
		m_x(0), m_y(0)
	{
		m_matrix = m_matrix.Identity();

		Vec3 eye{pos.x, pos.y, pos.z};
		m_matrix[0][1] = up.x;
		m_matrix[1][1] = up.y;
		m_matrix[2][1] = up.z;

		StartCamera(eye);
	}

	Camera::Camera(const Camera& camera):
		Component(camera.m_object) 
	{
		m_front = camera.m_front;
		m_pos = camera.m_pos;
		m_dir = camera.m_dir;
		m_angle = camera.m_angle;
		m_matrix = camera.m_matrix;
		m_x = camera.m_x;
		m_y = camera.m_y;
		m_elapseTime = camera.m_elapseTime;
	}

	const std::type_info&	Camera::GetType() const
	{
		return typeid(*this);
	}

	Core::DataStructure::Component*	Camera::Copy() const
	{
		return new Camera(*this);
	}

	void		Camera::Init(Vec3 pos, Vec3 up)
	{
		m_matrix = m_matrix.Identity();

		Vec3 eye{pos.x, -pos.y, pos.z};
		m_matrix[0][1] = up.x;
		m_matrix[1][1] = up.y;
		m_matrix[2][1] = up.z;

		m_x = 0;
		m_y = 1;

		m_dir = Vec3(0.f, 0.f, -1.f);
		m_angle = Vec3(0.f, (float)M_PI, 0.f);

		StartCamera(eye);
	}

	void		Camera::SetMatrix(const Mat4& mat)
	{
		m_matrix = mat;
	}

	/* Update Camera Direction */
	void		Camera::UpdateView(Vec3& view)
	{
		m_matrix[0][2] = view.x * -1.f;
		m_matrix[1][2] = view.y * -1.f;
		m_matrix[2][2] = view.z * -1.f;
	}

	/* Update Right Axis Camera */
	void		Camera::UpdateWaxis(Vec3& waxis)
	{
		m_matrix[0][0] = waxis.x;
		m_matrix[1][0] = waxis.y;
		m_matrix[2][0] = waxis.z;
	}

	/* Update Camera Up */
	void		Camera::UpdateUp(Vec3& up)
	{
		m_matrix[0][1] = up.x;
		m_matrix[1][1] = up.y;
		m_matrix[2][1] = up.z;
	}

	/* Update Camera Pos */
	void		Camera::UpdateEye(const Vec3& pos)
	{
		m_matrix[0][3] = pos.x;
		m_matrix[1][3] = pos.y;
		m_matrix[2][3] = pos.z;
	}

	/* Update Camera */
	void		Camera::UpdateCamera(const Vec3& pos)
	{
	//	GetObject()->SetPos(pos);
		UpdateEye(pos);

		Vec3 view(m_dir);
		view.Normalize();
		UpdateView(view);

		Vec3 waxis(view.Cross(Vec3(0.f, 1.f, 0.f)));
		waxis.Normalize();
		UpdateWaxis(waxis);

		m_up = waxis.Cross(view);
		UpdateUp(m_up);

		m_front.x = view.Dot({1.f, 0.f, 0.f });
		m_front.y = 0.f;
		m_front.z = view.Dot({0.f, 0.f, 1.f});
		m_front.Normalize();
	}

	/* Start Camera */
	void            Camera::StartCamera(Vec3& pos)
	{
		UpdateEye(pos);
		m_pos = pos;

		UpdateView(m_dir);

		m_front.x = m_dir.Dot({ 1.f, 0.f, 0.f });
		m_front.y = 0;
		m_front.z = m_dir.Dot({ 0.f , 0.f, 1.f });
		m_front.Normalize();

		Vec3    waxis(m_dir.Cross(Vec3(m_matrix[0][1],m_matrix[1][1],m_matrix[2][1])));
		waxis.Normalize();
		UpdateWaxis(waxis);

		Vec3    up(waxis.Cross(m_dir));
		m_up = up;
		UpdateUp(up);
	}

	void		Camera::SetInvMatrix()
	{
		m_invMatrix = m_matrix.Inverse();
	}

	/* Move Camera */
	void		Camera::Rotate(Vec3 rotate)
	{
		m_angle += rotate;

		if (m_angle.x > (float)M_PI / 3.f)
			m_angle.x = (float)M_PI / 3.f;
		if (m_angle.x < (float)(-M_PI) / 3.f)
			m_angle.x = (float)(-M_PI) / 3.f;

		m_dir.z = cos(m_angle.x) * cos(m_angle.y);
		m_dir.y = sin(m_angle.x);
		m_dir.x = cos(m_angle.x) * sin(m_angle.y);
	}

	void		Camera::ChangeView(CRenderWindow* render, double frameTime)
	{
		int     x, y, width, height;

		SDL_GetWindowSize(render->GetWindow(), &width, &height);
		SDL_GetMouseState(&x, &y);

		if (x == 0 && y == 0)
			return;

		x -= (int)(width * 0.5f);
		y -= (int)(height * 0.5f);

		Vec3	rotDir {(float)-y, (float)-x, 0.f};

		Rotate(rotDir * (float)frameTime * SENSIBILITY);
	}

	Camera&		Camera::operator=(const Camera& cam)
	{
		m_matrix = cam.m_matrix;
		m_front = cam.m_front;
		m_dir = cam.m_dir;
		m_angle = cam.m_angle;
		m_pos = cam.m_pos;
		m_up = cam.m_up;
		m_x = cam.m_x;
		m_y = cam.m_y;

		return *this;
	}
}
