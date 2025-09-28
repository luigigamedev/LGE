#include "TestScene.h"

#include <iostream>

#include <glad/glad.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/ShaderProgram.h"
#include "Rendering/Texture.h"

namespace LGE
{
	TestScene::TestScene()
		: m_BoxTexture(nullptr), m_FaceTexture(nullptr), m_BasicShaderProgram(nullptr)
	{
		std::cout << "[TestScene] TestScene(){" << '\n';

		// Generate the vertex array object.
		// Bind the VAO so any subsequent vertex attribute calls from that point on will be stored 
		// inside the VAO: vertex buffer data, elements buffer data, attributes layout.
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Generate the vertex buffer object.
		// Set the buffer data on OpenGL.
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Layout/setup vertex attribute pointers
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// tex coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Generate the elements buffer object.
		// Set the buffer data on OpenGL.
		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		// Unbind VAO and VBO and EBO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Bind the VAO to draw
		glBindVertexArray(vao);

		// Texture
		m_BoxTexture = new Texture("Resources/container.jpg");
		m_FaceTexture = new Texture("Resources/awesomeface.png");

		m_BoxTexture->Bind(0);
		m_FaceTexture->Bind(1);

		// Shader
		std::string vertexShaderSrc = R"(
			#version 330 core
			
			// attributes
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			layout(location = 2) in vec2 a_TexCoord;

			out vec3 v_Color; // color from vertex shader to fragment shader
			out vec2 v_TexCoord;
			
			uniform mat4 u_Matri;
			
			void main()
			{
				gl_Position = u_Matri * vec4(a_Position, 1.0);
				v_Color = a_Color;
				v_TexCoord = a_TexCoord;
			}
		)";

		std::string fragmentShaderSrc = R"(
			#version 330 core
			
			out vec4 fragColor; // obligatory fragment color output

			in vec3 v_Color; // color from vertex shader to fragment shader
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture0;
			uniform sampler2D u_Texture1;
			
			void main()
			{
				fragColor = mix(texture(u_Texture0, v_TexCoord), texture(u_Texture1, v_TexCoord), 0.2);
			}
		)";

		m_BasicShaderProgram = new ShaderProgram(vertexShaderSrc, fragmentShaderSrc);

		m_BasicShaderProgram->Bind();
		m_BasicShaderProgram->SetUniform1i("u_Texture0", 0);
		m_BasicShaderProgram->SetUniform1i("u_Texture1", 1);
		
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  
		
		m_BasicShaderProgram->SetUniformMatrix4f("u_Matri", trans);
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
		//m_BoxTexture->Bind(0);
		//m_FaceTexture->Bind(1);
		//m_BasicShaderProgram->SetUniform1i("u_Texture0", 0);
		//m_BasicShaderProgram->SetUniform1i("u_Texture1", 1);
		//m_BasicShaderProgram->SetUniformMatrix4f("u_Matri", Matrix4::Identity());
		//m_BasicShaderProgram->Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void TestScene::ImGuiRender()
	{
	}
}
