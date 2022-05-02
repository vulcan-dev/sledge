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
	Type_Joint = 6,
	Type_Vehicle = 7,
	Type_Wheel = 8,
	Type_Screen = 9,
	Type_Trigger = 10,
	Type_Script = 11
};