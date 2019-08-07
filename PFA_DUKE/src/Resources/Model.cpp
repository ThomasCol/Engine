#include "Model.h"
#include "Debug.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>

namespace Resources
{
	Model::~Model()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void 	Model::LoadOBJ(const char* path)
	{
		LOG(INFO, "Loading " + std::string(path));

		/* open obj file */
		std::ifstream	obj(path, std::ios::in);
		std::string		line;

		if (obj.is_open())
		{
			while (getline(obj, line))
				ParseOBJ(line);

			obj.close();

			LOG(INFO, "position : " + std::to_string(m_position.size()) + "\n\t> " +
			"uv : " + std::to_string(m_uvs.size()) + "\n\t> " +
			"normal : " + std::to_string(m_normals.size()) + "\n\t> " + "vertex : " +
			std::to_string(m_vertex.size()) + "\n\t> " + "indices : " +
			std::to_string(m_indices.size()));

			SetVAO();
		}
		else
		{
			LOG(ERROR, "Failed to open OBJ file !");
		}
		LOG(INFO, "Success");
	}

	void 	Model::ParseOBJ(std::string& line)
	{
		/* check what's in the line */
		if (line.substr(0, 2) == "v ")
		{
			ParsePosition(line.substr(2));
		}
		if (line.substr(0, 3) == "vn ")
		{
			ParseNormal(line.substr(3));
		}
		if (line.substr(0, 3) == "vt ")
		{
			ParseTexture(line.substr(3));
		}
		if (line.substr(0, 2) == "f ")
		{
			ParseIndex(line.substr(2));
		}
	}

	void 	Model::ParsePosition(std::string line)
	{
		Core::Maths::Vec3	pos;

		sscanf(line.c_str(), "%f %f %f", &pos.x, &pos.y, &pos.z);

		m_position.push_back(pos);
	}

	void 	Model::ParseNormal(std::string line)
	{
		Core::Maths::Vec3	normal;

		sscanf(line.c_str(), "%f %f %f", &normal.x, &normal.y, &normal.z);

		m_normals.push_back(normal);
	}

	void 	Model::ParseTexture(std::string line)
	{
		Core::Maths::Vec2				texture;

		sscanf(line.c_str(), "%f %f", &texture.x, &texture.y);

		m_uvs.push_back(texture);
	}

	void 	Model::ParseIndex(std::string line)
	{
		std::string					indexStr;
		std::vector<std::string>	triplet;
		size_t						space {CountSpace(line)};
		size_t						slash;

		for (size_t i = 0; i < space; i++)
		{
			ParseIndexLine(line, indexStr, triplet);
		}

		/* if  more than 3 vertex in a face */
		if (space > 3)
		{
			for (size_t i = 0; i < space - 2; i++)
			{
				slash = std::count(triplet[i].begin(), triplet[i].end(), '/');

				AddPolyIndices(slash, triplet, i);
			}
		}
		else /* if less than 3 vertex in a face */
		{
			for (size_t i = 0; i < space; i++)
			{
				slash = std::count(triplet[i].begin(), triplet[i].end(), '/');

				AddIndices(slash, triplet, i);
			}
		}
	}

	void 	Model::AddPolyIndices(size_t slash, std::vector<std::string>& triplet,
									size_t i)
	{
		/* parse in func of data in indece line */
		if (slash == 2)
		{
			/* parse all into triangles */
			ParseTwoSlash(triplet[0]);
			ParseTwoSlash(triplet[i + 1]);
			ParseTwoSlash(triplet[i + 2]);
		}

		if (slash == 1)
		{
			ParseOneSlash(triplet[0]);
			ParseOneSlash(triplet[i + 1]);
			ParseOneSlash(triplet[i + 2]);
		}

		if (slash == 0)
		{
			ParseZeroSlash(triplet[0]);
			ParseZeroSlash(triplet[i + 1]);
			ParseZeroSlash(triplet[i + 2]);
		}
	}

	void 	Model::AddIndices(size_t slash, std::vector<std::string>& triplet,
								size_t i)
	{
		/* parse in func of data in indece line */
		if (slash == 2)
		{
			ParseTwoSlash(triplet[i]);
		}

		if (slash == 1)
		{
			ParseOneSlash(triplet[i]);
		}

		if (slash == 0)
		{
			ParseZeroSlash(triplet[i]);
		}
	}

