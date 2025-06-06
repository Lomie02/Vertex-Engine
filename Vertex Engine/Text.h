#pragma once
#include "VertexRenderPackage.h"
#include "Camera.h"
#include "RectTransform.h"
#include "VertexComponent.h"
class Text : public VertexComponent
{
public:
	Text();
	~Text();
	std::string text = "Button";
	int m_FontSize = 24;

	void ChangeFont(std::string _path);
	void ConfigureRenderSystems(glm::mat4 _camera);

	void RenderEditorDisplay() override;
private:

	std::string m_FontName = "arial.ttf";
	int m_PreviousSize;
	std::string m_FontPath = "Builds/fonts/";
	
	void ConfigureSystems();
	VertexText2D* m_Text;

	RectTransform* ParentRect;
};