#pragma once

namespace LGE::Shaders::Materials
{
	constexpr const char* VERTEX = R"(
		#version 330 core
		
		// attributes
		layout(location = 0) in vec3 a_Pos;
		layout(location = 2) in vec3 a_Normal;
		
		out vec3 v_FragPos;	
		out vec3 v_Normal;
	
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
			v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;  
    
			gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core
        
        out vec4 f_Color;
        
        struct Material {
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;    
            float shininess;
        }; 
        
        struct Light {
            vec3 position;
        
            // intensity
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };
        
        in vec3 v_FragPos;  
        in vec3 v_Normal;  
          
        uniform vec3 u_ViewPos;
        uniform Material u_Material;
        uniform Light u_Light;
        
        void main()
        {
            // ambient
            vec3 ambient = u_Light.ambient * u_Material.ambient;
          	
            // diffuse 
            vec3 norm = normalize(v_Normal);
            vec3 lightDir = normalize(u_Light.position - v_FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);
            
            // specular
            vec3 viewDir = normalize(u_ViewPos - v_FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            vec3 specular = u_Light.specular * (spec * u_Material.specular);  
                
            vec3 result = ambient + diffuse + specular;
            f_Color = vec4(result, 1.0);
        } 
	)";
}
