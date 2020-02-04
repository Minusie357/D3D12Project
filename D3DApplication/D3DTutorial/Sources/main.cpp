#include <memory>

#include "SystemClass.h"

using namespace std;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPervInstance, PSTR pScmdline, int iCmdShow)
{
	unique_ptr<SystemClass> System(new SystemClass());
	if (System.get() == nullptr)
		return 0;
	
	// Initialize and run the system object
	bool result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object
	System->Shutdown();
	return 0;
}