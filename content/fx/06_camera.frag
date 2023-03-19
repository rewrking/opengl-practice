#version 330 core

// in vec4 VertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, TexCoord) /** VertexColor*/ * u_Color;

    // combine 2 textures
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
