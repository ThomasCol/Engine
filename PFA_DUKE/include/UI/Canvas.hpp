#ifndef __CANVAS__
#define __CANVAS__

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>

#include "UIObject.h"
#include "Component.h"
#include "GameObject.hpp"
#include "Vec2.h"
#include "TemplateButton.hpp"
#include "Text.h"
#include "UISprite.h"
#include "UIComponent.h"
#include "InputManager.h"

class CRenderWindow;

namespace UI
{
	class Canvas : public virtual Core::DataStructure::Component
	{
		private:
		protected:
			InputManager*	m_im;
			bool			m_isActive;
		public:
			Canvas();
			Canvas(const Canvas&);
			Canvas(Canvas&&);
			Canvas(Core::DataStructure::GameObject*);
			
			~Canvas();

			Canvas&	operator= (const Canvas&);

			template <typename Arg>
			TemplateButton<Arg>*	AddButton(const std::string& message, 
						const Core::Maths::Vec2& pos, const Core::Maths::Vec2& size, 
						const Arg& arg, Resources::ResourcesManager* m, float scale = 1,
						const Vec3& textColor = Vec3(1), const Vec4& backColor = Vec4(0, 0, 0, 1))
			{
				m_object->AddComponent<UIComponent>();
				std::vector<UIComponent*>	objects {m_object->GetComponents<UIComponent>()};
	
				objects.back()->SetUIObject(new TemplateButton<Arg>(pos.x, pos.y, size.x, size.y, message, arg, m, scale, textColor, backColor));
				return dynamic_cast<TemplateButton<Arg>*>(objects.back()->GetUIObject());
			}	

			TemplateButton<void>*	AddButton(const std::string& message,
				const Core::Maths::Vec2& pos, const Core::Maths::Vec2& size,
				Resources::ResourcesManager* m, float scale = 1, 
				const Vec3& textColor = Vec3(1), const Vec4& backColor = Vec4(0, 0, 0, 1))
			{
				m_object->AddComponent<UIComponent>();
				std::vector<UIComponent*>	objects{ m_object->GetComponents<UIComponent>() };

				objects.back()->SetUIObject(new TemplateButton<void>(pos.x, pos.y, size.x, size.y, message, m, scale, textColor, backColor));
				return dynamic_cast<TemplateButton<void>*>(objects.back()->GetUIObject());
			}

			void			Clear();
			Text*			AddText(const std::string& message,
								const Core::Maths::Vec2& center, const Core::Maths::Vec2& size,
								Resources::ResourcesManager* m, float scale = 1, Core::Maths::Vec3 color = Core::Maths::Vec3(1.f,0.f,0.f));
			UISprite*		AddSprite(const std::string& texture,
								const Core::Maths::Vec2& pos, const Core::Maths::Vec2& size,
								Resources::ResourcesManager* m);
			void			CheckClick(const float&, const float&);
			void			OnClick(const int&, const int&, SDL_Window*);
			
			void			OnDrawGUI(SDL_Window*) const override;
			
			const std::type_info&		GetType() const override;
			Core::DataStructure::Component*	Copy() const override;

			void			SetInput(InputManager*);
			void			DeleteIM();

			inline void		Enable() { m_isActive = true; }
			inline void		Disable() { m_isActive = false; }
	};
}


#endif
