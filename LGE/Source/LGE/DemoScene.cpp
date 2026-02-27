#include "DemoScene.h"

#include <iostream>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Included here only for input. Ideally all glfw should be handled only by Window class

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Application.h"
#include "Rendering/BufferLayout.h"
#include "Rendering/Meshes.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Texture.h"
#include "Rendering/VertexBuffer.h"
#include "Shaders/LitColorShader.h"
#include "Shaders/LitTextureShader.h"
#include "Shaders/UnlitColorShader.h"

namespace LGE
{
	DemoScene::DemoScene()
	{
		m_BufferLayout = new BufferLayout();
		m_BufferLayout->PushFloat3(); // position attribute
		m_BufferLayout->PushFloat3(); // normal attribute
		m_BufferLayout->PushFloat2(); // tex coord attribute

		m_QuadVb = new VertexBuffer(Meshes::QUAD, sizeof(Meshes::QUAD));
		m_CubeVb = new VertexBuffer(Meshes::CUBE, sizeof(Meshes::CUBE));

		m_LitColorShader = new ShaderProgram(Shaders::LitColor::VERTEX, Shaders::LitColor::FRAGMENT);
		m_LitTextureShader = new ShaderProgram(Shaders::LitTexture::VERTEX, Shaders::LitTexture::FRAGMENT);
		m_UnlitColorShader = new ShaderProgram(Shaders::UnlitColor::VERTEX, Shaders::UnlitColor::FRAGMENT);

		m_Camera.Pos = glm::vec3(0.0f, 1.7f, 0.0f);

		m_GroundTexture = new Texture("Resources/Textures/Ground037/Ground037_Color.jpg", true);

		m_BoundWallTexture = new Texture("Resources/Textures/BrickWall2/brick_wall2-diff.tga", true);
		m_BoundWallSpecularTexture = new Texture("Resources/Textures/BrickWall2/brick_wall2-spec.tga", true);

		m_LoglBoxTexture = new Texture("Resources/Textures/LearnOpenGL/container2.png", false);
		m_LoglBoxSpecularTexture = new Texture("Resources/Textures/LearnOpenGL/container2_specular.png", false);

		m_Torches.reserve(8);
		m_Torches.push_back({ glm::vec3(8.0f, 0.0f,  8.0f), m_TorchOrangeColor });
		m_Torches.push_back({ glm::vec3(-8.0f, 0.0f,  8.0f), m_TorchBlueColor });
		m_Torches.push_back({ glm::vec3(8.0f, 0.0f, -8.0f), m_TorchGreenColor });
		m_Torches.push_back({ glm::vec3(-8.0f, 0.0f, -8.0f), m_TorchPurpleColor });
		m_Torches.push_back({ glm::vec3(0.0f, 0.0f,  14.5f), m_TorchOrangeColor });
		m_Torches.push_back({ glm::vec3(0.0f, 0.0f, -14.5f), m_TorchBlueColor });
		m_Torches.push_back({ glm::vec3(14.5f, 0.0f, 0.0f),  m_TorchGreenColor });
		m_Torches.push_back({ glm::vec3(-14.5f, 0.0f, 0.0f),  m_TorchPurpleColor });
	}

	DemoScene::~DemoScene()
	{
		delete m_BufferLayout;

		delete m_QuadVb;
		delete m_CubeVb;

		delete m_LitColorShader;
		delete m_LitTextureShader;
		delete m_UnlitColorShader;

		delete m_GroundTexture;

		delete m_BoundWallTexture;
		delete m_BoundWallSpecularTexture;

		delete m_LoglBoxTexture;
		delete m_LoglBoxSpecularTexture;
	}

