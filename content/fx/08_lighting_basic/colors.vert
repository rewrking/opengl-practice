#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
    vec4 pos = vec4(a_Pos, 1.0);
    gl_Position = u_Projection * u_View * u_Model * pos;
    v_FragPos = vec3(u_Model * pos);
    v_Normal = a_Normal;
}
