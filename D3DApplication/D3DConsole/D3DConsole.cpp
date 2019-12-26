#include <vector>
using namespace std;

#include <dxgi.h>
#include <dxgi1_6.h>
#include "DXGISystem.h"




int main()
{
	vector<IDXGIAdapter*> Adapters;
	DXGIUtility::GetAdapters(Adapters);

	IDXGIOutput* Output;
	Adapters[0]->EnumOutputs(0, &Output);
	DXGIUtility::EnumerateOutputDisplayModes(Output, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
	return 0;
}