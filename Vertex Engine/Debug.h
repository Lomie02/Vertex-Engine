#pragma once
#include <iostream>
#include <vector>
class Debug
{
public:
	static void Log(std::string _log);
	static void LogWarning(std::string _log);
	static void LogError(std::string _log);
	//static std::vector<std::string> FetchLogs() { return m_ConsoleLogs; }

private: 
	Debug();
	//static std::vector<std::string> m_ConsoleLogs;
};

