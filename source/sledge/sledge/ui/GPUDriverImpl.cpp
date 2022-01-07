#include "GPUDriverImpl.h"

CGPUDriverImpl::CGPUDriverImpl() : m_BatchCount(0) {}
CGPUDriverImpl::~CGPUDriverImpl() {}

bool CGPUDriverImpl::HasCommandsPending() { return !m_CommandList.empty(); }

void CGPUDriverImpl::DrawCommandList() {
	if (m_CommandList.empty())
		return;

	m_BatchCount = 0;

	for (auto i = m_CommandList.begin(); i != m_CommandList.end(); i++) {
		switch (i->command_type) {
		case ultralight::CommandType::kCommandType_DrawGeometry:
			DrawGeometry(i->geometry_id, i->indices_count, i->indices_offset, i->gpu_state);
			break;
		case ultralight::CommandType::kCommandType_ClearRenderBuffer:
			ClearRenderBuffer(i->gpu_state.render_buffer_id);
			break;
		}
	}
}

void CGPUDriverImpl::UpdateCommandList(const ultralight::CommandList& List) {
	if (List.size) {
		m_CommandList.resize(List.size);
		memcpy(&m_CommandList[0], List.commands, sizeof(ultralight::Command) * List.size);
	}
}
