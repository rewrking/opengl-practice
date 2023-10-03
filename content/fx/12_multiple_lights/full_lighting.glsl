#pragma type : vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

uniform mat4 u_ProjectionViewModel;
uniform mat4 u_ViewModel;
uniform mat3 u_NormalMatrix;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;

void main()
{
    vec4 pos = vec4(a_Pos, 1.0);
    gl_Position = u_ProjectionViewModel * pos;

    v_FragPos = vec3(u_ViewModel * pos);
    v_Normal = u_NormalMatrix * a_Normal;
    v_TexCoords = a_TexCoords;

}

#pragma type : fragment
#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct DirLight {
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform DirLight u_DirLight;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

out vec4 FragColor;

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = calculateDirectionalLight(u_DirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calculatePointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    // phase 3: spot light
    result += calculateSpotLight(u_SpotLight, norm, v_FragPos, viewDir);

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
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));
    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

	// attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// combine results
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
