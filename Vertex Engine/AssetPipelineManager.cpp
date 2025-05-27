#include "AssetPipelineManager.h"

// Init vars
std::vector<std::string>  AssetPipelineManager::m_SupportedTextureExtensionTypes;
std::vector<std::string>  AssetPipelineManager::m_SupportedSoundTypes;
std::vector<std::string>  AssetPipelineManager::m_Supported3dAssetTypes;
std::vector<std::string>  AssetPipelineManager::m_TextureAssetsLoaded;

AssetPipelineManager::AssetPipelineManager()
{
}

void AssetPipelineManager::Init()
{

	// Assign basic default filetypes Supported.
	// Assign basic default filetypes Supported.
	m_SupportedTextureExtensionTypes.push_back(".png");
	m_SupportedTextureExtensionTypes.push_back(".jpg");
	m_SupportedTextureExtensionTypes.push_back(".tga");
	m_SupportedTextureExtensionTypes.push_back(".gif");

}

void AssetPipelineManager::ScanFolderForTextures()
{
	for (const auto& ent : std::filesystem::recursive_directory_iterator("Builds/")) {

		if (ent.is_regular_file() && std::find(m_SupportedTextureExtensionTypes.begin(), m_SupportedTextureExtensionTypes.end(), ent.path().filename().extension().string()) != m_SupportedTextureExtensionTypes.end()) {
			if (!ResourceManager::DoesTextureExist(ent.path().stem().string().c_str())) {

				std::string temp = std::string(ent.path().string().c_str());
				ResourceManager::LoadTexture(temp.c_str(), ent.path().stem().string().c_str()); std::cout << "Loaded: " << ent.path().filename().string() << std::endl;
			}
		}
	}
}

