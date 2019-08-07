#ifndef _MODEL_H_
#define _MODEL_H_

#include "Vec3.h"
#include "Vec2.h"
#include "Vertex.h"
#include "Triplet.h"
#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>

namespace Resources
{
	/* class for loading obj and set VAO */
	class Model
	{
	private:
		/* instance */
		std::vector<Core::Maths::Vec3>				m_position;
		std::vector<Core::Maths::Vec3>				m_normals;
		std::vector<Core::Maths::Vec2>				m_uvs;

		std::map<Core::DataStructure::Triplet, int>	m_tripMap;

		/* final vertex array */
		std::vector<Core::DataStructure::Vertex>	m_vertex;
		/* final indice array */
		std::vector<GLuint>							m_indices;

		GLuint										m_VAO;

	public:
		/* constructor & destructor */
		Model() = default;
		~Model();

		/* open obj file */
		void 	LoadOBJ(const char* path);

		/* parse manager */
		void 	ParseOBJ(std::string& line);

		/* parse each data */
		void 	ParsePosition(std::string line);
		void 	ParseNormal(std::string line);
		void 	ParseTexture(std::string line);

		void 	ParseIndex(std::string line);

		/* parse index in func of norme used */
		void 	ParseTwoSlash(std::string& indexStr);
		void 	ParseOneSlash(std::string& indexStr);
		void 	ParseZeroSlash(std::string& indexStr);

		/* parse line into indice triplet */
		void	ParseIndexLine(std::string& line, std::string& indexStr,
								std::vector<std::string>& triplet);

		/* if line contain more than 3 vertex */
		void 	AddPolyIndices(size_t slash, std::vector<std::string>& triplet,
								size_t i);
		/* if line contain 3 vertex */
		void 	AddIndices(size_t slash, std::vector<std::string>& triplet,
							size_t i);
		/* count number of space to get number of vertex */
		size_t	CountSpace(std::string& line);

		/* use triplet to set array */
		void 	SetVertexArray(GLuint pos, GLuint uv, GLuint norm);

		/* get */
		GLuint	GetIndexSize();
		GLuint&	GetVAO();

		/* set inside func */
		void 	SetVBO(GLuint& VBO);
		void 	SetEBO(GLuint& EBO);
		void 	SetVAO();
	};
}

#endif //_MODEL_H_
