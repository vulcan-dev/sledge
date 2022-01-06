#pragma once
#pragma warning (push)
#pragma warning (disable: 4201)
#include <Ultralight/platform/GPUDriver.h>
#include <Ultralight/platform/Config.h>
#pragma warning (pop)

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class CGPUContext {
public:
	CGPUContext();
	virtual ~CGPUContext() {}

	ultralight::GPUDriver* driver() const { return m_Driver.get(); }

	ultralight::FaceWinding face_winding() const { return ultralight::FaceWinding::kFaceWinding_CounterClockwise; }

	void BeginDrawing() {}
	void EndDrawing() {}
	
	GLFWwindow* window() { return m_Window; }

	virtual void set_active_window(GLFWwindow* Window) { m_ActiveWindow = Window; };
	virtual GLFWwindow* active_window() { return m_ActiveWindow; }

private:
	GLFWwindow* m_Window;
	GLFWwindow* m_ActiveWindow = nullptr;
	std::unique_ptr<ultralight::GPUDriver> m_Driver;
};