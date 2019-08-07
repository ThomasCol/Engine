#version 330 core
layout (location = 0) in vec2 pos; // <vec2 pos>
layout (location = 1) in vec2 texture; // <vec2 tex>
out vec2 TexCoords;

uniform vec2 size;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    TexCoords = texture;
}  
