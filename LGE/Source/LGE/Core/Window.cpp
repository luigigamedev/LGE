#include "Window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace LGE 
{
	Window::Window(const std::string& title, unsigned int width, unsigned int height)
		: m_GlfwWindow(nullptr)
	{
		std::cout << "[Window] Window(){" << '\n';
		
		// Initialize Glfw
		if (!glfwInit())
		{
			std::cout << "[Window] ERROR: Failed initializing GLFW." << '\n';
			return;
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_GlfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (!m_GlfwWindow)
		{
			std::cout << "[Window] ERROR: Failed creating GLFW window." << '\n';
			glfwTerminate();
			return;
		}

		// Initialize Gl Context
		glfwMakeContextCurrent(m_GlfwWindow);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "[Window] ERROR: Failed to initialize GLAD" << '\n';
			return;
		}

		std::cout << "[Window] " << glGetString(GL_VERSION) << '\n';

		glfwSwapInterval(1); // Enable vsync

		glEnable(GL_DEPTH_TEST);
		
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
}
