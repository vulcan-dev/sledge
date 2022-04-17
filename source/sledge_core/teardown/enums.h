#pragma once

enum class EGameState : unsigned int{
	Splash = 1,
	Menu = 2,
	UI = 3,
	Playing = 4,
	Editing = 5,
	Quitting = 6
};


enum EEntityType : unsigned char {
	Body = 1,
	Shape = 2,
	Light = 3,
	Location = 4,
	Water = 5,
	Enemy = 6,
	Joint = 7,
	Vehicle = 8,
	Wheel = 9,
	Screen = 10,
	Trigger = 11,
	Script = 12
};