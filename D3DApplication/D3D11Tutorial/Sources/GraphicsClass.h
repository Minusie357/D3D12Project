#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Windows.h>
#include <memory>
using namespace std;



// GLOBAL VARIABLES
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;



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
};
#endif // !_GRAPHICSCLASS_H_