#include "TextRenderer.h"

using namespace Core::Maths;

namespace LowRenderer
{
	TextRenderer::TextRenderer(Resources::ResourcesManager*& manager, const std::string& fontPath,
		const float& s, const Vec3& textColor, const Vec4& backColor) :
		m_vertices {{0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 1.f, 1.f}, 
			{0.f, 0.f, 0.f, 0.f},{0.f, 0.f, 1.f, 1.f},{0.f, 0.f, 1.f, 0.f}},
		m_textColor {textColor},
		m_backColor {backColor},
		m_fontName {fontPath},
		m_manager {manager},
		m_characters {&manager->AddFont(fontPath)->first},
		m_s {m_manager->AddShader("./resources/shader/textShader.vs", "resources/shader/textShader.fs")},
		m_s2 {m_manager->AddShader("./resources/shader/quadShader.vs", "resources/shader/quadShader.fs")},
		m_scale {s}
	{
		/* Init opengl for the renderer */
		GLuint  EBO;
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(GLvoid*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		GLuint  index[6] {0, 1, 2, 3, 4, 5};
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), index, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	/* Draws a text centered in a quad, with background texture. Coordinates are in floating -1/1 */
	void	TextRenderer::RenderButton(const std::string& s, const float& x, const float& y, 
		const float& width, const float& length, SDL_Window* window, GLuint backTexture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawRect(x, y, width, length, window, backTexture);
		RenderTextCentered(s, x, y, width, length, window);
	}

	/* Draws a text centered in a quad, without background texture. Coordinates are in floating -1/1 */
	void	TextRenderer::RenderButton(const std::string& s, const float& x, const float& y, 
		const float& width, const float& length, SDL_Window* window)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawRect(x, y, width, length, window);
		RenderTextCentered(s, x, y, width, length, window);
	}

	/* Draws a text centered in a quad, with background texture */
	void	TextRenderer::RenderButton(const std::string& s, const int& x, const int& y, 
		const int& width, const int& length, SDL_Window* window, GLuint backTexture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawRect(x, y, width, length, window, backTexture);
		RenderTextCentered(s, x, y, width, length, window);
	}

