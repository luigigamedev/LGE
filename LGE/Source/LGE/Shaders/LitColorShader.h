#pragma once

namespace LGE::Shaders::LitColor
{
	constexpr const char* VERTEX = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Pos;
		layout(location = 1) in vec3 a_Normal;
		
		out vec3 v_FragPos;
		out vec3 v_Normal;
			
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
			v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
		    
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core

		out vec4 f_Color;

		in vec3 v_FragPos;
		in vec3 v_Normal;
		
		struct Material 
		{
		    vec3 color;
		    float specularIntensity;
		    float shininess;
		};
		
		struct DirectionalLight
		{
			vec3 dir;
			vec3 color;
		};

		struct PointLight
		{
		    vec3 pos;
		    vec3 color;
		    
		    float constant;
		    float linear;
		    float quadratic;
		};
		
		uniform vec3 u_ViewPos;
		uniform vec3 u_AmbientColor;
		uniform DirectionalLight u_DirectionalLight;		
		uniform PointLight u_PointLight;
		uniform Material u_Material;
		
		// --- Function prototypes ---
		vec3 CalcDirectionalLight(vec3 norm, vec3 viewDir, vec3 baseColor, vec3 specSurface);
		vec3 CalcPointLight(vec3 norm, vec3 viewDir, vec3 baseColor, vec3 specSurface);

		void main()
		{
			// --- Shared surface data, computed once ---
			vec3 baseColor = u_Material.color;
			vec3 specSurface = vec3(u_Material.specularIntensity);

			vec3 norm = normalize(v_Normal);
			vec3 viewDir = normalize(u_ViewPos - v_FragPos);
		
		    // --- Ambient: global scene light, not tied to any light source ---
			vec3 ambient = u_AmbientColor * baseColor;

			// --- Light contributions ---
			vec3 result = ambient
			    + CalcDirectionalLight(norm, viewDir, baseColor, specSurface)
			    + CalcPointLight(norm, viewDir, baseColor, specSurface);

			f_Color = vec4(result, 1.0);
		}

		vec3 CalcDirectionalLight(vec3 norm, vec3 viewDir, vec3 baseColor, vec3 specSurface)
		{
		    vec3 lightDir = normalize(-u_DirectionalLight.dir);
		
		    float diff = max(dot(norm, lightDir), 0.0);
		
		    vec3 reflectDir = reflect(-lightDir, norm);
		    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
		
		    vec3 diffuse  = u_DirectionalLight.color * diff * baseColor;
		    vec3 specular = u_DirectionalLight.color * specSurface * spec;
		
		    return diffuse + specular;
		}
		
		vec3 CalcPointLight(vec3 norm, vec3 viewDir, vec3 baseColor, vec3 specSurface)
		{
		    vec3 lightDir = normalize(u_PointLight.pos - v_FragPos);
		
		    float diff = max(dot(norm, lightDir), 0.0);
		
		    vec3 reflectDir = reflect(-lightDir, norm);
		    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
		
		    float dist = length(u_PointLight.pos - v_FragPos);
		    float attenuation = 1.0 / (u_PointLight.constant + u_PointLight.linear * dist + u_PointLight.quadratic * dist * dist);
		
		    vec3 diffuse = u_PointLight.color * diff * baseColor * attenuation;
		    vec3 specular = u_PointLight.color * specSurface * spec * attenuation;
		
		    return diffuse + specular;
		}
	)";
}
