#version 330 core

in vec4 VertexColor;
out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = VertexColor * u_Color;
}
