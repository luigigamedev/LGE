#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Core/Core.h"
#include "Scene.h"

namespace LGE
{
	class IndexBuffer;
	class ShaderProgram;
	class Texture;
	class VertexArray;
	class VertexBuffer;

	struct Transform 
	{
		glm::vec3 Pos;
		glm::vec3 Rot; // Euler angles in degrees (x=pitch, y=yaw, z=roll)
		glm::vec3 Scale;
	};

	struct Camera
	{
		glm::vec3 Pos = glm::vec3(0.0f);
		float Yaw = -90.0f; // TODO: initialize from forward; glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		float Pitch = 0.0f;
		
		float Fov = 45.0f;
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

	struct Torch 
	{
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 color = glm::vec3(1.0f);
	};

	struct Campfire
	{
		Transform Transform = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f) };
	};

	class LGE_API DemoScene : public Scene
	{
	public:
		DemoScene();
		~DemoScene() override;
	private:
		// Meshes
		VertexArray* m_QuadVao = nullptr;
		VertexArray* m_CubeVao = nullptr;
		IndexBuffer* m_QuadIb = nullptr;
		IndexBuffer* m_CubeIb = nullptr;
		VertexBuffer* m_QuadVb = nullptr;
		VertexBuffer* m_CubeVb = nullptr;

		// Shaders
		ShaderProgram* m_LitColorShader = nullptr;
		ShaderProgram* m_LitTextureShader = nullptr;
		ShaderProgram* m_UnlitColorShader = nullptr;

		// Player
		glm::vec3 m_PlayerPos = glm::vec3(0.0f, 0.0f, 10.0f); // Origin at the bottom (ground)
		float m_PlayerYaw = -90.0f; // TODO: initialize from forward; glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);

		// Camera
		Camera m_Camera;

		// Input
		Input m_Input;

		// Ambient
		glm::vec3 m_AmbientColor = glm::vec3(0.01f, 0.01f, 0.015f); // darker, almost nothing

		// Directional light
		glm::vec3 m_DirectionalLightDir = glm::vec3(-0.5f, -0.4f, -0.3f);
		glm::vec3 m_DirectionalLightColor = glm::vec3(0.06f, 0.07f, 0.09f); // nearly off, just a cold hint

		// Ground
		float m_GroundScale = 32.0f;
		Texture* m_GroundTexture = nullptr;

		// Bound walls
		Texture* m_BoundWallTexture = nullptr;
		Texture* m_BoundWallSpecularTexture = nullptr;

		// Logl box (used in learnopengl chapter 2)
		Texture* m_LoglBoxTexture = nullptr;
		Texture* m_LoglBoxSpecularTexture = nullptr;

		// Torches
		std::vector<Torch> m_Torches;
		glm::vec3 m_TorchOrangeColor = glm::vec3(1.0f, 0.55f, 0.1f);  // fire
		glm::vec3 m_TorchBlueColor = glm::vec3(0.3f, 0.6f, 1.0f);  // cold magic
		glm::vec3 m_TorchGreenColor = glm::vec3(0.2f, 1.0f, 0.3f);  // poison/nature
		glm::vec3 m_TorchPurpleColor = glm::vec3(0.7f, 0.2f, 1.0f);  // arcane

		// Campfire
		Campfire m_Campfire = { { glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f), glm::vec3(1.0f) } };
	public:
		virtual void Update(const float deltaTime) override;
		virtual void Render() override;
	private:
		void ReadMouseInput();
		void ReadAxisInput();
		glm::mat4 BuildTransformModelMatrix(const Transform& t) const;
		glm::mat4 BuildCameraViewMatrix() const;
		void SetLightingUniforms(ShaderProgram* shader) const;
		void SetCameraUniforms(ShaderProgram* shader) const;
		void RenderGround() const;
		void RenderBoundWalls() const;
		void RenderLoglBoxes() const;
		void RenderTorches() const;

		void SetCampfireLightUniforms(ShaderProgram* shader, int* i) const;
		void RenderCampfire() const;
	};
}
