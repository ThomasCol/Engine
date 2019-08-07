#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "Vec3.h"
#include "Light.h"
#include "Shader.h"

namespace LowRenderer::Lights
{
	/* point light management */
	class PointLight : public virtual Light
	{
	private:
		/* instance */
		Core::Maths::Vec3	m_pos;

		float               m_constant;
        float               m_linear;
        float               m_quadratic;

	public:
		/* constructor */
		PointLight() = default;
		PointLight(Core::Maths::Vec3 pos, Core::Maths::Vec3 attenuation, Core::Maths::Vec3 ambient, 
			Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular);
		PointLight(const PointLight& light) = default;
		PointLight(PointLight&& light) = default;
		/* destructor */
		~PointLight() = default;

		/* func to set struct Light in fragment shader */
		void 	SendDataToShader(Resources::Shader*	shader, size_t indice) override;
	};
}

#endif //_POINTLIGHT_H_
