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

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_Normal;
in vec3 v_FragPos;
in vec3 v_LightPos;

uniform Material u_Material;
uniform Light u_Light;

out vec4 FragColor;

void main()
{
    // Ambient lighting
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    // Diffuse lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(v_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

    // Specular highlights
    vec3 viewDir = normalize(-v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * u_Material.specular);

    // output RGB
    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}
