#pragma type : vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;

out vec4 VertexColor;

void main()
{
    gl_Position = vec4(a_Pos, 1.0);
    VertexColor = a_Color;
}

#pragma type : fragment
#version 330 core

in vec4 VertexColor;
out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = VertexColor * u_Color;
}
