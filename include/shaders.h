// GLSL source code for shaders

#ifndef SHADERS_H
#define SHADERS_H

namespace shaders
{
    const char *vertexSource = R"glsl(
		#version 150 core

		in vec2 position;
		//in vec3 color;
		in vec4 color;
		in vec2 texcoord;

		//out vec3 Color;
		out vec4 Color;
		out vec2 Texcoord;

		void main()
		{
			Color = color;
			Texcoord = texcoord;
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)glsl";

    const char *fragmentSource = R"glsl(
		#version 150 core
		
		//in vec3 Color;
		in vec4 Color;
		in vec2 Texcoord;
		
		out vec4 outColor;
		uniform sampler2D tex;
		
		void main()
		{
			//outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
			outColor = texture(tex, Texcoord) * Color;
		}
	)glsl";
}

#endif /* SHADERS_H */
