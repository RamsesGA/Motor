#version 330

layout(location = 0) in vec4  position;
layout(location = 1) in vec4  normal;
layout(location = 2) in vec4  tangent;
layout(location = 3) in vec2  texCoords;
layout(location = 4) in vec4  boneWeights;
layout(location = 5) in ivec4 boneIds;

out vec2 TexCoord0;
out vec4 Normal0;
out vec4 WorldPos0;

const int MAX_BONES = 200;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;
uniform vec4 vMeshColor;

uniform mat4 bonesTransform[MAX_BONES];

void main()
{
  mat4 BoneTransform = bonesTransform[boneIds[0]] * boneWeights[0];
  BoneTransform += bonesTransform[boneIds[1]] * boneWeights[1];
  BoneTransform += bonesTransform[boneIds[2]] * boneWeights[2];
  BoneTransform += bonesTransform[boneIds[3]] * boneWeights[3];

  vec4 PosL = BoneTransform * position;
  PosL = PosL * World;
  PosL = PosL * View;
  PosL = PosL * Projection;

  gl_Position = PosL;

  TexCoord0 = texCoords;

  vec4 NormalL = BoneTransform * normal;

  Normal0 = World * NormalL;

  WorldPos0 = World * PosL;
}