#pragma once

#ifdef LGE_PLATFORM_WINDOWS

namespace LGE
{
	Application* CreateApplication();
}

int main(int argc, char** argv)
{
	auto app = LGE::CreateApplication();
	app->Run();
	delete app;
}

#endif
