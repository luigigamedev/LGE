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
		
		float Fov = 45.0f;
		float AspectRatio = 800.0f / 600.0f;
		float Near = 0.1f;
		float Far = 100.0f;

		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
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
		ShaderProgram* m_UnlitColorShader = nullptr;

		// Player
		glm::vec3 m_PlayerPos = glm::vec3(0.0f, 0.0f, 5.0f); // Origin at the bottom (ground)
		float m_PlayerYaw = -90.0f; // TODO: initialize from forward; glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);

		// Camera
		Camera m_Camera;

		// Input
		Input m_Input;

		// Ambient
		glm::vec3 m_AmbientColor = glm::vec3(0.02f, 0.02f, 0.03f); // nearly black, just enough to see shapes

		// Directional light
		glm::vec3 m_DirectionalLightDir = glm::vec3(-0.5f, -0.4f, -0.3f);
		glm::vec3 m_DirectionalLightColor = glm::vec3(0.3f, 0.32f, 0.38f); // dim cold blue-white

		// Point light
		glm::vec3 m_PointLightPos = glm::vec3(0.0f, 0.97f, 0.0f);
		// warm orange fire
		glm::vec3 m_PointLightColor = glm::vec3(1.0f, 0.55f, 0.1f);
		float m_PointLightLinear = 0.35f;   // ~13 unit range
		float m_PointLightQuadratic = 0.44f;

		// Ground
		float m_GroundScale = 32.0f;
		Texture* m_GroundTexture = nullptr;

		// Bound walls
		Texture* m_BoundWallTexture = nullptr;
		Texture* m_BoundWallSpecularTexture = nullptr;

		// Logl box (used in learnopengl chapter 2)
		Texture* m_LoglBoxTexture = nullptr;
		Texture* m_LoglBoxSpecularTexture = nullptr;
	public:
		virtual void Update(const float deltaTime) override;
		virtual void Render() override;
	private:
		void ReadMouseInput();
		void ReadAxisInput();
		glm::mat4 BuildCameraViewMatrix() const;
		void RenderGround() const;
		void RenderBoundWalls() const;
		void RenderLoglBox() const;
		void RenderTorch() const;
	};
}
