#pragma once
#include <fstream>
#include <ostream>
#include <iostream>

#include <filesystem>
#include <sstream>
#include <cstdio>
#include <direct.h>

#include <yaml-cpp/yaml.h>
#include <string>

#include <vector>
#include "VertexScene.h"

// Project Settings Container
struct ProjectSettings {

	std::string m_ApplicationName;
	std::string m_EngineEdition;
	std::string m_Aurthor;

	std::string m_StartScene = "Main.scene.scene";

	int m_WindowWidth = 1920;
	int m_WindowHeight = 1080;
	bool m_FullScreen = false;

	bool m_FrameworkIsEditor;
	std::vector<std::string> m_Scenes;
};

class VertexPrefs
{
public:

	void CreateConfig(const std::string& _filePath);
	void LoadConfig(const std::string& _filePath);

	void SaveProject(ProjectSettings& _proj, const std::string& _filePath);
	ProjectSettings LoadProject(const std::string& _filePath);

	void SaveScene(VertexScene* _scene, const std::string& _filePath);

private:

	// Create a default project if one cant be loaded.
	ProjectSettings CreateDefaultProjectSettings();
};

