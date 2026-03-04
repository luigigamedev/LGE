#include "Application.h"

#include <iostream>

#include <glad/glad.h>

#include "Scene.h"
#include "Window.h"

namespace LGE
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Window(nullptr), m_ActiveScene(nullptr)
	{
		// TODO: Assert there is only one instance of Application
		s_Instance = this;

		m_Window = new Window(1280, 720, "LGE", false);
		// TODO: Move graphics context away from window
		
		std::cout << "[INFO] [Application] Created" << '\n';
	}

	Application::~Application()
	{
		std::cout << "[INFO] [Application] Destroyed" << '\n';

		delete m_Window;
	}

	void Application::ActivateScene(Scene* scene)
	{
		m_ActiveScene = scene;
	}

	void Application::Run()
	{
		float time = m_Window->GetTime();

		while (!m_Window->ShouldClose())
		{
			// Frame time logic
			float nowTime = m_Window->GetTime();
			float deltaTime = nowTime - time;
			time = nowTime;
			
			m_Window->ProcessInput();

			// Simulation update
			if (m_ActiveScene != nullptr)
			{
				m_ActiveScene->Update(deltaTime);
			}

			// Render 
			// glClearColor(0.7f, 0.8f, 0.98f, 1.0f); 
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (m_ActiveScene != nullptr)
			{
				m_ActiveScene->Render();
			}

			// Window swap buffers and poll events
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

	double Application::GetMouseX() const
	{
		return m_Window->GetMouseX();
	}

	double Application::GetMouseY() const
	{
		return m_Window->GetMouseY();
	}

	unsigned int Application::GetWindowWidth() const
	{
		return m_Window->GetWidth();
	}

	unsigned int Application::GetWindowHeight() const
	{
		return m_Window->GetHeight();
	}
}
