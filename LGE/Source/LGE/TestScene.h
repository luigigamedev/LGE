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
		ShaderProgram* m_BasicShaderProgram = nullptr;
		glm::mat4 m_QuadMat;
	public:
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
	};
}
