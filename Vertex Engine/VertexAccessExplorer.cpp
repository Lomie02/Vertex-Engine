#include "VertexAccessExplorer.h"

bool VertexAccessExplorer:: OpenURL(const wchar_t* path)
{
	ShellExecute(0, 0, path, 0, 0, SW_HIDE);
	return true;
}

bool VertexAccessExplorer::OpenProgram(std::string _name)
{
	const char* path = _name.data();

	if (system(path)) {
		return true;
	}
	else {
		return false;
	}
}

void VertexAccessExplorer::OpenGoogle()
{
	ShellExecute(0, 0, L"http://google.com", 0, 0, SW_SHOW);
}

void VertexAccessExplorer::OpenBing()
{
	ShellExecute(0, 0, L"http://bing.com", 0, 0, SW_SHOW);
}
