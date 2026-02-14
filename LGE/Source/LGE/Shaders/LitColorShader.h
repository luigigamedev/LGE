#pragma once

namespace LGE::Shaders::LitColor
{
	constexpr const char* VERTEX = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Pos;
		layout(location = 1) in vec3 a_Normal;
		
		out vec3 v_Normal;
			
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
		    
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core

		out vec4 f_Color;
		
		in vec3 v_Normal;
		
		struct DirectionalLight
		{
		    vec3 dir;
		    vec3 color;
		};
		
		uniform vec3 u_AmbientColor;
		uniform DirectionalLight u_DirectionalLight;
		uniform vec3 u_Color; // Object color
		
		void main()
		{
		    vec3 ambient = u_AmbientColor * u_Color;
		    
		    vec3 norm = normalize(v_Normal);
		    vec3 lightDir = normalize(-u_DirectionalLight.dir);
		    vec3 diffuse = max(dot(norm, lightDir), 0.0) * u_DirectionalLight.color * u_Color;
		    
		    f_Color = vec4(ambient + diffuse, 1.0);
		}
	)";
}
