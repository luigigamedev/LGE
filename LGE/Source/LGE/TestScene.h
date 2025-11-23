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
	
	class LGE_API TestScene : public Scene
	{
	public:
		TestScene();
		~TestScene() override;
	private:
		// Textures
		Texture* m_SmoothStoneTexture = nullptr;
		
		// Shaders
		ShaderProgram* m_UnlitTextureShaderProgram = nullptr;
		ShaderProgram* m_UnlitColorShaderProgram = nullptr;
		ShaderProgram* m_MaterialsShaderProgram = nullptr;

		// Vertex
		VertexBuffer* m_QuadVb = nullptr;
		VertexBuffer* m_CubeVb = nullptr;
		BufferLayout* m_BufferLayout = nullptr;
		BufferLayout* m_CubeBufferLayout = nullptr;
		
		// Camera
		glm::vec3 m_CameraPos = glm::vec3(0.0f, 1.7f, 0.0f);
		float m_CameraYaw = -90.0f; // TODO: initialize from camera forward
		float m_CameraPitch = 0.0f;
		glm::vec3 m_CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);

		// Input
		bool m_FirstMouse = true;
		float m_LastMouseX = 0.0f;
		float m_LastMouseY = 0.0f;

		// Scene Setup
		glm::mat4 m_GroundModel;
		glm::mat4 m_LightCubeModel;
		glm::vec3 m_LightPos = glm::vec3(1.2f, 1.0f, 2.0f);
		glm::vec3 m_CubePos = glm::vec3(0.0f, 0.5f, 0.0f);
		float m_CubeYaw = 0.0f;
		glm::vec3 m_CubeScale = glm::vec3(1.0f);
	public:
		virtual void Update(const float deltaTime) override;
		void MoveFlyCamera(const glm::vec2 moveInput, const float deltaTime);
		void MoveFpsCamera(const glm::vec2 moveInput, const float deltaTime);
		virtual void Render() override;
		glm::mat4 BuildCameraViewMatrix() const;
	};
}
