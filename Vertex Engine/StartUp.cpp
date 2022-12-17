#include <iostream>
#include "Application.h"
#include "Graphics.h"

#include "GameSettings.h"
#include <crtdbg.h>
int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << "VERTEX ENGINE 2022 | ©TropicalGames 2022" << std::endl;
	std::cout << PROJECT_NAME << std::endl;
	std::cout << "Made by: "<< PROJECT_DEVELOPER << std::endl;

	Application* App = new Application();

	App->StartUp();
	App->Start();

	while (!glfwWindowShouldClose(App->GetWindow()))
	{
		App->Update();
		App->RenderAll();
	}
	App->ShutDown();

	delete App;
	App = nullptr;


	/*GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "glfw failed" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(1920, 1080, "App", nullptr, nullptr);
	const char* description = new char[512];

	if (!window) {
		glfwGetError(&description);
		std::cout << "Window Failed ERROR: " << description << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {

		glfwGetError(&description);
		std::cout << "Glad Failed ERROR: " << description << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;*/
}