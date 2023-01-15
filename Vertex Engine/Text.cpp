#include "Text.h"
#include "ResourceManager.h"
#include "GameSettings.h"

Text::Text() : GameObject("Text_1")
{
	m_Text = new VertexText2D(PROJECT_RESOLUTION);
	material.colour = glm::vec4(0, 1, 0, 1);
	transform.scale = 0.1f;
	ConfigureSystems();
}

Text::~Text()
{
	delete m_Text;
	m_Text = nullptr;
}

void Text::ConfigureRenderSystems(glm::mat4 _camera)
{
	m_Text->Text2D(text,transform.position.x, transform.position.y, transform.scale, material.colour,_camera);
}

void Text::ConfigureSystems()
{

	m_Text->Load(m_FontPath + m_FontName, m_FontSize);
}
