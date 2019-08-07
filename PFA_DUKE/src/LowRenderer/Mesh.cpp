#include <math.h>

#include "Mesh.h"
#include "Debug.h"
#include "GameObject.hpp"

#define IMG_W 900
#define IMG_H 450

using namespace Core::Maths;
using namespace Core::DataStructure;

namespace LowRenderer
{
	Mesh::Mesh() :
		Component()
	{
	}

	Mesh::Mesh(GameObject* obj) :
		Component(obj)
	{
		m_object->HaveToRender(this);
	}

	Mesh::Mesh(const Mesh& mesh):
		Component(mesh.m_object),
		m_model {mesh.m_model},
		m_text {mesh.m_text},
		m_shader {mesh.m_shader},
		m_tile {mesh.m_tile}
	{
	}

	Mesh::~Mesh()
	{
	}

	const std::type_info&	Mesh::GetType() const
	{
		return typeid(*this);
	}

	Component*	Mesh::Copy() const
	{
		return new Mesh(*this);
	}

	void	Mesh::Destroy()
	{
		m_object->RemoveRenderer(this);
	}

	void	Mesh::SetTile(Core::Maths::Vec2 tile)
	{
		m_tile = tile;
	}

	void Mesh::SetWireframe(const bool& b)
	{
		m_isWireframe = b;
	}

	void 	Mesh::Init(Resources::Model* model, Resources::Texture* texture, Resources::Shader* shader)
	{
		m_isWireframe = false;
		m_shader = shader;
		m_model = model;
		m_text = texture;
		m_tile = 1;
	}

	void	Mesh::SetTexture(Resources::Texture* text)
	{
		m_text = text;
	}

	void 	Mesh::Draw(LowRenderer::Camera* cam)
	{
		/* call shaders */
		m_shader->Use();

		/* send texture to shader */
		m_shader->SetTexture("sampler", 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_text->GetID());

		/* send data to shader */
		m_shader->SetMat4("TRS", m_object->GetTransform());
		m_shader->SetMat4("model", (cam->GetMatrix()));
		m_shader->SetVec2("Tile", m_tile);

		/* access to VAO */
		glBindVertexArray(m_model->GetVAO());
		/* draw */
		glDrawElements(m_isWireframe? GL_LINES : GL_TRIANGLES, m_model->GetIndexSize(), GL_UNSIGNED_INT, 0);
		/* unbind */
		glBindVertexArray(0);
		m_shader->Unuse();
	}

	void 	Mesh::DrawHUD()
	{
		/* call shaders */
		m_shader->Use();

		/* send texture to shader */
		m_shader->SetTexture("sampler", 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_text->GetID());

		/* mat for orthographic proj */
		m_shader->SetMat4("TRS", Core::Maths::Mat4::CreateTRSMatrix({0.f, -0.05f, 0.f},
				{0.f, 0.f, 0.f}, {0.3f, 0.4f, 1.f}));

		/* access to VAO */
		glBindVertexArray(m_model->GetVAO());
		/* draw */
		glDrawElements(GL_TRIANGLES, m_model->GetIndexSize(), GL_UNSIGNED_INT, 0);
		/* unbind */
		glBindVertexArray(0);
		
		m_shader->Unuse();
	}
}
