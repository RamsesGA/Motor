/*
* Shader for DirectX 11.
* Shadow Map
* Made by: Ramses Guerrero Ambriz.
*/
#define M_PI 3.1415926538
#define EPSILON 0.00001

//----------------------------------------------------------------------------
//Type: 0 = directional 1 = point 2 = spot
struct lightInformation
{
  float4 lightDirection;
  float4 lightPosition;
  float4 lightColor;
  uint type;
  uint numberOfLights;
};

struct VS_INPUT
{
  float4 position    : POSITION;
  float4 normal      : NORMAL0;
  float4 tangent     : TANGENT0;
  float3 biTangent   : BINORMAL0;
  float2 texCoords   : TEXCOORD0;
  float4 boneWeights : BLENDWEIGHT0;
  uint4 boneIds      : BLENDINDICES0;
};

struct PS_INPUT
{
  float4 position : SV_POSITION;
  float depth     : TEXCOORD0;
};

float Lambert_Diffuse(in float3 lightDir, in float3 surfNormal)
{
  return max(0.0f, dot(lightDir, surfNormal));
}

//----------------------------------------------------------------------------
cbuffer linkToBufferViews : register(b0)
{
  matrix View;
  matrix ViewInv;
};

cbuffer linkToBufferProjections : register(b1)
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

cbuffer linkToBufferBonesTrans : register (b3)
{
  matrix boneTransform[200];
};

cbuffer linkToBufferLights : register (b4)
{
  lightInformation lights[20];
};

//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
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