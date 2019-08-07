#include "DirectionalLight.h"
#include "Debug.h"

namespace LowRenderer::Lights
{
	DirectionalLight::DirectionalLight(Core::Maths::Vec3 dir, Core::Maths::Vec3 ambient,
		Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular):
		Light(ambient, diffuse, specular),
		m_direction {dir}
	{
	}

	void 	DirectionalLight::SendDataToShader(Resources::Shader* shader, size_t indice)
	{
		/* send directional instances to light array on shader */
		std::string prefix = "lightArray[" + std::to_string(indice) + "]";

		shader->SetVec3(prefix + ".direction", m_direction);
		shader->SetVec3(prefix + ".ambient", m_ambient);
		shader->SetVec3(prefix + ".diffuse", m_diffuse);
		shader->SetVec3(prefix + ".specular", m_specular);

		shader->SetInt(prefix + ".type", 1);
	}
}
