#pragma once

namespace LGE::Shaders::UnlitColor
{
	constexpr const char* VERTEX = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Pos;
				
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core
		
		out vec4 f_Color;

		struct Material 
		{
		    vec3 color;
		};

		uniform Material u_Material;
				
		void main()
		{
			f_Color = vec4(u_Material.color, 1.0);
		}
	)";
}
