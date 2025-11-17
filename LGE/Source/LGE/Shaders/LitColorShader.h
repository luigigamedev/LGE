#pragma once

namespace LGE::Shaders::LitColor
{
	constexpr const char* VERTEX = R"(
		#version 330 core
		
		// attributes
		layout(location = 0) in vec3 a_Position;
		layout(location = 2) in vec3 a_Normal;
		
		out vec3 v_FragPos;	
		out vec3 v_Normal;
	
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
			v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
			v_Normal = a_Normal;
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core
		
		in vec3 v_FragPos;
		in vec3 v_Normal;

		out vec4 f_Color; // obligatory fragment color output

		uniform vec3 u_Color;
		uniform float u_AmbientStrength;
		uniform vec3 u_AmbientColor;
		uniform vec3 u_DiffuseLightPos;
				
		void main()
		{
			vec3 ambient = u_AmbientStrength * u_AmbientColor;
			
			vec3 norm = normalize(v_Normal);
			vec3 lightDir = normalize(u_DiffuseLightPos - v_FragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 lightColor = u_AmbientColor;
			vec3 diffuse = diff * lightColor;
			
			f_Color = vec4((ambient + diffuse) * u_Color, 1.0);
		}
	)";
}
