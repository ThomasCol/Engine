#include <math.h>

#include "Light.h"
#include "Debug.h"

namespace LowRenderer::Lights
{
	Light::Light(Core::Maths::Vec3 ambient, Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular)
	{
		m_ambient = ambient;
		m_diffuse = diffuse;
		m_specular = specular;
	}
}
