#pragma once

namespace LGE::Shaders::LitTexture
{
	constexpr const char* VERTEX = R"(
		#version 330 core

		// attributes
		layout(location = 0) in vec3 a_Pos;
		layout(location = 1) in vec3 a_Normal;
		layout(location = 2) in vec2 a_TexCoords;
		
		out vec2 v_TexCoords;
			
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
		    v_TexCoords = a_TexCoords;
		    
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core

		out vec4 f_Color; // frag color
		
		struct Material 
		{
		    sampler2D baseMap; // diffuse map (a texture)
		};
		
		in vec2 v_TexCoords;
		
		uniform vec3 u_AmbientColor;
		uniform Material u_Material;
		
		void main()
		{
		    float ambientStrength = 1.0f;
		    vec3 ambient = ambientStrength * u_AmbientColor;
		
		    vec3 result = ambient * vec3(texture(u_Material.baseMap, v_TexCoords));
			
			f_Color = vec4(result, 1.0);
		}
	)";
}
