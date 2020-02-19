#include "GraphicsClass.h"

using namespace std;
using namespace DirectX;



bool GraphicsClass::Initialize(int screenHeight, int screenWidth, HWND hWnd)
{
	bool result;

	// Create the Direct3D object
	direct3D = unique_ptr<D3DClass>(new D3DClass());
	if (direct3D.get() == nullptr)
		return false;

	// Initialize the Direct3D object
	result = direct3D->Initiailize(
		screenWidth, screenHeight, App::VSYNC_ENABLED, hWnd,
		App::FULL_SCREEN, App::SCREEN_DEPTH, App::SCREEN_NEAR);
	if (result == false)
	{
		MessageBox(hWnd, L"Cannot initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	


	// Create the camera object
	camera = unique_ptr<CameraClass>(new CameraClass());
	if (camera.get() == nullptr)
	{
		return false;
	}
	// Create the initial position of the camera
	camera->SetPosition(0.0f, 0.0f, -5.0f);



	// Create the model object
	model = unique_ptr<ModelClass>(new ModelClass());
	if (model.get() == nullptr)
	{
		return false;
	}
	// Initialize the model object
	result = model->Initialize(*direct3D->GetDevice());
	if (result == false)
	{
		MessageBox(hWnd, L"Could not initialize the model object",
			L"Error", MB_OK);
		return false;
	}


	
	// Create the color shader object
	colorShader = unique_ptr<ColorShaderClass>(new ColorShaderClass());
	if (colorShader.get() == nullptr)
	{
		return false;
	}
	// Initialize the color shader object
	result = colorShader->Initialize(*direct3D->GetDevice(), hWnd);
	if (result == false)
	{
		MessageBox(hWnd, L"Could not initialize the color shader object",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the color shader object
	if (colorShader.get())
		colorShader->Shutdown();



	// Release the model object
	if (model.get())
		model->Shutdown();



	// Release the Direct3D object
	if (direct3D.get())
		direct3D->Shutdown();
}

bool GraphicsClass::Frame()
{
	// Render the graphics scene.
	return Render();
}

bool GraphicsClass::Render()
{
	// Clear the buffers to begin the scene
	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);



	// Generate the view matrix based on the camera's position
	camera->Render();

	// Get the world, view, projection matrices from the camera
	// and d3d objects
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the mmodel vertex and index buffers on the 
	// graphics pipeline to prepare them for drawing
	model->Render(*direct3D->GetDeviceContext());

	// Render the model using the color shader
	bool result;
	result = colorShader->Render(
		*direct3D->GetDeviceContext(), model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix);	
	if (result == false)
	{
		return false;
	}


	// Present the rendered scene to the screen
	direct3D->EndScene();

	return true;
}
