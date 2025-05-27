#pragma once
#include <vector>
#include <filesystem>
#include <string>
#define VERTEX_LOG(log) Debug::Log(log,__FILE__, __LINE__)
#define VERTEX_ERROR(log) Debug::Error(log,__FILE__, __LINE__)
#define VERTEX_WARNING(log) Debug::Warning(log,__FILE__, __LINE__)


class Debug
{
public:
	static std::vector<std::string> m_ConsoleCommandLib;
	static std::vector<std::string> GetDebugLogs() { return m_ConsoleCommandLib; };

	static void Log(const std::string& _logData, const char* file, int line);
	static void Error(const std::string& _logData, const char* file, int line);
	static void Warning(const std::string& _logData, const char* file, int line);
private:
	Debug();
};

