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
		ShaderProgram* m_LitColorShader = nullptr;
		ShaderProgram* m_LitTextureShader = nullptr;

		// Player
		glm::vec3 m_PlayerPos = glm::vec3(0.0f, 0.0f, 5.0f); // Origin at the bottom (ground)
		float m_PlayerYaw = -90.0f; // TODO: initialize from forward; glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);

		// Camera
		Camera m_Camera;

		// Input
		Input m_Input;

		// Ground
		float m_GroundScale = 32.0f;
		Texture* m_GroundTexture = nullptr;
		float m_GroundTiling = 5.0f;
		float m_GroundSpecularIntensity = 0.05f;
		float m_GroundShininess = 8.0f;

		// Bound walls
		float m_BoundWallHeight = 2.0f;
		Texture* m_BoundWallTexture = nullptr;
		Texture* m_BoundWallSpecularTexture = nullptr;
		glm::vec2 m_BoundWallTiling = glm::vec2(16.0f, 2.0f);
		float m_BoundWallShininess = 32.0f;

		// Logl box (used in learnopengl chapter 2)
		glm::vec3 m_LoglBoxPos = glm::vec3(0.5f, 0.5f, 10.5f);
		Texture* m_LoglBoxTexture = nullptr;
		Texture* m_LoglBoxSpecularTexture = nullptr;
		float m_LoglBoxShininess = 32.0f;
	public:
		virtual void Update(const float deltaTime) override;
		void ReadMouseInput();
		void ReadAxisInput();
		virtual void Render() override;
		glm::mat4 BuildCameraViewMatrix() const;
	};
}
