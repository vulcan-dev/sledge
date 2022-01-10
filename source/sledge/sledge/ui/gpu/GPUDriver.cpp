#include "gpudriver.h"

#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/FileSystem.h>

#include "shaders/shader_fill_frag.h"
#include "shaders/shader_fill_path_frag.h"
#include "shaders/shader_v2f_c4f_t2f_t2f_d28f_vert.h"
#include "shaders/shader_v2f_c4f_t2f_vert.h"

#include <string>

#include <GLFW/glfw3.h>

/*
	to-do:
		MSAA
		opengl error checking
*/

/*
	misc gl functions
*/
const char* glErrorString(GLenum err) {
	switch (err) {
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	default: return "UNKNOWN ERROR";
	}
}

std::string GetShaderLog(GLuint iShaderId) {
	GLint iLen, iResult;
	glGetShaderiv(iShaderId, GL_INFO_LOG_LENGTH, &iLen);
	std::string Log(iLen, ' ');
	glGetShaderInfoLog(iShaderId, iLen, &iResult, &Log[0]);
	return Log;
}

GLuint LoadShaderFromSource(GLenum eShaderType, const char* cSource, const char* cFilename) {
	GLuint iShaderId = glCreateShader(eShaderType);
	glShaderSource(iShaderId, 1, &cSource, NULL);
	glCompileShader(iShaderId);

	GLint iCompileStatus;
	glGetShaderiv(iShaderId, GL_COMPILE_STATUS, &iCompileStatus);

	if (iCompileStatus == GL_FALSE)
		printf("Unable to compile shader %s - Error: %s | %s", cFilename, glErrorString(glGetError()), GetShaderLog(iShaderId).c_str());

	return iShaderId;
}

/*
	CGPUDriver
*/

// texture
void CGPUDriver::CreateTexture(GLuint iTextureId, ultralight::Ref<ultralight::Bitmap> Bitmap) {
	STextureEntry Entry;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &Entry.m_TextureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Entry.m_TextureId);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, Bitmap->row_bytes() / Bitmap->bpp());


	if (Bitmap->format() == ultralight::BitmapFormat::kBitmapFormat_A8_UNORM) {
		const void* pPixels = Bitmap->LockPixels();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Bitmap->width(), Bitmap->height(), 0, GL_RED, GL_UNSIGNED_BYTE, pPixels);
		Bitmap->UnlockPixels();
	}
	else if (Bitmap->format() == ultralight::BitmapFormat::kBitmapFormat_BGRA8_UNORM_SRGB) {
		const void* pPixels = Bitmap->LockPixels();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Bitmap->width(), Bitmap->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, pPixels);
		Bitmap->UnlockPixels();
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	m_TextureMap[iTextureId] = Entry;
}

void CGPUDriver::UpdateTexture(GLuint iTextureId, ultralight::Ref<ultralight::Bitmap> Bitmap) {
	glActiveTexture(GL_TEXTURE0);

	STextureEntry& Entry = m_TextureMap[iTextureId];

	glBindTexture(GL_TEXTURE_2D, Entry.m_TextureId);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, Bitmap->row_bytes() / Bitmap->bpp());

	if (Bitmap->IsEmpty())
		return;

	if (Bitmap->format() == ultralight::BitmapFormat::kBitmapFormat_A8_UNORM) {
		const void* pPixels = Bitmap->LockPixels();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Bitmap->width(), Bitmap->height(), 0, GL_RED, GL_UNSIGNED_BYTE, pPixels);
		Bitmap->UnlockPixels();
	}
	else if (Bitmap->format() == ultralight::BitmapFormat::kBitmapFormat_BGRA8_UNORM_SRGB) {
		const void* pPixels = Bitmap->LockPixels();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Bitmap->width(), Bitmap->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, pPixels);
		Bitmap->UnlockPixels();
	}

	glGenerateMipmap(GL_TEXTURE_2D);
}

