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
	Type_Body = 1,
	Type_Shape = 2,
	Type_Light = 3,
	Type_Location = 4,
	Type_Water = 5,
	Type_Enemy = 6,
	Type_Joint = 7,
	Type_Vehicle = 8,
	Type_Wheel = 9,
	Type_Screen = 10,
	Type_Trigger = 11,
	Type_Script = 12
};