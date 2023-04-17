#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_ProjectionViewModel;
uniform mat4 u_ViewModel;
uniform mat4 u_View;
uniform mat3 u_NormalMatrix;

uniform vec3 u_LightPos;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_LightPos;

void main()
{
    vec4 pos = vec4(a_Pos, 1.0);
    gl_Position = u_ProjectionViewModel * pos;
    v_FragPos = vec3(u_ViewModel * pos);
    v_Normal = u_NormalMatrix * a_Normal;
    v_LightPos = vec3(u_View * vec4(u_LightPos, 1.0));
}
