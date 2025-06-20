#include "VertexEngineEditor.h"
#include "DebugComp.h"
#include <cstdint>
#include "AssetPipelineManager.h"

#include "RectTransform.h"
#include "SpriteRenderer.h"
#include <thread>

VertexEngineEditor::~VertexEngineEditor()
{
}

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
	if (m_EditorMode->EditorMode == EditorMode::EDITOR)
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
	else if (m_EditorMode->EditorMode == EditorMode::EDITOR_PLAY)
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
	ImGui::Begin("Scene View");
	//==============================================
	// Calculate View Port.
	//==============================================

	Camera* cameraCache = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->GetEditorCamera()->GetComponenet<Camera>();
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
		ImVec2(1920, 1080),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	//===============================================================
	// Drag & Drop
	//===============================================================
	if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft)) {

		ImVec2 ImagePos = ImGui::GetCursorScreenPos();
		ImVec2 mouse = ImGui::GetMousePos();
		ImVec2 min = ImagePos;
		ImVec2 max = ImVec2(min.x + 1080, min.y + 1080);

		if (mouse.x >= min.x && mouse.x < max.x && mouse.y >= min.y && mouse.y < max.y) {
			int localX = (int)(mouse.x - min.x);
			int localY = (int)(mouse.y - min.y);

			localY = (int)1080 - localY;

			m_SelectedGameObject = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->EditorPicker(glm::vec2(localX, localY));
		}
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* gameObjectPayload = ImGui::AcceptDragDropPayload("GAMEOBJECTS")) {
			auto object = *(GameObject**)gameObjectPayload->Data;

			m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
		}
		ImGui::EndDragDropTarget();
	}
	ImVec2 screenPos = ImGui::GetCursorScreenPos();

	if (m_SelectedGameObject) {
		ImGuizmo::SetOrthographic(true);

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(screenPos.x, screenPos.y, 640, 360);

		float dummy[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};


		glm::mat4 mod = m_SelectedGameObject->transform->GetWorldModelMat();

		ImGuizmo::Manipulate(glm::value_ptr(cameraCache->GetViewMatrix()),
			glm::value_ptr(cameraCache->GetProjection()),
			ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(mod));

		if (ImGuizmo::IsUsing()) {
			m_SelectedGameObject->transform->ApplyMatrix(mod);
		}

	}
	ImGui::End();
}

