#pragma once
#include "VertexRenderPackage.h"
#include "GameObject.h"
#include "Camera.h"
class Text : public GameObject
{
public:
	Text();
	~Text();
	std::string text = "Button";

	void ConfigureRenderSystems(glm::mat4 _camera);
private:

	int m_FontSize = 24;
	std::string m_FontPath = "Builds/fonts/arial.ttf";
	void ConfigureSystems();
	VertexText2D* m_Text;
};

