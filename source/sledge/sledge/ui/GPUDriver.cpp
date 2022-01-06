#include "GPUDriver.h"

#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/FileSystem.h>

#include "shaders/shader_fill_frag.h"
#include "shaders/shader_fill_path_frag.h"
#include "shaders/shader_v2f_c4f_t2f_t2f_d28f_vert.h"
#include "shaders/shader_v2f_c4f_t2f_vert.h"

#include "util/log.h"

#include <string>

/*
	to-do:
	reimplement MSAA
	reimplement FATAL / add some form of error handling
*/

/*
	misc functions
*/
void ReadFile(const char* cFilePath, std::string sResult) {
	ultralight::FileSystem* FS = ultralight::Platform::instance().file_system();
	
	ultralight::FileHandle File = FS->OpenFile(cFilePath, false);
	
	long long lFileSize = 0;
	FS->GetFileSize(File, lFileSize);
	sResult.resize(lFileSize);
	FS->ReadFromFile(File, &sResult[0], lFileSize);
	FS->CloseFile(File);
}

/*
	gl functions
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

std::string GetProgramLog(GLuint iProgramId) {
	GLint iLen, iResult;
	glGetProgramiv(iProgramId, GL_INFO_LOG_LENGTH, &iLen);
	std::string Log(iLen, ' ');
	glGetProgramInfoLog(iProgramId, iLen, &iResult, &Log[0]);
	return Log;
}

GLuint LoadShaderFromSource(GLenum eShaderType, const char* cSource, const char* cFilename) {
	GLuint iShaderId = glCreateShader(eShaderType);
	glShaderSource(iShaderId, 1, &cSource, NULL);
	glCompileShader(iShaderId);

	GLint iCompileStatus;
	glGetShaderiv(iShaderId, GL_COMPILE_STATUS, &iCompileStatus);
	
	if (iCompileStatus == GL_FALSE)
		LogError("Unable to compile shader {} - Error: {} | {}", cFilename, glErrorString(glGetError()), GetShaderLog(iShaderId));

	return iShaderId;
}

/*
	CGPUDriver
*/
CGPUDriver::CGPUDriver(CGPUContext* Context) : m_Context(Context) {}

