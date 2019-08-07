#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

#include "Vec3.h"
#include "Light.h"
#include "Shader.h"

namespace LowRenderer::Lights
{
	/* set directional light */
	class DirectionalLight : public virtual Light
	{
	private:
		/* instance */
		Core::Maths::Vec3	m_direction;

	public:
		/* constructor & destructor */
		DirectionalLight() = default;
		DirectionalLight(Core::Maths::Vec3 dir, Core::Maths::Vec3 ambient,
							Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular);
		DirectionalLight(const DirectionalLight& light) = default;
		DirectionalLight(DirectionalLight&& light) = default;
		~DirectionalLight() = default;

		/* func to set struct Light in fragment shader */
		void 	SendDataToShader(Resources::Shader* shader, size_t indice) override;
	};
}

#endif //_DIRECTIONALLIGHT_H_
