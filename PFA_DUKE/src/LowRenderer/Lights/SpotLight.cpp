#include <math.h>

#include "SpotLight.h"
#include "Debug.h"

namespace LowRenderer::Lights
{
	SpotLight::SpotLight(Core::Maths::Vec3 pos, Core::Maths::Vec3 dir, Core::Maths::Vec2 cut, 
		Core::Maths::Vec3 ambient, Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular):
		Light(ambient, diffuse, specular), m_pos {pos}, m_direction {dir}
	{
		m_cutOff = cut.x;
		m_outerCutOff = cut.y;
	}

	void 	SpotLight::SendDataToShader(Resources::Shader*	shader, size_t indice)
	{
		/* send spot instances to light array on shader */
		std::string ind = std::to_string(indice);

		shader->SetVec3("lightArray[" + ind + "].position", m_pos);
		shader->SetVec3("lightArray[" + ind + "].direction", m_direction);

		shader->SetFloat("lightArray[" + ind + "].cutOff",
													cos(m_cutOff * (float)M_PI / 180));
		shader->SetFloat("lightArray[" + ind + "].outerCutOff",
													cos(m_outerCutOff * (float)M_PI / 180));

		shader->SetVec3("lightArray[" + ind + "].ambient", m_ambient);
		shader->SetVec3("lightArray[" + ind + "].diffuse", m_diffuse);
		shader->SetVec3("lightArray[" + ind + "].specular", m_specular);

		shader->SetInt("lightArray[" + ind + "].type", 3);
	}
}
