#include "InputManager.h"

InputManager InputManager::instance = InputManager();

InputManager::InputManager()
{
	if(keyboard != 0)
	{
		delete keyboard;
	}
	this->keyboard = new Keyboard();
}

InputManager::~InputManager()
{
	delete keyboard;
	keyboard = 0;
}

void InputManager::Activate()
{
	if(instance.keyboard == 0)
	{
		instance.keyboard = new Keyboard();
	}
}

void InputManager::Deactivate()
{
	delete instance.keyboard;
	instance.keyboard = 0;
}

Keyboard* InputManager::getKeyboard()
{
	return instance.keyboard;
}