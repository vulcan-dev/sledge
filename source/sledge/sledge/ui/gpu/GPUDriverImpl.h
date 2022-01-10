#pragma once

#pragma warning (push)
#pragma warning (disable: 4201)
#include <Ultralight/platform/GPUDriver.h>
#pragma warning (pop)

#include <GL/glew.h>
#include <vector>

class CGPUDriverImpl : public ultralight::GPUDriver {
public:
	CGPUDriverImpl() {};
	virtual ~CGPUDriverImpl() {};

	virtual const char* name() = 0;

	virtual void BeginDrawing() = 0;
	virtual void EndDrawing() = 0;

	virtual void BindTexture(unsigned char cTextureUnit, GLuint iTextureId) = 0;
	virtual void BindRenderBuffer(GLuint iRenderBufferId) = 0;

	virtual void ClearRenderBuffer(GLuint iRenderBufferId) = 0;

	virtual void DrawGeometry(GLuint iGeometryId, unsigned int iIdxCount, unsigned int iIdxOffset, const ultralight::GPUState& State) = 0;

	virtual void DrawCommandList() = 0;
	virtual void UpdateCommandList(const ultralight::CommandList& List) override {
		if (List.size) {
			m_CommandList.resize(List.size);
			memcpy(&m_CommandList[0], List.commands, sizeof(ultralight::Command) * List.size);
		}
	};

	virtual void BeginSynchronize() override {};
	virtual void EndSynchronize() override {};
	virtual unsigned int NextTextureId() override { return m_NextTextureId++; };
	virtual unsigned int NextRenderBufferId() override { return m_NextRenderBufferId++; };
	virtual unsigned int NextGeometryId() override { return m_NextGeometryId++; };
protected:
	unsigned int m_NextTextureId = 1;
	unsigned int m_NextRenderBufferId = 1;
	unsigned int m_NextGeometryId = 1;
	unsigned int m_BatchCount;

	std::vector<ultralight::Command> m_CommandList;
};