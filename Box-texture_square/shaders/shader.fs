#version 330 core
out vec4 fragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D aTexture;

void main()
{
  fragColor = texture(aTexture, texCoord);
}