#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Keyboard.h"

class InputManager
{
public:
	
	static void Activate();

	static Keyboard* getKeyboard();
	static void Deactivate();
private:
	InputManager();
	~InputManager();

	//static void createKeyboard();
	//static void clear();
	
	static InputManager instance;
	Keyboard* keyboard;
};

#endif // ! IINPUTMANAGER_H
