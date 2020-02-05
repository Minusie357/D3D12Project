#pragma once
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <memory>

#include "InputClass.h"
#include "GraphicsClass.h"

using namespace std;



class SystemClass
{
public:
	SystemClass() = default;
	SystemClass(const SystemClass&) = default;
	~SystemClass() = default;

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	bool Frame();
	void InitializeWindows(int& screenHeight, int& screenWidth);
	void ShutdownWindows();

private:
	LPCWSTR applicationName;
	HINSTANCE hInstance;
	HWND hWnd;

	unique_ptr<InputClass> inputClass;
	unique_ptr<GraphicsClass> graphicsClass;
};


// FUNCTION PROTOTYPES
static LRESULT CALLBACK WndProc(HWND hWnd, UINT, WPARAM wParam, LPARAM lParam);

// GLOBAL VARIABLES
static SystemClass* gApplicationHandle = nullptr;
#endif // !_SYSTEMCLASS_H_