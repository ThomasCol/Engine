#include "ResourcesManager.h"
#include "Debug.h"

namespace Resources
{
	ResourcesManager::ResourcesManager()
	{
		if (FT_Init_FreeType(&m_ft))
			LOG(ERROR, "FREETYPE: Could not init FreeType Library");

	}

	ResourcesManager::~ResourcesManager()
	{
		/* delete all maps */
		for (auto it = m_modelMap.begin(); it != m_modelMap.end(); it++)
			delete it->second;

		for (auto it = m_textMap.begin(); it != m_textMap.end(); it++)
			delete it->second;

		for (auto it = m_shaderMap.begin(); it != m_shaderMap.end(); it++)
			delete it->second;
		
		for (auto it = m_soundMap.begin(); it != m_soundMap.end(); it++)
			delete it->second;
		
		for (auto it = m_fontMap.begin(); it != m_fontMap.end(); it++)
		{
			FT_Done_Face(it->second->second);
			for (auto it2 = it->second->first.begin(); it2 != it->second->first.end(); ++it2)
				delete it2->second;
			delete it->second;
		}
		FT_Done_FreeType(m_ft);
	}

	Shader*	ResourcesManager::AddShader(std::string vertexFile, std::string fragFile)
	{
		/* map shader and return them if already exist */
		std::pair<std::string, std::string> shaderPair;

		shaderPair = std::make_pair(vertexFile, fragFile);

		std::map<std::pair<std::string, std::string>, Shader*>::iterator	it;

		it = m_shaderMap.find(shaderPair);

		if (it != m_shaderMap.end())
		{
			return it->second;
		}

		Shader*	newShader =  new Shader(vertexFile.c_str(), fragFile.c_str());
		m_shaderMap.insert(std::pair<std::pair<std::string, std::string>, Shader*>
				(shaderPair, newShader));

		return newShader;
	}

	Model*	ResourcesManager::AddModel(std::string objFile)
	{
		/* map model and return them if already exist */
		std::map<std::string, Model*>::iterator	it;

		it = m_modelMap.find(objFile);

		if (it != m_modelMap.end())
		{
			return it->second;
		}

		Model*	newModel =  new Model();
		newModel->LoadOBJ(objFile.c_str());
		m_modelMap.insert(std::pair<std::string, Model*>(objFile, newModel));

		return newModel;
	}

	Texture*	ResourcesManager::AddTexture(std::string textFile)
	{
		/* map texture and return them if already exist */
		std::map<std::string, Texture*>::iterator	it;

		it = m_textMap.find(textFile);

		if (it != m_textMap.end())
		{
			return it->second;
		}

		Texture*	newText =  new Texture();
		newText->LoadTexture(textFile.c_str());
		m_textMap.insert(std::pair<std::string, Texture*>(textFile, newText));

		return newText;
	}

	Sound*          ResourcesManager::AddSound(std::string nameSound, std::string fileSound)
	{
		/* unordered_map sound and return them if already exist */
		std::unordered_map<std::string, Sound*>::iterator        it;

		it = m_soundMap.find(nameSound);

		if (it != m_soundMap.end())
			return it->second;

		Sound* newSound =  new Sound(fileSound);
		m_soundMap.insert(std::pair<std::string, Sound*>(nameSound, newSound));

		return newSound;
	}

	Character*      ResourcesManager::AddCharacter(const std::string& fontPath, const GLchar& c)
	{
		std::pair<std::map<GLchar, Character*>, FT_Face>* font = AddFont(fontPath);

		/* search for character and use it if already exist */
		std::map<GLchar, Character*>::iterator  it = font->first.find(c);
		if (it != font->first.end())
		{
			return it->second;
		}
		Character* newChar {new Character(font->second, c)};
		font->first.insert(std::pair<GLchar, Character*>({c, newChar}));

		return newChar;
	}

	std::pair<std::map<GLchar, Character*>, FT_Face>*       ResourcesManager::AddFont(const std::string& fontPath)
	{
		auto    it {m_fontMap.find(fontPath)};
		if (it != m_fontMap.end())
			return it->second;

		std::pair<std::map<GLchar, Character*>, FT_Face>*       newFont {new std::pair<std::map<GLchar, Character*>, FT_Face>()};
		if (FT_New_Face(m_ft, fontPath.c_str(), 0, &newFont->second))
			LOG(ERROR,  "FREETYPE: Failed to load font");
		else
			LOG(INFO, "Font Loaded");
		FT_Set_Pixel_Sizes(newFont->second, 64, 0);

		m_fontMap.insert(std::make_pair(fontPath, newFont));
		return newFont;

	}

	void	ResourcesManager::CreateMesh(LowRenderer::Mesh* mesh, const char* obj,
			const char* texture,
			const char* vertexShader,
			const char* fragmentShader)
	{
		/* create mesh using maps data and return it */
		Shader*		newShader {AddShader(vertexShader, fragmentShader)};
		Model*		newModel {AddModel(obj)};
		Texture*	newText {AddTexture(texture)};

		mesh->Init(newModel, newText,	newShader);
	}

	void    ResourcesManager::CreateSprite(Sprite* sprite, const char* obj,
			const char* texture,
			const char* vertexShader,
			const char* fragmentShader)
	{
		/* create mesh using maps data and return it */
		Shader*         newShader {AddShader(vertexShader, fragmentShader)};
		Model*          newModel {AddModel(obj)};
		Texture*        newText {AddTexture(texture)};

		sprite->Init(newModel, newText,   newShader);
	}

	void	ResourcesManager::InitLightsShader(std::vector<LowRenderer::Lights::Light*>& light)
	{
		for (auto it = m_shaderMap.begin(); it != m_shaderMap.end(); it++)
		{
			it->second->Use();
			/* set each light of array */
			for (size_t i = 0; i < light.size(); i++)
				light[i]->SendDataToShader(it->second, i);

			/* send material */
			it->second->SetVec3("material.ambient", { 0.2f, 0.2f, 0.2f });
			it->second->SetVec3("material.diffuse", { 1.f, 1.f, 1.f });
			it->second->SetVec3("material.specular", { 0.5f, 0.5f, 0.5f });
			it->second->SetFloat("material.shininess", 32.0f);
			it->second->Unuse();
		}
	}

	void	ResourcesManager::SetProjectionMatrix(const Core::Maths::Mat4& proj)
	{
		for (auto it = m_shaderMap.begin(); it != m_shaderMap.end(); it++)
		{
			it->second->Use();
			it->second->SetMat4("proj", proj);
			it->second->Unuse();
		}
	}
}
