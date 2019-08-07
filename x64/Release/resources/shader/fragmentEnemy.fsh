#version 330 core

out vec4 color;

in vec2 UV;

uniform sampler2D sampler;

void main()
{
    vec4 textColor = texture(sampler, UV);

    /* apply alpha blending */
	if (textColor.a == 0)
		discard;

	color = textColor;
}
