#include <iostream>

#include <LGE.h>

class SandboxApplication : public LGE::Application 
{
private:
	LGE::DemoScene* m_DemoScene = nullptr;

public:
	SandboxApplication()
	{
		m_DemoScene = new LGE::DemoScene();
		ActivateScene(m_DemoScene);
	}

	~SandboxApplication() override
	{
		delete m_DemoScene;
	}
};

LGE::Application* LGE::CreateApplication() 
{
	return new SandboxApplication();
}
