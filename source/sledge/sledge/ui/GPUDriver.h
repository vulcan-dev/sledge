#pragma once
#include "GPUContext.h"

#pragma warning (push)
#pragma warning (disable: 4201)
#include <Ultralight/platform/GPUDriver.h>
#pragma warning (pop)

#include <map>
#include <vector>

struct STextureEntry {
	GLuint m_TextureId = 0;
	GLuint m_RenderBufferId = 0;
	GLuint m_Width, m_Height;
	bool m_sRGB = false;
};


struct SRenderBufferEntry {
	std::map<GLFWwindow*, GLuint> m_FBOMap;
	GLuint m_TextureId;
};

struct SGeometryEntry {
	std::map<GLFWwindow*, GLuint> m_VAOMap;
	ultralight::VertexBufferFormat m_VertexFormat;
	GLuint m_VBOVertices;
	GLuint m_VBOIndices;
};

struct SProgramEntry {
	GLuint m_Id;
	GLuint m_VertexId;
	GLuint m_FragId;
};

class CGPUDriver : public ultralight::GPUDriver {
public:
	CGPUDriver(class CGPUContext* Context);
	virtual ~CGPUDriver() {};

	virtual const char* name() { return "sledge"; }

	virtual void BeginDrawing() = 0;
	virtual void EndDrawing() = 0;

	virtual unsigned int NextTextureId() override;
	void CreateTexture(GLuint iTextureId, ultralight::RefPtr<ultralight::Bitmap> Bitmap);
	void UpdateTexture(GLuint iTextureId, ultralight::RefPtr<ultralight::Bitmap> Bitmap);
	void BindTexture(unsigned char cTextureUnit, GLuint iTextureId);
	virtual void DestroyTexture(GLuint iTextureId) override;
	void BindUltralightTexture(GLuint iTextureId);

	virtual unsigned int NextRenderBufferId() override;
	virtual void CreateRenderBuffer(GLuint iRenderBufferId, const ultralight::RenderBuffer& Buffer) override;
	void BindRenderBuffer(GLuint iRenderBufferId);
	virtual void DestroyRenderBuffer(GLuint iRenderBufferId) override;
	void ClearRenderBuffer(GLuint iRenderBufferId);

	virtual unsigned int NextGeometryId() override;
	virtual void CreateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) override;
	virtual void DrawGeometry(GLuint iGeometryId, unsigned int iIdxCount, unsigned int iIdxOffset, const ultralight::GPUState& State);
	virtual void UpdateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) override;
	virtual void DestroyGeometry(GLuint iGeometryId) override;

	void LoadPrograms();
	void DestroyPrograms();
	void LoadProgram(ultralight::ShaderType eType);
	void SelectProgram(ultralight::ShaderType eType);

	virtual bool HasCommandsPending();
	virtual void DrawCommandList();
	virtual void UpdateCommandList(const ultralight::CommandList& List) override;

	int batch_count() const;

	virtual void BeginSynchronize() override;
	virtual void EndSynchronize() override;

	void UpdateUniforms(const ultralight::GPUState& State);
	void SetUniform1ui(const char* cName, GLuint iValue);
	void SetUniform1f(const char* cName, float fValue);
	void SetUniform1fv(const char* cName, size_t iCount, const float* pValue);
	void SetUniform4f(const char* cName, const float aValue[4]);
	void SetUniform4fv(const char* cName, size_t iCount, const float* pValue);
	void SetUniformMatrix4fv(const char* cName, size_t iCount, const float* pValue);
	void SetViewport(unsigned int iWidth, unsigned int iHeight);
private:
	void CreateFBOTexture(GLuint iTextureId, ultralight::RefPtr<ultralight::Bitmap> Bitmap);
	void CreateFBOIfNeededForActiveContext(GLuint iRenderBufferId);
	void CreateVAOIfNeededForActiveContext(GLuint iGeometryId);
	void ResolveIfNeeded(GLuint iRenderBufferId);

	ultralight::Matrix ApplyProjection(const ultralight::Matrix4x4& Transform, float fWidth, float fHeight, bool bFlipY);

	unsigned int m_NextTextureId = 1;
	unsigned int m_NextRenderBufferId = 1;
	unsigned int m_NextGeometryId = 1;
	unsigned int m_BatchCount;

	std::map<GLuint, STextureEntry> m_TextureMap;
	std::map<GLuint, SRenderBufferEntry> m_RenderBufferMap;
	std::map<GLuint, SGeometryEntry> m_GeometryMap;


	std::map<ultralight::ShaderType, SProgramEntry> m_Programs;
	GLuint m_CurrentProgram;

	std::vector<ultralight::Command> m_CommandList;

	CGPUContext* m_Context;
};