void CGPUDriver::BindTexture(unsigned char cTextureUnit, GLuint iTextureId) {
	STextureEntry& Texture = m_TextureMap[iTextureId];
	glActiveTexture(GL_TEXTURE0 + cTextureUnit);
	glBindTexture(GL_TEXTURE_2D, Texture.m_TextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void CGPUDriver::DestroyTexture(GLuint iTextureId) {
	glDeleteTextures(1, &m_TextureMap[iTextureId].m_TextureId);
}

// renderbuffer
void CGPUDriver::CreateRenderBuffer(GLuint iRenderBufferId, const ultralight::RenderBuffer& Buffer) {
	if (iRenderBufferId == 0)
		return;
	GLuint iFrameBufferId;
	glGenFramebuffers(1, &iFrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, iFrameBufferId);

	STextureEntry& Texture = m_TextureMap[Buffer.texture_id];
	Texture.m_RenderBufferId = iRenderBufferId;

	glBindTexture(GL_TEXTURE_2D, Texture.m_TextureId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture.m_TextureId, 0);

	GLenum aDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, aDrawBuffers);

	GLenum eResult = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (eResult != GL_FRAMEBUFFER_COMPLETE)
		printf("Failed to create FBO");

	m_RenderBufferMap[iRenderBufferId] = iFrameBufferId;
}

void CGPUDriver::BindRenderBuffer(GLuint iRenderBufferId) {
	if (iRenderBufferId == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderBufferMap[iRenderBufferId]);
}

void CGPUDriver::ClearRenderBuffer(GLuint iRenderBufferId) {
	BindRenderBuffer(iRenderBufferId);
	glDisable(GL_SCISSOR_TEST);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CGPUDriver::DestroyRenderBuffer(GLuint iRenderBufferId) {
	if (iRenderBufferId == 0)
		return;
	glDeleteFramebuffers(1, &m_RenderBufferMap[iRenderBufferId]);
}

// geometry
void CGPUDriver::CreateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) {
	SGeometryEntry Geometry;

	glGenVertexArrays(1, &Geometry.m_VAO);
	glBindVertexArray(Geometry.m_VAO);

	glGenBuffers(1, &Geometry.m_VBOVertices);
	glBindBuffer(GL_ARRAY_BUFFER, Geometry.m_VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size, Vertices.data, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &Geometry.m_VBOIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Geometry.m_VBOIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size, Indices.data, GL_STATIC_DRAW);

	if (Vertices.format == ultralight::VertexBufferFormat::kVertexBufferFormat_2f_4ub_2f_2f_28f) {
		GLsizei iStride = 140;

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(0));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, iStride, reinterpret_cast<GLvoid*>(8));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(12));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(20));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(28));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(44));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(60));
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(76));
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(92));
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(108));
		glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(124));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);
		glEnableVertexAttribArray(8);
		glEnableVertexAttribArray(9);
		glEnableVertexAttribArray(10);
	}
	else if (Vertices.format == ultralight::VertexBufferFormat::kVertexBufferFormat_2f_4ub_2f) {
		GLsizei iStride = 20;

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(0));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, iStride, reinterpret_cast<GLvoid*>(8));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(12));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
	else {
		printf("unknown vertex format passed to CreateGeometry");
		return;
	}

	glBindVertexArray(0);
	m_GeometryMap[iGeometryId] = Geometry;
}

