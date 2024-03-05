#pragma type : vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 1) in vec3 a_Tangent;
layout (location = 1) in vec3 a_Bitangent;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;

void main()
{
    v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_TexCoords = a_TexCoords;

    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}

#pragma type : fragment
#version 330 core

// #define MAX_TEXTURES 4
// #define POINT_LIGHT_COUNT 1

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    float shininess;
};

struct DirLight {
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform DirLight u_DirLight;

out vec4 FragColor;

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(v_Normal) * texture(u_Material.normal, v_TexCoords).rgb * texture(u_Material.height, v_TexCoords).rgb;
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 result = calculateDirectionalLight(u_DirLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // combine results
    vec3 ambient = light.ambient * texture(u_Material.diffuse, v_TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(u_Material.diffuse, v_TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(u_Material.specular, v_TexCoords).rgb;
    return (ambient + diffuse + specular);
}

