#pragma once

#include "Core/Core.h"
#include "Scene.h"

namespace LGE
{
	class LGE_API DemoScene : public Scene
	{
	public:
		DemoScene();
		~DemoScene() override;
	public:
		virtual void Update(const float deltaTime) override;
		virtual void Render() override;
	};
}
