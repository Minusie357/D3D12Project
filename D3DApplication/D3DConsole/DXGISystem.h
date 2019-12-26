#pragma once
#include <vector>
#include <dxgiformat.h>



struct IDXGIFactory;
struct IDXGIAdapter;
struct IDXGIOutput;

/* dxgisystem class represents DXGI resources set. */
class dxgisystem
{
public:
	dxgisystem();
	~dxgisystem();

	/* deleted list */
	dxgisystem(const dxgisystem&) = delete;
	dxgisystem(const dxgisystem&&) = delete;
	const dxgisystem& operator=(const dxgisystem&) = delete;
	const dxgisystem& operator=(const dxgisystem&&) = delete;

public:	
	inline IDXGIFactory* GetDXGIFactory() const
	{
		return DXGIFactory;
	};

protected:
	IDXGIFactory* DXGIFactory;
};



class DXGIUtility
{
public:
	/* enumerate adapters */
	static void EnumerateAdapters();

	/* enumerate adapter's output */
	static void EnumerateAdapterOutputs(IDXGIAdapter* Adapter);

	static void EnumerateOutputDisplayModes(IDXGIOutput* Output, DXGI_FORMAT Format);

public:
	/* get adapters */
	static void GetAdapters(std::vector<IDXGIAdapter*>& OutAdapters);
};