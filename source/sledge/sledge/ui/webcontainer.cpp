#include "webcontainer.h"

#include "globals.h"
#include "sledge/ui.h"

CWebContainer::CWebContainer(unsigned int iWidth, unsigned int iHeight, int iX, int iY) {
	m_Width = iWidth; m_Height = iHeight;
	m_X = iX; m_Y = iY;

	ultralight::ViewConfig Config;

	Config.is_transparent = true;
	Config.is_accelerated = true;
	Config.enable_javascript = true;

	m_View = CSledgeUI::Instance()->Renderer()->CreateView(iWidth, iHeight, Config, nullptr);

	WebContainers::RegisterMutex.lock();
	WebContainers::RegisteredContainers.push_back(this);
	WebContainers::RegisterMutex.unlock();
}

CWebContainer::~CWebContainer() {
	m_View = nullptr;

	WebContainers::RegisterMutex.lock();
	WebContainers::RegisteredContainers.erase(std::remove(WebContainers::RegisteredContainers.begin(), WebContainers::RegisteredContainers.end(), this), WebContainers::RegisteredContainers.end());
	WebContainers::RegisterMutex.unlock();
}

void CWebContainer::LoadURL(const char* cURL) { m_View->LoadURL(cURL); }

void CWebContainer::Resize(int iWidth, int iHeight) {
	m_View->Resize(iWidth, iHeight);
	m_Width = iWidth; m_Height = iHeight;
}

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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