#pragma once

#include <glm/glm.hpp>

#include "Core/Core.h"

namespace LGE
{
	class LGE_API Scene
	{
	protected:
		// OpenGL uses a right-handed coordinate system.
		// Forward is -Z (looking into the screen), Right is +X, Up is +Y.
		static constexpr glm::vec3 WORLD_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
		static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
		static constexpr glm::vec3 WORLD_FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);

		// NOTE: MAX_POINT_LIGHTS must match #define MAX_POINT_LIGHTS in shaders
		static constexpr int MAX_POINT_LIGHTS = 16;

	public:
		Scene() = default;
		virtual ~Scene() = default;

	public:
		virtual void Update(const float deltaTime) = 0;
		virtual void Render() = 0;
	};
}
