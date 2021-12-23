#include "ui/window.h"
#include "ui/menu.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GLFWwindow* GLWindow;

void Window::MoveWindow(int iX, int iY) {
	Window::iPosX = Window::iPosX + iX;
	Window::iPosY = Window::iPosY + iY;

	GLFWmonitor* GLMonitor = glfwGetWindowMonitor(GLWindow);

	glfwSetWindowMonitor(GLWindow, GLMonitor, Window::iPosX, Window::iPosY, Window::iSizeW, Window::iSizeH, 0);
}

void Window::Close() {
	glfwSetWindowShouldClose(GLWindow, 1);
}

void Window::Create() {
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_DECORATED, 0);

	GLWindow = glfwCreateWindow(Window::iSizeW, Window::iSizeH, "sledge", NULL, NULL);
	if (!GLWindow)
		return;

	glfwMakeContextCurrent(GLWindow);

	if (glewInit())
		return;

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(GLWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGui::GetIO().IniFilename = NULL;

	while (!glfwWindowShouldClose(GLWindow)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Menu::Draw();

		ImGui::Render();

		glViewport(0, 0, Window::iSizeW, Window::iSizeH);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(GLWindow);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(GLWindow);
	glfwTerminate();
}