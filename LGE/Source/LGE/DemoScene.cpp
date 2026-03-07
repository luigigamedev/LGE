#include "DemoScene.h"

#include <iostream>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Included here only for input. Ideally all glfw should be handled only by Window class

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Application.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Meshes.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Texture.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"
#include "Shaders/LitColorShader.h"
#include "Shaders/LitTextureShader.h"
#include "Shaders/UnlitColorShader.h"

namespace LGE
{
	DemoScene::DemoScene()
	{
		m_QuadVb = new VertexBuffer(Meshes::QUAD_VERTICES, sizeof(Meshes::QUAD_VERTICES));
		m_CubeVb = new VertexBuffer(Meshes::CUBE_VERTICES, sizeof(Meshes::CUBE_VERTICES));
		m_QuadIb = new IndexBuffer(Meshes::QUAD_INDICES, Meshes::QUAD_INDEX_COUNT);
		m_CubeIb = new IndexBuffer(Meshes::CUBE_INDICES, Meshes::CUBE_INDEX_COUNT);

		VertexBufferLayout layout;
		layout.PushFloat3(); // position
		layout.PushFloat3(); // normal
		layout.PushFloat2(); // texcoord

		m_QuadVao = new VertexArray(*m_QuadVb, *m_QuadIb, layout);
		m_CubeVao = new VertexArray(*m_CubeVb, *m_CubeIb, layout);

		m_LitColorShader = new ShaderProgram(Shaders::LitColor::VERTEX, Shaders::LitColor::FRAGMENT, "LitColor");
		m_LitTextureShader = new ShaderProgram(Shaders::LitTexture::VERTEX, Shaders::LitTexture::FRAGMENT, "LitTexture");
		m_UnlitColorShader = new ShaderProgram(Shaders::UnlitColor::VERTEX, Shaders::UnlitColor::FRAGMENT, "UnlitColor");

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
		delete m_QuadVao;
		delete m_CubeVao;
		delete m_QuadIb;
		delete m_CubeIb;
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
		float aspectRatio = (float)Application::Get().GetWindowWidth() / (float)Application::Get().GetWindowHeight();
		m_Camera.ProjectionMatrix = glm::perspective(glm::radians(m_Camera.Fov), aspectRatio, m_Camera.Near, m_Camera.Far);
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

	// Rotations follow the right-hand rule: point right thumb along the positive axis,
	// fingers curl in the positive (CCW) rotation direction.
	//   +X (pitch): CCW when looking left  (toward -X)
	//   +Y (yaw):   CCW when looking down  (toward -Y)
	//   +Z (roll):  CCW when looking back  (toward +Z, opposite of forward)
	//
	// glm::rotate post-multiplies, so the last call in code is applied first to vertices.
	// Rotation order applied to vertices: Y -> X -> Z (yaw -> pitch -> roll)
	glm::mat4 DemoScene::BuildTransformModelMatrix(const Transform& t) const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, t.Pos);
		model = glm::rotate(model, glm::radians(t.Rot.z), WORLD_FORWARD); // roll
		model = glm::rotate(model, glm::radians(t.Rot.x), WORLD_RIGHT); // pitch
		model = glm::rotate(model, glm::radians(t.Rot.y), WORLD_UP); // yaw
		model = glm::scale(model, t.Scale);
		return model;
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
		int i = 0;
		char name[64];
		for (i = 0; i < m_Torches.size(); i++)
		{
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

		SetCampfireLightUniforms(shader, &i);

		shader->SetUniform1i("u_PointLightCount", i);
	}

	void DemoScene::SetCameraUniforms(ShaderProgram* shader) const
	{
		shader->SetUniform3f("u_ViewPos", m_Camera.Pos.x, m_Camera.Pos.y, m_Camera.Pos.z);
		shader->SetUniformMatrix4f("u_View", m_Camera.ViewMatrix);
		shader->SetUniformMatrix4f("u_Projection", m_Camera.ProjectionMatrix);
	}

	void DemoScene::Render()
	{
		RenderGround();
		RenderBoundWalls();
		RenderLoglBoxes();
		RenderTorches();
		RenderCampfire();
	}

