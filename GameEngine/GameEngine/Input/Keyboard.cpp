#include "Keyboard.h"
#include <Windows.h>

Keyboard::Keyboard()
{
}

bool Keyboard::isPressed(Keyboard_Key key)
{
	if( GetAsyncKeyState(key) )
	{
		return true;
	}
	else
	{
		return false;
	}
}