#include "DemoScene.h"

#include <iostream>

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

		m_Camera.Pos = glm::vec3(0.0f, 1.7f, 0.0f);

		m_GroundTexture = new Texture("Resources/Textures/Ground037/Ground037_Color.jpg", true);

		m_BoundWallTexture = new Texture("Resources/Textures/BrickWall2/brick_wall2-diff.tga", true);
		m_BoundWallSpecularTexture = new Texture("Resources/Textures/BrickWall2/brick_wall2-spec.tga", true);

		m_LoglBoxTexture = new Texture("Resources/Textures/LearnOpenGL/container2.png", false);
		m_LoglBoxSpecularTexture = new Texture("Resources/Textures/LearnOpenGL/container2_specular.png", false);
	}

	DemoScene::~DemoScene()
	{
		delete m_BufferLayout;

		delete m_QuadVb;
		delete m_CubeVb;

		delete m_LitColorShader;
		delete m_LitTextureShader;

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

	void DemoScene::Render()
	{
		constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

		glm::vec3 ambientColor(0.05f, 0.05f, 0.07f);
		glm::vec3 directionalLightDir(-0.5f, -0.4f, -0.3f);
		glm::vec3 directionalLightColor(0.5f, 0.52f, 0.58f);

		// View and projection -----------------------------------------------------------------------------------------
		glm::mat4 cameraView = BuildCameraViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		// Ground ------------------------------------------------------------------------------------------------------
		m_LitTextureShader->Bind();
		m_LitTextureShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitTextureShader->SetUniformMatrix4f("u_View", cameraView);
		m_LitTextureShader->SetUniformMatrix4f("u_Projection", projection);
		m_LitTextureShader->SetUniform3f("u_AmbientColor", ambientColor.x, ambientColor.y, ambientColor.z);
		m_LitTextureShader->SetUniform3f("u_DirectionalLight.dir", directionalLightDir.x, directionalLightDir.y, directionalLightDir.z);
		m_LitTextureShader->SetUniform3f("u_DirectionalLight.color", directionalLightColor.x, directionalLightColor.y, directionalLightColor.z);
		m_GroundTexture->Bind(0);
		m_LitTextureShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_LitTextureShader->SetUniform2f("u_Tiling", m_GroundTiling, m_GroundTiling);
		m_LitTextureShader->SetUniform1i("u_Material.useSpecularMap", 0);
		m_LitTextureShader->SetUniform1f("u_Material.specularIntensity", m_GroundSpecularIntensity);
		m_LitTextureShader->SetUniform1f("u_Material.shininess", m_GroundShininess);

		m_QuadVb->Bind();
		m_BufferLayout->Attrib();

		glm::mat4 groundModel = glm::mat4(1.0f);
		groundModel = glm::rotate(groundModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		groundModel = glm::scale(groundModel, glm::vec3(m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", groundModel);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Bound walls -------------------------------------------------------------------------------------------------
		m_CubeVb->Bind();
		m_BufferLayout->Attrib();

		m_LitTextureShader->Bind();
		m_LitTextureShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitTextureShader->SetUniformMatrix4f("u_View", cameraView);
		m_LitTextureShader->SetUniformMatrix4f("u_Projection", projection);
		m_LitTextureShader->SetUniform3f("u_AmbientColor", ambientColor.x, ambientColor.y, ambientColor.z);
		m_LitTextureShader->SetUniform3f("u_DirectionalLight.dir", directionalLightDir.x, directionalLightDir.y, directionalLightDir.z);
		m_LitTextureShader->SetUniform3f("u_DirectionalLight.color", directionalLightColor.x, directionalLightColor.y, directionalLightColor.z);
		m_BoundWallTexture->Bind(0);
		m_BoundWallSpecularTexture->Bind(1);
		m_LitTextureShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_LitTextureShader->SetUniform1i("u_Material.specularMap", 1);
		m_LitTextureShader->SetUniform2f("u_Tiling", m_BoundWallTiling.x, m_BoundWallTiling.y);
		m_LitTextureShader->SetUniform1i("u_Material.useSpecularMap", 1);
		m_LitTextureShader->SetUniform1f("u_Material.shininess", m_BoundWallShininess);

		// North
		glm::mat4 wallModel = glm::mat4(1.0f);
		wallModel = glm::translate(wallModel, glm::vec3(0.0f, m_BoundWallHeight / 2.0f, -m_GroundScale / 2.0f));
		wallModel = glm::scale(wallModel, glm::vec3(m_GroundScale, m_BoundWallHeight, 0.2f));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// South
		wallModel = glm::mat4(1.0f);
		wallModel = glm::translate(wallModel, glm::vec3(0.0f, m_BoundWallHeight / 2.0f, m_GroundScale / 2.0f));
		wallModel = glm::scale(wallModel, glm::vec3(m_GroundScale, m_BoundWallHeight, 0.2f));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// West
		wallModel = glm::mat4(1.0f);
		wallModel = glm::translate(wallModel, glm::vec3(-m_GroundScale / 2.0f, m_BoundWallHeight / 2.0f, 0.0f));
		wallModel = glm::scale(wallModel, glm::vec3(0.2f, m_BoundWallHeight, m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// East
		wallModel = glm::mat4(1.0f);
		wallModel = glm::translate(wallModel, glm::vec3(m_GroundScale / 2.0f, m_BoundWallHeight / 2.0f, 0.0f));
		wallModel = glm::scale(wallModel, glm::vec3(0.2f, m_BoundWallHeight, m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Logl box ----------------------------------------------------------------------------------------------------
		m_CubeVb->Bind();
		m_BufferLayout->Attrib();

		m_LitTextureShader->Bind();
		m_LitTextureShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitTextureShader->SetUniformMatrix4f("u_View", cameraView);
		m_LitTextureShader->SetUniformMatrix4f("u_Projection", projection);
		m_LitTextureShader->SetUniform3f("u_AmbientColor", ambientColor.x, ambientColor.y, ambientColor.z);
		m_LitTextureShader->SetUniform3f("u_DirectionalLight.dir", directionalLightDir.x, directionalLightDir.y, directionalLightDir.z);
		m_LitTextureShader->SetUniform3f("u_DirectionalLight.color", directionalLightColor.x, directionalLightColor.y, directionalLightColor.z);
		m_LoglBoxTexture->Bind(0);
		m_LoglBoxSpecularTexture->Bind(1);
		m_LitTextureShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_LitTextureShader->SetUniform1i("u_Material.specularMap", 1);
		m_LitTextureShader->SetUniform2f("u_Tiling", 1.0f, 1.0f);
		m_LitTextureShader->SetUniform1i("u_Material.useSpecularMap", 1);
		m_LitTextureShader->SetUniform1f("u_Material.shininess", m_LoglBoxShininess);

		glm::mat4 loglBoxModel = glm::mat4(1.0f);
		loglBoxModel = glm::translate(loglBoxModel, m_LoglBoxPos);
		m_LitTextureShader->SetUniformMatrix4f("u_Model", loglBoxModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Color cube --------------------------------------------------------------------------------------------------
		m_LitColorShader->Bind();
		m_LitColorShader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		m_LitColorShader->SetUniformMatrix4f("u_View", cameraView);
		m_LitColorShader->SetUniformMatrix4f("u_Projection", projection);
		m_LitColorShader->SetUniform3f("u_AmbientColor", ambientColor.x, ambientColor.y, ambientColor.z);
		m_LitColorShader->SetUniform3f("u_DirectionalLight.dir", directionalLightDir.x, directionalLightDir.y, directionalLightDir.z);
		m_LitColorShader->SetUniform3f("u_DirectionalLight.color", directionalLightColor.x, directionalLightColor.y, directionalLightColor.z);
		m_LitColorShader->SetUniform3f("u_Material.color", 1.0f, 0.0f, 0.0f);
		m_LitColorShader->SetUniform1f("u_Material.specularIntensity", 0.5f);
		m_LitColorShader->SetUniform1f("u_Material.shininess", 32.0f);

		m_CubeVb->Bind();
		m_BufferLayout->Attrib();

		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 0.02f, 4.0f));
		cubeModel = glm::scale(cubeModel, glm::vec3(2.0f, 0.04f, 2.0f));
		m_LitColorShader->SetUniformMatrix4f("u_Model", cubeModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
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
}
