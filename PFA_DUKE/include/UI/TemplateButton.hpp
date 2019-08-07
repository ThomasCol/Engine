#ifndef __TEMPLATE_BUTTON__
#define __TEMPLATE_BUTTON__

#include "ButtonBase.h"
#include "Event.hpp"

namespace UI
{
	template <typename Arg>
		class TemplateButton : public virtual ButtonBase
	{
		protected:
				Core::Event<Arg>				m_onClick;
				std::function<bool(Arg)>		m_isClickable;
				Arg								m_param;
		public:
			TemplateButton(const float& x, const float& y, const float& width, 
					const float& height, const std::string& message, 
					Arg param, Resources::ResourcesManager* m, float scale = 1,
					const Vec3& textColor = Vec3(1), const Vec4& backColor = Vec4(0, 0, 0, 1))
				:		UIObject(x, y, width, height), ButtonBase (x, y, width, height, message, m, scale, textColor, backColor),
						m_param {param}

			{
				//static_assert(sizeof...(Arg) >= 1, "Error: Requires at least one argument, can be void.");
				m_isClickable = nullptr;
			}

			TemplateButton(const float& x, const float& y, const float& width, 
					const float& height, const std::string& message, 
					std::function<void(Arg)> callback, Arg param, 
					Resources::ResourcesManager* m, float scale = 1,
					const Vec3& textColor = Vec3(1), const Vec4& backColor = Vec4(0, 0, 0, 1))
				:	UIObject(x, y, width, height), ButtonBase (x, y, width, height, message, m, scale, textColor, backColor), 
					m_param {param}
			{
				//static_assert(sizeof...(Arg) >= 1, "Error: Requires at least one argument, can be void.");
				m_onClick += callback;
				m_isClickable = nullptr;
			}

			TemplateButton(const TemplateButton&);

			bool								Press(Arg args)
			{	
				if (m_isClickable != nullptr && !m_isClickable())
					return false;
				m_onClick(args);
				return true;
			}
			
			bool								Press()
			{	
				if (m_isClickable != nullptr && !m_isClickable(m_param))
					return false;
				m_onClick(m_param);
				return true;
			}	
		
			bool								IsPressed(const float& x, const float& y)
			{
				if (x >= m_x && x < m_x + m_width && 
						y <= m_y && y > m_y - m_height)
				{
					if (m_isClickable != nullptr && !m_isClickable(m_param))
						return false;
					m_onClick(m_param);
					return true;
				}

				return false;
			}


			inline std::function<void(Arg)>		GetCallback() const {return m_onClick;};
			inline std::function<void(Arg)>		GetIsClickable() const {return m_isClickable;};
			inline std::tuple<Arg>				GetParams() const {return m_param;};
			void								SetParams(Arg args) {m_param = std::make_tuple(args);};

			void								Draw(LowRenderer::TextRenderer& tr, SDL_Window* w) const
			{
				tr.RenderButton(m_message, m_x, m_y, m_width, m_height, w);
			}

			Core::Event<Arg>&					operator+=(std::function<void(Arg)> func)
			{
				return m_onClick += func;
			}

			Core::Event<Arg>&					operator-=(std::function<void(Arg)> func)
			{
				return m_onClick -= func;
			}
	};
	
	template<>
	class TemplateButton<void> : public virtual ButtonBase
	{
		protected:
			Core::Event<>				m_onClick;
			std::function<bool(void)>	m_isClickable;
		public:
			TemplateButton(const float& x, const float& y, const float& width, 
					const float& height, const std::string& message, 
					Resources::ResourcesManager* m, float scale = 1,
					const Vec3& textColor = Vec3(1), const Vec4& backColor = Vec4(0, 0, 0, 1))
				:	UIObject(x, y, width, height), ButtonBase (x, y, width, height, message, m, scale, textColor, backColor)
			{
				m_isClickable = nullptr;
			}

			TemplateButton(const float& x, const float& y, const float& width, const float& height, const std::string& message, 
				std::function<void(void)> callback, Resources::ResourcesManager* m, float scale = 1,
				const Vec3& textColor = Vec3(1), const Vec4& backColor = Vec4(0, 0, 0, 1))
				: UIObject(x, y, width, height), ButtonBase(x, y, width, height, message, m, scale, textColor, backColor)
			{
				m_onClick += callback;
				m_isClickable = nullptr;
			}

			TemplateButton(const TemplateButton&);

			bool									Press()
			{	
				if (m_isClickable != nullptr && !m_isClickable())
					return false;
				m_onClick();
				return true;
			}	
		
			bool									IsPressed(const float& x, const float& y)
			{
				if (x >= m_x && x < m_x + m_width && 
						y <= m_y && y > m_y - m_height)
				{
					if (m_isClickable != nullptr && !m_isClickable())
						return false;
					m_onClick();
					return true;
				}
				return false;
			}


			inline std::function<void(void)>		GetCallback() const {return m_onClick;};
			inline std::function<void(void)>		GetIsClickable() const {return m_isClickable;};
			
			void									Draw(LowRenderer::TextRenderer& tr, SDL_Window* w) const
			{
				tr.RenderButton(m_message, m_x, m_y, m_width, m_height, w);
			}

			Core::Event<>&							operator+=(std::function<void()> func)
			{
				return m_onClick += func;
			}

			Core::Event<>&							operator-=(std::function<void()> func)
			{
				return m_onClick -= func;
			}

	};

}


#endif
