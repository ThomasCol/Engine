#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 UV;

uniform mat4 TRS;
uniform mat4 model;
uniform mat4 proj;

uniform vec3 hitPos;

void 	main()
{
	gl_Position = proj * inverse(model) * TRS * vec4(position, 1.0);

	UV = uv;
}
