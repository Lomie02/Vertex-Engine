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
	int m_FontSize = 24;
	std::string m_FontName = "arial.ttf";

	void ConfigureRenderSystems(glm::mat4 _camera);
private:

	int m_PreviousSize;
	std::string m_FontPath = "Builds/fonts/";
	
	void ConfigureSystems();
	VertexText2D* m_Text;
};