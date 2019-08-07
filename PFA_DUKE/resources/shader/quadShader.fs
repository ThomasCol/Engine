#version 330 core
out vec4 color;
in vec2 TexCoords;

uniform vec4 Color;
uniform bool isText;
uniform sampler2D text;

void main()
{    
    vec4 sampled;
    if (isText)
    	sampled = texture(text, TexCoords);
    else
        sampled = vec4(1);
    color = Color * sampled;
}  
