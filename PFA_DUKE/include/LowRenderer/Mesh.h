#ifndef _MESH_H_
#define _MESH_H_

#include <GL/glew.h>

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Component.h"

namespace LowRenderer
{
	class Mesh : public virtual Core::DataStructure::Component
	{
	private:
		/* instance */
		Resources::Model*				m_model;
		Resources::Texture*				m_text;
		Resources::Shader*				m_shader;
		Core::Maths::Vec2				m_tile;
		bool							m_isWireframe;

	public:
		/* constructor & destructor */
		Mesh();
		Mesh(Core::DataStructure::GameObject* obj);
		Mesh(const Mesh& mesh);
		Mesh(Mesh&& mesh);
		~Mesh();

		/* for initialisation */
		void 						Init(Resources::Model* model, Resources::Texture* texture,
													Resources::Shader* shader);

		/* draw and send data to shader */
		void 						Draw(LowRenderer::Camera* cam);


		void 						DrawHUD();

		const std::type_info&		GetType() const override;
		Component*					Copy() const override;

		void						Destroy() override;

		void						SetTile(Core::Maths::Vec2 tile);
		void						SetWireframe(const bool& b);
		inline Resources::Texture*	GetTexture() const { return m_text; }		
		void						SetTexture(Resources::Texture* text);

		/* send light data to the shader */
		void 						SendLight(std::vector<LowRenderer::Lights::Light*>& light);
	};
}

#endif //_MESH_H_
