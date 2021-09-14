/*
* Shader for DirectX 11.
* Shadow Map
* Made by: Ramses Guerrero Ambriz.
*/
#define M_PI 3.14159265383
#define EPSILON 0.00001

//----------------------------------------------------------------------------
struct myLight
{
  float4 lDirection;
  float4 lColor;
  float3 lPosition;

  uint32 numberLights;
};

//----------------------------------------------------------------------------
cbuffer linkToBufferViewMatrixes : register(b0)
{
  matrix View;
  matrix ViewInv;
};

cbuffer linkToBufferProjectionMatrixes : register(b1)
{
  matrix Projection;
  matrix ProjectionInv;
  matrix ViewProjection;
  matrix ViewProjectionInv;
};

cbuffer linkToBufferWorldInfo : register(b2)
{
  matrix World;
  matrix WorldInv;
  matrix WorldView;
  matrix WorldViewInv;
  matrix WorldProj;
  matrix WorldProjInv;
  matrix WorldViewProj;
  matrix WorldViewProjInv;
  matrix depthMVP;
  float4 viewPosition;
  float4 vMeshColor;
};

cbuffer linkToBufferBonesTransform : register (b3)
{
  matrix boneTransform[200];
};

cbuffer linkToBufferLight : register (b4)
{
  myLight lights[5];
};

//----------------------------------------------------------------------------
struct VS_INPUT
{
  float4 position    : POSITION;
  float3 normal      : NORMAL0;
  float3 tangent     : TANGENT0;
  float3 biTangent   : BINORMAL0;
  float2 texCoords   : TEXCOORD0;
  float4 boneWeights : BLENDWEIGHT0;
  uint4  boneIds     : BLENDINDICES0;
};

struct PS_INPUT
{
  float4 position : SV_POSITION;
  float depth     : TEXCOORD0;
};

//----------------------------------------------------------------------------
float Lambert_Diffuse(in float3 lightDir, in float3 surfNormal)
{
  return max(0.0f, dot(lightDir, surfNormal));
}

//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------
PS_INPUT vs_ShadowMap(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;
  
  matrix boneTrans = boneTransform[input.boneIds[0]] * input.boneWeights[0];
  boneTrans += boneTransform[input.boneIds[1]] * input.boneWeights[1];
  boneTrans += boneTransform[input.boneIds[2]] * input.boneWeights[2];
  boneTrans += boneTransform[input.boneIds[3]] * input.boneWeights[3];
  
  float4 pos = mul(input.position, boneTrans);

  output.position = mul(pos, World);
  output.position = mul(output.position, depthMVP);
  output.depth = 1 - (output.position.z/ output.position.w);
  
  return output;
}

//----------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------
float4 ShadowMap(PS_INPUT input) : SV_Target
{
  return float4(input.depth, input.depth, input.depth, 1.0f);
}