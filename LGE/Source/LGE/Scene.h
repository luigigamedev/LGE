#pragma once

#include "Core/Core.h"

namespace LGE
{
	class LGE_API Scene
	{
	public:
		Scene() {}
		virtual ~Scene() {}
	public:
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
	};
}
