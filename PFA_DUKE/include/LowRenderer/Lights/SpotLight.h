#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "Light.h"
#include "Vec3.h"
#include "Vec2.h"

namespace LowRenderer::Lights
{
	/* manager for spot light */
	class SpotLight : public virtual Light
	{
	private:
		/* instance */
		Core::Maths::Vec3   m_pos;
        Core::Maths::Vec3   m_direction;

		float               m_cutOff;
        float               m_outerCutOff;

	public:
		/* constructor */
		SpotLight() = default;
		SpotLight(Core::Maths::Vec3 pos, Core::Maths::Vec3 dir, Core::Maths::Vec2 cut,
			Core::Maths::Vec3 ambient, Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular);
		SpotLight(const SpotLight& light) = default;
		SpotLight(SpotLight&& light) = default;
		/* destructor */
		~SpotLight() = default;

		/* func to set struct Light in fragment shader */
		void 	SendDataToShader(Resources::Shader*	shader, size_t indice) override;
	};
}

#endif //_SPOTLIGHT_H_
