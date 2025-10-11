#include "TestScene.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Included here only for input. Ideally all glfw should be handled only by Window class

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Application.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Texture.h"

namespace LGE
{
	namespace
	{
		// Shader
		const std::string k_VertexShaderSrc = R"(
			#version 330 core
			
			// attributes
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			out vec2 v_TexCoord;
			
			uniform mat4 u_Model;
			uniform mat4 u_View;
			uniform mat4 u_Projection;
			
			void main()
			{
				gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
				v_TexCoord = a_TexCoord;
			}
		)";

		const std::string k_FragmentShaderSrc = R"(
			#version 330 core
			
			out vec4 fragColor; // obligatory fragment color output

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture0;
			uniform sampler2D u_Texture1;
			
			void main()
			{
				fragColor = mix(texture(u_Texture0, v_TexCoord), texture(u_Texture1, v_TexCoord), 0.2);
			}
		)";

		constexpr float k_QuadVerts[] = {
			// positions             // texture coords
			0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,      1.0f, 0.0f,   // bottom right
		   -0.5f,  0.5f, 0.0f,      0.0f, 1.0f,   // top left 
			0.5f, -0.5f, 0.0f,      1.0f, 0.0f,   // bottom right
		   -0.5f, -0.5f, 0.0f,      0.0f, 0.0f,   // bottom left
		   -0.5f,  0.5f, 0.0f,      0.0f, 1.0f   // top left 
		};

		glm::vec3 s_CameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
		glm::vec3 s_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 s_CameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
		glm::vec3 s_CameraRight = glm::vec3(1.0f,  0.0f, 0.0f);
		float s_CameraYaw = -90.0f;

		bool s_FirstMouse = true;
		float s_LastMouseX;
		float s_LastMouseY;
	}
	
	TestScene::TestScene()
	{
		std::cout << "[TestScene] TestScene(){" << '\n';

		constexpr float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// tex coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		// Texture
		m_BoxTexture = new Texture("Resources/container.jpg");
		m_FaceTexture = new Texture("Resources/awesomeface.png");

		// Shader Program
		m_BasicShaderProgram = new ShaderProgram(k_VertexShaderSrc, k_FragmentShaderSrc);
	}

	TestScene::~TestScene()
	{
		std::cout << "[TestScene] ~TestScene(){" << '\n';

		delete m_BoxTexture;
		delete m_FaceTexture;
		delete m_BasicShaderProgram;

		std::cout << "[TestScene] ~TestScene()}" << '\n';
	}

	void TestScene::Update(const float deltaTime)
	{
		const float newMouseX = static_cast<float>(Application::Get().GetMouseX());
		const float newMouseY = static_cast<float>(Application::Get().GetMouseY());

		if (s_FirstMouse)
		{
			s_LastMouseX = newMouseX;
			s_LastMouseY = newMouseY;
			
			s_FirstMouse = false;
		}
		
		float mouseDeltaX = newMouseX - s_LastMouseX;
		float mouseDeltaY = newMouseY - s_LastMouseY;
		s_LastMouseX = newMouseX;
		s_LastMouseY = newMouseY;

		constexpr float sensitivity = 0.1f;

		s_CameraYaw += mouseDeltaX * sensitivity;

		s_CameraFront.x = cos(glm::radians(s_CameraYaw));
		s_CameraFront.y = 0.0f;
		s_CameraFront.z = sin(glm::radians(s_CameraYaw));
		s_CameraFront = glm::normalize(s_CameraFront);

		s_CameraRight = glm::cross(s_CameraFront, s_CameraUp);
		
		float cameraSpeed = 2.5f * deltaTime;
		
		if (Application::Get().GetKey(GLFW_KEY_W) == 1)
		{
			s_CameraPos += cameraSpeed * s_CameraFront;
		}

		if (Application::Get().GetKey(GLFW_KEY_S) == 1)
		{
			s_CameraPos -= cameraSpeed * s_CameraFront;
		}

		if (Application::Get().GetKey(GLFW_KEY_A) == 1)
		{
			s_CameraPos -= s_CameraRight * cameraSpeed;
		}

		if (Application::Get().GetKey(GLFW_KEY_D) == 1)
		{
			s_CameraPos += s_CameraRight * cameraSpeed;
		}
	}
	
	void TestScene::Render()
	{
		m_BoxTexture->Bind(0);
		m_FaceTexture->Bind(1);
		
		m_BasicShaderProgram->Bind();
		m_BasicShaderProgram->SetUniform1i("u_Texture0", 0);
		m_BasicShaderProgram->SetUniform1i("u_Texture1", 1);

		// glm::vec4 col0(cameraRight.x, cameraUp.x, cameraDirection.x, 0.0f);
		// glm::vec4 col1(cameraRight.y, cameraUp.y, cameraDirection.y, 0.0f);
		// glm::vec4 col2(cameraRight.z, cameraUp.z, cameraDirection.z, 0.0f);
		// glm::vec4 col3(0.0f, 0.0f, 0.0f, 1.0f);
		// glm::mat4 lookAt = glm::mat4(col0, col1, col2, col3);
		// lookAt = glm::translate(lookAt, glm::vec3(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z));

		glm::mat4 lookAt = glm::lookAt(s_CameraPos, s_CameraPos + s_CameraFront, s_CameraUp);

		m_BasicShaderProgram->SetUniformMatrix4f("u_View", lookAt);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		m_BasicShaderProgram->SetUniformMatrix4f("u_Projection", projection);

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(2.0f, 1.0f, -2.0f), 
			glm::vec3(-2.0f, 1.0f, -4.0f)
		};

		for (unsigned int i = 0; i < 3; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			m_BasicShaderProgram->SetUniformMatrix4f("u_Model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}
