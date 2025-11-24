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
		Texture* m_DiffuseMapTexture = nullptr;
		Texture* m_SpecularMapTexture = nullptr;

		// Shaders
		ShaderProgram* m_UnlitColorShaderProgram = nullptr;
		ShaderProgram* m_LightCasters1ShaderProgram = nullptr;

		// Vertex
		VertexBuffer* m_CubeVb = nullptr;
		BufferLayout* m_BufferLayout = nullptr;
		
		// Camera
		glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		float m_CameraYaw = -90.0f; // TODO: initialize from camera forward
		float m_CameraPitch = 0.0f;
		glm::vec3 m_CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);

		// Input
		bool m_FirstMouse = true;
		float m_LastMouseX = 0.0f;
		float m_LastMouseY = 0.0f;

		// Scene Setup
		glm::mat4 m_LightCubeModel;
		glm::vec3 m_LightPos = glm::vec3(1.2f, 1.0f, 2.0f);
		unsigned int m_CubeCount = 10;
		glm::vec3 m_CubesPos[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		float m_CubesAngle[10] = {
			0.0f,
			20.0f,
			40.0f,
			60.0f,
			80.0f,
			100.0f,
			120.0f,
			140.0f,
			160.0f,
			180.0f
		};
		glm::vec3 m_CubesRotateAxis = glm::vec3(1.0f, 0.3f, 0.5f);
		glm::vec3 m_CubesScale[10] = {
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f)
		};
	public:
		virtual void Update(const float deltaTime) override;
		void MoveFlyCamera(const glm::vec2 moveInput, const float deltaTime);
		void MoveFpsCamera(const glm::vec2 moveInput, const float deltaTime);
		virtual void Render() override;
		glm::mat4 BuildCameraViewMatrix() const;
	};
}
