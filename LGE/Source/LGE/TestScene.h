#pragma once

#include <glm/glm.hpp>

#include "Core/Core.h"
#include "Scene.h"

namespace LGE
{
	class Texture;
	class ShaderProgram;
	
	class LGE_API TestScene : public Scene
	{
	public:
		TestScene();
		~TestScene() override;
	private:
		Texture* m_BoxTexture = nullptr;
		Texture* m_FaceTexture = nullptr;
		Texture* m_StoneTexture = nullptr;
		Texture* m_SmoothStoneTexture = nullptr;
		Texture* m_DirtTexture = nullptr;
		ShaderProgram* m_UnlitTextureShaderProgram = nullptr;
		glm::vec3 m_CameraPos;
		float m_CameraYaw;
		float m_CameraPitch;
		glm::vec3 m_CameraForward;
		unsigned int m_QuadVbo;
		unsigned int m_CubeVbo;
		glm::mat4 m_GroundModel;
		glm::mat4 m_DirtBlockModel;
		bool m_FirstMouse = true;
		float m_LastMouseX;
		float m_LastMouseY;
	public:
		virtual void Update(const float deltaTime) override;
		void MoveFlyCamera(const glm::vec2 moveInput, const float deltaTime);
		void MoveFpsCamera(const glm::vec2 moveInput, const float deltaTime);
		glm::mat4 ManuallyCreateCameraViewMatrix() const;
		virtual void Render() override;
	};
}
