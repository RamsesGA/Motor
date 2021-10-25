/*
* Shader for DirectX 11.
* Depth shader
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------VS
cbuffer linkToBufferMatrixB : register(b0)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer linkToBufferModelData : register(b1)
{
  matrix modelMatrix;
  matrix bonesTransform[200];
}


//----------------------------------------------------------------------------VS
struct VS_INPUT
{
  float4 position    : POSITION0;
  float3 normal      : NORMAL0;
  float3 tangent     : TANGENT0;
  float3 biTangent   : BINORMAL0;
  float2 texCoords   : TEXCOORD0;
  float4 boneWeights : BLENDWEIGHT0;
  uint4  boneIds     : BLENDINDICES0;
};

struct PS_INPUT
{
  float4 position      : SV_POSITION;
  float4 depthPosition : TEXCOORD0;
};


//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------
PS_INPUT DepthVS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;
  
  matrix boneTrans = bonesTransform[input.boneIds[0]] * input.boneWeights[0];

  boneTrans += bonesTransform[input.boneIds[1]] * input.boneWeights[1];
  boneTrans += bonesTransform[input.boneIds[2]] * input.boneWeights[2];
  boneTrans += bonesTransform[input.boneIds[3]] * input.boneWeights[3];
  
  input.position.w = 1.0f;
  
  float4 position = mul(input.position, boneTrans);
  //position.w = 1.0f;

  matrix matWV = mul(worldMatrix, modelMatrix);
  matWV = mul(matWV, viewMatrix);

  matrix matClip = mul(matWV, projectionMatrix);

  output.position = mul(position, matClip);
  //output.position = mul(input.position, matClip);

  output.depthPosition = output.position;

  return output;
}


//----------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------
float4 DepthPS(PS_INPUT input) : SV_TARGET
{
  float value = input.depthPosition.z / input.depthPosition.w;
  return float4(value.xxx, 1.0f);
}