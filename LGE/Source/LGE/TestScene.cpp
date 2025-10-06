#include "TestScene.h"

#include <iostream>

#include <glad/glad.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
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
	}
	
	TestScene::TestScene()
		: m_BoxTexture(nullptr), m_FaceTexture(nullptr), m_BasicShaderProgram(nullptr)
	{
		std::cout << "[TestScene] TestScene(){" << '\n';
		
		float vertices[] = {
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

	void TestScene::Update(float deltaTime)
	{
	}

	void TestScene::Render()
	{
		m_BoxTexture->Bind(0);
		m_FaceTexture->Bind(1);
		
		m_BasicShaderProgram->Bind();
		m_BasicShaderProgram->SetUniform1i("u_Texture0", 0);
		m_BasicShaderProgram->SetUniform1i("u_Texture1", 1);
		
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

		m_BasicShaderProgram->SetUniformMatrix4f("u_View", view);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		m_BasicShaderProgram->SetUniformMatrix4f("u_Projection", projection);

		glm::vec3 cubePositions[] = {
			glm::vec3( 0.0f,  0.0f,  0.0f), 
			glm::vec3( 2.0f,  5.0f, -15.0f), 
			glm::vec3(-1.5f, -2.2f, -2.5f),  
			glm::vec3(-3.8f, -2.0f, -12.3f),  
			glm::vec3( 2.4f, -0.4f, -3.5f),  
			glm::vec3(-1.7f,  3.0f, -7.5f),  
			glm::vec3( 1.3f, -2.0f, -2.5f),  
			glm::vec3( 1.5f,  2.0f, -2.5f), 
			glm::vec3( 1.5f,  0.2f, -1.5f), 
			glm::vec3(-1.3f,  1.0f, -1.5f)  
		};

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			m_BasicShaderProgram->SetUniformMatrix4f("u_Model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void TestScene::ImGuiRender()
	{
	}
}
