#include "webcontainer.h"
#include "globals.h"

#include "sledge/ui.h"

CWebContainer::CWebContainer(unsigned int iWidth, unsigned int iHeight, int iX, int iY) {
	m_Width = iWidth; m_Height = iHeight;
	m_X = iX; m_Y = iY;

	ultralight::ViewConfig VConfig;
	VConfig.is_transparent = true;
	VConfig.is_accelerated = true;
	VConfig.enable_javascript = true;

	m_View = CSledgeUI::Instance()->Renderer()->CreateView(m_Width, m_Height, VConfig, nullptr);

	WebContainers::RegisterContainer(this);
}

CWebContainer::~CWebContainer() {
	m_View = nullptr;
	WebContainers::UnregisterContainer(this);
}

void CWebContainer::LoadURL(const char* cURL) {
	m_View->LoadURL(cURL);
	cLastURL = new char[strlen(cURL)];
	memcpy(cLastURL, cURL, strlen(cURL) + 1);
}

void CWebContainer::Resize(unsigned int iWidth, unsigned int iHeight) {
	m_Width = iWidth; m_Height = iHeight;
	m_View->Resize(m_Width, m_Height);
}

void CWebContainer::Reload() {
	m_View = nullptr;

	ultralight::ViewConfig VConfig;
	VConfig.is_transparent = true;
	VConfig.is_accelerated = true;
	VConfig.enable_javascript = true;
	m_View = CSledgeUI::Instance()->Renderer()->CreateView(m_Width, m_Height, VConfig, nullptr);
	m_View->LoadURL(cLastURL);
}

void CWebContainer::Draw() {
	CSledgeUI::Instance()->Driver()->BindReadRenderBuffer(m_View->render_target().render_buffer_id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, m_Width, m_Height, m_X, g_WindowHeight - m_Y , m_X + m_Width, g_WindowHeight - (m_Height + m_Y), GL_COLOR_BUFFER_BIT, GL_NEAREST);
}