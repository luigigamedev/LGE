#pragma once

#include <glm/glm.hpp>

#include "Core/Core.h"

namespace LGE
{
	class LGE_API Scene
	{
	protected:
		static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

	public:
		Scene() = default;
		virtual ~Scene() = default;

	public:
		virtual void Update(const float deltaTime) = 0;
		virtual void Render() = 0;
	};
}
