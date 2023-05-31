#include "Debug.h"
#include "AdvancedSettings.h"
void Debug::Log(std::string _log)
{
	if (ENABLE_VERTEX_CONSOLE)
	{
		//m_ConsoleLogs.push_back("<comment> " + _log);
	}
}

void Debug::LogWarning(std::string _log)
{
	if (ENABLE_VERTEX_CONSOLE) {
		//m_ConsoleLogs.push_back("<!!Warning!!> " + _log);
	}
}

void Debug::LogError(std::string _log)
{

	if (ENABLE_VERTEX_CONSOLE) 
	{
		//m_ConsoleLogs.push_back("<ERROR> " + _log);
	}
}
