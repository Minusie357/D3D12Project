#include "GraphicsClass.h"

bool GraphicsClass::Initialize(int screenHeight, int screenWidth, HWND hWnd)
{
	bool result;

	return true;
}

void GraphicsClass::Shutdown()
{
}

bool GraphicsClass::Frame()
{
	// Render the graphics scene.
	return Render();
}

bool GraphicsClass::Render()
{
	// Use the Direct3D object to render the scene
	return true;
}
