#include "VertexEngineEditor.h"
#include "DebugComp.h"
#include <cstdint>



void VertexEngineEditor::CreateEditorLayout(GLFWwindow* _GameWindow, SceneManager* _scenesManager)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(_GameWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	m_GameViewRenderTexture = new RenderTexture(1920, 1080);
	m_ApplicationCentralSceneManager = _scenesManager;

	ImGui::StyleColorsDark();
}

void VertexEngineEditor::PopulateDesk()
{
	for (VertexScene* scenes : m_ApplicationCentralSceneManager->m_SceneList) {
		for (int i = 0; i < scenes->GetAssets()->m_Objects.size(); i++) {
			m_GameObjectsInProject.push_back(scenes->GetAssets()->m_Objects.at(i));
		}
	}

}

void VertexEngineEditor::RenderEditorDisplays()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();

	ImGui::NewFrame();


	// ==========================================
	RenderDockingTaskBar();

	RenderEditorGameView();
	RenderEditorSceneView();
	RenderEditorInspector();
	RenderEditorInheritList();
	RenderEditorDesk();
	RenderGameCommandConsole();

	// ==========================================

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void VertexEngineEditor::CleanUpGui()
{
	if (m_GameViewRenderTexture) {

		delete m_GameViewRenderTexture;
		m_GameViewRenderTexture = nullptr;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void VertexEngineEditor::UpdateEditorColours()
{
	if (m_EditorMode == EDITOR)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_WindowBg] = ImVec4(EDITOR_BACKGROUND);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(EDITOR_MENU);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(EDITOR_TABS);

		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(EDITOR_TABS_SELECT);
		style->Colors[ImGuiCol_Tab] = ImVec4(EDITOR_TABS_SELECT);
		style->Colors[ImGuiCol_Button] = ImVec4(EDITOR_MENU);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(EDITOR_BUTTONS_SELECT);

	}
	else if (m_EditorMode == EDITOR_PLAY)
	{
		ImGuiStyle* style1 = &ImGui::GetStyle();
		style1->Colors[ImGuiCol_WindowBg] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_MenuBarBg] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_TitleBg] = ImVec4(PLAY_MODE_COLOUR);

		style1->Colors[ImGuiCol_TitleBgActive] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_Tab] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_Button] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_ButtonHovered] = ImVec4(PLAY_MODE_COLOUR);
	}
}

void VertexEngineEditor::RenderEditorGameView()
{
	if (m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->GetMainCamera() == nullptr) { VERTEX_ERROR("FAILED TO CREATE GAME VIEW. NO CAMERAS."); return; }

	Camera* cameraCache = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->GetMainCamera()->GetComponenet<Camera>();

	ImGui::Begin("Game View");

	ImVec2 screenSize = ImGui::GetContentRegionAvail();
	ImVec2 screenSizeA = ImGui::GetContentRegionAvail();


	if (cameraCache->renderTexture == nullptr) {
		cameraCache->renderTexture = m_GameViewRenderTexture;
		cameraCache->SetDisplay(2);
	}

	float imageAspect = (float)cameraCache->renderTexture->GetWidth() /
		(float)cameraCache->renderTexture->GetHeight();
	float windowAspect = screenSize.x / screenSize.y;

	ImVec2 ImageSize = screenSize;

	if (windowAspect > imageAspect) {
		ImageSize.x = screenSize.y * imageAspect;
	}
	else {
		ImageSize.y = screenSize.x / imageAspect;
	}

	ImGui::Image(
		cameraCache->renderTexture->GetTexture(),
		ImageSize,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();
}

void VertexEngineEditor::RenderEditorSceneView()
{
	if (m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->GetMainCamera() == nullptr) { VERTEX_ERROR("FAILED TO CREATE SCENE VIEW. NO CAMERAS."); return; }
	ImGui::Begin("Scene View");
	//==============================================
	// Calculate View Port.
	//==============================================



	Camera* cameraCache = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->GetMainCamera()->GetComponenet<Camera>();
	ImVec2 screenSize = ImGui::GetContentRegionAvail();
	ImVec2 screenSizeA = ImGui::GetContentRegionAvail();

	float imageAspect = (float)cameraCache->renderTexture->GetWidth() /
		(float)cameraCache->renderTexture->GetHeight();
	float windowAspect = screenSize.x / screenSize.y;

	ImVec2 ImageSize = screenSize;

	if (windowAspect > imageAspect) {
		ImageSize.x = screenSize.y * imageAspect;
	}
	else {
		ImageSize.y = screenSize.x / imageAspect;
	}

	ImGui::Image(
		cameraCache->renderTexture->GetTexture(),
		ImageSize,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	//===============================================================
	// Drag & Drop
	//===============================================================

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* gameObjectPayload = ImGui::AcceptDragDropPayload("GAMEOBJECTS")) {
			auto object = *(GameObject**)gameObjectPayload->Data;

			m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
		}
		ImGui::EndDragDropTarget();
	}
	ImVec2 screenPos = ImGui::GetCursorScreenPos();

	if (m_SelectedGameObject) {

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(screenPos.x, screenPos.y, ImageSize.x, ImageSize.y);

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(m_SelectedGameObject->transform.position, 0));
		model = glm::translate(model, glm::vec3(0.5f * m_SelectedGameObject->transform.size.x, 0.5f * -m_SelectedGameObject->transform.size.y, 0.0f));
		model = glm::rotate(model, glm::radians(m_SelectedGameObject->transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * m_SelectedGameObject->transform.size.x, -0.5f * -m_SelectedGameObject->transform.size.y, 0.0f));

		model = glm::scale(model, glm::vec3(m_SelectedGameObject->transform.size.x * m_SelectedGameObject->transform.scale, -m_SelectedGameObject->transform.size.y * m_SelectedGameObject->transform.scale, 1.0f));

		float dummy[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		glm::mat4 mod = m_SelectedGameObject->transform.GetLocalModelMat();

		ImGuizmo::Manipulate(glm::value_ptr(cameraCache->GetViewMatrix()),
			glm::value_ptr(cameraCache->GetProjection()),
			ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(mod));

	}
	ImGui::End();
}

