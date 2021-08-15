#version 460 core

const int MaxBones = 200;

layout(location = 0) in vec4  position;
layout(location = 1) in vec4  normal;
layout(location = 2) in vec4  tangent;
layout(location = 3) in vec2  texCoords;
layout(location = 4) in vec4  boneWeights;
layout(location = 5) in ivec4 boneIds;

layout(std140, binding = 0) uniform linkToCB1
{
  mat4 View;
  mat4 Projection;
};

layout(std140, binding = 1) uniform linkToCB2
{
  mat4 World;
  vec4 vMeshColor;
};

layout(std140, binding = 2) uniform linkToCB3
{
  mat4 bonesTransform[MaxBones];
};

out vec4 outNormal;
out vec2 outTexCoord;
out vec4 outWorldPos;

void main()
{
  mat4 boneTrans;

  boneTrans += bonesTransform[boneIds[0]] * boneWeights[0];
  boneTrans += bonesTransform[boneIds[1]] * boneWeights[1];
  boneTrans += bonesTransform[boneIds[2]] * boneWeights[2];
  boneTrans += bonesTransform[boneIds[3]] * boneWeights[3];

  vec4 newPos = position * boneTrans;
  newPos = newPos * World;
  newPos = newPos * View;
  newPos = newPos * Projection;

  gl_Position = newPos;

  outTexCoord = texCoords;

  //vec4 newNormal = normal * bonesTransform;
  //outNormal = World * newNormal;

  outWorldPos = World * newPos;
}