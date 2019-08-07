#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <GL/glew.h>

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Component.h"
#include "Camera.h"

class		Sprite : public virtual Core::DataStructure::Component
{
	private:
		 /* instance */
		Resources::Model*				m_model;
		Resources::Texture*				m_text;
		Resources::Shader*				m_shader;
		Core::Maths::Vec3				m_scale;
		Core::Maths::Vec3				m_pos;

	public:
		/* constructor & destructor */
		Sprite();
		Sprite(Core::DataStructure::GameObject* obj);
		~Sprite();

		const std::type_info&			GetType() const override;
        Component*						Copy() const override;
		inline void						Destroy() override {}

		inline Resources::Texture*		GetTexture() const { return m_text; }
		void							SetTexture(Resources::Texture* text);

		/* for initialisation */
        void			 				Init(Resources::Model* model, Resources::Texture* texture,                                                                                                        Resources::Shader* shader);

		inline Core::Maths::Vec3		GetPos() const { return m_pos; }
		void							SetPos(Core::Maths::Vec3 pos);

		inline Core::Maths::Vec3		GetScale() const { return m_scale; }
		void							SetScale(Core::Maths::Vec3 scale);

		/* draw and send data to shader */
		void							Draw();
		void							Draw(const Core::Maths::Mat4& trs, const LowRenderer::Camera* cam);
		void							Draw(const Core::Maths::Mat4& trs, const LowRenderer::Camera* cam,
											std::vector<Core::Maths::Vec3>& hitPos);
};

#endif // !_SPRITES_H_
