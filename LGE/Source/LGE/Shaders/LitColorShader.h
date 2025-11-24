#pragma once

// // Used in learnopengl Basic Lighting chapter

namespace LGE::Shaders::LitColor
{
	constexpr const char* VERTEX = R"(
		#version 330 core
		
		// attributes
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec3 a_Normal;
		
		out vec3 v_FragPos;	
		out vec3 v_Normal;
	
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
			v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
			v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core
		
		in vec3 v_FragPos;
		in vec3 v_Normal;

		out vec4 f_Color; // obligatory fragment color output

		uniform vec3 u_Color; // object color
		uniform vec3 u_LightPos;
		uniform vec3 u_LightColor;
		uniform vec3 u_ViewPos;
				
		void main()
		{
			// ambient
			float ambientStrength = 0.1;
			vec3 ambient = ambientStrength * u_LightColor;
  	
			// diffuse 
			vec3 norm = normalize(v_Normal);
			vec3 lightDir = normalize(u_LightPos - v_FragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * u_LightColor;
			
			// specular
			float specularStrength = 0.5;
			vec3 viewDir = normalize(u_ViewPos - v_FragPos);
			vec3 reflectDir = reflect(-lightDir, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = specularStrength * spec * u_LightColor;  
			    
			vec3 result = (ambient + diffuse + specular) * u_Color;
			f_Color = vec4(result, 1.0);
		}
	)";
}
