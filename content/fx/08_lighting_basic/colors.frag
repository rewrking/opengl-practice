#version 330 core

in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;
uniform vec3 u_lightPos;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * u_LightColor.rgb;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor.rgb;

    vec3 result = (ambient + diffuse) * u_ObjectColor.rgb;

    // vec3 result = ambient * u_ObjectColor.rgb;

    FragColor = vec4(result, 1.0);
}
