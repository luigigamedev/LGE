#pragma once

#include "Core.h"

namespace LGE
{
	class Scene;
	class Window;

	constexpr unsigned int k_ScreenWidth = 1280;
	constexpr unsigned int k_ScreenHeight = 720;

	class LGE_API Application
	{
	private:
		Window* m_Window;
		Scene* m_ActiveScene;

	private:
		static Application* s_Instance;

	public:
		Application();
		virtual ~Application();
		void ActivateScene(Scene* scene);
		void Run();
		static Application& Get() { return *s_Instance; }
	};
}
