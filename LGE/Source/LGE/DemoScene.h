#pragma once

#include <glm/glm.hpp>

#include "Core/Core.h"
#include "Scene.h"

namespace LGE
{
	class BufferLayout;
	class ShaderProgram;
	class Texture;
	class VertexBuffer;

	struct Camera
	{
		glm::vec3 Pos = glm::vec3(0.0f);
		float Yaw = -90.0f; // TODO: initialize from forward; glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		float Pitch = 0.0f;
	};

	struct Input 
	{
		bool FirstMouse = true;
		glm::vec2 Mouse; // Init on first frame, controlled by FirstMouse flag 
		glm::vec2 MouseDelta = glm::vec2(0.0f);

		glm::vec2 Axis = glm::vec2(0.0f); // x: horizontal, y: vertical
	};

	class LGE_API DemoScene : public Scene
	{
	public:
		DemoScene();
		~DemoScene() override;
	private:
		// Buffer Layout and Meshes
		BufferLayout* m_BufferLayout = nullptr;
		VertexBuffer* m_QuadVb = nullptr;
		VertexBuffer* m_CubeVb = nullptr;

		// Shaders
		ShaderProgram* m_UnlitColorShader = nullptr;
		ShaderProgram* m_UnlitTextureShader = nullptr;

		// Textures
		Texture* m_SmoothStoneTexture = nullptr;

		// Player
		glm::vec3 m_PlayerPos = glm::vec3(0.0f, 0.0f, 0.0f); // Origin at the bottom (ground)
		float m_PlayerYaw = -90.0f; // TODO: initialize from forward; glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);

		// Camera
		Camera m_Camera;

		// Input
		Input m_Input;

		// House
		unsigned int m_CubeCount = 18;

		glm::vec3 m_CubesPos[18] = {
			// --- Front Facade ---
			glm::vec3(-4.5f, 1.5f, 0.0f), glm::vec3(4.5f, 1.5f, 0.0f), glm::vec3(0.0f, 2.6f, 0.0f),

			// --- Outer Perimeter ---
			glm::vec3(-8.0f, 1.5f, -8.0f), glm::vec3(8.0f, 1.5f, -8.0f), glm::vec3(0.0f, 1.5f, -16.0f),

			// --- Hallway Left ---
			glm::vec3(-2.0f, 1.5f, -6.0f),

			// --- Hallway Right ---
			glm::vec3(2.0f, 1.5f, -1.5f), glm::vec3(2.0f, 2.6f, -4.0f),
			glm::vec3(2.0f, 1.5f, -7.0f), glm::vec3(2.0f, 2.6f, -10.0f),
			glm::vec3(2.0f, 1.5f, -11.5f),

			// --- Back Dividers (Z = -12) ---
			glm::vec3(-3.5f, 1.5f, -12.0f), // Wall segment moved to the right (next to hall)
			glm::vec3(-6.5f, 2.6f, -12.0f), // Door Lintel moved to the far left (next to outer wall)
			glm::vec3(5.0f, 1.5f, -8.0f),
			glm::vec3(5.0f, 1.5f, -12.0f)
		};

		float m_CubesYaw[18] = { 0.0f };

		glm::vec3 m_CubesScale[18] = {
			glm::vec3(7.0f, 3.0f, 0.2f), glm::vec3(7.0f, 3.0f, 0.2f), glm::vec3(2.0f, 0.8f, 0.2f),
			glm::vec3(0.2f, 3.0f, 16.0f), glm::vec3(0.2f, 3.0f, 16.0f), glm::vec3(16.2f, 3.0f, 0.2f),
			glm::vec3(0.2f, 3.0f, 12.0f),
			glm::vec3(0.2f, 3.0f, 3.0f), glm::vec3(0.2f, 0.8f, 2.0f),
			glm::vec3(0.2f, 3.0f, 4.0f), glm::vec3(0.2f, 0.8f, 2.0f),
			glm::vec3(0.2f, 3.0f, 1.0f),
			glm::vec3(3.0f, 3.0f, 0.2f), // Wall
			glm::vec3(3.0f, 0.8f, 0.2f), // Lintel (3m wide)
			glm::vec3(6.0f, 3.0f, 0.2f),
			glm::vec3(6.0f, 3.0f, 0.2f)
		};
	public:
		virtual void Update(const float deltaTime) override;
		void ReadMouseInput();
		void ReadAxisInput();
		virtual void Render() override;
		glm::mat4 BuildCameraViewMatrix() const;
	};
}
