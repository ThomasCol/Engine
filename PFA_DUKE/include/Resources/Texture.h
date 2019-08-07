#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>

#include <GL/glew.h>

namespace Resources
{
	/* class for loading texture */
	class Texture
	{
	private:
		/* instance */
		GLuint	m_textID;

	public:
		std::string		m_file;

		/* constructor & destructor */
		Texture() = default;
		Texture(const Texture& text) = default;
		Texture(Texture&& text) = default;
		~Texture() = default;

		void 				LoadTexture(const char* path);
		inline std::string	GetFile() const { return m_file; };

		/* return text id */
		GLuint&				GetID();
	};
}

#endif //_TEXTURE_H_
