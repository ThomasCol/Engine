#ifndef __TEXT_RENDERER__
#define __TEXT_RENDERER__

#include <ft2build.h>
#include FT_FREETYPE_H
#include <SDL2/SDL.h>
#include <string>

#include "Log.h"
#include "Character.h"
#include "ResourcesManager.h"
#include "Vec3.h"

namespace LowRenderer
{
	class TextRenderer
	{
		private:
			GLfloat										m_vertices[6][4];

			Vec3										m_textColor;
			Vec4										m_backColor;
			
			std::string									m_fontName;
			
			Resources::ResourcesManager*				m_manager;
			std::map<GLchar, Resources::Character*>*	m_characters;
			Resources::Shader*							m_s{ nullptr };
			Resources::Shader*							m_s2{ nullptr };

			float										m_scale {1.f};
			GLuint										m_VAO;
			GLuint										m_VBO;
			
			void	SetVertices(const float& xPos, const float& yPos, 
						const float& width, const float& height);
			void	DrawChar(int& xStart, int& yStart, const int& windowW,
						const int& windowH, const GLchar& c);
			void	DrawRect(const int& x, const int& y, const int& width, 
						const int& length, SDL_Window* window);
			void	DrawRect(const int& x, const int& y, const int& width, 
						const int& length, SDL_Window* window, GLuint);
			void	DrawChar(float& xStart, float& yStart, const int& windowW,
						const int& windowH, const GLchar& c);
			void	DrawRect(const float& x, const float& y, const float& width, 
						const float& length, SDL_Window* window);
			void	DrawRect(const float& x, const float& y, const float& width, 
						const float& length, SDL_Window* window, GLuint);

			void	FindStringLenAndHighest(const std::string& s, int& len, int& highest);
			
		public:
			TextRenderer(Resources::ResourcesManager*&, const std::string& fontPath, 
				const float&, const Core::Maths::Vec3&, const Core::Maths::Vec4&);

			void	RenderButton(const std::string& s, const int& x, const int& y, 
						const int& width, const int& length, SDL_Window*);
			void	RenderButton(const std::string& s, const float& x, const float& y, 
						const float& width, const float& length, SDL_Window*);
			void	RenderButton(const std::string& s, const int& x, const int& y, 
						const int& width, const int& length, SDL_Window*, GLuint);
			void	RenderButton(const std::string& s, const float& x, const float& y, 
						const float& width, const float& length, SDL_Window*, GLuint);
			void	RenderTextCentered(const std::string& s, const int& x, const int& y, 
						const int& width, const int& length, SDL_Window*);
			void	RenderTextCentered(const std::string& s, const float& x, const float& y, 
						const float& width, const float& length, SDL_Window*);
	};
}

#endif
