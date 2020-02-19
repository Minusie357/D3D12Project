#pragma once

#include <Windows.h>
#include <memory>
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"	



// GLOBAL VARIABLES
namespace App
{
	constexpr bool FULL_SCREEN = false;
	constexpr bool VSYNC_ENABLED = true;
	constexpr float SCREEN_DEPTH = 1000.f;
	constexpr float SCREEN_NEAR = 0.1f;
}



/*
 *	GraphicsClass
 */
class GraphicsClass final
{
public:
	GraphicsClass() = default;
	GraphicsClass(const GraphicsClass&) = default;
	~GraphicsClass() = default;

public:
	bool Initialize(int screenHeight, int screenWidth, HWND hWnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();



private:
	std::unique_ptr<D3DClass> direct3D;
	std::unique_ptr<CameraClass> camera;
	std::unique_ptr<ModelClass> model;
	std::unique_ptr<ColorShaderClass> colorShader;
};