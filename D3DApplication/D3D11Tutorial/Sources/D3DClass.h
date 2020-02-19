#pragma once

#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

/* LINGKINGS */
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/* INCLUDES */
#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>

/* STATIC VARIABLES */
constexpr size_t D3DNameLength = 128;



class D3DClass final
{
public:
	D3DClass() = default;
	D3DClass(const D3DClass&) = default;
	~D3DClass() = default;

public:
	/* Initialize the entire setup of Direct3D for DirectX 11. */
	bool Initiailize(
		int screenWidth, int screenHeight, bool vSync, 
		HWND hWnd, bool fullScreen, float screenDepth, float screenNear);

	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

public:
	inline ID3D11Device* GetDevice() const
	{
		return device.Get();
	}
	inline ID3D11DeviceContext* GetDeviceContext() const
	{
		return deviceContext.Get();
	}

	inline void GetProjectionMatrix(DirectX::XMMATRIX& outProjectionMatrix) const
	{
		outProjectionMatrix = projectionMatrix;
	}
	inline void GetWorldMatrix(DirectX::XMMATRIX& outWorldMatrix) const
	{
		outWorldMatrix = worldMatrix;
	}
	inline void GetOrthoMatrix(DirectX::XMMATRIX& outOrthoMatrix) const
	{
		outOrthoMatrix = orthoMatrix;
	}
	
	/* get video card's name and memory */
	void GetVideoCardInfo(char* outIVideoCardName, int& outMemory);
	


private:
	bool bVsyncEnabled;

	/* video card section */
	int videoCardMemory;
	char videoCardName[D3DNameLength];
	
	/* d3d11 section */
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;
};

#endif // !_D3DCLASS_H_