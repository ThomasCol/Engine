#include "PointLight.h"
#include "Debug.h"

namespace LowRenderer::Lights
{
	PointLight::PointLight(Core::Maths::Vec3 pos, Core::Maths::Vec3 attenuation,
		Core::Maths::Vec3 ambient, Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular):
		Light(ambient, diffuse, specular), m_pos {pos}
	{
		m_constant = attenuation.x;
		m_linear = attenuation.y;
		m_quadratic = attenuation.z;
	}

	void 	PointLight::SendDataToShader(Resources::Shader* shader, size_t indice)
	{
		/* send point instances to light array on shader */
		std::string ind = std::to_string(indice);

		shader->SetVec3("lightArray[" + ind + "].position", m_pos);

		shader->SetFloat("lightArray[" + ind + "].constant", m_constant);
		shader->SetFloat("lightArray[" + ind + "].linear", m_linear);
		shader->SetFloat("lightArray[" + ind + "].quadratic", m_quadratic);

		shader->SetVec3("lightArray[" + ind + "].ambient", m_ambient);
		shader->SetVec3("lightArray[" + ind + "].diffuse", m_diffuse);
		shader->SetVec3("lightArray[" + ind + "].specular", m_specular);

		shader->SetInt("lightArray[" + ind + "].type", 2);
	}
}
