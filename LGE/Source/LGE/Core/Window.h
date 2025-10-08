#pragma once
 
#include <string>

struct GLFWwindow;

namespace LGE 
{
	class Window 
	{
	private:
		GLFWwindow* m_GlfwWindow;
		double m_MouseX;
		double m_MouseY;
	public:
		Window(const std::string& title = "Learn Game Engine",
			unsigned int width = 800,
			unsigned int height = 600);
		~Window();

		GLFWwindow* GetGlfwWindow() { return m_GlfwWindow; }

		bool ShouldClose() const;

		float GetTime() const;

		void ProcessInput();

		// Called at the end of the Application run loop
		void PollEvents();

		void SwapBuffers();

		int GetKey(int keyCode);

		double GetMouseX() const;
		double GetMouseY() const;
	};
}