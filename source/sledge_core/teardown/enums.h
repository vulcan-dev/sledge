#pragma once

enum class EGameState : unsigned int{
	Splash = 1,
	Menu = 2,
	UI = 3,
	Playing = 4,
	Editing = 5,
	Quitting = 6
};