	void DemoScene::RenderGround() const
	{
		constexpr float tiling = 5.0f;
		constexpr float specularIntensity = 0.05f;
		constexpr float shininess = 8.0f;

		m_LitTextureShader->Bind();

		// Camera
		SetCameraUniforms(m_LitTextureShader);

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
		m_QuadVao->Bind();

		glm::mat4 groundModel = glm::mat4(1.0f);
		groundModel = glm::rotate(groundModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		groundModel = glm::scale(groundModel, glm::vec3(m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", groundModel);

		glDrawElements(GL_TRIANGLES, m_QuadIb->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void DemoScene::RenderBoundWalls() const
	{
		constexpr float height = 2.0f;
		constexpr glm::vec2 tiling = glm::vec2(16.0f, 2.0f);
		constexpr float shininess = 32.0f;

		m_LitTextureShader->Bind();

		// Camera
		SetCameraUniforms(m_LitTextureShader);

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
		m_CubeVao->Bind();

		// North
		glm::mat4 wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height / 2.0f, -m_GroundScale / 2.0f));
		wallModel = glm::scale(wallModel, glm::vec3(m_GroundScale, height, 0.2f));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);

		// South
		wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height / 2.0f, m_GroundScale / 2.0f));
		wallModel = glm::scale(wallModel, glm::vec3(m_GroundScale, height, 0.2f));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);

