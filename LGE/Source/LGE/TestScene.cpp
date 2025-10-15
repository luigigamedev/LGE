#include "TestScene.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Included here only for input. Ideally all glfw should be handled only by Window class

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Application.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Texture.h"
#include "Shaders/UnlitTextureShader.h"

namespace LGE
{
	namespace
	{
		constexpr float k_QuadVerts[] = {
			// positions             // texture coords
			0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
		   -0.5f,  0.5f, 0.0f,      0.0f, 1.0f,   // top left 
			0.5f, -0.5f, 0.0f,      1.0f, 0.0f,   // bottom right
			0.5f, -0.5f, 0.0f,      1.0f, 0.0f,   // bottom right
		   -0.5f,  0.5f, 0.0f,      0.0f, 1.0f,   // top left 
		   -0.5f, -0.5f, 0.0f,      0.0f, 0.0f,   // bottom left
		};

		constexpr float k_CubeVerts[] = {
			// positions            // texture coords

			// Front face (+Z)
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

			// Back face (-Z)
			 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

			// Left face (-X)
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

			// Right face (+X)
			 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

			// Top face (+Y)
			-0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f, 0.0f,

			// Bottom face (-Y)
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f
		};
	}
	
	TestScene::TestScene()
	{
		std::cout << "[TestScene] TestScene(){" << '\n';

		// Camera Object
		m_CameraPos = glm::vec3(0.0f, 1.7f,  0.0f);
		m_CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraYaw = -90.0f; // Should be initialized from camera forward
		m_CameraPitch = 0.0f;

		// Ground Object
		m_GroundModel = glm::mat4(1.0f);
		m_GroundModel = glm::rotate(m_GroundModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		m_GroundModel = glm::scale(m_GroundModel, glm::vec3(32.0f));

		// Dirt Block Object
		m_DirtBlockModel = glm::mat4(1.0f);
		m_DirtBlockModel = glm::translate(m_DirtBlockModel, glm::vec3(0.5f, 0.5f, -5.5f));

		// Quad Mesh Vertex Data
		glGenBuffers(1, &m_QuadVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(k_QuadVerts), k_QuadVerts, GL_STATIC_DRAW);

		// Cube Mesh Vertex Data
		glGenBuffers(1, &m_CubeVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_CubeVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(k_CubeVerts), k_CubeVerts, GL_STATIC_DRAW);
		
		// Textures
		m_BoxTexture = new Texture("Resources/container.jpg", false);
		m_FaceTexture = new Texture("Resources/awesomeface.png", false);
		m_StoneTexture = new Texture("Resources/Textures/Minecraft/stone.jpg", true);
		m_SmoothStoneTexture = new Texture("Resources/Textures/Minecraft/smooth-stone.jpg", true);
		m_DirtTexture = new Texture("Resources/Textures/Minecraft/dirt.jpg", true);

		// Shader Program
		m_UnlitTextureShaderProgram = new ShaderProgram(Shaders::UnlitTexture::VERTEX, Shaders::UnlitTexture::FRAGMENT);

		// Clear bindings before start rendering
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	TestScene::~TestScene()
	{
		std::cout << "[TestScene] ~TestScene(){" << '\n';

		glDeleteBuffers(1, &m_QuadVbo);
		
		delete m_BoxTexture;
		delete m_FaceTexture;
		delete m_StoneTexture;
		delete m_SmoothStoneTexture;
		delete m_DirtTexture;
		
		delete m_UnlitTextureShaderProgram;

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

	glm::mat4 TestScene::ManuallyCreateCameraViewMatrix() const
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
		return rotation * translation; // Remember to read from right to left (first translation then rotation)
	}

	void TestScene::Render()
	{
		// Camera View Matrix ------------------------------------------------------------------------------------------
		constexpr bool manuallyCreateCameraViewMatrix = true;

		glm::mat4 cameraView;

		if (manuallyCreateCameraViewMatrix)
		{
			cameraView = ManuallyCreateCameraViewMatrix();	
		}
		else
		{
			constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
			// normalize the vectors, because their length gets closer to 0 the more you look up or down
			glm::vec3 cameraRight = glm::normalize(glm::cross(m_CameraForward, worldUp));
			glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, m_CameraForward));
			cameraView = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraForward, cameraUp);
		}

		// -------------------------------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		
		m_UnlitTextureShaderProgram->Bind();
		m_UnlitTextureShaderProgram->SetUniformMatrix4f("u_View", cameraView);
		m_UnlitTextureShaderProgram->SetUniformMatrix4f("u_Projection", projection);
		
		// Render Ground -----------------------------------------------------------------------------------------------
		m_SmoothStoneTexture->Bind(0);
		m_UnlitTextureShaderProgram->SetUniform1i("u_Texture", 0);
		m_UnlitTextureShaderProgram->SetUniform2f("u_Tiling", 32.0f, 32.0f);

		// Bind quad mesh and layout attributes
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVbo);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// tex coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_UnlitTextureShaderProgram->SetUniformMatrix4f("u_Model", m_GroundModel);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Render Dirt Block -------------------------------------------------------------------------------------------
		m_DirtTexture->Bind(0);
		m_UnlitTextureShaderProgram->SetUniform1i("u_Texture", 0);
		m_UnlitTextureShaderProgram->SetUniform2f("u_Tiling", 1.0f, 1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, m_CubeVbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_UnlitTextureShaderProgram->SetUniformMatrix4f("u_Model", m_DirtBlockModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
