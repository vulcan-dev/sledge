#pragma once
#define UI_REFRESH_RATE 60

#include "ui/gpudriver.h"
#include "ui/webcontainer.h"

#include <Ultralight/platform/Logger.h>

class CSledgeUI : public ultralight::Logger {
	public:
		CSledgeUI();
		~CSledgeUI();

		static CSledgeUI* Instance() { return m_Instance; }
		static void CreateInstance() { if (m_Instance) return; m_Instance = new CSledgeUI(); }

		void Reset();

		void Update();
		void Draw();

		CGPUDriver* Driver() { return m_Driver; }
		ultralight::RefPtr<ultralight::Renderer> Renderer() { return m_Renderer; }

		virtual void LogMessage(ultralight::LogLevel eLogLevel, const ultralight::String16& Message) override;
private:
	static CSledgeUI* m_Instance;

	CGPUDriver* m_Driver;
	ultralight::RefPtr<ultralight::Renderer> m_Renderer;
};