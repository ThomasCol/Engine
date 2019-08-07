#version 330 core
layout (location = 0) in vec2 pos; // <vec2 pos>
layout (location = 1) in vec2 UV; // <vec2 pos>
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(pos, 0.1, 1.0);
    TexCoords = UV;
}  
