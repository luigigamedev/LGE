#pragma once

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
		~TestScene();
	private:
		Texture* m_BoxTexture;
		Texture* m_FaceTexture;
		ShaderProgram* m_BasicShaderProgram;
	public:
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
	};
}
