#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 UV;
out vec3 outNormal;
out vec3 fragPos;

uniform float Tile;
uniform mat4 TRS;
uniform mat4 model;
uniform mat4 proj;

void 	main()
{
	gl_Position = proj * inverse(model) * TRS * vec4(position, 1.0);

	/* set pos of fragment */
	fragPos = vec3(TRS * vec4(position, 1.0));

	UV = uv * Tile;

	outNormal = vec3(TRS * vec4(normal, 0.0));
}
