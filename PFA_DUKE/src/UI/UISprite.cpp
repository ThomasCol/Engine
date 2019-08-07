#include "UISprite.h"
#include "ResourcesManager.h"
#include <GL/glew.h>

namespace UI
{
	UISprite::UISprite(const float& x, const float& y, const float& width,
		const float& height, Resources::Texture* t, Resources::ResourcesManager* m)
		: UIObject(x, y, width, height), m_text{ t }, m_shader{ m->AddShader("resources/shader/vertexHUD.vsh", "resources/shader/fragmentHUD.fsh") }, m_model{m->AddModel("resources/obj/face.obj")}
	{
	}

	UISprite::UISprite(const UISprite& s) : UIObject(s), m_text{ s.m_text }, m_shader{ s.m_shader }
	{
	}

	UISprite&	UISprite::operator= (const UISprite& s)
	{
		m_text = s.m_text;
		m_shader = s.m_shader;
	}

	void		UISprite::Draw(SDL_Window* w) const
	{
		glDepthFunc(GL_ALWAYS);
		m_shader->Use();


		/* send texture to shader */
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_text->GetID());
		m_shader->SetTexture("sampler", 0);


		/* send data to shader */
		m_shader->SetMat4("TRS", Core::Maths::Mat4::CreateTRSMatrix(Vec3(m_x, m_y, 0), Core::Maths::Vec3::zero, Vec3(m_width, m_height, 1)));

		/* access to VAO */
		glBindVertexArray(m_model->GetVAO());
		/* draw */
		glDrawElements(GL_TRIANGLES, m_model->GetIndexSize(), GL_UNSIGNED_INT, 0);
		/* unbind */
		glBindVertexArray(0); 
		glDepthFunc(GL_LESS);
		m_shader->Unuse();
	}
}