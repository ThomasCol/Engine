#include "Sprites.h"
#include <math.h>
#include "Debug.h"

#include "GameObject.hpp"

using namespace Core::DataStructure;

Sprite::Sprite(): Component()
{
}

Sprite::Sprite(GameObject* obj) :
	Component(obj)
{
}

Sprite::~Sprite()
{
}

const std::type_info&   Sprite::GetType() const
{
	return typeid(*this);
}

Core::DataStructure::Component* Sprite::Copy() const
{
	return new Sprite(*this);
}

void	Sprite::SetTexture(Resources::Texture* text)
{
	m_text = text;
}

void	Sprite::SetPos(Core::Maths::Vec3 pos)
{
	m_pos = pos;
}

void	Sprite::SetScale(Core::Maths::Vec3 scale)
{
	m_scale = scale;
}

void    Sprite::Init(Resources::Model* model, Resources::Texture* texture, Resources::Shader* shader)
{
	m_shader = shader;
	m_model = model;
	m_text = texture;
}

void    Sprite::Draw()
{
	/* call shaders */
	glDepthFunc(GL_ALWAYS);
	m_shader->Use();

	/* send texture to shader */
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_text->GetID());
	m_shader->SetTexture("sampler", 0);

	/* send data to shader */
	m_shader->SetMat4("TRS", Core::Maths::Mat4::CreateTRSMatrix(m_pos, Core::Maths::Vec3::zero, m_scale));

	/* access to VAO */
	glBindVertexArray(m_model->GetVAO());
	/* draw */
	glDrawElements(GL_TRIANGLES, m_model->GetIndexSize(), GL_UNSIGNED_INT, 0);
	/* unbind */
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	m_shader->Unuse();
}

void    Sprite::Draw(const Core::Maths::Mat4& trs, const LowRenderer::Camera* cam)
{
	/* call shaders */
	m_shader->Use();

	/* send texture to shader */
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_text->GetID());
	m_shader->SetTexture("sampler", 0);

	/* send data to shader */
	m_shader->SetMat4("TRS", trs);
	m_shader->SetMat4("model", (cam->GetMatrix()));

	/* access to VAO */
	glBindVertexArray(m_model->GetVAO());
	/* draw */
	glDrawElements(GL_TRIANGLES, m_model->GetIndexSize(), GL_UNSIGNED_INT, 0);
	/* unbind */
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	m_shader->Unuse();
}

void    Sprite::Draw(const Core::Maths::Mat4& trs, const LowRenderer::Camera* cam, std::vector<Core::Maths::Vec3>& hitPos)
{
	/* call shaders */
	m_shader->Use();

	/* send texture to shader */
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_text->GetID());
	m_shader->SetTexture("sampler", 0);

	/* send data to shader */
	m_shader->SetMat4("TRS", trs);
	m_shader->SetMat4("model", (cam->GetMatrix()));

	if (!hitPos.empty())
	{
		int idx{ (int)hitPos.size() - 1} ;
		m_shader->SetVec3("hitPos", hitPos[0]);
	}

	/* access to VAO */
	glBindVertexArray(m_model->GetVAO());
	/* draw */
	glDrawElements(GL_TRIANGLES, m_model->GetIndexSize(), GL_UNSIGNED_INT, 0);
	/* unbind */
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	m_shader->Unuse();
}