// texture
void CGPUDriver::CreateTexture(GLuint iTextureId, ultralight::Ref<ultralight::Bitmap> Bitmap) {
	if (Bitmap->IsEmpty()) {
		CreateFBOTexture(iTextureId, Bitmap);
		return;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	STextureEntry& Entry = m_TextureMap[iTextureId];
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
	glActiveTexture(GL_TEXTURE0 + cTextureUnit);
	BindUltralightTexture(iTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void CGPUDriver::DestroyTexture(GLuint iTextureId) {
	STextureEntry& Entry = m_TextureMap[iTextureId];
	glDeleteTextures(1, &Entry.m_TextureId);
}

void CGPUDriver::BindUltralightTexture(GLuint iTextureId) {
	STextureEntry& Entry = m_TextureMap[iTextureId];
	ResolveIfNeeded(Entry.m_RenderBufferId);
	glBindTexture(GL_TEXTURE_2D, Entry.m_TextureId);
}

// renderbuffer
void CGPUDriver::CreateRenderBuffer(GLuint iRenderBufferId, const ultralight::RenderBuffer& Buffer) {
	if (iRenderBufferId == 0)
		return;

	SRenderBufferEntry& RenderBuffer = m_RenderBufferMap[iRenderBufferId];
	RenderBuffer.m_TextureId = Buffer.texture_id;

	STextureEntry& Texture = m_TextureMap[Buffer.texture_id];
	Texture.m_RenderBufferId = iRenderBufferId;
}

void CGPUDriver::BindRenderBuffer(GLuint iRenderBufferId) {
	if (iRenderBufferId == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, iRenderBufferId);
		return;
	}

	CreateFBOIfNeededForActiveContext(iRenderBufferId);

	SRenderBufferEntry& Entry = m_RenderBufferMap[iRenderBufferId];

	auto Iterator = Entry.m_FBOMap.find(glfwGetCurrentContext());
	if (Iterator == Entry.m_FBOMap.end())
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, Iterator->second);
}

void CGPUDriver::ClearRenderBuffer(GLuint iRenderBufferId) {
	glfwMakeContextCurrent(m_Context->active_window());

	BindRenderBuffer(iRenderBufferId);
	glDisable(GL_SCISSOR_TEST);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CGPUDriver::DestroyRenderBuffer(GLuint iRenderBufferId) {
	if (iRenderBufferId == 0)
		return;

	GLFWwindow* PrevContext = glfwGetCurrentContext();
	
	SRenderBufferEntry& Entry = m_RenderBufferMap[iRenderBufferId];

	for (auto i = Entry.m_FBOMap.begin(); i != Entry.m_FBOMap.end(); i++) {
		GLFWwindow* Ctx = i->first;
		GLuint iFBOId = i->second;

		glfwMakeContextCurrent(Ctx);
		glDeleteFramebuffers(1, &iFBOId);
	}

	m_RenderBufferMap.erase(iRenderBufferId);
	glfwMakeContextCurrent(PrevContext);
}

// geometry
void CGPUDriver::CreateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) {
	SGeometryEntry Geometry;
	Geometry.m_VertexFormat = Vertices.format;

	glGenBuffers(1, &Geometry.m_VBOVertices);
	glBindBuffer(GL_ARRAY_BUFFER, Geometry.m_VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size, Vertices.data, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &Geometry.m_VBOIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Geometry.m_VBOIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size, Indices.data, GL_STATIC_DRAW);

	m_GeometryMap[iGeometryId] = Geometry;
}

void CGPUDriver::UpdateGeometry(GLuint iGeometryId, const ultralight::VertexBuffer& Vertices, const::ultralight::IndexBuffer& Indices) {
	SGeometryEntry Entry = m_GeometryMap[iGeometryId];

	glBindBuffer(GL_ARRAY_BUFFER, Entry.m_VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size, Vertices.data, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Entry.m_VBOIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size, Indices.data, GL_STATIC_DRAW);
}

void CGPUDriver::DrawGeometry(GLuint iGeometryId, unsigned int iIdxCount, unsigned int iIdxOffset, const ultralight::GPUState& State) {
	glfwMakeContextCurrent(m_Context->active_window());

	if (m_Programs.empty())
		LoadPrograms();

	BindRenderBuffer(State.render_buffer_id);
	SetViewport(State.viewport_width, State.viewport_height);

	SGeometryEntry& Geometry = m_GeometryMap[iGeometryId];
	SelectProgram(static_cast<ultralight::ShaderType>(State.shader_type));
	UpdateUniforms(State);

	CreateVAOIfNeededForActiveContext(iGeometryId);

	GLuint iVAO = Geometry.m_VAOMap[glfwGetCurrentContext()];
	glBindVertexArray(iVAO);

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

	GLFWwindow* PrevCtx = glfwGetCurrentContext();

	for (auto i = Geometry.m_VAOMap.begin(); i != Geometry.m_VAOMap.end(); i++) {
		glfwMakeContextCurrent(i->first);
		glDeleteVertexArrays(1, &i->second);
	}

	m_GeometryMap.erase(iGeometryId);

	glfwMakeContextCurrent(PrevCtx);
}

// commands
void CGPUDriver::DrawCommandList() {
	if (m_CommandList.empty())
		return;

	glfwMakeContextCurrent(m_Context->active_window());

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

	for (auto i = m_Programs.begin(); i != m_Programs.end(); i++) {
		SProgramEntry Entry = i->second;
		
		glDetachShader(Entry.m_Id, Entry.m_VertexId);
		glDetachShader(Entry.m_Id, Entry.m_FragId);

		glDeleteShader(Entry.m_VertexId);
		glDeleteShader(Entry.m_FragId);
		
		glDeleteProgram(Entry.m_Id);
	}

	m_Programs.clear();
}

void CGPUDriver::LoadProgram(ultralight::ShaderType eType) {
	SProgramEntry Program = { };
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

	m_Programs[eType] = Program;
}

void CGPUDriver::SelectProgram(ultralight::ShaderType eType) {
	auto Program = m_Programs.find(eType);
	if (Program == m_Programs.end())
		return;

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

	SetUniform4fv("Scalar4", 2, &State.uniform_scalar[0]);
	SetUniform4fv("Vector", 8, &State.uniform_vector[0].x);
	SetUniform1ui("ClipSize", State.clip_size);
	SetUniformMatrix4fv("Clip", 8, &State.clip[0].data[0]);
}

void CGPUDriver::SetUniform1ui(const char* cName, GLuint iValue) { glUniform1ui(glGetUniformLocation(m_CurrentProgram, cName), iValue); }
void CGPUDriver::SetUniform1f(const char* cName, float fValue) { glUniform1f(glGetUniformLocation(m_CurrentProgram, cName), fValue); }
void CGPUDriver::SetUniform1fv(const char* cName, size_t iCount, const float* pValue) { glUniform1fv(glGetUniformLocation(m_CurrentProgram, cName), static_cast<GLsizei>(iCount), pValue ); }
void CGPUDriver::SetUniform4f(const char* cName, const float aValue[4]) { glUniform4f(glGetUniformLocation(m_CurrentProgram, cName), aValue[0], aValue[1], aValue[2], aValue[3]); }
void CGPUDriver::SetUniform4fv(const char* cName, size_t iCount, const float* pValue) { glUniform4fv(glGetUniformLocation(m_CurrentProgram, cName), static_cast<GLsizei>(iCount), pValue); }
void CGPUDriver::SetUniformMatrix4fv(const char* cName, size_t iCount, const float* pValue) { glUniformMatrix4fv(glGetUniformLocation(m_CurrentProgram, cName), static_cast<GLsizei>(iCount), false, pValue); }

// fbo
void CGPUDriver::CreateFBOTexture(GLuint iTextureId, ultralight::RefPtr<ultralight::Bitmap> Bitmap) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	STextureEntry& Entry = m_TextureMap[iTextureId];
	Entry.m_Width = Bitmap->width();
	Entry.m_Height = Bitmap->height();

	glGenTextures(1, &Entry.m_TextureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Entry.m_TextureId);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Entry.m_Width, Entry.m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

	glGenerateMipmap(GL_TEXTURE_2D);
}

