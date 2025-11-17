#include "TestScene.h"

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
#include "Shaders/UnlitTextureShader.h"
#include "Shaders/UnlitColorShader.h"

namespace LGE
{
	TestScene::TestScene()
	{
		std::cout << "[TestScene] TestScene(){" << '\n';

		// LOAD RESOURCES ----------------------------------------------------------------------------------------------
		m_SmoothStoneTexture = new Texture("Resources/Textures/Minecraft/smooth-stone.jpg", true);

		m_UnlitTextureShaderProgram = new ShaderProgram(Shaders::UnlitTexture::VERTEX, Shaders::UnlitTexture::FRAGMENT);
		m_UnlitColorShaderProgram = new ShaderProgram(Shaders::UnlitColor::VERTEX, Shaders::UnlitColor::FRAGMENT);
		m_LitColorShaderProgram = new ShaderProgram(Shaders::LitColor::VERTEX, Shaders::LitColor::FRAGMENT);
		
		// MESHES ------------------------------------------------------------------------------------------------------
		m_QuadVb = new VertexBuffer(Meshes::QUAD, sizeof(Meshes::QUAD));

		m_CubeVb = new VertexBuffer(Meshes::CUBE, sizeof(Meshes::CUBE));

		m_BufferLayout = new BufferLayout();
		m_BufferLayout->PushFloat3(); // position attribute
		m_BufferLayout->PushFloat2(); // tex coord attribute
		
		m_CubeBufferLayout = new BufferLayout();
		m_CubeBufferLayout->PushFloat3(); // position attribute
		m_CubeBufferLayout->PushFloat2(); // tex coord attribute
		m_CubeBufferLayout->PushFloat3(); // normal attribute

		// CAMERA ------------------------------------------------------------------------------------------------------
		m_CameraPos = glm::vec3(0.0f, 1.7f,  0.0f);
		m_CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraYaw = -90.0f; // TODO: initialize from camera forward
		m_CameraPitch = 0.0f;

		// -------------------------------------------------------------------------------------------------------------
		m_GroundModel = glm::mat4(1.0f);
		m_GroundModel = glm::rotate(m_GroundModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		m_GroundModel = glm::scale(m_GroundModel, glm::vec3(32.0f));

		m_LightCubeModel = glm::mat4(1.0f);
		m_LightCubeModel = glm::translate(m_LightCubeModel, glm::vec3(1.2f, 1.0f, 2.0f));
		m_LightCubeModel = glm::scale(m_LightCubeModel, glm::vec3(0.2f)); 

		m_CubeModel = glm::mat4(1.0f);
	}

	TestScene::~TestScene()
	{
		std::cout << "[TestScene] ~TestScene(){" << '\n';

		delete m_SmoothStoneTexture;
		
		delete m_UnlitTextureShaderProgram;
		delete m_UnlitColorShaderProgram;
		delete m_LitColorShaderProgram;

		delete m_QuadVb;
		delete m_CubeVb;
		delete m_BufferLayout;
		delete m_CubeBufferLayout;

		std::cout << "[TestScene] ~TestScene()}" << '\n';
	}

	void TestScene::Update(const float deltaTime)
	{
		// Rotate Camera -----------------------------------------------------------------------------------------------
		const float newMouseX = static_cast<float>(Application::Get().GetMouseX());
		const float newMouseY = static_cast<float>(Application::Get().GetMouseY());

		if (m_FirstMouse)
		{
			m_LastMouseX = newMouseX;
			m_LastMouseY = newMouseY;
			
			m_FirstMouse = false;
		}
		
		float mouseDeltaX = newMouseX - m_LastMouseX;
		float mouseDeltaY = newMouseY - m_LastMouseY;
		m_LastMouseX = newMouseX;
		m_LastMouseY = newMouseY;

		constexpr float sensitivity = 0.1f;

		m_CameraYaw += mouseDeltaX * sensitivity;

		m_CameraPitch -= mouseDeltaY * sensitivity;
		if (m_CameraPitch > 89.0f)
			m_CameraPitch = 89.0f;
		if (m_CameraPitch < -89.0f)
			m_CameraPitch = -89.0f;
		
		m_CameraForward.x = cos(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		m_CameraForward.y = sin(glm::radians(m_CameraPitch));
		m_CameraForward.z = sin(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		m_CameraForward = glm::normalize(m_CameraForward);

		// Move Camera -------------------------------------------------------------------------------------------------

		// Read move input
		glm::vec2 moveInput(0.0f);
		if (Application::Get().GetKey(GLFW_KEY_W) == 1)
			moveInput.y = 1.0f;
		else if (Application::Get().GetKey(GLFW_KEY_S) == 1)
			moveInput.y = -1.0f;
		if (Application::Get().GetKey(GLFW_KEY_D) == 1)
			moveInput.x = 1.0f;
		else if (Application::Get().GetKey(GLFW_KEY_A) == 1)
			moveInput.x = -1.0f;

		if (glm::length(moveInput) > 0.0f)
		{
			// MoveFlyCamera(glm::normalize(moveInput), deltaTime);
			MoveFpsCamera(glm::normalize(moveInput), deltaTime);
		}
	}

	void TestScene::MoveFlyCamera(const glm::vec2 moveInput, const float deltaTime)
	{
		constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
		const glm::vec3 cameraRight = glm::normalize(glm::cross(m_CameraForward, worldUp));

		constexpr float cameraSpeed = 5.0f;
		const glm::vec3 vel = (m_CameraForward * moveInput.y + cameraRight * moveInput.x) * cameraSpeed;

		m_CameraPos += vel * deltaTime;
	}

	void TestScene::MoveFpsCamera(const glm::vec2 moveInput, const float deltaTime)
	{
		glm::vec3 forward = glm::normalize(glm::vec3(cos(glm::radians(m_CameraYaw)), 0.0f, sin(glm::radians(m_CameraYaw))));
		constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));

		constexpr float cameraSpeed = 5.0f;
		const glm::vec3 vel = (forward * moveInput.y + right * moveInput.x) * cameraSpeed;

		m_CameraPos += vel * deltaTime;
	}

	void TestScene::Render()
	{
		// Camera View Matrix ------------------------------------------------------------------------------------------
		glm::mat4 cameraView = BuildCameraViewMatrix();

		// -------------------------------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		
		// Render Ground -----------------------------------------------------------------------------------------------
		m_SmoothStoneTexture->Bind(0);
		m_UnlitTextureShaderProgram->Bind();
		m_UnlitTextureShaderProgram->SetUniformMatrix4f("u_View", cameraView);
		m_UnlitTextureShaderProgram->SetUniformMatrix4f("u_Projection", projection);
		m_UnlitTextureShaderProgram->SetUniform1i("u_Texture", 0);
		m_UnlitTextureShaderProgram->SetUniform2f("u_Tiling", 32.0f, 32.0f);

		m_QuadVb->Bind();
		m_BufferLayout->Attrib();

		m_UnlitTextureShaderProgram->SetUniformMatrix4f("u_Model", m_GroundModel);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Render Light Cube -------------------------------------------------------------------------------------------
		m_UnlitColorShaderProgram->Bind();
		m_UnlitColorShaderProgram->SetUniformMatrix4f("u_View", cameraView);
		m_UnlitColorShaderProgram->SetUniformMatrix4f("u_Projection", projection);
		m_UnlitColorShaderProgram->SetUniform3f("u_Color", 1.0f, 1.0f, 1.0f);

		m_CubeVb->Bind();
		m_CubeBufferLayout->Attrib();
		
		m_UnlitColorShaderProgram->SetUniformMatrix4f("u_Model", m_LightCubeModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// Render Cube -------------------------------------------------------------------------------------------------
		m_LitColorShaderProgram->Bind();
		m_LitColorShaderProgram->SetUniformMatrix4f("u_View", cameraView);
		m_LitColorShaderProgram->SetUniformMatrix4f("u_Projection", projection);
		m_LitColorShaderProgram->SetUniform3f("u_Color", 1.0f, 0.5f, 0.31f);
		m_LitColorShaderProgram->SetUniform1f("u_AmbientStrength", 0.2f);
		m_LitColorShaderProgram->SetUniform3f("u_AmbientColor", 1.0f, 1.0f, 1.0f);
		m_LitColorShaderProgram->SetUniform3f("u_DiffuseLightPos", 1.2f, 1.0f, 2.0f);

		m_CubeVb->Bind();
		m_CubeBufferLayout->Attrib();
		
		m_LitColorShaderProgram->SetUniformMatrix4f("u_Model", m_CubeModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	glm::mat4 TestScene::BuildCameraViewMatrix() const
	{
		constexpr bool manuallyCreateCameraViewMatrix = true;

		glm::mat4 cameraView;

		if (manuallyCreateCameraViewMatrix)
		{
			// glm::vec4 col0(cameraRight.x, cameraUp.x, cameraDirection.x, 0.0f);
			// glm::vec4 col1(cameraRight.y, cameraUp.y, cameraDirection.y, 0.0f);
			// glm::vec4 col2(cameraRight.z, cameraUp.z, cameraDirection.z, 0.0f);
			// glm::vec4 col3(0.0f, 0.0f, 0.0f, 1.0f);
			// glm::mat4 lookAt = glm::mat4(col0, col1, col2, col3);
			// lookAt = glm::translate(lookAt, glm::vec3(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z));
		
			// 1. Position = known
			// 2. Calculate cameraDirection
			glm::vec3 target = m_CameraPos + m_CameraForward;
			glm::vec3 zaxis = glm::normalize(m_CameraPos - target);
			// 3. Get positive right axis vector
			constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
			glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
			// 4. Calculate camera up vector
			glm::vec3 yaxis = glm::cross(zaxis, xaxis);

			// Create translation and rotation matrix
			// In glm we access elements as mat[col][row] due to column-major layout
			glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
			translation[3][0] = -m_CameraPos.x; // Fourth column, first row
			translation[3][1] = -m_CameraPos.y;
			translation[3][2] = -m_CameraPos.z;
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
		}
		else
		{
			constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
			// normalize the vectors, because their length gets closer to 0 the more you look up or down
			glm::vec3 cameraRight = glm::normalize(glm::cross(m_CameraForward, worldUp));
			glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, m_CameraForward));
			cameraView = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraForward, cameraUp);
		}

		return cameraView;
	}
}
