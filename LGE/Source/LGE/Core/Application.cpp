#include "Application.h"

#include <iostream>

#include <glad/glad.h>
// #include "imgui/imgui.h"
// #include "imgui/imgui_impl_glfw.h"
// #include "imgui/imgui_impl_opengl3.h"

#include "Scene.h"
#include "Window.h"

namespace LGE
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Window(nullptr), m_ActiveScene(nullptr)
	{
		std::cout << "[Application] Application(){" << '\n';

		// TODO: Assert there is only one instance of Application
		s_Instance = this;

		m_Window = new Window();
		// TODO: Move graphics context away from window
	}

	Application::~Application()
	{
		std::cout << "[Application] ~Application(){" << '\n';

		delete m_Window;

		std::cout << "[Application] ~Application()}" << '\n';
	}

	void Application::ActivateScene(Scene* scene)
	{
		m_ActiveScene = scene;
	}

	void Application::Run()
	{
		float time = m_Window->GetTime();

		// ImGui ----------------------------------------------------------------------------------
		// ImGui::CreateContext();
		// ImGuiIO& imGuiIo = ImGui::GetIO();
		// (void)imGuiIo;
		// imGuiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		// imGuiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
		// ImGui::StyleColorsDark();
		// // Setup Platform/Renderer backends
		// ImGui_ImplGlfw_InitForOpenGL(m_Window->GetGlfwWindow(), true);
		// const char* glsl_version = "#version 130";
		// ImGui_ImplOpenGL3_Init(glsl_version);
		// // Our state
		// bool show_demo_window = true;
		// bool show_another_window = false;
		// ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		// ----------------------------------------------------------------------------------------

		while (!m_Window->ShouldClose())
		{
			float nowTime = m_Window->GetTime();
			float deltaTime = nowTime - time;
			time = nowTime;

			// Logic/Simulation update
			if (m_ActiveScene != nullptr)
			{
				m_ActiveScene->Update(deltaTime);
			}

			// Render here 
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // LearnOpenGL example color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (m_ActiveScene != nullptr)
			{
				m_ActiveScene->Render();

				// m_ActiveScene->ImGuiRender();
			}

			// Window swap buffers (vsync) and poll events
			m_Window->SwapBuffers();
			m_Window->PollEvents();
		}
	}

	float Application::GetTime() const
	{
		return m_Window->GetTime();
	}

	int Application::GetKey(int keyCode) const
	{
		return m_Window->GetKey(keyCode);
	}
}