void VertexEngineEditor::RenderEditorInspector()
{
	ImGui::Begin("Inspector");

	if (m_SelectedGameObject) {

		ImGui::Text(m_SelectedGameObject->name.c_str());
		ImGui::SameLine();

		if (ImGui::Button("Edit")) {

			ImGui::OpenPopup("Rename");
		}

		//TODO: causes name to change to a bunch of "?????"
		if (ImGui::BeginPopup("Rename")) {

			static char newText[15] = "";
			ImGui::InputText("##name", newText, IM_ARRAYSIZE(newText));

			if (ImGui::Button("Confirm")) {
				m_SelectedGameObject->name = std::string(newText).c_str();
			}

			ImGui::EndPopup();

		}

		ImGui::Text("ID: "); ImGui::SameLine(); ImGui::Text(std::to_string(m_SelectedGameObject->GetUniqueIdentity()).c_str());
		ImGui::SameLine();

		VertexSpacer();
		ImGui::Checkbox("Active", &m_SelectedGameObject->m_Active);

		ImGui::Text("Layers");
		ImGui::SameLine();
		ImGui::InputInt("##Layers", &m_SelectedGameObject->layer, 2);

		VertexSpacer(3);

		int iter = 0;
		for (auto& comp : m_SelectedGameObject->GetEntireComponenetList()) {
			iter++;
			ImGui::PushID(iter);

			comp->RenderEditorDisplay();

			ImGui::PopID();
		}

		VertexSpacer();

		if (ImGui::Button("Add Component")) {

			ImGui::OpenPopup("ComponentMenu");
		}

		// Componenet Menu
		if (ImGui::BeginPopup("ComponentMenu")) {
			if (ImGui::MenuItem("Comp")) {
				m_SelectedGameObject->AddComponent<DebugComp>();
			}

			if (ImGui::MenuItem("Rect Transform")) {
				m_SelectedGameObject->AddComponent<RectTransform>();
			}

			if (ImGui::MenuItem("Text")) {
				if (m_SelectedGameObject->GetComponenet<Text>() == nullptr)
					m_SelectedGameObject->AddComponent<Text>();

				m_SelectedGameObject->AddComponent<Text>();
			}

			if (ImGui::MenuItem("Sprite Renderer")) {
				if (m_SelectedGameObject->GetComponenet<SpriteRenderer>() == nullptr)
					m_SelectedGameObject->AddComponent<SpriteRenderer>();

			}

			if (ImGui::MenuItem("Button")) {

				if (m_SelectedGameObject->GetComponenet<Button>() == nullptr)
					m_SelectedGameObject->AddComponent<Button>();

			}

			// Camera
			if (ImGui::MenuItem("Camera")) {
				if (m_SelectedGameObject->GetComponenet<Camera>() == nullptr)
					m_SelectedGameObject->AddComponent<Camera>();
				else
					VERTEX_WARNING("Only one Camera componet per object. " + std::string(m_SelectedGameObject->name) + " Already has one.");
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End();
}

void VertexEngineEditor::RenderEditorInheritList()
{
	if (m_ApplicationCentralSceneManager == nullptr) return;
	ImGui::Begin("Hierachy");

	if (ImGui::Button("Create")) {
		ImGui::OpenPopup("CreateMenu");
	}

	// Create button
	if (ImGui::BeginPopup("CreateMenu")) {
		if (ImGui::MenuItem("Empty GameObject")) {
			m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
		}

		// Create rect ransform object
		if (ImGui::MenuItem("Empty UI Element")) {
			GameObject* temp = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
			temp->AddComponent<RectTransform>();
			temp->AddComponent<SpriteRenderer>();
		}

		// Create a default camera
		if (ImGui::MenuItem("Camera")) {
			GameObject* temp = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
			temp->AddComponent<Camera>();
		}

		if (ImGui::MenuItem("Sprite")) {
			GameObject* temp = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
			temp->AddComponent<SpriteRenderer>();
		}

		if (ImGui::MenuItem("Button")) {
			// Create main button element.
			GameObject* temp = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
			temp->AddComponent<RectTransform>();
			temp->AddComponent<SpriteRenderer>();
			temp->AddComponent<Button>();

			// Create the child which will hold the text for the button.
			GameObject* child = m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew();
			child->AddComponent<RectTransform>();
			child->GetComponenet<RectTransform>()->SetParent(temp, true);
		}

		ImGui::EndPopup();
	}

	for (int i = 0; i < m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects.size(); i++) {
		if (m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects[i]->transform->GetParent() == nullptr)
			RenderGameObjectNodes(m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->m_Objects[i]->transform, m_SelectedGameObject);
	}

	VertexSpacer();
	// Parenting Drag & Drop

	ImVec2 screenSize = ImGui::GetContentRegionAvail();
	ImGui::Dummy(ImVec2(screenSize.x, screenSize.y));

	// Unparent zone
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* gameObjectPayload = ImGui::AcceptDragDropPayload("OBJECT_RE_PARENT")) {
			GameObject* object = *(GameObject**)gameObjectPayload->Data;
			if (object && object->transform->GetParent()) {
				object->transform->RemoveParent();
			}
		}
		ImGui::EndDragDropTarget();
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

	if (!AssetPipelineManager::IsReloadInProgress()) {


		//======================================= Texture Displaying
		for (const auto& [name, texture] : ResourceManager::Textures) {

			if (xSpace + (256 * m_DeskPanelItemMultiplier) > spaceLeft.x) {
				xSpace = 0;
				ImGui::NewLine();
			}

			// Display Textures that the engine has registered

			ImGui::ImageButton(name.c_str(), texture.ID, ImVec2(256 * m_DeskPanelItemMultiplier, 256 * m_DeskPanelItemMultiplier), ImVec2(0, 1), ImVec2(1, 0));

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


	ImGui::BeginMenuBar();

	if (ImGui::ArrowButton("Play", ImGuiDir_Right) && m_EditorMode->EditorMode == EditorMode::EDITOR)
	{
		m_EditorMode->EditorMode = EditorMode::EDITOR_PLAY;
		m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->Start();
		UpdateEditorColours();

	}
	if (ImGui::Button("PAUSE"))
	{
		if (m_EditorMode->EditorMode == EditorMode::EDITOR_PLAY)
		{
			m_EditorMode->EditorMode = EditorMode::EDITOR_PAUSED;
		}
		else if (m_EditorMode->EditorMode == EditorMode::EDITOR_PAUSED)
		{
			m_EditorMode->EditorMode = EditorMode::EDITOR_PLAY;
		}
	}
	if (ImGui::Button("STOP"))
	{
		VERTEX_LOG("Stop Test");

		if (m_EditorMode->EditorMode == EditorMode::EDITOR_PLAY || m_EditorMode->EditorMode == EditorMode::EDITOR_PAUSED) {

			m_EditorMode->EditorMode = EditorMode::EDITOR;
			m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->Start();
			UpdateEditorColours();
		}
	}

	if (ImGui::Button("Reload Pipeline"))
	{
		VERTEX_LOG("Reloading Pipeline. ");
		AssetPipelineManager::HotReload();
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

void VertexEngineEditor::RenderGameObjectNodes(Transform* _obj, GameObject*& selection)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (selection == _obj->partner2d)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (_obj->GetChildren().empty())
		flags |= ImGuiTreeNodeFlags_Selected;


	bool open = ImGui::TreeNodeEx((void*)_obj, flags, "%s", _obj->partner2d->name.c_str());

	if (ImGui::IsItemClicked()) {
		m_SelectedGameObject = _obj->partner2d;
	}

	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::Button("Rename")) { // Rename Object

		}

		if (ImGui::Button("Create Empty Parent")) { // Create Empty Parent Object

			m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->RegisterGameObjectNew(nullptr, _obj->partner2d);
		}

		if (ImGui::Button("Delete")) { // Delete the gameobject from the engine.
			m_ApplicationCentralSceneManager->m_SceneList.at(m_ApplicationCentralSceneManager->GetActiveScene())->GetAssets()->UnRegister(_obj->partner2d);
		}

		ImGui::EndPopup();
	}

	// Drag & drop unparent.
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* gameObjectPayload = ImGui::AcceptDragDropPayload("OBJECT_RE_PARENT")) {
			GameObject* object = *(GameObject**)gameObjectPayload->Data;
			if (object && !object->transform->IsChildOf(_obj)) {
				if (object->GetComponenet<RectTransform>())
					object->GetComponenet<RectTransform>()->SetParent(_obj->partner2d, true);
				else
					_obj->SetChild(object->transform);
			}
		}
		ImGui::EndDragDropTarget();
	}

	// Drag & drop parenting
	if (ImGui::BeginDragDropSource()) {

		ImGui::SetDragDropPayload("OBJECT_RE_PARENT", &_obj->partner2d, sizeof(GameObject*));
		ImGui::Text("Dragging: %s", _obj->partner2d->name.c_str());

		ImGui::EndDragDropSource();
	}

	// Draw objects children
	if (open) {
		for (Transform* child : _obj->GetChildren())
			RenderGameObjectNodes(child, selection);
		ImGui::TreePop();
	}

}