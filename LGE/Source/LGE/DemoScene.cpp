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
#include "Shaders/UnlitColorShader.h"
#include "Shaders/UnlitTextureShader.h"

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

		m_UnlitColorShader = new ShaderProgram(Shaders::UnlitColor::VERTEX, Shaders::UnlitColor::FRAGMENT);
		m_UnlitTextureShader = new ShaderProgram(Shaders::UnlitTexture::VERTEX, Shaders::UnlitTexture::FRAGMENT);

		m_SmoothStoneTexture = new Texture("Resources/Textures/Minecraft/smooth-stone.jpg", true);

		m_Camera.Pos = glm::vec3(0.0f, 1.7f, 0.0f);
	}

	DemoScene::~DemoScene()
	{
		delete m_BufferLayout;

		delete m_QuadVb;
		delete m_CubeVb;

		delete m_UnlitColorShader;
		delete m_UnlitTextureShader;

		delete m_SmoothStoneTexture;
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
		glm::mat4 cameraView = BuildCameraViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		m_UnlitTextureShader->Bind();
		m_UnlitTextureShader->SetUniformMatrix4f("u_View", cameraView);
		m_UnlitTextureShader->SetUniformMatrix4f("u_Projection", projection);
		m_SmoothStoneTexture->Bind(0);
		m_UnlitTextureShader->SetUniform1i("u_Texture", 0);
		m_UnlitTextureShader->SetUniform2f("u_Tiling", 32.0f, 32.0f);

		m_QuadVb->Bind();
		m_BufferLayout->Attrib();

		glm::mat4 groundModel = glm::mat4(1.0f);
		groundModel = glm::rotate(groundModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		groundModel = glm::scale(groundModel, glm::vec3(32.0f));
		m_UnlitTextureShader->SetUniformMatrix4f("u_Model", groundModel);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		m_UnlitColorShader->Bind();
		m_UnlitColorShader->SetUniformMatrix4f("u_View", cameraView);
		m_UnlitColorShader->SetUniformMatrix4f("u_Projection", projection);
		m_UnlitColorShader->SetUniform3f("u_Color", 1.0f, 1.0f, 1.0f);

		m_CubeVb->Bind();
		m_BufferLayout->Attrib();

		constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
		
		for (unsigned int i = 0; i < m_CubeCount; i++)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_CubesPos[i]);
			model = glm::rotate(model, glm::radians(m_CubesYaw[i]), worldUp);
			model = glm::scale(model, m_CubesScale[i]);
			m_UnlitColorShader->SetUniformMatrix4f("u_Model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
