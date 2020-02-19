#include "ColorShaderClass.h"
#include <fstream>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;


bool ColorShaderClass::Initialize(ID3D11Device& device, HWND hWnd)
{
	bool result;
	
	// Initialize the vertex and pixel shaders
	result = InitializeShader(
		device,
		hWnd,
		L"Shaders/ColorVertexShader.hlsl",
		L"Shaders/ColorPixelShader.hlsl");
	return result;
}

void ColorShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as
	// the related objects
	ShutdownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext& deviceContext, int numIndices, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	bool result;

	// Set the shader parameters that it will use for rendering
	result = SetShaderParameters(
		deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (result == false)
	{
		return false;
	}

	// Now render the prepared buffers with the shader
	RenderShader(deviceContext, numIndices);
	return true;
}



bool ColorShaderClass::InitializeShader(ID3D11Device& device, HWND hWnd, const std::wstring vsFile, const std::wstring psFile)
{
	HRESULT result;

	ComPtr<ID3D10Blob> errorMessage;
	ComPtr<ID3D10Blob> vertexShaderBuffer;
	ComPtr<ID3D10Blob> pixelShaderBuffer;

	// Compile the vertex shader code
	result = D3DCompileFromFile(
		vsFile.c_str(), nullptr, nullptr, "ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		vertexShaderBuffer.GetAddressOf(),
		errorMessage.GetAddressOf());
	if (FAILED(result))
	{
		// If the shader failded to compile it should have
		// writen something to the error message
		if (errorMessage.Get())
		{
			OutputShaderErrorMessage(*errorMessage.Get(), hWnd, vsFile);
		}
		// if there was nothing in the error message then
		// it simply could not find the shader file itself
		else
		{
			MessageBox(hWnd, vsFile.c_str(), L"Missing Shader File", MB_OK);
		}
		return false;
	}
	
	// Compile the pixel shader code
	result = D3DCompileFromFile(
		psFile.c_str(), nullptr, nullptr, "ColorPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		pixelShaderBuffer.GetAddressOf(),
		errorMessage.GetAddressOf());
	if (FAILED(result))
	{
		// If the shader failed to compile it should have
		// written something to the error message
		if (errorMessage.Get())
		{
			OutputShaderErrorMessage(*errorMessage.Get(), hWnd, psFile);
		}
		else
		{
			MessageBox(hWnd, psFile.c_str(), L"Missing Shader File", MB_OK);
		}
		return false;
	}



	// Create the vertex shader from the buffer
	result = device.CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		nullptr, vertexShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer
	result = device.CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		nullptr, pixelShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}



	// Creates the vertex input layout description
	// This setup needs to match the Vertex structure
	// in the ModelClass in the shader
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	ZeroMemory(&polygonLayout, sizeof(polygonLayout));
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout
	unsigned int numElements;
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout
	result = device.CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		layout.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}



	// Setup the description of the dynamic matrix constant buffer
	// that is in the vertex shader
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access
	// the vertex shader constant buffer from within this class
	result = device.CreateBuffer(
		&matrixBufferDesc, nullptr, matrixBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// As all D3D11 components are wrapped in ComPtr
	// Object doesnt need to release memebers
	return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob& errorMessage, HWND hWnd, std::wstring shaderFilename)
{
	// Get a pointer to the error message text buffer
	string compileErrors;
	compileErrors = reinterpret_cast<char*>(errorMessage.GetBufferPointer());

	// Open a file to write the error message to
	// and write out the error message 
	ofstream fout;
	fout.open(App::ERROR_FILE);
	fout << compileErrors;
	fout.close();

	// Pop a message up on the screen to notify the user
	// to check the text file for compile errors
	wstring ErrorMessage =
		L"Error compiling shader. Check "
		+ App::ERROR_FILE
		+ L"for message.";
	MessageBox(hWnd, ErrorMessage.c_str(), shaderFilename.c_str(), MB_OK);
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	HRESULT result;
	
	// Transpost the matrices to prepare them for the shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);



	// Lock the constant buffer so it can be written to
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext.Map(matrixBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer
	MatrixBufferType* dataPtr = reinterpret_cast<MatrixBufferType*>(mappedResource.pData);
	// Copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer
	deviceContext.Unmap(matrixBuffer.Get(), 0);



	// Set the position of the constant buffer in the vertex shader
	unsigned int bufferNumber = 0;
	
	// Finally set the constant buffer in the 
	// vertex shader with the updated values.
	deviceContext.VSSetConstantBuffers(bufferNumber, 1, matrixBuffer.GetAddressOf());

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext& deviceContext, int numIndices)
{
	// Set the vertex input layout
	deviceContext.IASetInputLayout(layout.Get());

	// Set the vertex and pixel shaders that will be used
	// to render this triangle
	deviceContext.VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext.PSSetShader(pixelShader.Get(), nullptr, 0);

	// Render the triangle
	deviceContext.DrawIndexed(numIndices, 0, 0);
}



