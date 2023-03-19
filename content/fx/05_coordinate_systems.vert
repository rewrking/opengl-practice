#version 330 core

layout (location = 0) in vec3 a_Pos;
// layout (location = 1) in vec4 a_Color;
layout (location = 1) in vec2 a_TexCoord;

// out vec4 VertexColor;
out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
    // VertexColor = a_Color;
    TexCoord = a_TexCoord;
}
