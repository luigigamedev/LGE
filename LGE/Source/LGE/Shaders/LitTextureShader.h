#pragma once

namespace LGE::Shaders::LitTexture
{
	constexpr const char* VERTEX = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Pos;
		layout(location = 1) in vec3 a_Normal;
		layout(location = 2) in vec2 a_TexCoords;
		
		out vec2 v_TexCoords;
		out vec3 v_Normal;
			
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		uniform vec2 u_Tiling = vec2(1.0, 1.0);
		uniform vec2 u_Offset = vec2(0.0, 0.0);
		
		void main()
		{
		    v_TexCoords = a_TexCoords * u_Tiling + u_Offset;
			v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
		    
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core

		out vec4 f_Color;

		in vec3 v_Normal;
		in vec2 v_TexCoords;
		
		struct Material 
		{
		    sampler2D diffuseMap;
		};
		
		struct DirectionalLight
		{
			vec3 dir;
			vec3 color;
		};
		
		uniform vec3 u_AmbientColor;
		uniform DirectionalLight u_DirectionalLight;		
		uniform Material u_Material;
		
		void main()
		{
			vec3 baseColor = vec3(texture(u_Material.diffuseMap, v_TexCoords));
		
		    vec3 ambient = u_AmbientColor * baseColor;
			
			vec3 norm = normalize(v_Normal);
			vec3 lightDir = normalize(-u_DirectionalLight.dir);
			vec3 diffuse = max(dot(norm, lightDir), 0.0) * u_DirectionalLight.color * baseColor;
			
			f_Color = vec4(ambient + diffuse, 1.0);
		}
	)";
}
