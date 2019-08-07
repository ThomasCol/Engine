#include "Texture.h"
#include "Debug.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define RGB 3
#define RGBA 4

namespace Resources
{
	void 	Texture::LoadTexture(const char* path)
	{
		m_file = std::string(path);
		LOG(INFO, "Loading Texture " + std::string(path));
		glGenTextures(1, &m_textID);

		glBindTexture(GL_TEXTURE_2D, m_textID);

		int	width, height, chanel = 0;

		/* load image */
		unsigned char*	image =
    				stbi_load(path, &width, &height, &chanel, 0);

		if (image == nullptr)
		{
			LOG(WARNING, "Failed to load texture !");
		}
		else
		{
			LOG(INFO, "Success");
			/* check chanel RGB or RGBA */
			if (chanel == RGB)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
															GL_UNSIGNED_BYTE, image);
			if (chanel == RGBA)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
															GL_UNSIGNED_BYTE, image);
		}

		/* set parameter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(image);
	}

	GLuint&	Texture::GetID()
	{
		return m_textID;
	}
}
