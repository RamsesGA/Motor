/*
* Shader for DirectX 11.
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------
cbuffer linkToBuffer1 : register(b0)
{
  matrix mView;
  matrix mProjection;
}

cbuffer linkToBuffer2 : register(b1)
{
  matrix mWorld;
  float4 objectPosition;
}

//----------------------------------------------------------------------------
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

struct VS_OUTPUT
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;
};

VS_OUTPUT vs_ssAligned(VS_INPUT input)
{
  VS_OUTPUT output = (VS_OUTPUT)0;

  output.position = input.position;

  output.texCoord = input.texCoords;

  return(output);
}