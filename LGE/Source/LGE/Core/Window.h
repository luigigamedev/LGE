#pragma once
 
#include <string>

struct GLFWwindow;

namespace LGE 
{
	class Window 
	{
	private:
		unsigned int m_Width;
		unsigned int m_Height;
		GLFWwindow* m_GlfwWindow = nullptr;
		double m_MouseX = 0.0;
		double m_MouseY = 0.0;
	public:
		Window(unsigned int width, unsigned int height, const std::string& title, bool fullscreen);
		~Window();

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

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