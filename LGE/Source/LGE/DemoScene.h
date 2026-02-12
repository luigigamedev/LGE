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
			// --- Front Facade (Z = 0) ---
			glm::vec3(-4.5f, 1.5f,  0.0f), glm::vec3(4.5f, 1.5f,  0.0f), glm::vec3(0.0f, 2.6f,  0.0f),

			// --- Outer Perimeter ---
			glm::vec3(-8.0f, 1.5f, -8.0f), glm::vec3(8.0f, 1.5f, -8.0f), glm::vec3(0.0f, 1.5f, -16.0f),

			// --- Hallway Left (With Doorway) ---
			glm::vec3(-2.0f, 1.5f, -2.0f), // Segment 1
			glm::vec3(-2.0f, 2.6f, -5.0f), // Left Room Door Lintel
			glm::vec3(-2.0f, 1.5f, -9.0f), // Segment 2

			// --- Hallway Right (Two Doors) ---
			glm::vec3(2.0f, 1.5f, -1.5f), // Segment 1
			glm::vec3(2.0f, 2.6f, -4.0f), // Door 1 Lintel
			glm::vec3(2.0f, 1.5f, -7.0f), // Segment 2
			glm::vec3(2.0f, 2.6f, -10.0f),// Door 2 Lintel
			glm::vec3(2.0f, 1.5f, -11.5f),// Segment 3

			// --- Dividers & Arch ---
			glm::vec3(-5.0f, 1.5f, -12.0f), glm::vec3(5.0f, 1.5f, -8.0f),
			glm::vec3(5.0f, 1.5f, -12.0f), glm::vec3(0.0f, 2.6f, -12.0f)
		};

		float m_CubesYaw[18] = {
			90.0f, 90.0f, 90.0f, 0.0f, 0.0f, 90.0f, // Shell
			0.0f, 0.0f, 0.0f,                       // Hall Left
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f,           // Hall Right
			90.0f, 90.0f, 90.0f, 90.0f              // Dividers
		};

		glm::vec3 m_CubesScale[18] = {
			glm::vec3(0.2f, 3.0f, 7.0f), glm::vec3(0.2f, 3.0f, 7.0f), glm::vec3(0.2f, 0.8f, 2.0f),
			glm::vec3(0.2f, 3.0f, 16.0f), glm::vec3(0.2f, 3.0f, 16.0f), glm::vec3(0.2f, 3.0f, 16.2f),

			glm::vec3(0.2f, 3.0f, 4.0f), glm::vec3(0.2f, 0.8f, 2.0f), glm::vec3(0.2f, 3.0f, 6.0f),

			glm::vec3(0.2f, 3.0f, 3.0f), glm::vec3(0.2f, 0.8f, 2.0f),
			glm::vec3(0.2f, 3.0f, 4.0f), glm::vec3(0.2f, 0.8f, 2.0f),
			glm::vec3(0.2f, 3.0f, 1.0f),

			glm::vec3(0.2f, 3.0f, 6.0f), glm::vec3(0.2f, 3.0f, 6.0f),
			glm::vec3(0.2f, 3.0f, 6.0f), glm::vec3(0.2f, 0.8f, 4.0f)
		};
	public:
		virtual void Update(const float deltaTime) override;
		void ReadMouseInput();
		void ReadAxisInput();
		virtual void Render() override;
		glm::mat4 BuildCameraViewMatrix() const;
	};
}
