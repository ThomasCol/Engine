#ifndef _ENDLEVEL_H_
#define _ENDLEVEL_H_

#include "ResourcesManager.h"
#include "Define.h"
#include "Behaviour.h"

using namespace Core::Maths;

class PlayerHandler;

class			Sound;

namespace       Resources
{
	class		Animation;
}

namespace LowRenderer
{
	class		Mesh;
}

class EndLevel : public virtual Core::DataStructure::Behaviour
{

private:
	PlayerHandler*					m_player;
	LowRenderer::Mesh*				m_endMesh;
	Resources::Animation*			m_endAnim;
	Sound*							m_endSound;
	Resources::ResourcesManager*	m_rm;
	bool							m_doEndSound;

public:
	EndLevel();
	EndLevel(Core::DataStructure::GameObject* object);
	EndLevel(const EndLevel& gm);
	EndLevel(EndLevel&& gm);
	~EndLevel() override;

	const std::type_info&                   GetType() const override;
	Core::DataStructure::Component*         Copy() const override;
	void									Start(Resources::ResourcesManager* rm) override;
	void									Update(CRenderWindow* render, double frameTime) override;

	inline Resources::Animation*			GetAnimation() const { return m_endAnim; };
	void									SetPlayer(PlayerHandler* p);

	void									Action(CRenderWindow* render, double frameTime);
};

#endif // !_ENDLEVEL_H_