void VertexEngineEditor::RenderEditorInspector()
{
	ImGui::Begin("Inspector");

	if (m_SelectedGameObject) {

		ImGui::Text(m_SelectedGameObject->name);
		ImGui::SameLine();
		VertexSpacer();
		ImGui::Checkbox("Active", &m_SelectedGameObject->m_Active);

		ImGui::Text("Layers");
		ImGui::SameLine();
		ImGui::InputInt("##Layers", &m_SelectedGameObject->layer, 2);
		VertexSpacer();

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0.5f, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0.5f, 0, 1));
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {


			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.0f, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.0f, 0, 1));

			// ============================= Transform Positions
			// X Axis
			ImGui::Button("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::InputFloat("##Position X Axis", &m_SelectedGameObject->transform.position.x, -0.5f);
			ImGui::PopStyleColor(2);

			// Y Axis
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0, 1));

			ImGui::SameLine();
			ImGui::Button("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::InputFloat("##Position Y Axis", &m_SelectedGameObject->transform.position.y);
			VertexSpacer();
			ImGui::PopStyleColor(2);

			VertexSpacer();
			ImGui::Text("Scale");
			ImGui::SameLine();
			ImGui::InputFloat("##Scale", &m_SelectedGameObject->transform.scale);
		}
		ImGui::PopStyleColor(2);

		VertexSpacer(3);

		int iter = 0;
		for (auto& comp : m_SelectedGameObject->GetEntireComponenetList()) {
			iter++;
			ImGui::PushID(iter);

			if (Camera* cam = dynamic_cast<Camera*>(comp)) {
				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0, 0, 1));
					ImGui::Text("COMPONENET_NOT_COMPLETE");
					ImGui::PopStyleColor(1);
				}
			}
			else if (DebugComp* deb = dynamic_cast<DebugComp*>(comp)) {
				if (ImGui::CollapsingHeader("Debug Component", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0, 0, 1));
					ImGui::Text("COMPONENET_NOT_COMPLETE");
					ImGui::PopStyleColor(1);
				}
			}

			ImGui::PopID();
		}

		VertexSpacer();

		if (ImGui::Button("Add Component")) {

			ImGui::OpenPopup("ComponentMenu");
		}

		// Texture Swapping.

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* gameObjectPayload = ImGui::AcceptDragDropPayload("TEXTURE")) {
				const char* texture = (const char*)gameObjectPayload->Data;

				m_SelectedGameObject->material.AlbedoMap = ResourceManager::GetTexture(texture);

			}
			ImGui::EndDragDropTarget();
		}

		// Componenet Menu
		if (ImGui::BeginPopup("ComponentMenu")) {
			if (ImGui::MenuItem("Comp")) {
				m_SelectedGameObject->AddComponent<DebugComp>();
			}
			if (ImGui::MenuItem("Rigidbody")) {
				m_SelectedGameObject->AddComponent<DebugComp>();
			}
			// Camera
			if (ImGui::MenuItem("Camera")) {
				if (m_SelectedGameObject->GetComponenet<Camera>() == nullptr)
					m_SelectedGameObject->AddComponent<Camera>();
				else
					VERTEX_WARNING("Failed to add Componenet Camera. " + std::string(m_SelectedGameObject->name) + " Already has one.");
			}

			ImGui::EndPopup();
		}
	}
	else {
		ImGui::Text("NO_SELECTION");
	}

	ImGui::End();
}

