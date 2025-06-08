#include "Text.h"
#include "ResourceManager.h"
#include "GameSettings.h"
Text::Text()
{
	m_Text = new VertexText2D(PROJECT_RESOLUTION);
	if (partner2d)
		ParentRect = partner2d->GetComponenet<RectTransform>();
	ChangeFont("Arial");
	ConfigureSystems();
}

Text::~Text()
{
	delete m_Text;
	m_Text = nullptr;
}

/// <summary>
/// Name of the font. do not include the file type.
/// </summary>
/// <param name="_name"></param>
void Text::ChangeFont(std::string _name)
{
	m_Text->Load(m_FontPath + _name + ".ttf", m_FontSize);
}

void Text::ConfigureRenderSystems(glm::mat4 _camera)
{
	if (partner2d)
		m_Text->Text2D(text, partner2d->GetComponenet<RectTransform>()->anchoredPosition.x, partner2d->GetComponenet<RectTransform>()->anchoredPosition.y, partner2d->GetComponenet<RectTransform>()->scale, partner2d->material.colour, _camera);
}

void Text::RenderEditorDisplay()
{
	if (ImGui::CollapsingHeader("Text", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Text: "); ImGui::SameLine();
		ImGui::Text(text.c_str());

		ImGui::Spacing();

		ImGui::InputInt("Font Size", &m_FontSize);
	}
}

void Text::ConfigureSystems()
{
	m_Text->Load(m_FontPath + m_FontName, m_FontSize);
}
