#include "ModelClass.h"
#include <vector>

using namespace DirectX;
using namespace std;



bool ModelClass::Initialize(ID3D11Device& device)
{
	// Initialize the vertex and index buffer
	return InitializeBuffers(device);
}

void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffer
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext& deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline
	// to prepare them for drawing
	RenderBuffers(deviceContext);
}





bool ModelClass::InitializeBuffers(ID3D11Device& device)
{	
	// Set the number of vertices and indices in the vertex array
	numVertices = 3;
	numIndices = 3;

	vector<Vertex> vertices(numVertices);
	vector<unsigned long> indices(numIndices);

	// Load the vertex array with data
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;



	HRESULT result;
	// Set up the description of the static vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	//ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * numVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer
	result = device.CreateBuffer(
		&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}



	// Set up the description of the static index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	//ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * numIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer
	result = device.CreateBuffer(
		&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ModelClass::ShutdownBuffers()
{
	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext& deviceContext)
{
	// Set the vertex buffer stride and offset
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler 
	// so it can be rendered
	deviceContext.IASetVertexBuffers(
		0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active input assembler
	// so it can be rendered
	deviceContext.IASetIndexBuffer(
		indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from
	// this vertex buffer, in this case triangles.
	deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}