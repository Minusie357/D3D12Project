#include "InputClass.h"

void InputClass::Initialize()
{
	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// if a key is pressed then save that state in the key array
	keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	// if a key is released then clear that state in the key array
	keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return keys[key];
}
