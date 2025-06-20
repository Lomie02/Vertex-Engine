#include "VertexPrefs.h"

void VertexPrefs::LoadConfig(const std::string& _filePath)
{
	
}

void VertexPrefs::SaveProject(ProjectSettings& _proj, const std::string& _filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;

	out << YAML::Key << "AppName" << YAML::Value << _proj.m_ApplicationName;
	out << YAML::Key << "EngineEdition" << YAML::Value << _proj.m_EngineEdition;
	out << YAML::Key << "Aurthor" << YAML::Value << _proj.m_Aurthor;

	out << YAML::Key << "WindowWidth" << YAML::Value << _proj.m_WindowWidth;
	out << YAML::Key << "WindowHeight" << YAML::Value << _proj.m_WindowHeight;
	out << YAML::Key << "Fullscreen" << YAML::Value << _proj.m_FullScreen;

	std::ofstream fout(_filePath);
	fout << out.c_str();
}

ProjectSettings VertexPrefs::LoadProject(const std::string& _filePath)
{
	if (!std::filesystem::exists(_filePath)) { ProjectSettings empty; return empty; }

	ProjectSettings settingsFromFile;
	YAML::Node config = YAML::LoadFile(_filePath);

	settingsFromFile.m_ApplicationName = config["AppName"].as<std::string>();
	settingsFromFile.m_EngineEdition = config["EngineEdition"].as<std::string>();
	settingsFromFile.m_Aurthor = config["Aurthor"].as<std::string>();

	settingsFromFile.m_WindowWidth = config["WindowWidth"].as<int>();
	settingsFromFile.m_WindowHeight = config["WindowHeight"].as<int>();
	settingsFromFile.m_FullScreen = config["Fullscreen"].as<bool>();

	return settingsFromFile;
}

//void VertexPrefs::SaveScene(VertexScene* _scene, const std::string& _filePath)
//{
//}
//
//std::shared_ptr<VertexScene> VertexPrefs::LoadScene(const std::string& _filePath)
//{
//	if (!std::filesystem::exists(_filePath)) return nullptr;
//
//	auto scene = std::make_shared<VertexScene>();
//	YAML::Node config = YAML::LoadFile(_filePath);
//
//	scene.get()->m_SceneName = config["sceneName"].as<std::string>();
//
//	auto gameObjects = config["GameObjects"];
//
//	if (gameObjects) {
//
//		for (auto obj : gameObjects) {
//			auto Gameobject = std::make_shared<GameObject>();
//			Gameobject.get()->name = config["Name"].as<std::string>();
//
//		}
//	}
//
//	return scene;
//}
