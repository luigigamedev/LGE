#pragma once

// Used in learnopengl Multiple lights

namespace LGE::Shaders::MultipleLights
{
    constexpr const char* VERTEX = R"(
		#version 330 core
		
		// attributes
		layout(location = 0) in vec3 a_Pos;
		layout(location = 1) in vec3 a_Normal;
        layout(location = 2) in vec2 a_TexCoords;
		
		out vec3 v_FragPos;	
		out vec3 v_Normal;
        out vec2 v_TexCoords;
	
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
			v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;  
            v_TexCoords = a_TexCoords;
    
			gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
		}
	)";

    constexpr const char* FRAGMENT = R"(
		#version 330 core

        out vec4 f_Color; // Frag color
        
        struct Material {
            sampler2D diffuse; // diffuse map (a texture)
            sampler2D specular; // specular map
            float shininess;
        }; 
        
        struct DirLight {
            vec3 direction;
        	
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };
        
        struct PointLight {
            vec3 position;
            
            float constant;
            float linear;
            float quadratic;
        	
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };
        
        struct SpotLight {
            vec3 position;
            vec3 direction;
            float cutOff;
            float outerCutOff;
          
            float constant;
            float linear;
            float quadratic;
          
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;       
        };
        
        #define NR_POINT_LIGHTS 4
        
        in vec3 v_FragPos;
        in vec3 v_Normal;
        in vec2 v_TexCoords;
        
        uniform vec3 u_ViewPos;
        uniform DirLight u_DirLight;
        uniform PointLight u_PointLights[NR_POINT_LIGHTS];
        uniform SpotLight u_SpotLight;
        uniform Material u_Material;
        
        // function prototypes
        vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
        vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
        vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
        
        void main()
        {    
            // properties
            vec3 norm = normalize(v_Normal);
            vec3 viewDir = normalize(u_ViewPos - v_FragPos);
            
            // == =====================================================
            // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
            // For each phase, a calculate function is defined that calculates the corresponding color
            // per lamp. In the main() function we take all the calculated colors and sum them up for
            // this fragment's final color.
            // == =====================================================
            // phase 1: directional lighting
            vec3 result = CalcDirLight(u_DirLight, norm, viewDir);
            // phase 2: point lights
            for(int i = 0; i < NR_POINT_LIGHTS; i++)
                result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir);    
            // phase 3: spot light
            result += CalcSpotLight(u_SpotLight, norm, v_FragPos, viewDir);    
            
            f_Color = vec4(result, 1.0);
        }
        
        // calculates the color when using a directional light.
        vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
        {
            vec3 lightDir = normalize(-light.direction);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            // combine results
            vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));
            return (ambient + diffuse + specular);
        }
        
        // calculates the color when using a point light.
        vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            // attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
            // combine results
            vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
            return (ambient + diffuse + specular);
        }
        
        // calculates the color when using a spot light.
        vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            // attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
            // spotlight intensity
            float theta = dot(lightDir, normalize(-light.direction)); 
            float epsilon = light.cutOff - light.outerCutOff;
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
            // combine results
            vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));
            ambient *= attenuation * intensity;
            diffuse *= attenuation * intensity;
            specular *= attenuation * intensity;
            return (ambient + diffuse + specular);
        } 
	)";
}
