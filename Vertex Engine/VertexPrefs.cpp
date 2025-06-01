#include "VertexPrefs.h"

void VertexPrefs::GetFile(std::string _file, std::vector<GameObject*> _data)
{
	std::ifstream Vertex("Builds/Data/0x021_0KAW/" + _file + ".bin", std::ios::in | std::ios::binary);

	int current = 0;

	if (Vertex.is_open())
	{
		while (!Vertex.eof() && Vertex.peek() != EOF)
		{
			Vertex.read((char*) &_data[current], sizeof(GameObject));
			current++;
		}
		Vertex.close();
	}
}

void VertexPrefs::SaveFile(std::string _file, std::vector<GameObject*> _data)
{
	std::ofstream Vertex("Builds/Data/0x021_0KAW/" + _file + ".bin", std::ios::out | std::ios::binary);

	if (Vertex.is_open())
	{
		for (int i = 0; i < _data.size(); i++)
		{
			Vertex.write((char*)&_data[i], sizeof(GameObject));
		}
		Vertex.close();
	}
}

void VertexPrefs::SaveTransform(GameObject* _target)
{
	std::string ObjectsName = _target->name;

	std::ofstream Xpos("Builds/Data/0x021_0KAW/" + ObjectsName + "x1" + ".txt");
	std::ofstream Ypos("Builds/Data/0x021_0KAW/" + ObjectsName + "y1" + ".txt");
	if (Xpos.is_open())
	{
		Xpos << _target->transform->position.x;
		Xpos.close();
	}

	if (Ypos.is_open())
	{
		Ypos << _target->transform->position.y;
		Ypos.close();

	}
}

void VertexPrefs::SaveTextfile(std::string _fileName,std::string _text)
{
	std::ofstream Vertex("Builds/Data/0x021_0KAW/" + _fileName + ".txt", std::ios::out | std::ios::binary);

	if (Vertex.is_open())
	{
		Vertex << _text;
		Vertex.close();
	}
}

void VertexPrefs::OpenTextfile(std::string _fileName)
{
	//TODO: Implement text file opening.
}

Transform VertexPrefs::GetTransform(std::string _name)
{
	std::ifstream Xpos("Builds/Data/0x021_0KAW/" + _name + "x1" + ".txt");
	std::ifstream Ypos("Builds/Data/0x021_0KAW/" + _name + "y1" + ".txt");

	Transform convertedTransform;

	std::string DataX;
	std::string DataY;

	std::stringstream Con;
	std::stringstream Con1;

	if (Xpos.is_open())
	{
		Xpos >> DataX;

		Con << DataX;
		Con >> convertedTransform.position.x;
		Xpos.close();
	}

	if (Ypos.is_open())
	{
		Ypos >> DataY;

		Con1 << DataY;
		Con1 >> convertedTransform.position.y;
		Ypos.close();
	}
	return convertedTransform;
}

void VertexPrefs::SetFloat(std::string _key, float _target)
{
	std::ofstream Vertex("Builds/Data/0x021_0KAW/" + _key);

	if (Vertex.is_open())
	{
		for (int i = Vertex.beg; i != Vertex.end; i++) {

			Vertex << _target;
		}
		Vertex.close();
	}
}

float VertexPrefs::GetFloat(std::string _key)
{
	std::ifstream Vertex("Builds/Data/0x021_0KAW/" + _key + ".txt");
	std::string line;

	std::string Data;

	std::stringstream Con;

	float ConvertedValue = 0;
	if (Vertex.is_open())
	{
		while (std::getline(Vertex, line)) {

			for (int i = Vertex.beg; i != Vertex.end; i++)
			{
				Vertex >> Data;
			}
		}
		Vertex.close();
	}
	Con << Data;
	Con >> ConvertedValue;

	return ConvertedValue;
}

void VertexPrefs::SetInt(std::string _key, int _target)
{
	std::ofstream Vertex("Builds/Data/0x021_0KAW/" + _key + ".bin");

	if (Vertex.is_open())
	{
		Vertex.write((char*)&_target, sizeof(int));
		Vertex.close();
	}
}

int VertexPrefs::GetInt(std::string _key)
{
	std::ifstream Vertex("Builds/Data/0x021_0KAW/" + _key + ".bin");
	std::string line;

	std::string Data;

	int ConvertedValue = 0;
	if (Vertex.is_open())
	{
		while (!Vertex.eof() && Vertex.peek() != EOF)
		{
			Vertex.read((char*)&ConvertedValue, sizeof(int));
		}
		Vertex.close();
	}

	return ConvertedValue;
}

void VertexPrefs::SetString(std::string _key, std::string _target)
{
	std::ofstream Vertex("Builds/Data/0x021_0KAW/" + _key + ".bin");

	if (Vertex.is_open())
	{
		Vertex.write((char*)&_target, sizeof(std::string));
		Vertex.close();
	}
}

std::string VertexPrefs::GetString(std::string _key)
{
	std::ifstream Vertex("Builds/Data/0x021_0KAW/" + _key + ".bin");
	std::string line;

	std::string ConvertedValue = "null";
	if (Vertex.is_open())
	{
		while (!Vertex.eof() && Vertex.peek() != EOF)
		{
			Vertex.read((char*)&ConvertedValue, sizeof(std::string));
		}
		Vertex.close();
	}

	return ConvertedValue;
}

void VertexPrefs::SetBool(std::string _key, bool _target)
{
	std::ofstream Vertex("Builds/Data/0x021_0KAW/" + _key + ".bin");

	if (Vertex.is_open())
	{
		Vertex.write((char*)&_target, sizeof(bool));
		Vertex.close();
	}
}

bool VertexPrefs::Getbool(std::string _key)
{
	std::ifstream Vertex("Builds/Data/0x021_0KAW/" + _key + ".bin");
	std::string line;
	std::string Data;

	std::stringstream Con;

	bool ConvertedValue = "null";

	if (Vertex.is_open())
	{
		while (!Vertex.eof() && Vertex.peek() != EOF)
		{
			Vertex.read((char*)&ConvertedValue, sizeof(bool));
		}
		Vertex.close();
	}

	return ConvertedValue;
}

void VertexPrefs::DeleteKey(std::string _key)
{
	std::string Path = "Builds/Data/0x021_0KAW/" + _key + ".bin";

	std::remove(Path.c_str());

}

void VertexPrefs::DeleteAllKeys()
{
	std::remove("Builds/Data/0x021_0KAW/");
	int i = 0;
	//i = mkdir("Builds/Data/0x021_0KAW");
}
