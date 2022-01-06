#include "GPUContext.h"
#include "GPUDriver.h"

#include "util/log.h"

CGPUContext::CGPUContext() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	glfwWindowHint(GLFW_VISIBLE, 0);
	
	m_Window = glfwCreateWindow(10, 10, "", NULL, NULL);
	if (!m_Window) {
		glfwTerminate();
		LogError("failed to create GLFW window for CGPUContext");
		assert(true);
	}
	
	glfwMakeContextCurrent(m_Window);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSwapInterval(0);

	m_Driver.reset(new CGPUDriver(this));
}