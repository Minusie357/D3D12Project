#pragma once

#ifndef _D3DCLASS_H_
#define _DEDCLASS_H_

/* LINKING */
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

/* INCLLUDES */
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>

using namespace Microsoft::WRL;



/*
 *	D3DClass have responsibility for managing d3d objects for
 *	initializing and releasing.
 */
class D3DClass
{
public:
	D3DClass() = default;
	D3DClass(const D3DClass&) = default;
	~D3DClass() = default;

public:
	/* setting up for DirectX 12*/
	bool Initialize(int screenHeight, int screenWidth, HWND hWnd, bool vSync, bool fullScreen);
	void Shutdown();

	bool Render();



private:
	bool vSyncEnabled;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;
	int videoCardMemory;
	char videoCardDesc[128];

	ComPtr<IDXGISwapChain3> swapChain;
	ComPtr<ID3D12DescriptorHeap> renderTargetViewHeap;
	ComPtr<ID3D12Resource> backBufferRenderTarget[2];
	unsigned int bufferIndex;

	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12PipelineState> pipelineState;

	ComPtr<ID3D12Fence> fence;
	HANDLE fenceEvent;
	unsigned long long fenceValue;
};
#endif // !_D3DCLASS_H_