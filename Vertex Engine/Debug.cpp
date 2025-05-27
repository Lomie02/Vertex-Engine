#include "Debug.h"

std::vector<std::string> Debug::m_ConsoleCommandLib;

void Debug::Log(const std::string& _logData, const char* file, int line)
{
	std::string fileOnly = std::filesystem::path(file).filename().string();
	std::string finalLog = "Vertex Log: " + _logData + " [" + fileOnly + ", " + std::to_string(line) + "]";
	m_ConsoleCommandLib.push_back(finalLog);
}

void Debug::Error(const std::string& _logData, const char* file, int line)
{
	std::string fileOnly = std::filesystem::path(file).filename().string();
	std::string finalLog = "Vertex Error: " + _logData + " [" + fileOnly + ", " + std::to_string(line) + "]";
	m_ConsoleCommandLib.push_back(finalLog);
}

void Debug::Warning(const std::string& _logData, const char* file, int line)
{
	std::string fileOnly = std::filesystem::path(file).filename().string();
	std::string finalLog = "Vertex Warning: " + _logData + " [" + fileOnly + ", " + std::to_string(line) + "]";
	m_ConsoleCommandLib.push_back(finalLog);
}
