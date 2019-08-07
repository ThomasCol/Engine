#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 TRS;
out vec2 UV;

void 	main()
{
	gl_Position = TRS * vec4(position, 1.0);
	
	UV = uv;
}