		// West
		wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(-m_GroundScale / 2.0f, height / 2.0f, 0.0f));
		wallModel = glm::scale(wallModel, glm::vec3(0.2f, height, m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);

		// East
		wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(m_GroundScale / 2.0f, height / 2.0f, 0.0f));
		wallModel = glm::scale(wallModel, glm::vec3(0.2f, height, m_GroundScale));
		m_LitTextureShader->SetUniformMatrix4f("u_Model", wallModel);
		glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void DemoScene::RenderLoglBoxes() const
	{
		constexpr float shininess = 32.0f;

		constexpr Transform transforms[] = {
			// --- Entry corridor (lining the path from spawn toward campfire) ---
			// Left side
			{ glm::vec3(-2.0f, 0.5f,  6.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-2.0f, 0.5f,  4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-2.0f, 0.5f,  2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-2.0f, 0.5f,  0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Right side
			{ glm::vec3(2.0f, 0.5f,  6.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(2.0f, 0.5f,  4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(2.0f, 0.5f,  2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(2.0f, 0.5f,  0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },

			// --- North-west ruined arch ---
			// Left pillar
			{ glm::vec3(-9.0f, 0.5f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-9.0f, 1.5f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Right pillar
			{ glm::vec3(-7.0f, 0.5f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-7.0f, 1.5f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Lintel across the top
			{ glm::vec3(-8.0f, 2.5f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Rubble scattered around base of arch
			{ glm::vec3(-10.0f, 0.5f, -7.5f), glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-6.0f, 0.5f, -8.5f), glm::vec3(0.0f,-15.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-9.5f, 0.5f,-10.0f), glm::vec3(0.0f, 35.0f, 0.0f), glm::vec3(1.0f) },

			// --- North-east watchtower platform ---
			// Four corner pillars
			{ glm::vec3(10.0f, 0.5f,  -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(10.0f, 1.5f,  -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 0.5f,  -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 1.5f,  -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(10.0f, 0.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(10.0f, 1.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 0.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 1.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Platform top — open flat surface on top of pillars
			{ glm::vec3(10.0f, 2.5f,  -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(11.0f, 2.5f,  -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 2.5f,  -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(10.0f, 2.5f,  -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(11.0f, 2.5f,  -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 2.5f,  -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(10.0f, 2.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(11.0f, 2.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 2.5f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Rubble at base
			{ glm::vec3(9.0f, 0.5f,  -7.5f), glm::vec3(0.0f, 25.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(13.0f, 0.5f,  -9.0f), glm::vec3(0.0f,-20.0f, 0.0f), glm::vec3(1.0f) },

			// --- South-east staircase pyramid ---
			// Base layer 3x1
			{ glm::vec3(10.0f, 0.5f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(11.0f, 0.5f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(12.0f, 0.5f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Mid layer 2x1
			{ glm::vec3(10.0f, 1.5f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(11.0f, 1.5f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },
			// Top layer 1x1
			{ glm::vec3(10.0f, 2.5f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f) },

			// --- South-west lone pillar with collapsed top ---
			{ glm::vec3(-10.0f, 0.5f, 4.0f), glm::vec3(0.0f,  0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-10.0f, 1.5f, 4.0f), glm::vec3(0.0f,  0.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-10.0f, 2.5f, 4.0f), glm::vec3(0.0f,  0.0f, 0.0f), glm::vec3(1.0f) },
			// Collapsed block leaning off to the side
			{ glm::vec3(-9.0f, 0.5f, 4.5f), glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(1.0f) },
			{ glm::vec3(-11.0f, 0.5f, 3.5f), glm::vec3(0.0f,-25.0f, 0.0f), glm::vec3(1.0f) },
		};

		m_LitTextureShader->Bind();

		SetCameraUniforms(m_LitTextureShader);
		SetLightingUniforms(m_LitTextureShader);

		m_LoglBoxTexture->Bind(0);
		m_LoglBoxSpecularTexture->Bind(1);
		m_LitTextureShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_LitTextureShader->SetUniform1i("u_Material.specularMap", 1);
		m_LitTextureShader->SetUniform2f("u_Tiling", 1.0f, 1.0f);
		m_LitTextureShader->SetUniform1i("u_Material.useSpecularMap", 1);
		m_LitTextureShader->SetUniform1f("u_Material.shininess", shininess);

		m_CubeVao->Bind();

		for (const Transform& t : transforms)
		{
			m_LitTextureShader->SetUniformMatrix4f("u_Model", BuildTransformModelMatrix(t));
			glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void DemoScene::RenderTorches() const
	{
		// Sticks
		m_LitColorShader->Bind();
		
		SetCameraUniforms(m_LitColorShader);

		SetLightingUniforms(m_LitColorShader);

		m_CubeVao->Bind();

		for (const Torch& torch : m_Torches)
		{
			glm::mat4 stickModel = glm::translate(glm::mat4(1.0f), torch.pos + glm::vec3(0.0f, 0.4f, 0.0f));
			stickModel = glm::scale(stickModel, glm::vec3(0.08f, 0.8f, 0.08f));
			m_LitColorShader->SetUniform3f("u_Material.color", 0.45f, 0.3f, 0.15f);
			m_LitColorShader->SetUniform1f("u_Material.specularIntensity", 0.1f);
			m_LitColorShader->SetUniform1f("u_Material.shininess", 8.0f);
			m_LitColorShader->SetUniformMatrix4f("u_Model", stickModel);
			glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);
		}

		// Heads
		m_UnlitColorShader->Bind();

		SetCameraUniforms(m_UnlitColorShader);

		m_CubeVao->Bind();

		for (const Torch& torch : m_Torches)
		{
			m_UnlitColorShader->SetUniform3f("u_Material.color", torch.color.x, torch.color.y, torch.color.z);
			glm::mat4 headModel = glm::translate(glm::mat4(1.0f), torch.pos + glm::vec3(0.0f, 0.85f, 0.0f));
			headModel = glm::scale(headModel, glm::vec3(0.12f, 0.12f, 0.12f));
			m_UnlitColorShader->SetUniformMatrix4f("u_Model", headModel);
			glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void DemoScene::SetCampfireLightUniforms(ShaderProgram* shader, int* i) const
	{
		glm::mat4 parentMatrix = BuildTransformModelMatrix(m_Campfire.Transform);
		char name[64];
		glm::vec3 worldPos;

		// Campfire light A — main, center, warm orange, strongest
		worldPos = glm::vec3(parentMatrix * glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
		snprintf(name, sizeof(name), "u_PointLights[%d].pos", *i);
		shader->SetUniform3f(name, worldPos.x, worldPos.y, worldPos.z);
		snprintf(name, sizeof(name), "u_PointLights[%d].color", *i);
		shader->SetUniform3f(name, 1.0f, 0.50f, 0.12f);
		snprintf(name, sizeof(name), "u_PointLights[%d].linear", *i);
		shader->SetUniform1f(name, 0.30f);
		snprintf(name, sizeof(name), "u_PointLights[%d].quadratic", *i);
		shader->SetUniform1f(name, 0.35f);
		*i += 1;

		// Campfire light B — secondary, offset right, cooler orange, weaker
		worldPos = glm::vec3(parentMatrix * glm::vec4(0.2f, 0.35f, 0.1f, 1.0f));
		snprintf(name, sizeof(name), "u_PointLights[%d].pos", *i);
		shader->SetUniform3f(name, worldPos.x, worldPos.y, worldPos.z);
		snprintf(name, sizeof(name), "u_PointLights[%d].color", *i);
		shader->SetUniform3f(name, 1.0f, 0.38f, 0.05f);
		snprintf(name, sizeof(name), "u_PointLights[%d].linear", *i);
		shader->SetUniform1f(name, 0.50f);
		snprintf(name, sizeof(name), "u_PointLights[%d].quadratic", *i);
		shader->SetUniform1f(name, 0.60f);
		*i += 1;

		// Campfire light C — tertiary, offset left, redder, weakest
		worldPos = glm::vec3(parentMatrix * glm::vec4(-0.15f, 0.30f, -0.1f, 1.0f));
		snprintf(name, sizeof(name), "u_PointLights[%d].pos", *i);
		shader->SetUniform3f(name, worldPos.x, worldPos.y, worldPos.z);
		snprintf(name, sizeof(name), "u_PointLights[%d].color", *i);
		shader->SetUniform3f(name, 0.9f, 0.25f, 0.04f);
		snprintf(name, sizeof(name), "u_PointLights[%d].linear", *i);
		shader->SetUniform1f(name, 0.60f);
		snprintf(name, sizeof(name), "u_PointLights[%d].quadratic", *i);
		shader->SetUniform1f(name, 0.80f);
		*i += 1;
	}

	void DemoScene::RenderCampfire() const
	{
		glm::mat4 parentMatrix = BuildTransformModelMatrix(m_Campfire.Transform);

		// --- Woods ---
		constexpr glm::vec3 woodColor = { 0.32f, 0.18f, 0.07f };
		constexpr float woodSpecular = 0.05f;
		constexpr float woodShininess = 4.0f;

		constexpr Transform woods[] = {
			{ glm::vec3(0.0f,  0.07f,  0.12f), glm::vec3(0.0f,  12.0f, 0.0f), glm::vec3(1.2f, 0.14f, 0.20f) },
			{ glm::vec3(0.10f, 0.07f, -0.05f), glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(1.2f, 0.14f, 0.20f) },
			{ glm::vec3(-0.08f, 0.22f,  0.05f), glm::vec3(6.0f,  55.0f, 5.0f), glm::vec3(1.0f, 0.14f, 0.18f) },
		};

		m_LitColorShader->Bind();
		SetCameraUniforms(m_LitColorShader);
		SetLightingUniforms(m_LitColorShader);
		m_LitColorShader->SetUniform3f("u_Material.color", woodColor.x, woodColor.y, woodColor.z);
		m_LitColorShader->SetUniform1f("u_Material.specularIntensity", woodSpecular);
		m_LitColorShader->SetUniform1f("u_Material.shininess", woodShininess);

		m_CubeVao->Bind();

		for (const Transform& wood : woods)
		{
			m_LitColorShader->SetUniformMatrix4f("u_Model", parentMatrix * BuildTransformModelMatrix(wood));
			glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);
		}

		// --- Embers ---
		struct Ember { Transform transform; glm::vec3 color; };

		constexpr Ember embers[] = {
			// Far outer ring
			{ { glm::vec3(0.55f, 0.04f,  0.30f), glm::vec3(0.0f,  18.0f, 0.0f), glm::vec3(0.32f, 0.07f, 0.08f) }, { 1.0f, 0.30f, 0.0f } }, // deep red orange
			{ { glm::vec3(-0.50f, 0.04f,  0.40f), glm::vec3(0.0f,  72.0f, 0.0f), glm::vec3(0.30f, 0.07f, 0.08f) }, { 1.0f, 0.40f, 0.0f } }, // orange
			{ { glm::vec3(0.45f, 0.04f, -0.50f), glm::vec3(0.0f,  48.0f, 0.0f), glm::vec3(0.34f, 0.07f, 0.08f) }, { 1.0f, 0.50f, 0.0f } }, // mid orange
			{ { glm::vec3(-0.55f, 0.04f, -0.35f), glm::vec3(0.0f, 115.0f, 0.0f), glm::vec3(0.28f, 0.07f, 0.08f) }, { 0.9f, 0.25f, 0.0f } }, // dark ember
			{ { glm::vec3(-0.25f, 0.04f, -0.55f), glm::vec3(0.0f,  35.0f, 0.0f), glm::vec3(0.30f, 0.07f, 0.07f) }, { 0.95f, 0.28f, 0.0f } }, // dark orange
			// Mid ring
			{ { glm::vec3(0.38f, 0.06f, -0.12f), glm::vec3(0.0f,  88.0f, 0.0f), glm::vec3(0.28f, 0.07f, 0.07f) }, { 1.0f, 0.65f, 0.02f } }, // orange yellow
			{ { glm::vec3(-0.15f, 0.06f,  0.35f), glm::vec3(0.0f, 135.0f, 0.0f), glm::vec3(0.26f, 0.07f, 0.07f) }, { 1.0f, 0.75f, 0.03f } }, // bright orange
			// Core
			{ { glm::vec3(0.02f, 0.35f,  0.03f), glm::vec3(0.0f,  22.0f, 0.0f), glm::vec3(0.22f, 0.08f, 0.09f) }, { 1.0f, 0.92f, 0.15f } }, // bright yellow
		};

		m_UnlitColorShader->Bind();
		m_CubeVao->Bind();

		for (const Ember& ember : embers)
		{
			m_UnlitColorShader->SetUniform3f("u_Material.color", ember.color.x, ember.color.y, ember.color.z);
			m_UnlitColorShader->SetUniformMatrix4f("u_Model", parentMatrix * BuildTransformModelMatrix(ember.transform));
			glDrawElements(GL_TRIANGLES, m_CubeIb->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}
}
