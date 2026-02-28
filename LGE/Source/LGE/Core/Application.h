#pragma once

#include "Core.h"

namespace LGE
{
	class Scene;
	class Window;

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
		float GetTime() const;
		int GetKey(int keyCode) const;
		double GetMouseX() const;
		double GetMouseY() const;
		unsigned int GetWindowWidth() const;
		unsigned int GetWindowHeight() const;
		static Application& Get() { return *s_Instance; }
	};
}
