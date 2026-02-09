#include "DemoScene.h"

#include <iostream>

namespace LGE
{
	DemoScene::DemoScene()
	{
		std::cout << "[DemoScene] DemoScene(){" << '\n';
	}

	DemoScene::~DemoScene()
	{
		std::cout << "[DemoScene] ~DemoScene(){" << '\n';

		std::cout << "[DemoScene] ~DemoScene()}" << '\n';
	}

	void DemoScene::Update(const float deltaTime)
	{
		
	}

	void DemoScene::Render()
	{
		
	}
}
