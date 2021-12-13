#pragma once

namespace Window {
	bool Create();
	void MoveWindow(int iDeltaX, int iDeltaY);
	void Close();

	inline const int iSizeW = 500;
	inline const int iSizeH = 400;

	inline const int iPosX = 0;
	inline const int iPosY = 0;
}