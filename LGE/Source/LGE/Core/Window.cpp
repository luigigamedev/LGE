#include "Window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace LGE 
{
	Window::Window(unsigned int width, unsigned int height, const std::string& title, bool fullscreen)
	{
		std::cout << "[Window] Window(){" << '\n';
		
		// Initialize Glfw
		if (!glfwInit())
		{
			std::cout << "[Window] ERROR: Failed initializing GLFW." << '\n';
			return;
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* primaryMonitorMode = glfwGetVideoMode(primaryMonitor);

		if (fullscreen)
		{
			width = primaryMonitorMode->width;
			height = primaryMonitorMode->height;
		}

		m_Width = width;
		m_Height = height;

		m_GlfwWindow = glfwCreateWindow(width, height, title.c_str(), fullscreen ? primaryMonitor : NULL, NULL);

		if (!m_GlfwWindow)
		{
			std::cout << "[Window] ERROR: Failed creating GLFW window." << '\n';
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_GlfwWindow);

		if (!fullscreen) 
		{
			glfwSetWindowPos(m_GlfwWindow, (primaryMonitorMode->width - width) / 2, (primaryMonitorMode->height - height) / 2);
		}
		
		// Hides and grabs the cursor, providing virtual and unlimited cursor movement
		glfwSetInputMode(m_GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "[Window] ERROR: Failed to initialize GLAD" << '\n';
			return;
		}

		std::cout << "[Window] " << glGetString(GL_VERSION) << '\n';

		// Configure global opengl state
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW); // Defines what counts as a front face, counter-clockwise vertex order (default)
		
		glViewport(0, 0, width, height);
	}
	
	Window::~Window()
	{
		std::cout << "[Window] ~Window(){" << '\n';

		glfwDestroyWindow(m_GlfwWindow);
		glfwTerminate();

		std::cout << "[Window] ~Window()}" << '\n';
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_GlfwWindow);
	}

	float Window::GetTime() const
	{
		return static_cast<float>(glfwGetTime());
	}

	void Window::ProcessInput()
	{
		glfwGetCursorPos(m_GlfwWindow, &m_MouseX, &m_MouseY);

		if (GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(m_GlfwWindow, true);
		}
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_GlfwWindow);
	}

	int Window::GetKey(int keyCode)
	{
		return glfwGetKey(m_GlfwWindow, keyCode);
	}

	double Window::GetMouseX() const
	{
		return m_MouseX;
	}

	double Window::GetMouseY() const
	{
		return m_MouseY;
	}
}
