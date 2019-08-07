#include "EndLevel.h"
#include "PlayerHandler.h"
#include "Mesh.h"
#include "Animation.h"
#include "Sound.h"
#include "Define.h"
#include "Scene.h"

EndLevel::EndLevel()
{
}

EndLevel::EndLevel(Core::DataStructure::GameObject* object) :
	Component(object), Behaviour(object)
{
}

EndLevel::EndLevel(const EndLevel& gm)
{
}

EndLevel::EndLevel(EndLevel && gm)
{
	m_player = std::move(gm.m_player);
	m_endMesh = std::move(gm.m_endMesh);
}

EndLevel::~EndLevel()
{
	delete m_endAnim;
}

const std::type_info & EndLevel::GetType() const
{
	return typeid(*this);
}

Core::DataStructure::Component* EndLevel::Copy() const
{
	return new EndLevel(*this);
}

void EndLevel::Start(Resources::ResourcesManager* rm)
{
	m_rm = rm;
	m_doEndSound = true;
	m_endMesh = m_object->GetScene()->GetGameObject("w1")->GetComponent<LowRenderer::Mesh>();
	if (m_object->GetScene()->SecretMode())
	{
		if (m_endMesh->GetTexture()->GetFile().find("FEndTW.png") != std::string::npos)
			m_endAnim = new Resources::Animation(std::string("resources/media/Level/FEndTW"), 3);
		else
			m_endAnim = new Resources::Animation(std::string("resources/media/Level/SEndW"), 3);
	}
	else
	{
		if (m_endMesh->GetTexture()->GetFile().c_str() == std::string(ENDL) + PNG)
			m_endAnim = new Resources::Animation(std::string(ENDL), 3);
		else
			m_endAnim = new Resources::Animation(std::string(ENDL2), 3);
	}
	m_endAnim->SetSpeed(0.25f);
	m_endSound = m_rm->AddSound("SoundEndLevel", SOUNDENDLEVEL);
}

void EndLevel::SetPlayer(PlayerHandler * p)
{
	m_player = p;
}

void		EndLevel::Action(CRenderWindow* render, double frameTime)
{
	if (m_player->GetEnd())
	{
		m_endMesh->SetTexture(m_endAnim->SetFrameLoop(m_rm, frameTime, false));	
		if (m_doEndSound)
		{
			m_doEndSound = false;
			m_endSound->Play(frameTime, false);
		}
	}

}

void EndLevel::Update(CRenderWindow* render, double frameTime)
{
	Action(render, frameTime);
}