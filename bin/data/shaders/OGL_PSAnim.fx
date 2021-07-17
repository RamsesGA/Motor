#version 420 core

in vec4 outNormal;
in vec2 outTexCoord;
in vec4 outWorldPos;

out vec4 outColor;

layout(binding = 0) uniform sampler2D diffuseSampler;

void main()
{
  outColor = texture(diffuseSampler, outTexCoord);
}
