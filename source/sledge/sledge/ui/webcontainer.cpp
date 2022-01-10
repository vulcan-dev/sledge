#include "webcontainer.h"

#include "globals.h"
#include "sledge/ui.h"

CWebContainer::CWebContainer(int iWidth, int iHeight, int iX, int iY) {
	m_Width = iWidth; m_Height = iHeight;
	m_X = iX; m_Y = iY;

	ultralight::ViewConfig Config;

	Config.is_transparent = true;
	Config.is_accelerated = true;

	m_View = CSledgeUI::Instance()->Renderer()->CreateView(iWidth, iHeight, Config, nullptr);
	CSledgeUI::Instance()->_RegisterWebContainer(this);
}

CWebContainer::~CWebContainer() {
	m_View->Release();
	CSledgeUI::Instance()->_UnregisterWebContainer(this);
}

void CWebContainer::LoadURL(const char* cURL) { m_View->LoadURL(cURL); }

void CWebContainer::Draw() {
	ultralight::RenderTarget RTInfo = m_View->render_target();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, g_WindowWidth, g_WindowHeight, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	CSledgeUI::Instance()->Driver()->BindTexture(0, RTInfo.texture_id);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(static_cast<GLfloat>(m_X), static_cast<GLfloat>(m_Y), 0); // TL
	glTexCoord2f(1, 0);
	glVertex3f(static_cast<GLfloat>(m_X + m_Width), static_cast<GLfloat>(m_Y), 0); // TR
	glTexCoord2f(1, 1);
	glVertex3f(static_cast<GLfloat>(m_X + m_Width), static_cast<GLfloat>(m_Y + m_Height), 0); // BR
	glTexCoord2f(0, 1);
	glVertex3f(static_cast<GLfloat>(m_X), static_cast<GLfloat>(m_Y + m_Height), 0); // BL
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}