#include "uApplication.h"
/*
	Vertex UltraLight is a 3D model rendering engine designed by Dylan Smith. 2024

*/
int main() {
	UltraLight::uApplication* VertexUltraLightApplication = new UltraLight::uApplication();

	VertexUltraLightApplication->StartUp();
	while (VertexUltraLightApplication->IsApplicationRunning())
	{
		VertexUltraLightApplication->UpdateSystems();
	}

	// Delete main application.
	delete VertexUltraLightApplication;
	VertexUltraLightApplication = nullptr;

	return 0;
}