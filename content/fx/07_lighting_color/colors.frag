#version 330 core

out vec4 FragColor;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;

void main()
{
    FragColor = vec4(u_ObjectColor.rgb * u_LightColor.rgb, 1.0);
}
