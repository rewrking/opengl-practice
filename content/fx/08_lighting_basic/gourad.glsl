#pragma type : vertex
#version 330 core

out vec3 v_LightingColor;

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_ProjectionViewModel;
uniform mat4 u_ViewModel;
uniform mat4 u_View;
uniform mat3 u_NormalMatrix;
uniform vec4 u_LightColor;

uniform vec3 u_LightPos;

#define AMBIENT_STRENGTH 0.5
#define SPECULAR_STRENGTH 0.5 // this is set higher to better show the effect of Gouraud shading
#define SHININESS 32

void main()
{
    gl_Position = u_ProjectionViewModel * vec4(a_Pos, 1.0);

    vec4 pos = vec4(a_Pos, 1.0);
    vec3 Position = vec3(u_ViewModel * pos);
    vec3 Normal = u_NormalMatrix * a_Normal;
    vec3 LightPos = vec3(u_View * vec4(u_LightPos, 1.0));

    // ambient
    vec3 ambient = AMBIENT_STRENGTH * u_LightColor.rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor.rgb;

    // specular
    vec3 viewDir = normalize(-Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);
    vec3 specular = SPECULAR_STRENGTH * spec * u_LightColor.rgb;

    v_LightingColor = ambient + diffuse + specular;

}

#pragma type : fragment
#version 330 core

out vec4 FragColor;

in vec3 v_LightingColor;

uniform vec4 u_ObjectColor;

void main()
{
    FragColor = vec4(v_LightingColor * u_ObjectColor.rgb, 1.0);
}
