#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Windows.h>

enum Keyboard_Key
{
	/* Printable keys */
	KEY_SPACE           = VK_SPACE,
	KEY_APOSTROPHE      = 39,  /* ' */
	KEY_COMMA           = 44,  /* , */
	KEY_MINUS           = 45,  /* - */
	KEY_PERIOD          = 46,  /* . */
	KEY_SLASH           = 47,  /* / */
	KEY_0               = 48,
	KEY_1               = 49,
	KEY_2               = 50,
	KEY_3               = 51,
	KEY_4               = 52,
	KEY_5               = 53,
	KEY_6               = 54,
	KEY_7               = 55,
	KEY_8               = 56,
	KEY_9               = 57,
	KEY_SEMICOLON       = 59,  /* ; */
	KEY_EQUAL           = 61,  /* = */
	KEY_A               = 65,
	KEY_B               = 66,
	KEY_C               = 67,
	KEY_D               = 68,
	KEY_E               = 69,
	KEY_F               = 70,
	KEY_G               = 71,
	KEY_H               = 72,
	KEY_I               = 73,
	KEY_J               = 74,
	KEY_K               = 75,
	KEY_L               = 76,
	KEY_M               = 77,
	KEY_N               = 78,
	KEY_O               = 79,
	KEY_P               = 80,
	KEY_Q               = 81,
	KEY_R               = 82,
	KEY_S               = 83,
	KEY_T               = 84,
	KEY_U               = 85,
	KEY_V               = 86,
	KEY_W               = 87,
	KEY_X               = 88,
	KEY_Y               = 89,
	KEY_Z               = 90,
	KEY_LEFT_BRACKET    = 91,  /* [ */
	KEY_BACKSLASH       = 92,  /* \ */
	KEY_RIGHT_BRACKET   = 93,  /* ] */
	KEY_GRAVE_ACCENT    = 96,  /* ` */

	KEY_ARROW_UP        = VK_UP,     
	KEY_ARROW_DOWN      = VK_DOWN,       
	KEY_ARROW_LEFT      = VK_LEFT ,    
	KEY_ARROW_RIGHT     = VK_RIGHT, 

	/* Function keys */
	KEY_ESCAPE          = VK_ESCAPE,
	KEY_ENTER           = VK_RETURN,
	KEY_TAB             = VK_TAB,
	KEY_BACKSPACE       = VK_BACK,
	KEY_INSERT          = VK_INSERT,
	KEY_DELETE          = VK_DELETE,
	//KEY_RIGHT           = 262,
	//KEY_LEFT            = 263,
	//KEY_DOWN            = 264,
	//KEY_UP              = 265,
	KEY_PAGE_UP         = VK_PRIOR,
	KEY_PAGE_DOWN       = VK_NEXT,
	KEY_HOME            = VK_HOME,
	KEY_END             = VK_END,
	KEY_CAPS_LOCK       = VK_CAPITAL,
	KEY_SCROLL_LOCK     = VK_SCROLL,
	KEY_NUM_LOCK        = VK_NUMLOCK,
	KEY_PRINT_SCREEN    = VK_PRINT,
	KEY_PAUSE           = VK_PAUSE,
	KEY_F1              = VK_F1,
	KEY_F2              = VK_F2,
	KEY_F3              = VK_F3,
	KEY_F4              = VK_F4,
	KEY_F5              = VK_F5,
	KEY_F6              = VK_F6,
	KEY_F7              = VK_F7,
	KEY_F8              = VK_F8,
	KEY_F9              = VK_F9,
	KEY_F10             = VK_F10,
	KEY_F11             = VK_F11,
	KEY_F12             = VK_F12,
	KEY_F13             = VK_F13,
	KEY_F14             = VK_F14,
	KEY_F15             = VK_F15,
	KEY_F16             = VK_F16,
	KEY_F17             = VK_F17,
	KEY_F18             = VK_F18,
	KEY_F19             = VK_F19,
	KEY_F20             = VK_F20,
	KEY_F21             = VK_F21,
	KEY_F22             = VK_F22,
	KEY_F23             = VK_F23,
	KEY_F24             = VK_F24,
	//KEY_F25             = 319,
	KEY_NUMPAD_0        = VK_NUMPAD0,
	KEY_NUMPAD_1        = VK_NUMPAD1,
	KEY_NUMPAD_2        = VK_NUMPAD2,
	KEY_NUMPAD_3        = VK_NUMPAD3,
	KEY_NUMPAD_4        = VK_NUMPAD4,
	KEY_NUMPAD_5        = VK_NUMPAD5,
	KEY_NUMPAD_6        = VK_NUMPAD6,
	KEY_NUMPAD_7        = VK_NUMPAD7,
	KEY_NUMPAD_8        = VK_NUMPAD8,
	KEY_NUMPAD_9        = VK_NUMPAD9,
	KEY_NUMPAD_DECIMAL  = VK_DECIMAL,
	KEY_NUMPAD_DIVIDE   = VK_DIVIDE,
	KEY_NUMPAD_MULTIPLY = VK_MULTIPLY,
	KEY_NUMPAD_SUBTRACT = VK_SUBTRACT,
	KEY_NUMPAD_ADD      = VK_ADD,
	//KEY_NUMPAD_ENTER    = 335,
	//KEY_NUMPAD_EQUAL    = 336,
	KEY_LEFT_SHIFT      = VK_LSHIFT,
	KEY_LEFT_CONTROL    = VK_LCONTROL,
	//KEY_LEFT_ALT        = 342,
	//KEY_LEFT_SUPER      = 343,
	KEY_RIGHT_SHIFT     = VK_RSHIFT,
	KEY_RIGHT_CONTROL   = VK_RCONTROL,
	//KEY_RIGHT_ALT       = 346,
	//KEY_RIGHT_SUPER     = 347,
	KEY_MENU            = VK_MENU,
};

class Keyboard
{
public:
	Keyboard();

	bool isPressed(Keyboard_Key key);
};

#endif