void CGPUDriver::CreateFBOIfNeededForActiveContext(GLuint iRenderBufferId) {
	if (iRenderBufferId == 0)
		return;

	auto i = m_RenderBufferMap.find(iRenderBufferId);
	if (i == m_RenderBufferMap.end())
		return;

	SRenderBufferEntry& RenderBuffer = i->second;

	if (RenderBuffer.m_FBOMap.find(glfwGetCurrentContext()) != RenderBuffer.m_FBOMap.end())
		return;

	GLuint iFBOId = RenderBuffer.m_FBOMap[glfwGetCurrentContext()];

	glGenFramebuffers(1, &iFBOId);
	glBindFramebuffer(GL_FRAMEBUFFER, iFBOId);

	STextureEntry& Texture = m_TextureMap[RenderBuffer.m_TextureId];

	glBindTexture(GL_TEXTURE_2D, Texture.m_TextureId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture.m_TextureId, 0);

	GLenum aBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, aBuffers);

	GLenum eResult = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (eResult != GL_FRAMEBUFFER_COMPLETE)
		return;
}

// vao
void CGPUDriver::CreateVAOIfNeededForActiveContext(GLuint iGeometryId) {
	auto i = m_GeometryMap.find(iGeometryId);
	if (i == m_GeometryMap.end())
		return;

	SGeometryEntry& Geometry = i->second;


	if (Geometry.m_VAOMap.find(glfwGetCurrentContext()) != Geometry.m_VAOMap.end())
		return;

	GLuint iVAOId;

	glGenVertexArrays(1, &iVAOId);
	glBindVertexArray(iVAOId);

	glBindBuffer(GL_ARRAY_BUFFER, Geometry.m_VBOVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Geometry.m_VBOIndices);

	if (Geometry.m_VertexFormat == ultralight::VertexBufferFormat::kVertexBufferFormat_2f_4ub_2f_2f_28f) {
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
	else if (Geometry.m_VertexFormat == ultralight::VertexBufferFormat::kVertexBufferFormat_2f_4ub_2f) {
		GLsizei iStride = 20;

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(0));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, iStride, reinterpret_cast<GLvoid*>(8));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, iStride, reinterpret_cast<GLvoid*>(12));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
	else {
		return;
	}

	glBindVertexArray(0);

	Geometry.m_VAOMap[glfwGetCurrentContext()] = iVAOId;
}

// misc

void CGPUDriver::ResolveIfNeeded(GLuint) {
	return;
}

void CGPUDriver::SetViewport(unsigned int iWidth, unsigned int iHeight) { glViewport(0, 0, iWidth, iHeight); }

ultralight::Matrix CGPUDriver::ApplyProjection(const ultralight::Matrix4x4& Transform, float fWidth, float fHeight, bool bFlipY) {
	ultralight::Matrix TransformMtx;
	TransformMtx.Set(Transform);

	ultralight::Matrix Result;
	Result.SetOrthographicProjection(fWidth, fHeight, bFlipY);
	Result.Transform(TransformMtx);
	return Result;
}

