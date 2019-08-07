#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vec3.h"
#include "Shader.h"

namespace LowRenderer::Lights
{
    /* abstract class */
    class Light
    {
    public:
        /* instance */
        Core::Maths::Vec3   m_ambient;
        Core::Maths::Vec3   m_diffuse;
        Core::Maths::Vec3   m_specular;

    public:
        /* constructor */
        Light() = default;
        Light(Core::Maths::Vec3 ambient, Core::Maths::Vec3 diffuse, Core::Maths::Vec3 specular);
        Light(const Light& light) = default;
        Light(Light&& light) = default;
        /* virtual destructor */
        virtual ~Light() = default;

        /* func for override*/
        virtual void SendDataToShader(Resources::Shader* shader, size_t indice) = 0;
    };
}

#endif //_LIGHT_H_
