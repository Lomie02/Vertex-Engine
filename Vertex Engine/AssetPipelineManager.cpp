#include "AssetPipelineManager.h"

// Init vars
std::vector<std::string>  AssetPipelineManager::m_SupportedTextureExtensionTypes;
std::vector<std::string>  AssetPipelineManager::m_SupportedSoundTypes;
std::vector<std::string>  AssetPipelineManager::m_Supported3dAssetTypes;
std::vector<std::string>  AssetPipelineManager::m_TextureAssetsLoaded;
bool AssetPipelineManager::m_HotReloadInProgress;

AssetPipelineManager::AssetPipelineManager()
{
}

void AssetPipelineManager::Init()
{

	m_SupportedTextureExtensionTypes.push_back(".png");
	m_SupportedTextureExtensionTypes.push_back(".jpg");
	m_SupportedTextureExtensionTypes.push_back(".tga");
	m_SupportedTextureExtensionTypes.push_back(".gif");
	m_SupportedTextureExtensionTypes.push_back(".jpeg");

	m_Supported3dAssetTypes.push_back(".obj");
	m_Supported3dAssetTypes.push_back(".fbx");
}

void AssetPipelineManager::ScanFolderForTextures()
{
	for (const auto& ent : std::filesystem::recursive_directory_iterator("Builds/")) {
		if (ent.is_regular_file() && std::find(m_SupportedTextureExtensionTypes.begin(), m_SupportedTextureExtensionTypes.end(), ent.path().filename().extension().string()) != m_SupportedTextureExtensionTypes.end()) {
			if (!ResourceManager::DoesTextureExist(ent.path().stem().string().c_str())) {
				std::string temp = std::string(ent.path().string().c_str());
				ResourceManager::LoadTexture(temp.c_str(), ent.path().stem().string().c_str());
			}
		}
	}
	m_HotReloadInProgress = false;
}

void AssetPipelineManager::HotReload()
{
	m_HotReloadInProgress = true;
	ResourceManager::Textures.clear();
	ScanFolderForTextures();
	VERTEX_LOG("Reloading Complete. ");
}

void AssetPipelineManager::ScanFolderForMesh()
{
	for (const auto& ent : std::filesystem::recursive_directory_iterator("Builds/")) {
		if (ent.is_regular_file() && std::find(m_Supported3dAssetTypes.begin(), m_Supported3dAssetTypes.end(), ent.path().filename().extension().string()) != m_Supported3dAssetTypes.end()) {
			if (!ResourceManager::DoesTextureExist(ent.path().stem().string().c_str())) {
				std::string temp = std::string(ent.path().string().c_str());
				ResourceManager::LoadModel(temp.c_str(), ent.path().stem().string().c_str());
			}
		}
	}
}
