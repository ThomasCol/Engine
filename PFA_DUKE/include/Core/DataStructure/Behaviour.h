#ifndef _BEHAVIOUR_H_
#define _BEHAVIOUR_H_

#include "Component.h"
#include "ResourcesManager.h"


namespace Physics::Collision
{
	class Collider;
}

class	CRenderWindow;

namespace Core::DataStructure
{
    class Behaviour : public virtual Component
    {
    private:

    public:
        Behaviour() = default;
        Behaviour(GameObject* object);
        Behaviour(const Behaviour& behaviour) = default;
        Behaviour(Behaviour&& behaviour) = default;
        virtual ~Behaviour() = default;

        inline virtual void	Start(Resources::ResourcesManager* rm) {(void) rm;};
        inline virtual void	Update(CRenderWindow* win, double frameTime) {(void) win; (void)frameTime;};
		inline virtual void	OnCollision(Physics::Collision::Collider* collider) {};

		void				Destroy() override;
    };
}

#endif //_BEHAVIOUR_H_
