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
		ShaderProgram* m_LitColorShaderProgram = nullptr;

		// Vertex
		VertexBuffer* m_QuadVb = nullptr;
		VertexBuffer* m_CubeVb = nullptr;
		BufferLayout* m_BufferLayout = nullptr;
		
		// Camera
		glm::vec3 m_CameraPos;
		float m_CameraYaw;
		float m_CameraPitch;
		glm::vec3 m_CameraForward;

		// Input
		bool m_FirstMouse = true;
		float m_LastMouseX;
		float m_LastMouseY;

		// Transforms
		glm::mat4 m_GroundModel;
		glm::mat4 m_LightCubeModel;
		glm::mat4 m_CubeModel;
	public:
		virtual void Update(const float deltaTime) override;
		void MoveFlyCamera(const glm::vec2 moveInput, const float deltaTime);
		void MoveFpsCamera(const glm::vec2 moveInput, const float deltaTime);
		virtual void Render() override;
		glm::mat4 BuildCameraViewMatrix() const;
	};
}
