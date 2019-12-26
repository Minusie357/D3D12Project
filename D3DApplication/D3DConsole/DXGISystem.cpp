#include "DXGISystem.h"
#include <iostream>
#include <string>
using namespace std;

#include <dxgi.h>
#include "DXGIInterface.h"



dxgisystem::dxgisystem()
{
	/* create DXGIFactory */
	HRESULT FacResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&DXGIFactory));
}

dxgisystem::~dxgisystem()
{
}

dxgisystem& DXGISystem()
{
	static dxgisystem gDXGISystem;
	return gDXGISystem;
}










void DXGIUtility::EnumerateAdapters()
{
	IDXGIAdapter* Adapter = nullptr;
	for (size_t i = 0; ; ++i)
	{
		if (DXGISystem().GetDXGIFactory()->EnumAdapters(i, &Adapter) == DXGI_ERROR_NOT_FOUND)
			return;

		DXGI_ADAPTER_DESC AdapterDesc;
		Adapter->GetDesc(&AdapterDesc);

		wstring Text;
		Text += L"* Adapter[";
		Text += to_wstring(i);
		Text += L"] : ";
		Text += AdapterDesc.Description;

		wcout << Text << "\n";
		Adapter->Release();
	}
}

void DXGIUtility::EnumerateAdapterOutputs(IDXGIAdapter* Adapter)
{
	for (size_t i = 0; ; ++i)
	{
		IDXGIOutput* Output = nullptr;
		if (Adapter->EnumOutputs(i, &Output) == DXGI_ERROR_NOT_FOUND)
			break;
		
		DXGI_OUTPUT_DESC OutputDesc;
		Output->GetDesc(&OutputDesc);

		wstring Text;
		Text += L"* Output[";
		Text += to_wstring(i);
		Text += L"] : ";
		Text += OutputDesc.DeviceName;

		wcout << Text << "\n";
		Output->Release();
	}
}

void DXGIUtility::EnumerateOutputDisplayModes(IDXGIOutput* Output, DXGI_FORMAT Format)
{
	UINT count = 0;
	UINT flags = 0;
	Output->GetDisplayModeList(Format, flags, &count, nullptr);

	/* 디스플레이 모드 개수만큼 벡터를 생성 */
	vector<DXGI_MODE_DESC> DisplayModes(count);
	Output->GetDisplayModeList(Format, flags, &count, &DisplayModes[0]);

	DXGI_OUTPUT_DESC OutputDesc;
	Output->GetDesc(&OutputDesc);
	wcout << L"* " << OutputDesc.DeviceName << L" : " << to_wstring(count) << L" Display Modes \n";
	for (const auto& it : DisplayModes)
	{
		cout << "** Width, Height : " << it.Width << "," << it.Height << "\n";
		cout << "** RefreshRate : " << it.RefreshRate.Numerator 
			<< "/" << it.RefreshRate.Denominator << "\n\n";
	}
}





void DXGIUtility::GetAdapters(vector<IDXGIAdapter*>& OutAdapters)
{
	IDXGIAdapter* Adapter = nullptr;
	for (size_t i = 0; ;i++)
	{
		if(DXGISystem().GetDXGIFactory()->EnumAdapters(i, &Adapter) == DXGI_ERROR_NOT_FOUND)
			break;
		
		OutAdapters.push_back(Adapter);
	}
}
