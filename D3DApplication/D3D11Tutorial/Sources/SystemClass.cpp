#include "SystemClass.h"

bool SystemClass::Initialize()
{
	// Initializes the width and height of the screen to zero 
	// before sending the variables into the function
	int screenHeignt = 0, screenWidth = 0;

	// Initialize the windows api
	InitializeWindows(screenHeignt, screenWidth);



	// Creates the input object. This object will be used to 
	// handle reading the keyboard input from the user
	inputClass.reset(new InputClass());
	if (inputClass.get() == nullptr)
		return false;

	// Initializes the input object
	inputClass->Initialize();



	// Creates the graphics object. This object will
	// handle rendering all the graphics for this application
	graphicsClass.reset(new GraphicsClass());
	if (graphicsClass.get() == nullptr)
		return false;

	// Initializes the graphics object
	bool result;
	result = graphicsClass->Initialize(screenHeignt, screenWidth, hWnd);
	if (result == false)
		return false;



	return true;
}

void SystemClass::Shutdown()
{
	// Releases the graphics object
	if (graphicsClass.get())
	{
		graphicsClass->Shutdown();
		graphicsClass.release();
	}

	if (inputClass.get())
	{
		inputClass.release();
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done = false;

	// Initializes the message streucture
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from 
	// the window or the user
	bool result;
	while (done == false)
	{
		// Handle the windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		// Otherwise do the frame processing
		else
		{
			result = Frame();
			if (result == false)
			{
				done = true;
			}
		}
	}
}





LRESULT SystemClass::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// Check if a key has been pressed on the keyboard
	case WM_KEYDOWN:
		inputClass->KeyDown((unsigned int)wParam);
		return 0;

		// Check if a key has been released on the keyboard
	case WM_KEYUP:
		inputClass->KeyUp((unsigned int)wParam);
		return 0;

		// Any other messages send to the default message handler
		// as our application won't make use of them
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}





bool SystemClass::Frame()
{
	bool result;

	// Check if the user pressed escape and wants to exit
	if (inputClass->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Processes the frame for the graphics objects
	result = graphicsClass->Frame();
	if (result == false)
	{
		return false;
	}

	return true;
}

void SystemClass::InitializeWindows(int& screenHeight, int& screenWidth)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;



	// Register this to global pointer
	gApplicationHandle = this;

	// Get the instance of this application
	hInstance = GetModuleHandle(NULL);

	// Give the application a name
	applicationName = L"Engine";



	// Setup the windows class with defualt settings	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	// Registers the window class
	RegisterClassEx(&wc);



	// Determines the resolution of the clients desktop screen	
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	screenWidth = GetSystemMetrics(SM_CXSCREEN);

	// Setup the screen settings depending on whether it is running
	// in full screen or in windows mode
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmPelsWidth = (unsigned)screenWidth;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Changes the display settings to full screen	
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner
		posX = posY = 0;
	}
	// otherwise set it to 800x600 resolution
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		// Places the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Creates the window with the screen settings 
	// and get the handle to it
	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		applicationName,
		applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX,
		posY,
		screenWidth,
		screenHeight,
		NULL, NULL, hInstance, NULL);

	// Brings the window up on the screen and set it as main focus
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	// Hides the mouse cursor
	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	// Shows mouse cursors
	ShowCursor(true);

	// Fixes the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Removes the window
	DestroyWindow(hWnd);
	hWnd = NULL;

	// Remove the application instances
	UnregisterClass(applicationName, hInstance);
	hInstance = NULL;

	// Releases the pointer to this class
	gApplicationHandle = NULL;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
		// Check if the window is being destroyed;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;


		// Check if the window is being closed
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// All other messages pass to the message handler in system class
	default:
		return gApplicationHandle->MessageHandler(hWnd, uMessage, wParam, lParam);
	}
}