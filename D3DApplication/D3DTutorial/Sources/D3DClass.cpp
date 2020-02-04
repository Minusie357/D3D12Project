#include "D3DClass.h"

#include <vector>
using namespace std;



bool D3DClass::Initialize(int screenHeight, int screenWidth, HWND hWnd, bool vSync, bool fullScreen)
{
	// Store the vsync setting
	vSyncEnabled = vSync;

	// Check each intiiailize state. if value set false then escape 
	HRESULT result;

	// Create D3D12 device
	result = D3D12CreateDevice(
			nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device));
	if (FAILED(result))
	{
		MessageBox(
			hWnd,
			L"Could not create a DirectX 12.1 device. \
			The default video card does not support DirectX 12.1",
			L"DirectX Device Failure",
			MB_OK);
		return false;
	}



	// Initialize and set up the description of the command queue.
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;
	result = device->CreateCommandQueue(
		&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}



	// Create a DirectX graphics interface factory
	ComPtr<IDXGIFactory4> factory;
	result = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(result))
	{
		return false;
	}
	
	// Use the factory to create an adapter for the 
	// primary graphics interface (video card).
	ComPtr<IDXGIAdapter> adapter;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumberate the primary adapter output (monitor).
	ComPtr<IDXGIOutput> adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM
	// display format for the adapter output (monitor).
	unsigned int numModes;
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		nullptr);
	if (FAILED(result))
	{
		return false;
	}

	// Creates list to hold all the possible display mmodes for 
	// this monitor/video card combination
	vector<DXGI_MODE_DESC> displayModeList(numModes);
	if (displayModeList.size() != numModes)
	{
		return false;
	}

	// fill display mode list structures
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModeList.data());
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one 
	// matches the screen height and width
	// When a match is found store the numerator and denominator
	// of the refresh rate for that monitor
	unsigned int numerator, denominator;
	for (int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Height == (unsigned int)screenHeight
			&& displayModeList[i].Width == (unsigned int)screenWidth)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	// Get the adapter (video card) description
	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);

	// Stores the dedicated video card memory in megabytes
	videoCardMemory = (int)adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	
	// Convert the name of the video card to caharacter array
	unsigned long long stringLength;
	int error;
	error = wcstombs_s(&stringLength, videoCardDesc, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}





	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	// set swap chain to use double buffering
	swapChainDesc.BufferCount = 2;
	// set the height and width of the back buffers in the swap chain
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Width = screenWidth;
	// set a regular 32-bit surface for the back buffers
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// set usage of back buffers to be render target output
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// set the swap effect to discard the previous buffer contents
	// after swapping
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// set the handle for the window to render to
	swapChainDesc.OutputWindow = hWnd;
	// set full screen of windowed mode
	if (fullScreen)	swapChainDesc.Windowed = false;
	else			swapChainDesc.Windowed = true;
	// set the refresh rate of the back buffer
	if (vSyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// don't set advanced flags
	swapChainDesc.Flags = 0;

	

	// Finally create the swap chain using the swap chain description
	ComPtr<IDXGISwapChain> olderSwapChain;
	result = factory->CreateSwapChain(
		commandQueue.Get(), &swapChainDesc, olderSwapChain.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Next upgrade the IDXGISwapChain to IDXGISwapChain3 interface
	// and store it in class member
	// This will allow us to use the newer functionallity such as
	// getting the current back buffer index
	result = olderSwapChain->QueryInterface(IID_PPV_ARGS(swapChain.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	




	D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc;
	ZeroMemory(&renderTargetViewHeapDesc, sizeof(renderTargetViewHeapDesc));
	// set the number of descriptors to two for our two back buffers.
	// also set the heap type to render target views.
	renderTargetViewHeapDesc.NumDescriptors = 2;
	renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	renderTargetViewHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// Create the render target view heap for the back buffers
	result = device->CreateDescriptorHeap(
		&renderTargetViewHeapDesc, IID_PPV_ARGS(renderTargetViewHeap.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}



	// Get a handle to the starting memory location in the render 
	// target views will be located for the two back buffers
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	renderTargetViewHandle 
		= renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();

	// Get the size of the memory location 
	// for the render target view descriptors
	unsigned int renderTargetViewDescriptorSize;
	renderTargetViewDescriptorSize
		= device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	


	// Get a pointer to the first back buffer from the swap chain
	result = swapChain->GetBuffer(0, IID_PPV_ARGS(backBufferRenderTarget[0].GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}
	// Create a render target view for the first back buffer
	device->CreateRenderTargetView(backBufferRenderTarget[0].Get(), nullptr, renderTargetViewHandle);
	// Increments the view handle to the next descriptor location in the render target view heap
	renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;

	// Get a pointer to the second back buffer from the swap chain
	result = swapChain->GetBuffer(1, IID_PPV_ARGS(backBufferRenderTarget[1].GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	// Create a render target view for the second back buffer
	device->CreateRenderTargetView(backBufferRenderTarget[1].Get(), nullptr, renderTargetViewHandle);

	// Finally get the initial index to which buffer is the current back buffer
	bufferIndex = swapChain->GetCurrentBackBufferIndex();





	// Create a command allocator
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		IID_PPV_ARGS(commandAllocator.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	// Create a basic command list
	result = device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(commandList.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	// Initially we need to close the command list during initiailization
	// as it is created in recording state
	result = commandList->Close();
	if (FAILED(result))
	{
		return false;
	}





	// Create a fence for GPU synchronization
	result = device->CreateFence(
		0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}
	// Create an event object for the fence
	fenceEvent = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	if (fenceEvent == nullptr)
	{
		return false;
	}
	// Initialize the starting fence value
	fenceValue = 1;

	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down, set to windowed mode or when you release
	// the swap chian it will throw an exception
	if (swapChain.Get() != nullptr)
	{
		swapChain->SetFullscreenState(false, nullptr);
	}

	// Close the object handle to the fence event
	int error;
	error = CloseHandle(fenceEvent);
	// TODO : why this code is inserted?
	if(error == 0)
	{ }

	// other all resources is automatically released 
	// as ComPtr do
}





bool D3DClass::Render()
{
	HRESULT result;

	// Reset (re-use) the memory associated command allocator
	result = commandAllocator->Reset();
	if (FAILED(result))
	{
		return false;
	}
	// Reset the command list, use empty pipeline state for now
	// since there are no shaders and we are just clearing the screen
	result = commandList->Reset(commandAllocator.Get(), pipelineState.Get());
	if (FAILED(result))
	{
		return false;
	}



	// Record commands in the command list now
	// Start by setting the resource barrier
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = backBufferRenderTarget[bufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	commandList->ResourceBarrier(1, &barrier);



	// Get the render target view handle for the current back buffer
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	unsigned int renderTargetViewDescriptorSize;
	renderTargetViewHandle = renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewDescriptorSize
		= device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (bufferIndex == 1)
	{
		renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	}
	// Set the back buffer as the render target
	commandList->OMSetRenderTargets(1, &renderTargetViewHandle, false, nullptr);



	// Then set the color to clear the window to
	float color[4];
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;
	commandList->ClearRenderTargetView(renderTargetViewHandle, color, 0, nullptr);

	// Indicate that the back buffer will now be used to present
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrier);



	// Close the list of commands
	result = commandList->Close();
	if (FAILED(result))
	{
		return false;
	}
	// Load the command list array(only one command list for now)
	ComPtr<ID3D12CommandList> ppCommandLists[1];
	ppCommandLists[0] = commandList.Get();
	// Execute the list of commands
	commandQueue->ExecuteCommandLists(1, ppCommandLists[0].GetAddressOf());

	// Finally present the back buffer to the screen 
	// since rendering is complete
	if (vSyncEnabled)
	{
		// Loct to screen refresh reate
		result = swapChain->Present(1, 0);
		if (FAILED(result))
		{
			return false;
		}
	}
	else
	{
		// Present as fast as possible
		result = swapChain->Present(0, 0);
		if (FAILED(result))
		{
			return false;
		}
	}



	// Signal and increment the fence value
	unsigned long long fenceToWaitFor = fenceValue;
	result = commandQueue->Signal(fence.Get(), fenceToWaitFor);
	if (FAILED(result))
	{
		return false;
	}
	fenceValue++;
	// Wait until the GPU is done rendering
	if (fence->GetCompletedValue() < fenceToWaitFor)
	{
		result = fence->SetEventOnCompletion(fenceToWaitFor, fenceEvent);
		if (FAILED(result))
		{
			return false;
		}
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	// Alternate the back buffer index back and forth
	// between 0 and 1 each frame
	bufferIndex = bufferIndex == 0 ? 1 : 0;
	return true;
}