	void DemoScene::Update(const float deltaTime)
	{
		ReadMouseInput();

		ReadAxisInput();

		constexpr float sensitivity = 0.1f;

		m_PlayerYaw += m_Input.MouseDelta.x * sensitivity;
		glm::vec3 playerForward = glm::normalize(glm::vec3(cos(glm::radians(m_PlayerYaw)), 0.0f, sin(glm::radians(m_PlayerYaw))));
		constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
		glm::vec3 playerRight = glm::normalize(glm::cross(playerForward, worldUp));

		if (m_Input.Axis != glm::vec2(0.0f))
		{
			constexpr float playerSpeed = 5.0f;
			const glm::vec3 playerVel = (playerForward * m_Input.Axis.y + playerRight * m_Input.Axis.x) * playerSpeed;
			m_PlayerPos += playerVel * deltaTime;
		}

		m_Camera.Yaw += m_Input.MouseDelta.x * sensitivity;

		m_Camera.Pitch -= m_Input.MouseDelta.y * sensitivity;
		if (m_Camera.Pitch > 89.0f)
			m_Camera.Pitch = 89.0f;
		if (m_Camera.Pitch < -89.0f)
			m_Camera.Pitch = -89.0f;

		constexpr float playerHeight = 1.7f;
		m_Camera.Pos = m_PlayerPos + glm::vec3(0.0f, playerHeight, 0.0f);

		// View and projection -----------------------------------------------------------------------------------------
		m_Camera.ViewMatrix = BuildCameraViewMatrix();
		m_Camera.ProjectionMatrix = glm::perspective(glm::radians(m_Camera.Fov), m_Camera.AspectRatio, m_Camera.Near, m_Camera.Far);
	}

	void DemoScene::ReadAxisInput()
	{
		m_Input.Axis = glm::vec2(0.0f);

		if (Application::Get().GetKey(GLFW_KEY_W) == 1)
			m_Input.Axis.y = 1.0f;
		else if (Application::Get().GetKey(GLFW_KEY_S) == 1)
			m_Input.Axis.y = -1.0f;
		if (Application::Get().GetKey(GLFW_KEY_D) == 1)
			m_Input.Axis.x = 1.0f;
		else if (Application::Get().GetKey(GLFW_KEY_A) == 1)
			m_Input.Axis.x = -1.0f;
	}

	void DemoScene::ReadMouseInput()
	{
		const float newMouseX = static_cast<float>(Application::Get().GetMouseX());
		const float newMouseY = static_cast<float>(Application::Get().GetMouseY());

		if (m_Input.FirstMouse)
		{
			m_Input.Mouse.x = newMouseX;
			m_Input.Mouse.y = newMouseY;
			m_Input.FirstMouse = false;
		}

		m_Input.MouseDelta.x = newMouseX - m_Input.Mouse.x;
		m_Input.MouseDelta.y = newMouseY - m_Input.Mouse.y;
		m_Input.Mouse.x = newMouseX;
		m_Input.Mouse.y = newMouseY;
	}

	glm::mat4 DemoScene::BuildCameraViewMatrix() const
	{
		// -------------------------------------------------------------------------------------------------------------
		glm::vec3 cameraForward = glm::vec3(0.0f);
		cameraForward.x = cos(glm::radians(m_Camera.Yaw)) * cos(glm::radians(m_Camera.Pitch));
		cameraForward.y = sin(glm::radians(m_Camera.Pitch));
		cameraForward.z = sin(glm::radians(m_Camera.Yaw)) * cos(glm::radians(m_Camera.Pitch));
		cameraForward = glm::normalize(cameraForward);
		// -------------------------------------------------------------------------------------------------------------

		glm::mat4 cameraView;

		// 1. Position = known
		// 2. Calculate cameraDirection
		glm::vec3 target = m_Camera.Pos + cameraForward;
		glm::vec3 zaxis = glm::normalize(m_Camera.Pos - target);
		// 3. Get positive right axis vector
		constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
		glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
		// 4. Calculate camera up vector
		glm::vec3 yaxis = glm::cross(zaxis, xaxis);

		// Create translation and rotation matrix
		// In glm we access elements as mat[col][row] due to column-major layout
		glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
		translation[3][0] = -m_Camera.Pos.x; // Fourth column, first row
		translation[3][1] = -m_Camera.Pos.y;
		translation[3][2] = -m_Camera.Pos.z;
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = xaxis.x; // First column, first row
		rotation[1][0] = xaxis.y;
		rotation[2][0] = xaxis.z;
		rotation[0][1] = yaxis.x; // First column, second row
		rotation[1][1] = yaxis.y;
		rotation[2][1] = yaxis.z;
		rotation[0][2] = zaxis.x; // First column, third row
		rotation[1][2] = zaxis.y;
		rotation[2][2] = zaxis.z;

		// Return lookAt matrix as combination of translation and rotation matrix
		cameraView = rotation * translation;

		return cameraView;
	}

