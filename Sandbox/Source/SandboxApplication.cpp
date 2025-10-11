#include <iostream>

#include <LGE.h>

class SandboxApplication : public LGE::Application 
{
private:
	LGE::TestScene* m_TestScene = nullptr;

public:
	SandboxApplication()
	{
		std::cout << "[SandboxApplication] SandboxApplication(){" << '\n';

		m_TestScene = new LGE::TestScene();
		ActivateScene(m_TestScene);
	}

	~SandboxApplication() override
	{
		std::cout << "[SandboxApplication] ~SandboxApplication(){" << '\n';

		delete m_TestScene;

		std::cout << "[SandboxApplication] ~SandboxApplication()}" << '\n';
	}
};

LGE::Application* LGE::CreateApplication() 
{
	return new SandboxApplication();
}
