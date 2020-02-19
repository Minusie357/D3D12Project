#pragma once

/* INCLUDES */
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>	
#include <DirectXMath.h>
#include <string>

/* GLOBALS */
namespace App
{
	const std::wstring ERROR_FILE = L"shader_error.txt";
}



/* ColorShaderClass */
class ColorShaderClass
{
private:
	/* MatrixBufferType changes local coordinate system
	 * to projection coordinate system. */
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ColorShaderClass() = default;
	ColorShaderClass(const ColorShaderClass&) = default;
	~ColorShaderClass() = default;

public:
	bool Initialize(ID3D11Device& inDevice, HWND hWnd);
	void Shutdown();
	bool Render(
		ID3D11DeviceContext& inDeviceContext, int numIndices,
		DirectX::XMMATRIX worldMatrix, 
		DirectX::XMMATRIX viewMatrix, 
		DirectX::XMMATRIX projectionMatrix);

private:
	bool InitializeShader(
		ID3D11Device& inDevice, HWND hWnd, 
		const std::wstring vsFilename, const std::wstring psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(
		ID3D10Blob& errorMessage, HWND hWnd, std::wstring shaderFilename);

	bool SetShaderParameters(
		ID3D11DeviceContext& deviceContext, DirectX::XMMATRIX worldMatrix, 
		DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void RenderShader(ID3D11DeviceContext& deviceContext, int numIndices);



private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> matrixBuffer;
};