	void DemoScene::SetLightingUniforms(ShaderProgram* shader) const
	{
		shader->SetUniform3f("u_AmbientColor", m_AmbientColor.x, m_AmbientColor.y, m_AmbientColor.z);

		shader->SetUniform3f("u_DirectionalLight.dir", m_DirectionalLightDir.x, m_DirectionalLightDir.y, m_DirectionalLightDir.z);
		shader->SetUniform3f("u_DirectionalLight.color", m_DirectionalLightColor.x, m_DirectionalLightColor.y, m_DirectionalLightColor.z);

		// TODO: Replace with point lights
		for (int i = 0; i < m_Torches.size(); i++)
		{
			char name[64];

			snprintf(name, sizeof(name), "u_PointLights[%d].pos", i);
			glm::vec3 pos = m_Torches[i].pos + glm::vec3(0.0f, 0.97f, 0.0f);
			shader->SetUniform3f(name, pos.x, pos.y, pos.z);

			snprintf(name, sizeof(name), "u_PointLights[%d].color", i);
			shader->SetUniform3f(name, m_Torches[i].color.x, m_Torches[i].color.y, m_Torches[i].color.z);

			snprintf(name, sizeof(name), "u_PointLights[%d].linear", i);
			shader->SetUniform1f(name, 0.35f);

			snprintf(name, sizeof(name), "u_PointLights[%d].quadratic", i);
			shader->SetUniform1f(name, 0.44f);
		}

		shader->SetUniform1i("u_PointLightCount", m_Torches.size());
	}

	void DemoScene::Render()
	{
		RenderGround();
		RenderBoundWalls();
		RenderLoglBoxes();
		RenderTorches();
	}

