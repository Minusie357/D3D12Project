#include "GraphicsClass.h"

bool GraphicsClass::Initialize(int screenHeight, int screenWidth, HWND hWnd)
{
	bool result;

	// Create the Direct3D Object
	direct3D.reset(new D3DClass());
	if (direct3D.get() == nullptr)
	{
		return false;
	}

	// Initialize the Direct3D Object
	result = direct3D->Initialize(
		screenHeight, screenWidth, hWnd, VSYNC_ENABLED, FULL_SCREEN);
	if (result == false)
	{
		MessageBox(hWnd, L"Could not Initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	direct3D->Shutdown();
}

bool GraphicsClass::Frame()
{
	// Render the graphics scene.
	return Render();
}

bool GraphicsClass::Render()
{
	// Use the Direct3D object to render the scene
	return direct3D->Render();
}
