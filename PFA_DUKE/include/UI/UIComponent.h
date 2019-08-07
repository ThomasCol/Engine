#ifndef __UICOMPONENT__
#define __UICOMPONENT__

#include <SDL2/SDL.h>

#include "Component.h"
#include "UIObject.h"

namespace UI
{
	class UIComponent : public virtual Core::DataStructure::Component
	{
		private:
			UIObject*	m_object;
		protected:
		public:
			UIComponent();
			UIComponent(Core::DataStructure::GameObject*);
			UIComponent(const UIComponent&);
			UIComponent(UIComponent&&);

			virtual ~UIComponent();

			UIComponent&					operator=(const UIComponent&);

			const std::type_info&			GetType() const override;
			Core::DataStructure::Component*	Copy() const override;
			
			void							Draw(SDL_Window*) const;

			void							OnClick(const float&, const float&);
			bool							Press();

			void							SetUIObject(UIObject*);
			inline UIObject*				GetUIObject() const {return m_object;};
	};
}

#endif