	void DemoScene::RenderGround() const
	{
		constexpr float tiling = 5.0f;
		constexpr float specularIntensity = 0.05f;
		constexpr float shininess = 8.0f;

		m_LitTextureShader->Bind();

		// Camera
		m_LitTextureShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitTextureShader->SetUniformMatrix4f("u_View", m_Camera.ViewMatrix);
		m_LitTextureShader->SetUniformMatrix4f("u_Projection", m_Camera.ProjectionMatrix);

		// Lights
		SetLightingUniforms(m_LitTextureShader);

		// Material
		m_GroundTexture->Bind(0);
		m_LitTextureShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_LitTextureShader->SetUniform2f("u_Tiling", tiling, tiling);
		m_LitTextureShader->SetUniform1i("u_Material.useSpecularMap", 0);
		m_LitTextureShader->SetUniform1f("u_Material.specularIntensity", specularIntensity);
		m_LitTextureShader->SetUniform1f("u_Material.shininess", shininess);

		// Geometry
		m_QuadVb->Bind();
		m_BufferLayout->Attrib();

		glm::mat4 groundModel = glm::mat4(1.0f);
		groundModel = glm::rotate(groundModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		groundModel = glm::scale(groundModel, glm::vec3(m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", groundModel);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void DemoScene::RenderBoundWalls() const
	{
		constexpr float height = 2.0f;
		constexpr glm::vec2 tiling = glm::vec2(16.0f, 2.0f);
		constexpr float shininess = 32.0f;

		m_LitTextureShader->Bind();

		// Camera
		m_LitTextureShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitTextureShader->SetUniformMatrix4f("u_View", m_Camera.ViewMatrix);
		m_LitTextureShader->SetUniformMatrix4f("u_Projection", m_Camera.ProjectionMatrix);

		// Lights
		SetLightingUniforms(m_LitTextureShader);

		// Material
		m_BoundWallTexture->Bind(0);
		m_BoundWallSpecularTexture->Bind(1);
		m_LitTextureShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_LitTextureShader->SetUniform1i("u_Material.specularMap", 1);
		m_LitTextureShader->SetUniform2f("u_Tiling", tiling.x, tiling.y);
		m_LitTextureShader->SetUniform1i("u_Material.useSpecularMap", 1);
		m_LitTextureShader->SetUniform1f("u_Material.shininess", shininess);

		// Geometry
		m_CubeVb->Bind();
		m_BufferLayout->Attrib();

		// North
		glm::mat4 wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height / 2.0f, -m_GroundScale / 2.0f));
		wallModel = glm::scale(wallModel, glm::vec3(m_GroundScale, height, 0.2f));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// South
		wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height / 2.0f, m_GroundScale / 2.0f));
		wallModel = glm::scale(wallModel, glm::vec3(m_GroundScale, height, 0.2f));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// West
		wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(-m_GroundScale / 2.0f, height / 2.0f, 0.0f));
		wallModel = glm::scale(wallModel, glm::vec3(0.2f, height, m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// East
		wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(m_GroundScale / 2.0f, height / 2.0f, 0.0f));
		wallModel = glm::scale(wallModel, glm::vec3(0.2f, height, m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void DemoScene::RenderLoglBoxes() const
	{
		constexpr glm::vec3 boxesPos[] = {
			glm::vec3(7.0f, 0.5f,  7.0f),
			glm::vec3(-7.0f, 0.5f,  7.0f),
			glm::vec3(7.0f, 0.5f, -7.0f),
			glm::vec3(-7.0f, 0.5f, -7.0f),
			glm::vec3(0.0f, 0.5f,  6.0f),
			glm::vec3(0.0f, 0.5f, -6.0f),
		};

		constexpr float shininess = 32.0f;

		m_LitTextureShader->Bind();

		// Camera
		m_LitTextureShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitTextureShader->SetUniformMatrix4f("u_View", m_Camera.ViewMatrix);
		m_LitTextureShader->SetUniformMatrix4f("u_Projection", m_Camera.ProjectionMatrix);

		// Lights
		SetLightingUniforms(m_LitTextureShader);

		// Material
		m_LoglBoxTexture->Bind(0);
		m_LoglBoxSpecularTexture->Bind(1);
		m_LitTextureShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_LitTextureShader->SetUniform1i("u_Material.specularMap", 1);
		m_LitTextureShader->SetUniform2f("u_Tiling", 1.0f, 1.0f);
		m_LitTextureShader->SetUniform1i("u_Material.useSpecularMap", 1);
		m_LitTextureShader->SetUniform1f("u_Material.shininess", shininess);

		// Geometry
		m_CubeVb->Bind();
		m_BufferLayout->Attrib();

		for (auto& pos : boxesPos) 
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
			m_LitTextureShader->SetUniformMatrix4f("u_Model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void DemoScene::RenderTorches() const
	{
		m_CubeVb->Bind();
		m_BufferLayout->Attrib();

		// Sticks
		m_LitColorShader->Bind();
		m_LitColorShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitColorShader->SetUniformMatrix4f("u_View", m_Camera.ViewMatrix);
		m_LitColorShader->SetUniformMatrix4f("u_Projection", m_Camera.ProjectionMatrix);

		SetLightingUniforms(m_LitColorShader);

		for (const Torch& torch : m_Torches)
		{
			glm::mat4 stickModel = glm::translate(glm::mat4(1.0f), torch.pos + glm::vec3(0.0f, 0.4f, 0.0f));
			stickModel = glm::scale(stickModel, glm::vec3(0.08f, 0.8f, 0.08f));
			m_LitColorShader->SetUniform3f("u_Material.color", 0.45f, 0.3f, 0.15f);
			m_LitColorShader->SetUniform1f("u_Material.specularIntensity", 0.1f);
			m_LitColorShader->SetUniform1f("u_Material.shininess", 8.0f);
			m_LitColorShader->SetUniformMatrix4f("u_Model", stickModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Heads
		m_UnlitColorShader->Bind();
		m_UnlitColorShader->SetUniformMatrix4f("u_View", m_Camera.ViewMatrix);
		m_UnlitColorShader->SetUniformMatrix4f("u_Projection", m_Camera.ProjectionMatrix);

		for (const Torch& torch : m_Torches)
		{
			m_UnlitColorShader->SetUniform3f("u_Material.color", torch.color.x, torch.color.y, torch.color.z);
			glm::mat4 headModel = glm::translate(glm::mat4(1.0f), torch.pos + glm::vec3(0.0f, 0.85f, 0.0f));
			headModel = glm::scale(headModel, glm::vec3(0.12f, 0.12f, 0.12f));
			m_UnlitColorShader->SetUniformMatrix4f("u_Model", headModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}
