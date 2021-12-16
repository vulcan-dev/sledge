#pragma once

namespace Window {
	void Create();
	void MoveWindow(int iX, int iY);
	void Close();

	inline int iPosX = 0;
	inline int iPosY = 0;

	/*
		configurable variables
	*/
	inline const int iSizeW = 500;
	inline const int iSizeH = 400;
}