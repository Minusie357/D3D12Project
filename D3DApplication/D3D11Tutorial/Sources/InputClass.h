#pragma once

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_



/*
 * InputClass handles the user input from the keyboard.
 * This class is given input from the SystemClass::MessageHnadler
 * function. The input object will store the state of each key in
 * a keyboard array. When queried it will tell the calling funct-
 * ions if a certain key is pressed
 */
class InputClass final
{
public:
	InputClass() = default;
	InputClass(const InputClass&) = default;
	~InputClass() = default;

public:
	void Initialize();

	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);
	bool IsKeyDown(unsigned int);

private:
	// pressed keys array
	bool keys[256] = { false };
};
#endif // !_INPUTCLASS_H_
