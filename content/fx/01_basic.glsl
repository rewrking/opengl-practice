#pragma type : vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

void main()
{
    gl_Position = vec4(a_Pos.xyz, 1.0);
}

#pragma type : fragment
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
