#pragma once
#include <fstream>
#include <ostream>
#include <iostream>

#include <vector>
#include "GameObject.h"
class VertexPrefs
{
public:
	static void GetFile(std::string _file, std::vector<GameObject*> _data);
	static void SaveFile(std::string _file, std::vector<GameObject*> _data);


private:
	VertexPrefs();
};

