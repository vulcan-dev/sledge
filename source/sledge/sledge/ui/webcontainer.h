#pragma once

#pragma warning (push)
#pragma warning (disable: 4201)
#pragma warning (disable: 4100)
#include <Ultralight/Ultralight.h>
#pragma warning (pop)

class CWebContainer {
public:
	CWebContainer(int iWidth, int iHeight, int iX = 0, int iY = 0);
	~CWebContainer();

	void LoadURL(const char* cURL);
	void Draw();


	int m_Width, m_Height;
	int m_X, m_Y;

	ultralight::RefPtr<ultralight::View> View() { return m_View; }
private:
	ultralight::RefPtr<ultralight::View> m_View;
};