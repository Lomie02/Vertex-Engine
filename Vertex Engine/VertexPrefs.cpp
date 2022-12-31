#include "VertexPrefs.h"

void VertexPrefs::GetFile(std::string _file, std::vector<GameObject*> _data)
{
	std::ifstream Vertex("Builds/Data/" + _file);
	std::string line;
	if (Vertex.is_open())
	{
		while (std::getline(Vertex, line)) {

			for (int i = Vertex.beg; i != Vertex.end; i + 2) {

				if (i < _data.size())
				{
					Vertex >>_data.at(i)->transform.position.x;
					Vertex >>_data.at(i)->transform.position.y;
				}
			}
			printf("%s", line.c_str());
		}
		Vertex.close();
	}
}

void VertexPrefs::SaveFile(std::string _file, std::vector<GameObject*> _data)
{
	std::ofstream Vertex("Builds/Data/" + _file);



	if (Vertex.is_open())
	{
		for (int i = Vertex.beg; i != Vertex.end; i++) {

			if (i < _data.size())
			{
				Vertex << _data.at(i)->transform.position.x << _data.at(i)->transform.position.y;
			}
		}
		Vertex.close();
	}

}
