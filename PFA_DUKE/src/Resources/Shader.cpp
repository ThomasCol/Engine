#include "Shader.h"

#include "Debug.h"

#include <fstream>
#include <exception>
#include <climits>

namespace Resources
{
	Shader::Shader()
	{
		m_shaderProgram = UINT_MAX;
	}

	Shader::Shader(const char* vertex, const char* fragment)
	{
		LOG(INFO, "Creating shader program");
		/* create shaders */
		GLuint	vertexShader = CreateVertexShader(vertex);
		GLuint	fragmentShader = CreateFragmentShader(fragment);

		/* create program */
		m_shaderProgram = glCreateProgram();

		LOG(INFO, "Linking shaders");

		/* set program */
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		/* compil */
		glLinkProgram(m_shaderProgram);

		GLint succes {1};
		char infoLog[512];

		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &succes);

		if (!succes)
		{
			glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
			LOG(ERROR, "Shader failed to link !\n\t> " + std::string(infoLog));
		}
		else
			LOG(INFO, "Create program with success");

		/* delete useless shaders */
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader()
	{
		if (m_shaderProgram != UINT_MAX)
			glDeleteProgram(m_shaderProgram);
	}

	GLuint	Shader::CreateVertexShader(const char* path)
	{
		LOG(INFO, "Create vertex shader with " + std::string(path));
		GLint			succes {1};
		char			infoLog[512];

		try
		{
			/* send shader on string */
			const GLchar*	vs = ParseShader(path);

			/* create vertex shader */
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

			/* compil vertex shader */
			glShaderSource(vertexShader, 1, &vs, NULL);
			LOG(INFO, "Compiling vertex shader");
			glCompileShader(vertexShader);

			delete[] vs;

			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succes);
			if (!succes)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				LOG(ERROR, "Shader failed to compile !\n\t> " + std::string(infoLog));
			}
			else
				LOG(INFO, "Success");
			return vertexShader;
		}
		catch (std::string e)
		{

		}

		return 0;
	}

	GLuint	Shader::CreateFragmentShader(const char* path)
	{
		LOG(INFO, "Create fragment shader with " + std::string(path));
		GLint			succes {1};
		char			infoLog[512];

		try
		{
			/* send shader on string */
			const GLchar*	fs = ParseShader(path);

			/* create fragment shader */
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(fragmentShader, 1, &fs, NULL);
			LOG(INFO, "Compiling fragment shader");
			/* compil fragment shader */
			glCompileShader(fragmentShader);

			delete[] fs;

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succes);
			if (!succes)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				LOG(ERROR, "Shader failed to compile !\n\t> " + std::string(infoLog));
			}
			else
				LOG(INFO, "Success");

			return fragmentShader;
		}
		catch (std::string error)
		{

		}

		return 0;
	}

	void 	Shader::Use()
	{
		glUseProgram(m_shaderProgram);
	}

	void 	Shader::Unuse()
	{
		glUseProgram(0);
	}

	GLchar*	Shader::ParseShader(const char* path)
	{
		GLuint	size;
		GLchar*	stringShader;

		std::ifstream file(path, std::ios::in);

		if (file.is_open())
		{
			/* send all file in string */
			file.seekg(0, std::ios::end);
			size = (GLuint)file.tellg();
			stringShader = new GLchar[size + 1];
			file.seekg(0, std::ios::beg);

			int idx{ 0 };
			char c;

			while (file.get(c))
			{
				stringShader[idx] = c;
				idx++;
			}
			file.read(stringShader, size);
			file.close();

			stringShader[idx] = '\0';

			return stringShader;
		}

		LOG(ERROR, "Can't open " + std::string(path));
		STATICASSERT(false);
		return nullptr;
	}

	void    Shader::SetVec2(const std::string& location, Core::Maths::Vec2 vec)
        {
                /* map vector and return if already exist */
                std::map<std::string, GLuint>::iterator it;

                it = m_ID.find(location);

                if (it != m_ID.end())
                {
                        glUniform2f(it->second, vec.x, vec.y);
                }
                else
                {
                        GLuint newID = glGetUniformLocation(m_shaderProgram, location.c_str());

						if (newID == -1)
							return;

                        m_ID.insert(std::pair<std::string, GLuint>(location, newID));

                        glUniform2f(newID, vec.x, vec.y);
                }
        }

	void 	Shader::SetVec3(const std::string& location, Core::Maths::Vec3 vec)
	{
		/* map vector and return if already exist */
		std::map<std::string, GLuint>::iterator	it;

		it = m_ID.find(location);

		if (it != m_ID.end())
		{
			glUniform3f(it->second, vec.x, vec.y, vec.z);
		}
		else
		{
			GLint newID = glGetUniformLocation(m_shaderProgram, location.c_str());

			if (newID == -1)
				return;

			m_ID.insert(std::pair<std::string, GLuint>(location, newID));

			glUniform3f(newID, vec.x, vec.y, vec.z);
		}
	}

	void 	Shader::SetVec4(const std::string& location, Core::Maths::Vec4 vec)
	{
		/* map vector and return if already exist */
		std::map<std::string, GLuint>::iterator	it;

		it = m_ID.find(location);

		if (it != m_ID.end())
		{
			glUniform4f(it->second, vec.x, vec.y, vec.z, vec.w);
		}
		else
		{
			GLint newID = glGetUniformLocation(m_shaderProgram, location.c_str());

			if (newID == -1)
				return;

			m_ID.insert(std::pair<std::string, GLuint>(location, newID));

			glUniform4f(newID, vec.x, vec.y, vec.z, vec.w);
		}
	}

	void 	Shader::SetMat4(const std::string& location, const Core::Maths::Mat4& mat)
	{
		/* map vector and return if already exist */
		std::map<std::string, GLuint>::iterator	it;

		it = m_ID.find(location);

		GLuint id;
		if (it != m_ID.end())
		{
			id = it->second;
		}
		else
		{
			GLint newID = glGetUniformLocation(m_shaderProgram, location.c_str());

			if (newID == -1)
				return;

			m_ID.insert(std::pair<std::string, GLuint>(location, newID));

			id = newID;
		}

		glUniformMatrix4fv(id, 1, GL_TRUE, mat.array);
	}

	void 	Shader::SetTexture(const std::string& location, GLuint textureID)
	{
		/* map texture and return if already exist */
		std::map<std::string, GLuint>::iterator	it;

		it = m_ID.find(location);

		if (it != m_ID.end())
		{
			glUniform1i(it->second, textureID);
		}
		else
		{
			GLint newID = glGetUniformLocation(m_shaderProgram, location.c_str());

			if (newID == -1)
				return;

			m_ID.insert(std::pair<std::string, GLuint>(location, newID));

			glUniform1i(newID, textureID);
		}
	}

	void 	Shader::SetFloat(std::string const& location, float nb)
	{
		/* map float and return if already exist */
		std::map<std::string, GLuint>::iterator	it;

		it = m_ID.find(location);

		if (it != m_ID.end())
		{
			glUniform1f(it->second, nb);
		}
		else
		{
			GLint newID = glGetUniformLocation(m_shaderProgram, location.c_str());

			if (newID == -1)
				return;

			m_ID.insert(std::pair<std::string, GLuint>(location, newID));

			glUniform1f(newID, nb);
		}
	}

	void 	Shader::SetInt(const std::string& location, int nb)
	{
		/* map int and return if already exist */
		std::map<std::string, GLuint>::iterator	it;

		it = m_ID.find(location);

		if (it != m_ID.end())
		{
			glUniform1i(it->second, nb);
		}
		else
		{
			GLint newID = glGetUniformLocation(m_shaderProgram, location.c_str());

			if (newID == -1)
				return;

			m_ID.insert(std::pair<std::string, GLuint>(location, newID));

			glUniform1i(newID, nb);
		}
	}
}
