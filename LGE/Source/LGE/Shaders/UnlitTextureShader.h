#pragma once

namespace LGE::Shaders::UnlitTexture
{
	// Shader
	constexpr const char* VERTEX = R"(
		#version 330 core
		
		// attributes
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec2 a_TexCoord;
		
		out vec2 v_TexCoord;
		
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		uniform vec2 u_Tiling = vec2(1.0, 1.0);
		uniform vec2 u_Offset = vec2(0.0, 0.0);
		
		void main()
		{
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
			v_TexCoord = a_TexCoord * u_Tiling + u_Offset;
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core
		
		out vec4 fragColor; // obligatory fragment color output
		
		in vec2 v_TexCoord;
		
		uniform sampler2D u_Texture;
		
		void main()
		{
			fragColor = texture(u_Texture, v_TexCoord);
		}
	)";
}
