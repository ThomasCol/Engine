#ifndef _RESOURCESMANAGER_H_
#define _RESOURCESMANAGER_H_

#include <map>
#include <unordered_map>
#include <string>
#include <utility>

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Sprites.h"
#include "Sound.h"
#include "Character.h"

namespace Resources
{
	/* manager for loading resources */
	class ResourcesManager
	{
	private:
		/* map of resources */
		std::map<std::string, Model*>												m_modelMap;
		std::map<std::string, Texture*>												m_textMap;
		std::map<std::pair<std::string, std::string>, Shader*>						m_shaderMap;
		std::unordered_map<std::string, Sound*>         							m_soundMap;
		std::map<std::string, std::pair<std::map<GLchar, Character*>, FT_Face>*>	m_fontMap;

		FT_Library																	m_ft;
	public:
		/* constructor & destructor */
		ResourcesManager();
		ResourcesManager(const ResourcesManager& resources) = delete;
		ResourcesManager(ResourcesManager&& resources) = delete;
		~ResourcesManager();

		/* check if resources already exist and create them if it doesn't */
		Shader*							AddShader(std::string vertexFile, std::string fragFile);
		Model*							AddModel(std::string objFile);
		Texture*						AddTexture(std::string textFile);
		Sound*							AddSound(std::string nameSound, std::string fileSound);
		Character*						AddCharacter(const std::string& fontPath, const GLchar&);
		std::pair<std::map<GLchar, Character*>, FT_Face>*  	AddFont(const std::string& fontPath);

		/* create a mesh with resources loaded */
		void							CreateMesh(LowRenderer::Mesh* mesh, const char* obj,
													const char* texture,
													const char* vertexShader,
													const char* fragmentShader);
		/* create a sprite with resources loaded */
		void							CreateSprite(Sprite* sprite, const char* obj,
													const char* texture,
													const char* vertexShader,
													const char* fragmentShader);

		void							InitLightsShader(std::vector<LowRenderer::Lights::Light*>& light);

		void							SetProjectionMatrix(const Core::Maths::Mat4& proj);
	};
}

#endif //_RESOURCESMANAGER_H_
