#pragma once
#include "GameObject.h"
#include <vector>
#include "VertexRenderPackage.h"
#include "VertexUIPackage.h"
#include "Camera.h"
class AssetManager
{
public:
	AssetManager() { m_ActiveCamera = 0; };
	void Register(GameObject* _object);
	void RegisterUi(GameObject* _object);
	void RegisterUi(Button* _object);
	void Register(Camera *camera);

	void GiveWindow(GLFWwindow* _window) { m_Window = _window; };
	bool OnTrigger(GameObject* A, GameObject* B);

	bool CollisionCheck();
	void ConfigureSystems();
	void ConfigureRenderSystems(Vertex2D* render);

	void LogEvents();
	void SetActiveCamera(int _index);

	std::vector<GameObject*> m_Objects;
	std::vector<GameObject*> m_UiObjects;
	std::vector<Camera*> m_Cameras;

private:
	GLFWwindow* m_Window;
	Transform mouse;

	std::vector<Transform*> m_PreviousLocations;
	std::vector<Button*> m_UiButtonObjects;
	std::vector<Text*> m_UiTextObjects;
	int m_ActiveCamera;
	bool m_ShutDownManager = false;
};