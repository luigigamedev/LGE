#pragma once

namespace LGE::Shaders::LitTexture
{
	constexpr const char* VERTEX = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Pos;
		layout(location = 1) in vec3 a_Normal;
		layout(location = 2) in vec2 a_TexCoords;
		
		out vec3 v_FragPos;	
		out vec3 v_Normal;
		out vec2 v_TexCoords;
			
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		uniform vec2 u_Tiling = vec2(1.0, 1.0);
		uniform vec2 u_Offset = vec2(0.0, 0.0);
		
		void main()
		{
			v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
		    v_TexCoords = a_TexCoords * u_Tiling + u_Offset;
			v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
		    
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core

		#define MAX_POINT_LIGHTS 8

		const float ATTENUATION_CONSTANT = 1.0;
		
		out vec4 f_Color;

		in vec3 v_FragPos;
		in vec3 v_Normal;
		in vec2 v_TexCoords;
		
		struct Material 
		{
		    sampler2D diffuseMap;
			sampler2D specularMap;
			bool useSpecularMap;
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
		    
		    float linear;
		    float quadratic;
		};
		
		uniform vec3 u_ViewPos;
		uniform vec3 u_AmbientColor;
		uniform DirectionalLight u_DirectionalLight;		
		uniform int u_PointLightCount;
		uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
		uniform Material u_Material;
		
		// --- Function prototypes ---
		vec3 CalcDirectionalLight(vec3 norm, vec3 viewDir, vec3 baseColor, vec3 specSurface);
		vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 baseColor, vec3 specSurface);

		void main()
		{
			// --- Shared surface data, computed once ---
			vec3 baseColor  = vec3(texture(u_Material.diffuseMap, v_TexCoords));
			vec3 specSurface = u_Material.useSpecularMap 
			    ? vec3(texture(u_Material.specularMap, v_TexCoords)) 
			    : vec3(u_Material.specularIntensity);

			vec3 norm = normalize(v_Normal);
			vec3 viewDir = normalize(u_ViewPos - v_FragPos);
		
		    // --- Ambient: global scene light, not tied to any light source ---
			vec3 ambient = u_AmbientColor * baseColor;

			// --- Light contributions ---
			vec3 result = ambient + CalcDirectionalLight(norm, viewDir, baseColor, specSurface);
			
			for (int i = 0; i < u_PointLightCount; i++)
				result += CalcPointLight(u_PointLights[i], norm, viewDir, baseColor, specSurface);

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
		
		vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 baseColor, vec3 specSurface)
		{
		    vec3 lightDir = normalize(light.pos - v_FragPos);
		
		    float diff = max(dot(norm, lightDir), 0.0);
		
		    vec3 reflectDir = reflect(-lightDir, norm);
		    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
		
		    float dist = length(light.pos - v_FragPos);
		    float attenuation = 1.0 / (ATTENUATION_CONSTANT + light.linear * dist + light.quadratic * dist * dist);
		
		    vec3 diffuse = light.color * diff * baseColor * attenuation;
		    vec3 specular = light.color * specSurface * spec * attenuation;
		
		    return diffuse + specular;
		}
	)";
}
