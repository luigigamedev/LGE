#pragma once

// Used in learnopengl Light casters chapter part 1: directional light

namespace LGE::Shaders::LightCasters1
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
        
        out vec4 f_Color;
        
        struct Material {
            sampler2D diffuse; // diffuse map (a texture)
            sampler2D specular; // specular map    
            float shininess;
        }; 
        
        struct Light {
            // vec3 position; // no longer necessary when using directional lights.
            vec3 direction;
        
            // intensity
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };
        
        in vec3 v_FragPos;  
        in vec3 v_Normal;  
        in vec2 v_TexCoords;
          
        uniform vec3 u_ViewPos;
        uniform Material u_Material;
        uniform Light u_Light;
        
        void main()
        {
            // ambient
            vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, v_TexCoords).rgb;
          	
            // diffuse 
            vec3 norm = normalize(v_Normal);
            vec3 lightDir = normalize(-u_Light.direction);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, v_TexCoords).rgb;
            
            // specular
            vec3 viewDir = normalize(u_ViewPos - v_FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            vec3 specular = u_Light.specular * spec * texture(u_Material.specular, v_TexCoords).rgb;  
                
            f_Color = vec4(ambient + diffuse + specular, 1.0);
        } 
	)";
}
