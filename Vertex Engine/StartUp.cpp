#include <iostream>
#include "Application.h"
#include "Graphics.h"

#include "GameSettings.h"
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << "VERTEX ENGINE 2.00 | ©TropicalGames 2025" << std::endl;
	std::cout << PROJECT_NAME << std::endl;
	std::cout << "Made by: "<< PROJECT_DEVELOPER << std::endl << std::endl;

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
}