void VertexEngineEditor::RenderEditorInheritList()
{
	if (m_ApplicationCentralSceneManager == nullptr) return;
	ImGui::Begin("Hierachy");

	if (ImGui::Button("Create")) {
		GameObject* temp = new GameObject("Game");

		VERTEX_LOG(std::to_string(m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects.size()));
		m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->Register(temp);
		VERTEX_LOG(std::to_string(m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects.size()));
	}

	for (int i = 0; i < m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects.size(); i++) {
		if (m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects[i]->GetParent() == nullptr)
			RenderGameObjectNodes(m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects[i], m_SelectedGameObject);
	}



	ImGui::End();
}

void VertexEngineEditor::RenderEditorDesk()
{
	ImGui::Begin("Desk");

	ImGui::SliderFloat("Asset Size", &m_DeskPanelItemMultiplier, 0.2f, 3.0f);

	// Dymanic spacing.

	float padding = 4.0f;

	ImVec2 spaceLeft = ImGui::GetContentRegionAvail();
	float xSpace = 0.0f;

	//======================================= Texture Displaying
	for (const auto& [name, texture] : ResourceManager::Textures) {

		if (xSpace + (256 * m_DeskPanelItemMultiplier) > spaceLeft.x) {
			xSpace = 0;
			ImGui::NewLine();
		}

		// Display Textures that the engine has registered

		ImGui::ImageButton(name.c_str(), texture.ID, ImVec2(256 * m_DeskPanelItemMultiplier, 256 * m_DeskPanelItemMultiplier));

		ImGui::SameLine(0.0f, padding);
		xSpace += (256 * m_DeskPanelItemMultiplier) + padding;

		// Drag & Drop texture Space.
		if (ImGui::BeginDragDropSource()) {

			ImGui::SetDragDropPayload("TEXTURE", name.c_str(), strlen(name.c_str()) + 1);
			ImGui::Text("Dragging: %s", name.c_str());
			ImGui::Image(texture.ID, ImVec2(64, 64));

			ImGui::EndDragDropSource();
		}

	}

	ImGui::End();
}

void VertexEngineEditor::RenderDockingTaskBar()
{
	static bool dockOpen = true;
	static bool fullscreen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	if (fullscreen) {
		const ImGuiViewport* view = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(view->WorkPos);
		ImGui::SetNextWindowSize(view->WorkSize);
		ImGui::SetNextWindowViewport(view->ID);

		window |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	ImGui::Begin("DockSpace", &dockOpen, window);

	ImGui::Text("VERTEX ENGINE 2: EDITOR");
	ImGui::SameLine();
	ImGui::Text(PROJECT_NAME);

	ImGui::BeginMenuBar();



	if (ImGui::ArrowButton("Play", ImGuiDir_Right) && m_EditorMode == EDITOR)
	{
		m_EditorMode = EDITOR_PLAY;
		m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->Start();
		UpdateEditorColours();

	}
	if (ImGui::Button("PAUSE"))
	{
		if (m_EditorMode == EDITOR_PLAY)
		{
			m_EditorMode = EDITOR_PAUSED;
		}
		else if (m_EditorMode == EDITOR_PAUSED)
		{
			m_EditorMode = EDITOR_PLAY;
		}
	}
	if (ImGui::Button("STOP"))
	{
		VERTEX_LOG("Stop Test");

		if (m_EditorMode == EDITOR_PLAY || m_EditorMode == EDITOR_PAUSED) {

			m_EditorMode = EDITOR;
			m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->Start();
			UpdateEditorColours();
		}
	}

	ImGui::EndMenuBar();

	ImGuiID dockingId = ImGui::GetID("DockSpacing");
	ImGui::DockSpace(dockingId, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();


}

void VertexEngineEditor::RenderGameCommandConsole()
{
	ImGui::Begin("Console");

	if (ImGui::Button("Clear")) { Debug::m_ConsoleCommandLib.clear(); }

	VertexSpacer(2);

	int iter = 0;
	for (auto logs : Debug::GetDebugLogs()) {
		ImGui::Text(logs.c_str());
	}

	ImGui::End();
}

void VertexEngineEditor::RenderGameObjectNodes(GameObject* _obj, GameObject*& selection)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (selection == _obj)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (_obj->GetChildren().empty())
		flags |= ImGuiTreeNodeFlags_Selected;

	bool open = ImGui::TreeNodeEx((void*)_obj, flags, "%s", _obj->name);

	if (ImGui::IsItemClicked()) {
		m_SelectedGameObject = _obj;
	}

	if (open) {
		for (GameObject* child : _obj->GetChildren())
			RenderGameObjectNodes(child, selection);
		ImGui::TreePop();
	}

}
