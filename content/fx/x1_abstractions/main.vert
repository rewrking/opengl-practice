#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;

out vec2 TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_Pos, 1.0);
    // VertexColor = a_Color;
    TexCoord = a_TexCoord;
}
