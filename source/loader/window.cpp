/*
	to-do:
		handle intiialization errors
*/

#include "window.h"
#include "globals.h"
#include "menu.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int iWindowPosX = 0;
int iWindowPosY = 0;

GLFWwindow* GLWindow;

void Window::MoveWindow(int iDeltaX, int iDeltaY) {
	iWindowPosX = iWindowPosX + iDeltaX;
	iWindowPosY = iWindowPosY + iDeltaY;

	GLFWmonitor* GLMonitor = glfwGetWindowMonitor(GLWindow);

	glfwSetWindowMonitor(GLWindow, GLMonitor, iWindowPosX, iWindowPosY, Window::iSizeW, Window::iSizeH, 0);
}

void Window::Close() {
	glfwSetWindowShouldClose(GLWindow, 1);
}

bool Window::Create() {
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_DECORATED, 0);

	GLWindow = glfwCreateWindow(Window::iSizeW, Window::iSizeH, "sledge", NULL, NULL);
	if (!GLWindow)
		return false;

	glfwMakeContextCurrent(GLWindow);

	if (glewInit())
		return false;

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(GLWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");

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

	return true;
}