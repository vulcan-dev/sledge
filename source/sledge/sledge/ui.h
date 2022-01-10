#pragma once

#define REFRESH_RATE 60

#include "ui/gpu/gpudriver.h"
#include "ui/webcontainer.h"

#pragma warning (push)
#pragma warning (disable: 4201)
#pragma warning (disable: 4100)
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Logger.h>
#pragma warning (pop)

#include <vector>

class CSledgeUI : public ultralight::Logger {
public:
	CSledgeUI();
	~CSledgeUI();

	void Update();
	void Draw();

	void _RegisterWebContainer(CWebContainer* Container);
	void _UnregisterWebContainer(CWebContainer* Container);

	CGPUDriver* Driver() { return m_Driver; }
	ultralight::RefPtr<ultralight::Renderer> Renderer() { return m_Renderer; }

	virtual void LogMessage(ultralight::LogLevel eLogLevel, const ultralight::String16& Message) override;
private:
	CGPUDriver* m_Driver;;
	ultralight::RefPtr<ultralight::Renderer> m_Renderer;
	std::vector<CWebContainer*> m_Containers;
};

inline CSledgeUI* g_UI;