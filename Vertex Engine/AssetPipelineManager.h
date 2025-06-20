#pragma once
#include <filesystem>
#include <iostream>
#include "VertexGeneral.h"
#include <vector>
#include <string.h>
#include "Vertex.h"

#include <assimp/Importer.hpp>

class AssetPipelineManager
{
public:
	static void Init();
	static void AddCustomImageType(std::string _extension) { m_SupportedTextureExtensionTypes.push_back(_extension); }

	static void ScanFolderForTextures();
	static void HotReload();
	static bool IsReloadInProgress() { return m_HotReloadInProgress; }
	static void ScanFolderForMesh();
private:
	AssetPipelineManager();
	// File Types
	static std::vector<std::string> m_SupportedTextureExtensionTypes; // Textures
	static std::vector<std::string> m_SupportedSoundTypes; // Audio Files
	static std::vector<std::string> m_Supported3dAssetTypes; // 3D Models

	// Asset Holders.
	static std::unordered_map<std::string, std::shared_ptr<MeshData>> m_Models;
	static std::vector<std::string> m_TextureAssetsLoaded;
	static bool m_HasBeenInit;
	static bool m_HotReloadInProgress;
};

