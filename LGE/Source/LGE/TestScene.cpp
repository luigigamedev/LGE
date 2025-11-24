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
#include "Shaders/LightCasters1Shader.h"
#include "Shaders/UnlitColorShader.h"

namespace LGE
{
	TestScene::TestScene()
	{
		std::cout << "[TestScene] TestScene(){" << '\n';

		// LOAD RESOURCES ----------------------------------------------------------------------------------------------
		m_DiffuseMapTexture = new Texture("Resources/Textures/LearnOpenGL/container2.png", false);
		m_SpecularMapTexture = new Texture("Resources/Textures/LearnOpenGL/container2_specular.png", false);

		m_UnlitColorShaderProgram = new ShaderProgram(Shaders::UnlitColor::VERTEX, Shaders::UnlitColor::FRAGMENT);
		m_LightCasters1ShaderProgram = new ShaderProgram(Shaders::LightCasters1::VERTEX, Shaders::LightCasters1::FRAGMENT);
		
		// MESHES ------------------------------------------------------------------------------------------------------
		m_CubeVb = new VertexBuffer(Meshes::CUBE, sizeof(Meshes::CUBE));

		m_BufferLayout = new BufferLayout();
		m_BufferLayout->PushFloat3(); // position attribute
		m_BufferLayout->PushFloat3(); // normal attribute
		m_BufferLayout->PushFloat2(); // tex coord attribute

		// -------------------------------------------------------------------------------------------------------------

		m_LightCubeModel = glm::translate(glm::mat4(1.0f), m_LightPos);
		m_LightCubeModel = glm::scale(m_LightCubeModel, glm::vec3(0.2f)); 
	}

	TestScene::~TestScene()
	{
		std::cout << "[TestScene] ~TestScene(){" << '\n';

		delete m_DiffuseMapTexture;
		delete m_SpecularMapTexture;
		
		delete m_UnlitColorShaderProgram;
		delete m_LightCasters1ShaderProgram;

		delete m_CubeVb;
		delete m_BufferLayout;

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
			MoveFlyCamera(glm::normalize(moveInput), deltaTime);
			//MoveFpsCamera(glm::normalize(moveInput), deltaTime);
		}

		// Move Cubes --------------------------------------------------------------------------------------------------

		//m_CubePos.x = glm::sin(Application::Get().GetTime());
		//m_CubeScale.y = 1.0f + glm::sin(Application::Get().GetTime()) * 1.0f;
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

		// Projection --------------------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		// Render Light Cube -------------------------------------------------------------------------------------------
		/*m_UnlitColorShaderProgram->Bind();
		m_UnlitColorShaderProgram->SetUniformMatrix4f("u_View", cameraView);
		m_UnlitColorShaderProgram->SetUniformMatrix4f("u_Projection", projection);
		m_UnlitColorShaderProgram->SetUniform3f("u_Color", 1.0f, 1.0f, 1.0f);

		m_CubeVb->Bind();
		m_BufferLayout->Attrib();
		
		m_UnlitColorShaderProgram->SetUniformMatrix4f("u_Model", m_LightCubeModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);*/
		
		// Render Cube -------------------------------------------------------------------------------------------------
		m_LightCasters1ShaderProgram->Bind();

		m_LightCasters1ShaderProgram->SetUniform3f("u_Light.direction", -0.2f, -1.0f, -0.3f);
		m_LightCasters1ShaderProgram->SetUniform3f("u_ViewPos", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);

		// light properties
		m_LightCasters1ShaderProgram->SetUniform3f("u_Light.ambient", 0.2f, 0.2f, 0.2f);
		m_LightCasters1ShaderProgram->SetUniform3f("u_Light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		m_LightCasters1ShaderProgram->SetUniform3f("u_Light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		m_DiffuseMapTexture->Bind(0);
		m_LightCasters1ShaderProgram->SetUniform1i("u_Material.diffuse", 0);
		m_SpecularMapTexture->Bind(1);
		m_LightCasters1ShaderProgram->SetUniform1i("u_Material.specular", 1);
		m_LightCasters1ShaderProgram->SetUniform1f("u_Material.shininess", 64.0f);

		// view/projection
		m_LightCasters1ShaderProgram->SetUniformMatrix4f("u_View", cameraView);
		m_LightCasters1ShaderProgram->SetUniformMatrix4f("u_Projection", projection);

		// bind mesh
		m_CubeVb->Bind();
		m_BufferLayout->Attrib();
		
		for (unsigned int i = 0; i < m_CubeCount; i++)
		{
			// model (world transformation)
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_CubesPos[i]);
			model = glm::rotate(model, glm::radians(m_CubesAngle[i]), m_CubesRotateAxis);
			model = glm::scale(model, m_CubesScale[i]);
			m_LightCasters1ShaderProgram->SetUniformMatrix4f("u_Model", model);

			// render
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
