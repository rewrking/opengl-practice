#pragma type : vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;

out vec4 VertexColor;
out vec2 TexCoord;

uniform mat4 u_Transform;

void main()
{
    gl_Position = u_Transform * vec4(a_Pos, 1.0);
    VertexColor = a_Color;
    TexCoord = a_TexCoord;
}

#pragma type : fragment
#version 330 core

in vec4 VertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, TexCoord) * VertexColor * u_Color;

    // combine 2 textures
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
