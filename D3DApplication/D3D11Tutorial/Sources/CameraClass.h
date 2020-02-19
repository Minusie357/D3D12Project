#pragma once

#include <DirectXMath.h>



/* CameraClass */
class CameraClass
{
public:
	CameraClass() = default;
	CameraClass(const CameraClass&) = default;
	~CameraClass() = default;

public:
	inline void SetPosition(float x, float y, float z)
	{
		posX = x, posY = y, posZ = z;
	}
	inline void SetRotation(float x, float y, float z)
	{
		rotX = x, rotY = y, rotZ = z;
	}

	inline DirectX::XMFLOAT3 GetPosition() const
	{
		return DirectX::XMFLOAT3(posX, posY, posZ);
	}
	inline DirectX::XMFLOAT3 GetRotation() const
	{
		return DirectX::XMFLOAT3(rotX, rotY, rotZ);
	}
	inline void GetViewMatrix(DirectX::XMMATRIX& outViewMatrix)
	{
		outViewMatrix = ViewMatrix;
	}

public:
	void Render();



private:
	float posX = 0.f, posY = 0.f, posZ = 0.f;
	float rotX = 0.f, rotY = 0.f, rotZ = 0.f;
	DirectX::XMMATRIX ViewMatrix;
};

