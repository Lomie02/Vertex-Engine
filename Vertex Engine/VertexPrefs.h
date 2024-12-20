#pragma once
#include <fstream>
#include <ostream>
#include <iostream>

#include <vector>
#include "GameObject.h"
#include "Transform.h"
#include <sstream>
#include <cstdio>
#include <direct.h>

/*
	Need to do a complete overhaul on the VertexPrefs system.
*/

class VertexPrefs
{
public:
	static void GetFile(std::string _file, std::vector<GameObject*> _data);
	static void SaveFile(std::string _file, std::vector<GameObject*> _data);
	static void SaveTextfile(std::string _fileName, std::string _text);
	static void OpenTextfile(std::string _fileName);

	static void SaveTransform(GameObject* _target);
	static Transform GetTransform(std::string _name);

	static void SetFloat(std::string _key, float _target);
	static float GetFloat(std::string _key);

	static void SetInt(std::string _key, int _target);
	static int GetInt(std::string _key);

	static void SetString(std::string _key, std::string _target);
	static std::string GetString(std::string _key);

	static void SetBool(std::string _key, bool _target);
	static bool Getbool(std::string _key);

	static void DeleteKey(std::string _key);
	static void DeleteAllKeys();

	static bool CreateFolder(std::string _key);
	static bool DeleteFolder(std::string _key);
private:
	VertexPrefs();
};

