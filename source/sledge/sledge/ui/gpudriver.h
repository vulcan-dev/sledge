#pragma once

#include "gpudriverimpl.h"

#include <GL/glew.h>

#include <map>
#include <vector>

struct STextureEntry {
	GLuint m_TextureId = 0;
	GLuint m_RenderBufferId = 0;
	int m_Width;
	int m_Height;
};

struct SGeometryEntry {
	GLuint m_VAO = 0;
	GLuint m_VBOVertices = 0;
	GLuint m_VBOIndices = 0;
};

struct SProgramEntry {
	GLuint m_Id = 0;
	GLuint m_VertexId = 0;
	GLuint m_FragId = 0;
};

class CGPUDriver : public CGPUDriverImpl {
public:
	CGPUDriver();
	~CGPUDriver();

	void Clear();

	virtual const char* name() { return "sledge"; }

	virtual void BeginDrawing() override {};
	virtual void EndDrawing() override {};

	virtual void CreateTexture(GLuint iTextureId, ultralight::Ref<ultralight::Bitmap> Bitmap) override;
	virtual void CreateFBOTexture(GLuint iTextureId, ultralight::Ref<ultralight::Bitmap> Bitmap);
	virtual void UpdateTexture(GLuint iTextureId, ultralight::Ref<ultralight::Bitmap> Bitmap) override;
	virtual void BindTexture(unsigned char cTextureUnit, GLuint iTextureId) override;
	virtual void DestroyTexture(GLuint iTextureId) override;

	virtual void CreateRenderBuffer(GLuint iRenderBufferId, const ultralight::RenderBuffer& Buffer) override;
	void BindRenderBuffer(GLuint iRenderBufferId);
	void BindReadRenderBuffer(GLuint iRenderBufferId);
	virtual void DestroyRenderBuffer(GLuint iRenderBufferId) override;
	void ClearRenderBuffer(GLuint iRenderBufferId);

	virtual void CreateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) override;
	virtual void DrawGeometry(GLuint iGeometryId, unsigned int iIdxCount, unsigned int iIdxOffset, const ultralight::GPUState& State) override;
	virtual void UpdateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) override;
	virtual void DestroyGeometry(GLuint iGeometryId) override;

	void LoadPrograms();
	void DestroyPrograms();
	void LoadProgram(ultralight::ShaderType eType);
	void SelectProgram(ultralight::ShaderType eType);

	virtual void DrawCommandList() override;

	void UpdateUniforms(const ultralight::GPUState& State);
	void SetUniform1ui(const char* cName, GLuint iValue);
	void SetUniform1f(const char* cName, float fValue);
	void SetUniform1fv(const char* cName, size_t iCount, const float* pValue);
	void SetUniform4f(const char* cName, const float aValue[4]);
	void SetUniform4fv(const char* cName, size_t iCount, const float* pValue);
	void SetUniformMatrix4fv(const char* cName, size_t iCount, const float* pValue);
	void SetViewport(unsigned int iWidth, unsigned int iHeight);
private:
	GLuint m_CurrentProgram;

	ultralight::Matrix ApplyProjection(const ultralight::Matrix4x4& Transform, float fWidth, float fHeight, bool bFlipY);

	std::map<GLuint, STextureEntry> m_TextureMap;
	std::map<GLuint, GLuint> m_RenderBufferMap;
	std::map<GLuint, SGeometryEntry> m_GeometryMap;
	std::map<ultralight::ShaderType, SProgramEntry> m_ProgramMap;
};