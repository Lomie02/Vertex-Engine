#pragma once
#include <fstream>
#include <ostream>
#include <iostream>

#include <vector>
#include "GameObject.h"
#include "Transform.h"
#include <sstream>

class VertexPrefs
{
public:
	static void GetFile(std::string _file, std::vector<GameObject*> _data);
	static void SaveFile(std::string _file, std::vector<GameObject*> _data);

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

private:
	VertexPrefs();


};

