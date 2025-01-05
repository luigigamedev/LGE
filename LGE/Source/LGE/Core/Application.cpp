#include "Application.h"

#include <iostream>

#include <glad/glad.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
		ImGui::CreateContext();
		ImGuiIO& imGuiIo = ImGui::GetIO();
		(void)imGuiIo;
		imGuiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		imGuiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
		ImGui::StyleColorsDark();
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_Window->GetGlfwWindow(), true);
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);
		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			if (m_ActiveScene != nullptr)
			{
				m_ActiveScene->Render();

				m_ActiveScene->ImGuiRender();
			}

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))
					// Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imGuiIo.Framerate, imGuiIo.Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Window swap buffers and poll events
			m_Window->Update();
		}
	}
}
