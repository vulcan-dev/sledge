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

class CSledgeUI : public ultralight::Logger {
public:
	static CSledgeUI* Instance() {
		if (!m_Instance)
			m_Instance = new CSledgeUI();
		return m_Instance;
	}

	void Update();
	void Draw();

	CGPUDriver* Driver() { return m_Driver; }
	ultralight::RefPtr<ultralight::Renderer> Renderer() { return m_Renderer; }

	virtual void LogMessage(ultralight::LogLevel eLogLevel, const ultralight::String16& Message) override;
private:
	CSledgeUI();
	~CSledgeUI();

	static CSledgeUI* m_Instance;
	CGPUDriver* m_Driver;;
	ultralight::RefPtr<ultralight::Renderer> m_Renderer;
};