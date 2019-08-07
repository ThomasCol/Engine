#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>
#include <map>
#include <string>

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"

namespace Resources
{
	/* func for shader management */
	class Shader
	{
	private:
		GLuint							m_shaderProgram;

		/* map for ID in shader */
		std::map<std::string, GLuint>	m_ID;

	public:
		/* constructor & destructor */
		Shader();
		Shader(const char* vertex, const char* fragment);
		~Shader();

		/* use shader program */
		void 	Use();
		void 	Unuse();

		/* set shader */
		GLuint	CreateVertexShader(const char* path);
		GLuint	CreateFragmentShader(const char* path);

		/* parse shader file */
		GLchar*	ParseShader(const char* path);

		/* send data to shader */
		void 	SetVec2(const std::string& location, Core::Maths::Vec2 vec);
		void 	SetVec3(const std::string& location, Core::Maths::Vec3 vec);
		void 	SetVec4(const std::string& location, Core::Maths::Vec4 vec);
		void 	SetMat4(const std::string& location, const Core::Maths::Mat4& mat);
		void 	SetTexture(const std::string& location, GLuint textureID);
		void 	SetFloat(const std::string& location, float nb);
		void 	SetInt(const std::string& location, int nb);
	};
}

#endif //_SHADER_H_
