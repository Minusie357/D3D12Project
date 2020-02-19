#include "D3DClass.h"

#include <vector>
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;


bool D3DClass::Initiailize(
	int screenWidth, int screenHeight, 
	bool vSync, HWND hWnd, bool fullScreen, 
	float screenDepth, float screenNear)
{
	// Store the vsync setting
	bVsyncEnabled = vSync;
	HRESULT result;
	

	
	// Create DirectX graphics interface factory
	ComPtr<IDXGIFactory> factory;
	result = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	if (FAILED(result))
	{
		return false;
	}



	// Use the factory to create an adapter for
	// the primary grapchis interface (video card)
	ComPtr<IDXGIAdapter> adapter;
	result = factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (moniter)
	ComPtr<IDXGIOutput> adapterOutput;
	result = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}



	// Get the number of modes that fit the DXGI_FORMAT_R8G88A8_UNORM
	// display format for the adapter output (moniter)
	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for
	// this monitor/video card combination
	vector<DXGI_MODE_DESC> displayModeList(numModes);
	if (FAILED(result))
	{
		return false;
	}

	// Now fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList.data());
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one 
	// that matches the screen width and height. When a match
	// is found store the numerator and denominator of the re
	// -fresh rate for that monitor
	unsigned int numerator, denominator;
	for (const auto& it : displayModeList)
	{
		if (it.Width == static_cast<unsigned int>(screenWidth)
			&& it.Height == static_cast<unsigned int>(screenHeight))
		{
			numerator = it.RefreshRate.Numerator;
			denominator = it.RefreshRate.Denominator;
		}
	}



	// Get the adapter (video card) description
	DXGI_ADAPTER_DESC adapterDesc;
	adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes(MB).
	videoCardMemory = (int)adapterDesc.DedicatedVideoMemory / 1024 / 1024;

	// Convert the name of the video card to a character array 
	// and store it
	size_t stringLength;
	int error = wcstombs_s(&stringLength, videoCardName, D3DNameLength, adapterDesc.Description, D3DNameLength);
	if (error != 0)
	{
		return false;
	}





	// Initialize the swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	// Set to a single back buffer
	swapChainDesc.BufferCount = 1;
	// Set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	// Set regular 32-bit surface for the back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate of the back buffer
	if (bVsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// Set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to
	swapChainDesc.OutputWindow = hWnd;
	// Turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Set to full screen or windowed mode
	swapChainDesc.Windowed = fullScreen ? false : true;
	// Set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//Don't save the advanced flags
	swapChainDesc.Flags = 0;

	// Create the swap chain, Direct3D device, and Direct3D device context
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		&featureLevel, 1, D3D11_SDK_VERSION,
		&swapChainDesc, swapChain.GetAddressOf(),
		device.GetAddressOf(), nullptr, deviceContext.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}



	// Get the pointer to the back buffer
	ComPtr<ID3D11Texture2D> backBufferPtr;
	result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferPtr));
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer
	result = device->CreateRenderTargetView(
		backBufferPtr.Get(), nullptr, renderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}



	// Initialize the description of the depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out desc
	result = device->CreateTexture2D(
		&depthBufferDesc, nullptr, depthStencilBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}



	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// Initialize the description of the stencil state
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// Set up the description of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// Stencil operations if pixel if front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state
	result = device->CreateDepthStencilState(
		&depthStencilDesc, depthStencilState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil State
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);



	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initialize the depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	result = device->CreateDepthStencilView(
		depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer 
	// to the output render pipeline
	deviceContext->OMSetRenderTargets(
		1, renderTargetView.GetAddressOf(), depthStencilView.Get());



	// Setup the raster description which will determine how and what
	// polygons will be drawn
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.AntialiasedLineEnable = false;

	// Create the reasterizer state from the description we just filled out
	result = device->CreateRasterizerState(
		&rasterDesc, rasterizerState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// Now set the rasterizer state
	deviceContext->RSSetState(rasterizerState.Get());



	// Setup the viewport for rendering
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport
	deviceContext->RSSetViewports(1, &viewport);



	// Setup the projection matrix;
	float fieldOfView, screenAspect;
	fieldOfView = XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;
	
	// Create the projection matrix for 3D rendering
	projectionMatrix = XMMatrixPerspectiveFovLH(
		fieldOfView, screenAspect, screenNear, screenDepth);
	// Initialize the world matrix to the identity matrix
	worldMatrix = XMMatrixIdentity();
	// Creates an orthographic projection matrix for 2D rendering
	orthoMatrix = XMMatrixOrthographicLH(
		(float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release
	// the swap chain it will throw an exception
	if (swapChain.Get())	
		swapChain->SetFullscreenState(false, nullptr);

	// all D3D members wrapped in ComPtr, so they are automatically
	// released when dtor is called
	return;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float bgColor[4];
	bgColor[0] = red;
	bgColor[1] = green;
	bgColor[2] = blue;
	bgColor[3] = alpha;

	// Clear the back buffer
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgColor);
	// Clear the depth buffer
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete
	bVsyncEnabled ?
		swapChain->Present(1, 0) :
		swapChain->Present(0, 0);
}






void D3DClass::GetVideoCardInfo(char* outVideoCardName, int& outMemory)
{
	strcpy_s(outVideoCardName, D3DNameLength, videoCardName);
	outMemory = videoCardMemory;
}