	void	Model::ParseIndexLine(std::string& line, std::string& indexStr,
									std::vector<std::string>& triplet)
	{
		/* parse line to get triplet */
		std::istringstream iss(line);
		while (iss >> indexStr)
			triplet.push_back(indexStr);
	}

	size_t 	Model::CountSpace(std::string& line)
	{
		/* count number of vertex in line */
		line += " ";
		return std::count(line.begin(), line.end(), ' ');
	}

	void 	Model::ParseTwoSlash(std::string& indexStr)
	{
		std::istringstream	iss(indexStr);
		std::string			str;
		GLuint				pos;
		GLuint				uv;
		GLuint				norm;

		/* get pos indice */
		std::getline(iss, str, '/');
		if (str.size() != 0)
			pos = std::stoi(str) - 1;
		else
			pos = 0;

		/* get UV indice */
		std::getline(iss, str, '/');
		if (str.size() != 0)
			uv = std::stoi(str) - 1;
		else
			uv = 0;

		/* get normal indice */
		std::getline(iss, str, '/');
		if (str.size() != 0)
			norm = std::stoi(str) - 1;
		else
			norm = 0;

		SetVertexArray(pos, uv, norm);
	}

	void 	Model::ParseOneSlash(std::string& indexStr)
	{
		if (indexStr.size() == 0)
			return;

		std::istringstream	iss (indexStr);
		std::string			str;
		GLuint				pos;
		GLuint				uv;
		GLuint				norm;

		std::getline(iss, str, '/');
		if (str.size() != 0)
			pos = std::stoi(str) - 1;
		else
			pos = 0;

		std::getline(iss, str, '/');
		if (str.size() != 0)
			uv = std::stoi(str) - 1;
		else
			uv = 0;

		/* if one slash, no normal so push back 0 */
		norm = 0;

		SetVertexArray(pos, uv, norm);
	}

	void 	Model::ParseZeroSlash(std::string& indexStr)
	{
		if (indexStr.size() == 0)
			return;

		std::istringstream	iss (indexStr);
		std::string			str;
		GLuint				pos;
		GLuint				uv;
		GLuint				norm;

		/* if zero slash, only get pos */
		std::getline(iss, str, '/');
		if (str.size() != 0)
			pos = std::stoi(str) - 1;
		else
			pos = 0;

		uv = 0;

		norm = 0;

		SetVertexArray(pos, uv, norm);
	}

	void 	Model::SetVertexArray(GLuint pos, GLuint uv, GLuint norm)
	{
		Core::DataStructure::Triplet	trip(pos, uv, norm);

		/* send vertex into triplet map */
		std::map<Core::DataStructure::Triplet, int>::iterator	it;

		it = m_tripMap.find(trip);

		if (it != m_tripMap.end())
		{
			/* if already exist take and push back */
			m_indices.push_back(it->second);
		}
		else
		{
			/* else create vertex */
			Core::DataStructure::Vertex	vertex(m_position[pos], m_uvs[uv],
																	m_normals[norm]);
			m_tripMap.insert(std::pair<Core::DataStructure::Triplet, int>(trip,
																(int)m_vertex.size()));
			m_indices.push_back((GLuint)m_vertex.size());

			m_vertex.push_back(vertex);
		}
	}

	GLuint	Model::GetIndexSize()
	{
		return (GLuint)m_indices.size();
	}

	void 	Model::SetVAO()
	{
		GLuint	VBO {0};
		GLuint	EBO {0};
		/* gen VAO */
		glGenVertexArrays(1, &m_VAO);

		/* bind VAO */
		glBindVertexArray(m_VAO);

		SetVBO(VBO);

		SetEBO(EBO);

		/* unbind VAO */
		glBindVertexArray(0);

		//glDeleteBuffers(1, &VBO);
		//glDeleteBuffers(1, &EBO);
	}

	void 	Model::SetVBO(GLuint& VBO)
	{
		glGenBuffers(1, &VBO);

		/* bind VBO */
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		/* send data */
		glBufferData(GL_ARRAY_BUFFER, sizeof(Core::DataStructure::Vertex)
					* m_vertex.size(), (GLfloat*)m_vertex.data(), GL_STATIC_DRAW);

		/* set VBO properties */
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
								(GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
								(GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
								(GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	}

	void 	Model::SetEBO(GLuint& EBO)
	{
		/* create EBO */
		glGenBuffers(1, &EBO);

		/* set EBO */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(),
													m_indices.data(), GL_STATIC_DRAW);
	}

	GLuint&	Model::GetVAO()
	{
		return m_VAO;
	}
}