void CGPUDriver::UpdateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) {
	SGeometryEntry Geometry = m_GeometryMap[iGeometryId];

	glBindVertexArray(Geometry.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Geometry.m_VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size, Vertices.data, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Geometry.m_VBOIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size, Indices.data, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void CGPUDriver::DrawGeometry(GLuint iGeometryId, unsigned int iIdxCount, unsigned int iIdxOffset, const ultralight::GPUState& State) {
	if (m_ProgramMap.empty())
		LoadPrograms();

	BindRenderBuffer(State.render_buffer_id);

	SetViewport(State.viewport_width, State.viewport_height);

	SGeometryEntry& Geometry = m_GeometryMap[iGeometryId];
	SelectProgram(static_cast<ultralight::ShaderType>(State.shader_type));
	UpdateUniforms(State);

	glBindVertexArray(Geometry.m_VAO);

	BindTexture(0, State.texture_1_id);
	BindTexture(1, State.texture_2_id);
	BindTexture(2, State.texture_3_id);

	if (State.enable_scissor) {
		glEnable(GL_SCISSOR_TEST);

		const ultralight::IntRect& Rect = State.scissor_rect;
		glScissor(Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top);
	}
	else
		glDisable(GL_SCISSOR_TEST);

	if (State.enable_blend)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);


	glDrawElements(GL_TRIANGLES, iIdxCount, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(iIdxOffset * sizeof(unsigned int)));
	glBindVertexArray(0);

	m_BatchCount++;
}

void CGPUDriver::DestroyGeometry(GLuint iGeometryId) {
	SGeometryEntry& Geometry = m_GeometryMap[iGeometryId];

	glDeleteBuffers(1, &Geometry.m_VBOIndices);
	glDeleteBuffers(1, &Geometry.m_VBOVertices);

	glDeleteVertexArrays(1, &Geometry.m_VAO);

	m_GeometryMap.erase(iGeometryId);
}

// commands
void CGPUDriver::DrawCommandList() {
	if (m_CommandList.empty())
		return;

	m_BatchCount = 0;

	glEnable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_NEVER);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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

	m_CommandList.clear();
	glDisable(GL_SCISSOR_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// programs
void CGPUDriver::LoadPrograms() {
	LoadProgram(ultralight::ShaderType::kShaderType_Fill);
	LoadProgram(ultralight::ShaderType::kShaderType_FillPath);
}

void CGPUDriver::DestroyPrograms() {
	glUseProgram(0);

	for (auto i = m_ProgramMap.begin(); i != m_ProgramMap.end(); i++) {
		SProgramEntry Entry = i->second;

		glDetachShader(Entry.m_Id, Entry.m_VertexId);
		glDetachShader(Entry.m_Id, Entry.m_FragId);

		glDeleteShader(Entry.m_VertexId);
		glDeleteShader(Entry.m_FragId);

		glDeleteProgram(Entry.m_Id);
	}

	m_ProgramMap.clear();
}

void CGPUDriver::LoadProgram(ultralight::ShaderType eType) {
	SProgramEntry Program;
	if (eType == ultralight::ShaderType::kShaderType_Fill) {
		Program.m_VertexId = LoadShaderFromSource(GL_VERTEX_SHADER, shader_v2f_c4f_t2f_t2f_d28f_vert().c_str(), "shader_v2f_c4f_t2f_t2f_d28f.vert");
		Program.m_FragId = LoadShaderFromSource(GL_FRAGMENT_SHADER, shader_fill_frag().c_str(), "shader_fill.frag");
	}
	else if (eType == ultralight::ShaderType::kShaderType_FillPath) {
		Program.m_VertexId = LoadShaderFromSource(GL_VERTEX_SHADER, shader_v2f_c4f_t2f_vert().c_str(), "shader_v2f_c4f_t2f.vert");
		Program.m_FragId = LoadShaderFromSource(GL_FRAGMENT_SHADER, shader_fill_path_frag().c_str(), "shader_fill_path.frag");
	}

	Program.m_Id = glCreateProgram();

	glAttachShader(Program.m_Id, Program.m_VertexId);
	glAttachShader(Program.m_Id, Program.m_FragId);

	glBindAttribLocation(Program.m_Id, 0, "in_Position");
	glBindAttribLocation(Program.m_Id, 1, "in_Color");
	glBindAttribLocation(Program.m_Id, 2, "in_TexCoord");

	if (eType == ultralight::ShaderType::kShaderType_Fill) {
		glBindAttribLocation(Program.m_Id, 3, "in_ObjCoord");
		glBindAttribLocation(Program.m_Id, 4, "in_Data0");
		glBindAttribLocation(Program.m_Id, 5, "in_Data1");
		glBindAttribLocation(Program.m_Id, 6, "in_Data2");
		glBindAttribLocation(Program.m_Id, 7, "in_Data3");
		glBindAttribLocation(Program.m_Id, 8, "in_Data4");
		glBindAttribLocation(Program.m_Id, 9, "in_Data5");
		glBindAttribLocation(Program.m_Id, 10, "in_Data6");
	}

	glLinkProgram(Program.m_Id);
	glUseProgram(Program.m_Id);

	if (eType == ultralight::ShaderType::kShaderType_Fill) {
		glUniform1i(glGetUniformLocation(Program.m_Id, "Texture1"), 0);
		glUniform1i(glGetUniformLocation(Program.m_Id, "Texture2"), 1);
		glUniform1i(glGetUniformLocation(Program.m_Id, "Texture3"), 2);
	}

	m_ProgramMap[eType] = Program;
}

void CGPUDriver::SelectProgram(ultralight::ShaderType eType) {
	auto Program = m_ProgramMap.find(eType);
	if (Program == m_ProgramMap.end()) {
		printf("missing program");
		return;
	}

	m_CurrentProgram = Program->second.m_Id;
	glUseProgram(Program->second.m_Id);
}

// uniforms
void CGPUDriver::UpdateUniforms(const ultralight::GPUState& State) {
	bool bFlipY = State.render_buffer_id != 0;

	ultralight::Matrix MVP = ApplyProjection(State.transform, static_cast<float>(State.viewport_width), static_cast<float>(State.viewport_height), bFlipY);

	float aParams[4] = { static_cast<float>(glfwGetTime()) / 1000.0f, static_cast<float>(State.viewport_width), static_cast<float>(State.viewport_height), 1.0f };

	SetUniform4f("State", aParams);

	ultralight::Matrix4x4 MVP4x4 = MVP.GetMatrix4x4();
	SetUniformMatrix4fv("Transform", 1, MVP4x4.data);

	SetUniform4fv("Scalar4", 2, &State.uniform_scalar[0]);
	SetUniform4fv("Vector", 8, &State.uniform_vector[0].x);
	SetUniform1ui("ClipSize", State.clip_size);
	SetUniformMatrix4fv("Clip", 8, &State.clip[0].data[0]);
}

void CGPUDriver::SetUniform1ui(const char* cName, GLuint iValue) { glUniform1ui(glGetUniformLocation(m_CurrentProgram, cName), iValue); }
void CGPUDriver::SetUniform1f(const char* cName, float fValue) { glUniform1f(glGetUniformLocation(m_CurrentProgram, cName), fValue); }
void CGPUDriver::SetUniform1fv(const char* cName, size_t iCount, const float* pValue) { glUniform1fv(glGetUniformLocation(m_CurrentProgram, cName), static_cast<GLsizei>(iCount), pValue); }
void CGPUDriver::SetUniform4f(const char* cName, const float aValue[4]) { glUniform4f(glGetUniformLocation(m_CurrentProgram, cName), aValue[0], aValue[1], aValue[2], aValue[3]); }
void CGPUDriver::SetUniform4fv(const char* cName, size_t iCount, const float* pValue) { glUniform4fv(glGetUniformLocation(m_CurrentProgram, cName), static_cast<GLsizei>(iCount), pValue); }
void CGPUDriver::SetUniformMatrix4fv(const char* cName, size_t iCount, const float* pValue) { glUniformMatrix4fv(glGetUniformLocation(m_CurrentProgram, cName), static_cast<GLsizei>(iCount), false, pValue); }

// misc
void CGPUDriver::SetViewport(unsigned int iWidth, unsigned int iHeight) { glViewport(0, 0, iWidth, iHeight); }

ultralight::Matrix CGPUDriver::ApplyProjection(const ultralight::Matrix4x4& Transform, float fWidth, float fHeight, bool bFlipY) {
	ultralight::Matrix TransformMtx;
	TransformMtx.Set(Transform);

	ultralight::Matrix Result;
	Result.SetOrthographicProjection(fWidth, fHeight, bFlipY);
	Result.Transform(TransformMtx);
	return Result;
}