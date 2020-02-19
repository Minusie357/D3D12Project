#include "CameraClass.h"

using namespace DirectX;



void CameraClass::Render()
{
	XMFLOAT3 up, pos, lookAt;
	XMVECTOR upVector, posVector, lookAtVector;

	// Setup the vector that points upwards
	// and load it into a XMVECTOR structure
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	upVector = XMLoadFloat3(&up);
	
	// Setup the pos fo the camera in the world
	// and load it into a XMVECTOR structure
	pos.x = posX;
	pos.y = posY;
	pos.z = posZ;
	posVector = XMLoadFloat3(&pos);

	// Setup the where camera is looking by default
	// and load it into a XMVECTOR structure
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	lookAtVector = XMLoadFloat3(&lookAt);



	// Set the pitch(X axis), yaw(Y axis), roll(Z axis)
	// rotations in radians
	float pitch, yaw, roll;
	//pitch = XMConvertToRadians(rotX);
	//yaw = XMConvertToRadians(rotY);
	//roll = XMConvertToRadians(rotZ);
	pitch = rotX * 0.0174532925f;
	yaw = rotY * 0.0174532925f;
	roll = rotZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);



	// Transformm the lookAt and up veector by the rotation matrix
	// so the view is correctly rotated at the origin
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the 
	// location of the viewer
	lookAtVector = XMVectorAdd(posVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors
	ViewMatrix = XMMatrixLookAtLH(posVector, lookAtVector, upVector);
}
