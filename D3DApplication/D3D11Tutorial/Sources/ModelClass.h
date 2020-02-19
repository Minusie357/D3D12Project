#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>



/* ModelClass */
class ModelClass
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	ModelClass() = default;
	ModelClass(const ModelClass&) = default;
	~ModelClass() = default;

public:
	bool Initialize(ID3D11Device& device);
	void Shutdown();
	void Render(ID3D11DeviceContext& deviceContext);

private:
	bool InitializeBuffers(ID3D11Device& device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext& deviceContext);



public:
	inline int GetIndexCount() const
	{
		return numIndices;
	}



private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int numVertices = 0;
	int numIndices = 0;
};