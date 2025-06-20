#include "VertexPrefs.h"
#include "GameSettings.h"
void VertexPrefs::LoadConfig(const std::string& _filePath)
{

}

void VertexPrefs::SaveProject(ProjectSettings& _proj, const std::string& _filePath)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "Project" << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "AppName" << YAML::Value << _proj.m_ApplicationName;
	out << YAML::Key << "EngineEdition" << YAML::Value << _proj.m_EngineEdition;
	out << YAML::Key << "Aurthor" << YAML::Value << _proj.m_Aurthor;

	out << YAML::Key << "WindowWidth" << YAML::Value << _proj.m_WindowWidth;
	out << YAML::Key << "WindowHeight" << YAML::Value << _proj.m_WindowHeight;
	out << YAML::Key << "Fullscreen" << YAML::Value << _proj.m_FullScreen;

	out << YAML::Key << "Framework" << YAML::Value << _proj.m_FrameworkIsEditor;

	out << YAML::Key << "Scenes" << YAML::Value << YAML::BeginSeq;

	for (const auto& scenePath : _proj.m_Scenes) {
		out << scenePath;
	}

	out << YAML::EndSeq;

	out << YAML::EndMap; // Project end
	out << YAML::EndMap;

	std::ofstream fout(_filePath);
	fout << out.c_str();
}

ProjectSettings VertexPrefs::LoadProject(const std::string& _filePath)
{
	// If no project file exists, create a default settings profile
	if (!std::filesystem::exists(_filePath)) { return CreateDefaultProjectSettings(); }

	ProjectSettings settingsFromFile;
	YAML::Node config = YAML::LoadFile(_filePath);
	YAML::Node project = config["Project"];

	settingsFromFile.m_ApplicationName = project["AppName"].as<std::string>();
	settingsFromFile.m_EngineEdition = project["EngineEdition"].as<std::string>();
	settingsFromFile.m_Aurthor = project["Aurthor"].as<std::string>();

	settingsFromFile.m_WindowWidth = project["WindowWidth"].as<int>();
	settingsFromFile.m_WindowHeight = project["WindowHeight"].as<int>();
	settingsFromFile.m_FullScreen = project["Fullscreen"].as<bool>();

	if (project["Framework"])
		settingsFromFile.m_FrameworkIsEditor = project["Framework"].as<bool>();

	YAML::Node scenes = project["Scenes"];

	if (scenes && scenes.IsSequence()) {
		for (auto sceneData : scenes) {
			settingsFromFile.m_Scenes.push_back(sceneData.as<std::string>());
		}
	}

	return settingsFromFile;
}

ProjectSettings VertexPrefs::CreateDefaultProjectSettings()
{
	ProjectSettings defaultSettings;

	defaultSettings.m_ApplicationName = "untitled";
	defaultSettings.m_Aurthor = "company";
	defaultSettings.m_EngineEdition = ENGINE_EDITION;
	defaultSettings.m_FullScreen = true;

	defaultSettings.m_WindowWidth = 1920;
	defaultSettings.m_WindowHeight = 1080;
	defaultSettings.m_FrameworkIsEditor = true;

	defaultSettings.m_StartScene = "untitled.scene.scene"; 
	defaultSettings.m_Scenes.push_back("untitled.scene.scene");

		return defaultSettings;
}
