#include <iostream>

#include <LGE.h>

class SandboxApplication : public LGE::Application 
{
private:
	LGE::DemoScene* m_DemoScene = nullptr;

public:
	SandboxApplication()
	{
		std::cout << "[SandboxApplication] SandboxApplication(){" << '\n';

		m_DemoScene = new LGE::DemoScene();
		ActivateScene(m_DemoScene);
	}

	~SandboxApplication() override
	{
		std::cout << "[SandboxApplication] ~SandboxApplication(){" << '\n';

		delete m_DemoScene;

		std::cout << "[SandboxApplication] ~SandboxApplication()}" << '\n';
	}
};

LGE::Application* LGE::CreateApplication() 
{
	return new SandboxApplication();
}
