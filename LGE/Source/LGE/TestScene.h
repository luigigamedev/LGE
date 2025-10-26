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
		Texture* m_BoxTexture = nullptr;
		Texture* m_FaceTexture = nullptr;
		Texture* m_StoneTexture = nullptr;
		Texture* m_SmoothStoneTexture = nullptr;
		Texture* m_DirtTexture = nullptr;

		// Shaders
		ShaderProgram* m_UnlitTextureShaderProgram = nullptr;

		// Vertex
		VertexBuffer* m_QuadVb = nullptr;
		VertexBuffer* m_CubeVb = nullptr;
		BufferLayout* m_CommonBufferLayout = nullptr;

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
		glm::mat4 m_DirtBlockModel;
	public:
		virtual void Update(const float deltaTime) override;
		void MoveFlyCamera(const glm::vec2 moveInput, const float deltaTime);
		void MoveFpsCamera(const glm::vec2 moveInput, const float deltaTime);
		glm::mat4 ManuallyCreateCameraViewMatrix() const;
		virtual void Render() override;
	};
}
