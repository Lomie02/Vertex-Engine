#include "Text.h"
#include "ResourceManager.h"
#include "GameSettings.h"

Text::Text() : GameObject("Text_1")
{
	m_Text = new VertexText2D(1920,1080);
	material.colour = glm::vec4(0, 1, 0, 1);
	transform.scale = 1.0f;
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
	std::cout << "Render Text" << std::endl;
}

void Text::ConfigureSystems()
{

	m_Text->Load("Builds/fonts/arial.ttf", 24);
	std::cout << "Booted" << std::endl;
}
