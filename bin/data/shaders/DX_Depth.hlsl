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

  matrix newMatWorld;
  newMatWorld[0] = float4(1, 0, 0, 0);
  newMatWorld[1] = float4(0, 1, 0, 0);
  newMatWorld[2] = float4(0, 0, 1, 0);
  newMatWorld[3] = float4(0, 0, 0, 1);

  matrix matWV = mul(newMatWorld, viewMatrix);
  
  input.position.w = 1.0f;
  
  matrix matClip = mul(matWV, projectionMatrix);

  output.position = mul(input.position, matClip);

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