	/* Draws a text centered in a quad, without background texture */
	void	TextRenderer::RenderButton(const std::string& s, const int& x, const int& y, 
		const int& width, const int& length, SDL_Window* window)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawRect(x, y, width, length, window);
		RenderTextCentered(s, x, y, width, length, window);
	}

	/* Finds length in pixel of the string + the highest character */
	void	TextRenderer::FindStringLenAndHighest(const std::string& s, int& len, int& highest)
	{
		len = highest = 0;
		std::map<GLchar, Resources::Character*>::iterator  it;
		Resources::Character*  c = nullptr;

		//load size of string and all characters in case they were not loaded
		for (unsigned i {0}; i < s.size(); ++i)
		{
			it = m_characters->find(s[i]);
			if (it == m_characters->end())
				c = m_manager->AddCharacter(m_fontName, s[i]);
			else
				c = it->second;

			//A glyph uses 2^6 pixels, using 2^7 because later divided by two.
			len += (c->advance >> 7);
			if (highest < c->bearingY)
				highest = c->bearingY;
		}
	}

	/* Render text that is centered in the box formed by position + width/length */
	void	TextRenderer::RenderTextCentered(const std::string& s, const float& x, 
		const float& y, const float& width, const float& height, SDL_Window* window)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_ALWAYS);

		int	windowW,
			windowH,
			len{ 0 },
			highest{ 0 };
			
		SDL_GetWindowSize(window, &windowW, &windowH);

		//int  len {0}, highest {0};
		FindStringLenAndHighest(s, len, highest);

		float	xPixelStart {x + (width * 0.5f) - ((len / (windowW * 0.5f)) * m_scale)}, 
				yPixelStart {-y + height - 0.5f * highest / windowH};

		m_s->Use();
		glBindVertexArray(m_VAO);

		m_s->SetVec3("textColor", m_textColor);

		for (unsigned i {0}; i < s.size(); ++i)
			DrawChar(xPixelStart, yPixelStart, windowW, windowH, s[i]);

		m_s->Unuse();
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	/* Render text that is centered in the box formed by position + width/length */
	void	TextRenderer::RenderTextCentered(const std::string& s, const int& x, const int& y,
		const int& width, const int& length, SDL_Window* window)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_ALWAYS);

		int	len{0},
			highest{0},
			xPixelStart{ x + (width / 2) - len}, yPixelStart {y + ((length + highest) / 2) };

		FindStringLenAndHighest(s, len, highest);
		m_s->Use();
		glBindVertexArray(m_VAO);

		int windowW, windowH;
		SDL_GetWindowSize(window, &windowW, &windowH);

		m_s->SetVec3("textColor", m_textColor);

		for (unsigned i {0}; i < s.size(); ++i)
			DrawChar(xPixelStart, yPixelStart, windowW, windowH, s[i]);

		m_s->Unuse();
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	/* Sets vertices of quad to given values */
	void	TextRenderer::SetVertices(const float& xPos, const float& yPos, const float& xEnd, const float& yEnd)
	{
		m_vertices[0][0] = m_vertices[1][0] = m_vertices[3][0] = xPos;
		m_vertices[0][1] = m_vertices[3][1] = m_vertices[5][1] = yPos;
		m_vertices[2][0] = m_vertices[4][0] = m_vertices[5][0] = xEnd;
		m_vertices[1][1] = m_vertices[2][1] = m_vertices[4][1] = yEnd;
	}

	/* Draws a char to given pos, needs other function to setup opengl beforehand */
	void	TextRenderer::DrawChar(int& xStart, int& yStart,
			const int& windowW, const int& windowH, const GLchar& c)
	{
		Resources::Character*  ch {m_manager->AddCharacter(m_fontName, c)};
		GLfloat  xPos {(xStart - windowW * 0.5f) / (windowW * 0.5f)};
		GLfloat  yPos {-(yStart + (ch->sizeY - ch->bearingY) - windowH * 0.5f) / (windowH * 0.5f)};
		GLfloat  w = ch->sizeX / (windowW * 0.5f) * m_scale;
		GLfloat  h = ch->sizeY / (windowH * 0.5f) * m_scale;

		SetVertices(xPos, yPos + h, xPos + w, yPos);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, ch->textureID);
		m_s->SetTexture("text", 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		xStart += (int)((ch->advance >> 6) * m_scale);
	}

	/* Draws a background rect with given texture */
	void	TextRenderer::DrawRect(const int& x, const int& y, const int& width,
		const int& length, SDL_Window* window, GLuint backTexture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_ALWAYS);

		m_s2->Use();
		glBindVertexArray(m_VAO);
		m_s2->SetVec4("Color", m_backColor);

		glBindTexture(GL_TEXTURE_2D, backTexture);
		m_s2->SetInt("isText", true);
		m_s2->SetTexture("text", 0);

		int windowW, windowH;
		SDL_GetWindowSize(window, &windowW, &windowH);

		float  xPos {(x - windowW * 0.5f) / (windowW * 0.5f)}, yPos {-(y - windowH * 0.5f) / (windowH * 0.5f)},
		       xEnd {((x + width) - (windowW * 0.5f)) / (float)(windowW * 0.5f)}, yEnd {yPos - length / (windowH * 0.5f)};

		SetVertices(xPos, yPos, xEnd, yEnd);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		m_s2->Unuse();

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	/* Draws a background rect with default color */
	void	TextRenderer::DrawRect(const int& x, const int& y, const int& width, const int& length, SDL_Window* window)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_ALWAYS);

		m_s2->Use();
		glBindVertexArray(m_VAO);
		m_s2->SetVec4("Color", m_backColor);
		m_s2->SetInt("isText", false);

		int windowW, windowH;
		SDL_GetWindowSize(window, &windowW, &windowH);

		float  xPos {(x - windowW * 0.5f) / (windowW * 0.5f)}, yPos {-(y - windowH * 0.5f) / (windowH * 0.5f)},
		       xEnd {((x + width) - (windowW * 0.5f)) / (float)(windowW * 0.5f)}, yEnd {yPos - length / (windowH * 0.5f)};

		SetVertices(xPos, yPos, xEnd, yEnd);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		m_s2->Unuse();

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	/* Draws a char to given pos, needs other function to setup opengl beforehand */
	void	TextRenderer::DrawChar(float& xStart, float& yStart,
			const int& windowW, const int& windowH, const GLchar& c)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Resources::Character*  ch {m_manager->AddCharacter(m_fontName, c)};
		GLfloat  yPos {-yStart - (((ch->sizeY - ch->bearingY) * m_scale) / (windowH * 0.5f))};
		GLfloat  w = (ch->sizeX / (windowW * 0.5f)) * m_scale;
		GLfloat  h = (ch->sizeY / (windowH * 0.5f)) * m_scale;

		SetVertices(xStart, yPos + h, xStart + w, yPos);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, ch->textureID);
		m_s->SetTexture("text", 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		xStart += ((ch->advance >> 6) * m_scale) / (windowW * 0.5f);
	}

	/* Draws a background rect with given texture */
	void	TextRenderer::DrawRect(const float& x, const float& y, const float& width,
		const float& length, SDL_Window* window, GLuint backTexture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_ALWAYS);

		m_s2->Use();
		glBindVertexArray(m_VAO);
		m_s2->SetVec4("Color", m_backColor);

		glBindTexture(GL_TEXTURE_2D, backTexture);
		m_s2->SetInt("isText", true);
		m_s2->SetTexture("text", 0);

		int windowW, windowH;
		SDL_GetWindowSize(window, &windowW, &windowH);

		float	xEnd {x + ((width) - (windowW * 0.5f)) / (float)(windowW * 0.5f)}, yEnd {y - length / (windowH * 0.5f)};

		SetVertices(x, y, xEnd, yEnd);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		m_s2->Unuse();

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	/* Draws a background rect with default color */
	void	TextRenderer::DrawRect(const float& x, const float& y, const float& width, const float& length, SDL_Window* window)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_ALWAYS);

		m_s2->Use();
		glBindVertexArray(m_VAO);
		m_s2->SetVec4("Color", m_backColor);
		m_s2->SetInt("isText", false);
		
		int windowW, windowH;
		SDL_GetWindowSize(window, &windowW, &windowH);

		float	xEnd {x + width}, yEnd {y - length};

		SetVertices(x, y, xEnd, yEnd);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		m_s2->Unuse();

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
}
