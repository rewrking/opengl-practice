#pragma type : vertex
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

#pragma type : fragment
#version 330 core

in vec3 v_Normal;
in vec3 v_FragPos;
in vec3 v_LightPos;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;

out vec4 FragColor;

#define AMBIENT_STRENGTH 0.5
#define SPECULAR_STRENGTH 0.5
#define SHININESS 32

void main()
{
    // Ambient lighting
    vec3 ambient = AMBIENT_STRENGTH * u_LightColor.rgb;

    // Diffuse lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(v_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor.rgb;

    // Specular highlights
    vec3 viewDir = normalize(-v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);
    vec3 specular = SPECULAR_STRENGTH * spec * u_LightColor.rgb;

    // output RGB
    vec3 result = (ambient + diffuse + specular) * u_ObjectColor.rgb;

    // vec3 result = ambient * u_ObjectColor.rgb;

    FragColor = vec4(result, 1.0);
}
