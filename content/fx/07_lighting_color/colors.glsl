#pragma type : vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

#pragma type : fragment
#version 330 core

out vec4 FragColor;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;

void main()
{
    FragColor = vec4(u_ObjectColor.rgb * u_LightColor.rgb, 1.0);
}
