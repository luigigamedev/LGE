#pragma once

namespace LGE::Shaders::UnlitColor
{
	constexpr const char* VERTEX = R"(
		#version 330 core
		
		// attributes
		layout(location = 0) in vec3 a_Position;
				
		uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		
		void main()
		{
			gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
		}
	)";

	constexpr const char* FRAGMENT = R"(
		#version 330 core
		
		out vec4 f_Color; // obligatory fragment color output

		uniform vec3 u_Color;
				
		void main()
		{
			f_Color = vec4(u_Color, 1.0);
		}
	)";
}
