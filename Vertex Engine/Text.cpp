#include "Text.h"
#include "ResourceManager.h"
#include "GameSettings.h"

Text::Text()
{
	m_Text = new VertexText2D(PROJECT_RESOLUTION);
	ConfigureSystems();
}

Text::~Text()
{
	delete m_Text;
	m_Text = nullptr;
}

void Text::ConfigureSystems()
{
	m_Text->Load(m_FontPath, m_FontSize);
}
