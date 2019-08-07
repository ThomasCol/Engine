#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <typeinfo>

struct	SDL_Window;

namespace Physics
{
	class Transform;
}


namespace Core::DataStructure
{
	class GameObject;
    
	class Component
    {
    protected:
        GameObject*         m_object;
		bool				m_isDestroyed;
		bool				m_isEnable;

    public:
        Component();
        Component(const Component& comp);
        Component(Component&& comp);
        Component(GameObject* obj);
        virtual ~Component() = default;

        virtual const std::type_info&   GetType() const = 0;

        virtual Component*              Copy() const = 0;

		inline virtual	void			Destroy() {};

		GameObject*						GetObject();

		void							EreaseEndOfFrame();
		bool							Isdestroyed() const;
		inline bool						IsEnable() const { return m_isEnable; };
		inline void						SetActive(bool enable) { m_isEnable = enable; };

		inline virtual void				OnDrawGUI(SDL_Window* window) const {};
    };
}

#endif //_COMPONENT_H_
