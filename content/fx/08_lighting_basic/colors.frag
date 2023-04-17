#version 330 core

in vec3 v_Normal;
in vec3 v_FragPos;
in vec3 v_LightPos;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;

out vec4 FragColor;

#define AMBIENT_STRENGTH 0.5
#define SPECULAR_STRENGTH 0.5
#define SHININESS 64

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
